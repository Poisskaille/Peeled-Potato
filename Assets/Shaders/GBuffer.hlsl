// GBuffer.hlsl — geometry pass: outputs albedo, normals, ORM, emissive, motion vectors

#include "Common/Bindless.hlsli"

#pragma vertex VSMain
#pragma pixel FSMain

struct ModelVertex
{
    float3 Position;
    float3 Normal;
    float2 UV;
    float4 Tangent; // xyz = tangent, w = handedness
};

struct PushConstants
{
    column_major float4x4 MVP;
    int VertexBufferIndex;
    int BaseVertex;
    int AlbedoIndex;    // -1 = no texture
    int NormalIndex;    // -1 = no texture
    int ORMIndex;       // -1 = no texture (R=occlusion, G=roughness, B=metallic)
    int EmissiveIndex;  // -1 = no texture
    int SamplerIndex;
    int _pad;
};
PUSH_CONSTANTS(PushConstants);

struct VSOut
{
    float4 Position  : SV_Position;
    float3 WorldNormal   : NORMAL;
    float3 WorldTangent  : TANGENT;
    float3 WorldBitangent : BINORMAL;
    float2 UV        : TEXCOORD0;
};

struct GBufferOut
{
    float4 Albedo       : SV_Target0; // RGBA8_UNORM
    float4 Normals      : SV_Target1; // RGBA16_FLOAT  world-space normal
    float4 ORM          : SV_Target2; // RGBA8_UNORM   R=occlusion, G=roughness, B=metallic
    float4 Emissive     : SV_Target3; // RGBA16_FLOAT
    float4 MotionVectors: SV_Target4; // RGBA16_FLOAT  RG=screen-space motion (zeroed for now)
};

VSOut VSMain(uint vertexID : SV_VertexID)
{
    StructuredBuffer<ModelVertex> vertices = ResourceDescriptorHeap[PUSH.VertexBufferIndex];
    ModelVertex v = vertices[vertexID + PUSH.BaseVertex];

    VSOut o;
    o.Position = mul(PUSH.MVP, float4(v.Position, 1.0f));
    o.WorldNormal    = v.Normal;
    o.WorldTangent   = v.Tangent.xyz;
    o.WorldBitangent = cross(v.Normal, v.Tangent.xyz) * v.Tangent.w;
    o.UV             = v.UV;
    return o;
}

GBufferOut FSMain(VSOut i)
{
    SamplerState samp = SamplerDescriptorHeap[PUSH.SamplerIndex];

    // --- Albedo ---
    float4 albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (PUSH.AlbedoIndex >= 0)
    {
        Texture2D<float4> tex = ResourceDescriptorHeap[PUSH.AlbedoIndex];
        albedo = tex.Sample(samp, i.UV);
        if (albedo.a < 0.25f)
            discard;
    }

    // --- Normal ---
    float3 N = normalize(i.WorldNormal);
    if (PUSH.NormalIndex >= 0)
    {
        Texture2D<float4> normalTex = ResourceDescriptorHeap[PUSH.NormalIndex];
        float3 ts = normalTex.Sample(samp, i.UV).rgb * 2.0f - 1.0f;
        float3 T  = normalize(i.WorldTangent);
        float3 B  = normalize(i.WorldBitangent);
        N = normalize(T * ts.x + B * ts.y + N * ts.z);
    }

    // --- ORM (Occlusion, Roughness, Metallic) ---
    float3 orm = float3(1.0f, 0.5f, 0.0f); // defaults
    if (PUSH.ORMIndex >= 0)
    {
        Texture2D<float4> ormTex = ResourceDescriptorHeap[PUSH.ORMIndex];
        orm = ormTex.Sample(samp, i.UV).rgb;
    }

    // --- Emissive ---
    float3 emissive = float3(0.0f, 0.0f, 0.0f);
    if (PUSH.EmissiveIndex >= 0)
    {
        Texture2D<float4> emTex = ResourceDescriptorHeap[PUSH.EmissiveIndex];
        emissive = emTex.Sample(samp, i.UV).rgb;
    }

    GBufferOut o;
    o.Albedo        = float4(albedo.rgb, 1.0f);
    o.Normals       = float4(N * 0.5f + 0.5f, 0.0f); // encode to [0,1]
    o.ORM           = float4(orm, 0.0f);
    o.Emissive      = float4(emissive, 0.0f);
    o.MotionVectors = float4(0.0f, 0.0f, 0.0f, 0.0f);
    return o;
}
