#include "Fade.hlsli"
Texture2D tex : register(t0);

SamplerState samLinear : register(s0);


//�摜�ɉ������t�F�[�h�iif�����j
float4 main(PS_INPUT input) : SV_TARGET
{
    float4 fade = tex.Sample(samLinear, input.Tex);
    fade.a = 1.0f - ceil(fade.b - Time.y);
    fade.rgb = float3(0, 0, 0);
    return fade;
}


