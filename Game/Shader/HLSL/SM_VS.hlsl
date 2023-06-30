#include "SM_Header.hlsli"

struct ColorPair
{
    float3 Diffuse;
    float3 Specular;
};

struct CommonVSOutput
{
    float4 Pos_ps;
    float4 Diffuse;
    float3 Specular;
    float  FogFactor;
};

ColorPair ComputeLights(float3 eyeVector, float3 worldNormal, uniform int numLights)
{
    float3x3 lightDirections = 0;
    float3x3 lightDiffuse = 0;
    float3x3 lightSpecular = 0;
    float3x3 halfVectors = 0;

    [unroll]
    for (int i = 0; i < numLights; i++)
    {
        lightDirections[i] = LightDirection[i];
        lightDiffuse[i] = LightDiffuseColor[i];
        lightSpecular[i] = LightSpecularColor[i];

        halfVectors[i] = normalize(eyeVector - lightDirections[i]);
    }

    float3 dotL = mul(-lightDirections, worldNormal);
    float3 dotH = mul(halfVectors, worldNormal);

    float3 zeroL = step(0, dotL);

    float3 diffuse = zeroL * dotL;
    float3 specular = pow(max(dotH, 0) * zeroL, SpecularPower) * dotL;

    ColorPair result;

    result.Diffuse = mul(diffuse, lightDiffuse) * DiffuseColor.rgb + EmissiveColor;
    result.Specular = mul(specular, lightSpecular) * SpecularColor;

    return result;
}

CommonVSOutput ComputeCommonVSOutputWithLighting(float4 position, float3 normal, uniform int numLights)
{
    CommonVSOutput vout;

    float4 pos_ws = mul(position, World);
    float3 eyeVector = normalize(EyePosition - pos_ws.xyz);
    float3 worldNormal = normalize(mul(normal, WorldInverseTranspose));

    ColorPair lightResult = ComputeLights(eyeVector, worldNormal, numLights);

    vout.Pos_ps = mul(position, WorldViewProj);
    vout.Diffuse = float4(lightResult.Diffuse, DiffuseColor.a);
    vout.Specular = lightResult.Specular;
 
    return vout;
}

// DirectXTK�̒��_�V�F�[�_�[���x�[�X�ɉ����������_�V�F�[�_�[�ino Fog)
VSOutput main(VSInput vin)
{
    VSOutput vout;

    // ���W�ϊ��ƃ��C�g�̌v�Z
    CommonVSOutput cout = ComputeCommonVSOutputWithLighting(vin.Position, vin.Normal, 3);
    // ���ʂ��o��
    vout.PositionPS = cout.Pos_ps;
    vout.Diffuse = cout.Diffuse;
    vout.Specular = cout.Specular;
    vout.TexCoord = vin.TexCoord;

    // ���_���W�����C�g�ڐ��̍��W��Ԃ֕ϊ����A���̉��s���g�p����
    vout.ZCalcTex = mul(vin.Position, mul(World, LightViewProj));

    return vout;
}

