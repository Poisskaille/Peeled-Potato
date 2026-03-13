// Deferred.hlsl — fullscreen lighting pass reading the GBuffer (compute).
// Currently passes albedo through to HDR; lighting to be added later.

#include "Common/Bindless.hlsli"

#pragma compute CSMain

struct PushConstants
{
    int AlbedoIndex;
    int NormalsIndex;
    int ORMIndex;
    int EmissiveIndex;
    int OutputIndex;  // RWTexture2D HDR UAV
    int Width;
    int Height;
};
PUSH_CONSTANTS(PushConstants);

[numthreads(8, 8, 1)]
void CSMain(uint3 id : SV_DispatchThreadID)
{
    if (id.x >= (uint)PUSH.Width || id.y >= (uint)PUSH.Height)
        return;

    Texture2D<float4>    albedo = ResourceDescriptorHeap[PUSH.AlbedoIndex];
    RWTexture2D<float4>  hdr    = ResourceDescriptorHeap[PUSH.OutputIndex];

    float3 color = albedo[id.xy].rgb;
    hdr[id.xy] = float4(color, 1.0f);
}
