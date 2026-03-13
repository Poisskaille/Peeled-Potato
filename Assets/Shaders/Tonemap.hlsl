// Tonemap.hlsl — ACES filmic tonemap, HDR RGBA16F → LDR RGBA8.

#include "Common/Bindless.hlsli"

#pragma compute CSMain

struct PushConstants
{
    int InputIndex;   // Texture2D<float4>   HDR SRV
    int OutputIndex;  // RWTexture2D<float4> LDR UAV
    int Width;
    int Height;
};
PUSH_CONSTANTS(PushConstants);

// Narkowicz 2015 ACES fitted curve
float3 ACESFilm(float3 x)
{
    const float a = 2.51f;
    const float b = 0.03f;
    const float c = 2.43f;
    const float d = 0.59f;
    const float e = 0.14f;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

[numthreads(8, 8, 1)]
void CSMain(uint3 id : SV_DispatchThreadID)
{
    if (id.x >= (uint)PUSH.Width || id.y >= (uint)PUSH.Height)
        return;

    Texture2D<float4>   hdr = ResourceDescriptorHeap[PUSH.InputIndex];
    RWTexture2D<float4> ldr = ResourceDescriptorHeap[PUSH.OutputIndex];

    float3 color = hdr[id.xy].rgb;
    ldr[id.xy] = float4(ACESFilm(color), 1.0f);
}
