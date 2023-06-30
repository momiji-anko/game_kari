
// DirectXTK�Őݒ肳��Ă���萔�o�b�t�@
cbuffer Parameters_DirectXTK : register(b0)
{
    float4 DiffuseColor             : packoffset(c0);
    float3 EmissiveColor            : packoffset(c1);
    float3 SpecularColor            : packoffset(c2);
    float  SpecularPower            : packoffset(c2.w);

    float3 LightDirection[3]        : packoffset(c3);
    float3 LightDiffuseColor[3]     : packoffset(c6);
    float3 LightSpecularColor[3]    : packoffset(c9);

    float3 EyePosition              : packoffset(c12);

    float3 FogColor                 : packoffset(c13);
    float4 FogVector                : packoffset(c14);

    float4x4 World                  : packoffset(c15);
    float3x3 WorldInverseTranspose  : packoffset(c19);
    float4x4 WorldViewProj          : packoffset(c22);
};

// �萔�o�b�t�@
cbuffer Parameters_User : register(b1)
{
    // �V���h�E�}�b�v��̎ˉe��Ԃ֕ϊ�����s��
    float4x4 LightViewProj          : packoffset(c0);
}

// ���_�V�F�[�_�[�̓���
struct VSInput
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float4 Tangent  : TANGENT;
    float4 Color    : COLOR;
    float2 TexCoord : TEXCOORD;
};

// ���_�V�F�[�_�[�̏o��
struct VSOutput
{
    float4 Diffuse    : COLOR0;
    float3 Specular   : COLOR1;
    float4 ZCalcTex   : TEXCOORD0;
    float2 TexCoord   : TEXCOORD1;
    float4 PositionPS : SV_Position;
};

// �s�N�Z���V�F�[�_�[�̓���
struct PSInput
{
    float4 Diffuse    : COLOR0;
    float3 Specular   : COLOR1;
    float4 ZCalcTex   : TEXCOORD0;
    float2 TexCoord   : TEXCOORD1;
};

// �e�N�X�`��
Texture2D Texture    : register(t0);
// �V���h�E�}�b�v
Texture2D SM_Texture : register(t1);

// �ʏ�̃T���v���[
SamplerState Sampler    : register(s0);
// �V���h�E�}�b�v�p�̃T���v���[
SamplerState SM_Sampler : register(s1);
