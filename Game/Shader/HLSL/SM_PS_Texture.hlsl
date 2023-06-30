#include "SM_Header.hlsli"
#include "SM_Common.hlsli"

// �e�N�X�`���L�̃s�N�Z���V�F�[�_�[
float4 main(VSOutput pin) : SV_TARGET0
{
    float4 color = Texture.Sample(Sampler, pin.TexCoord) * pin.Diffuse;
 
    AddSpecular(color, pin.Specular);

    return saturate(color * CalcShadow(pin.ZCalcTex));
}
