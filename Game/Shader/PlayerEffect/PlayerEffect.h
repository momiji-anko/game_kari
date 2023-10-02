#pragma once
#include<SimpleMath.h>
#include <d3dcompiler.h>


class PlayerEffect
{
public:
	PlayerEffect();



private:
	// csoの場所
	std::wstring m_Path;

	// 頂点シェーダーのデータ
	Microsoft::WRL::ComPtr<ID3DBlob> m_vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> m_vsBlobDepth;

	// ピクセルシェーダーのデータ
	Microsoft::WRL::ComPtr<ID3DBlob> m_psBlobTexture;
	Microsoft::WRL::ComPtr<ID3DBlob> m_psBlobNoneTex;

	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShaderDepth;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShaderTexture;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShaderNoneTex;

	// シャドウマップ用テクスチャ
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_shadowMap;

	// 深度バッファビュー
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthView;

	// シェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderView;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// シャドウマップ作成時のビューポート
	D3D11_VIEWPORT m_viewPort[1];

	// 定数バッファの構造体の定義
	struct ConstantBuffer
	{
		DirectX::XMMATRIX LightViewProj;
	};

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// サンプラー
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler;

};