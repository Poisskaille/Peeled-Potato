#ifndef SCENE_HLSLI
#define SCENE_HLSLI

/// Per-draw instance (80 bytes) — matches GPUInstance in GPUInstance.hpp
struct GPUInstance
{
    column_major float4x4 WorldMatrix;   // Combined actor × mesh local transform
    int   MaterialIndex;                 // Index into material buffer
    int   VertexBufferIndex;             // Bindless handle to the vertex buffer
    int   BaseVertex;                    // Base vertex offset
    int   _pad;
};

// GPUMaterial::Flags bits
#define MAT_FLAG_ALPHA_TEST         (1 << 0)
#define MAT_FLAG_OVERRIDE_METALLIC  (1 << 1)
#define MAT_FLAG_OVERRIDE_ROUGHNESS (1 << 2)

/// Per-material (48 bytes) — matches GPUMaterial in Material.hpp
struct GPUMaterial
{
    int   AlbedoIndex;      // -1 = white
    int   NormalIndex;      // -1 = vertex normal
    int   ORMIndex;         // -1 = defaults
    int   EmissiveIndex;    // -1 = black
    int   SamplerIndex;
    int   Flags;            // MAT_FLAG_* bitmask
    float MetallicFactor;   // used when MAT_FLAG_OVERRIDE_METALLIC is set
    float RoughnessFactor;  // used when MAT_FLAG_OVERRIDE_ROUGHNESS is set
    float3 Color;           // base color factor (linear)
    float  AlphaCutoff;     // discard threshold when MAT_FLAG_ALPHA_TEST is set
};

/// Shared mesh vertex format — matches ModelVertex in C++
struct ModelVertex
{
    float3 Position;
    float3 Normal;
    float2 UV;
    float4 Tangent; // xyz = tangent, w = handedness
};

#endif // SCENE_HLSLI
