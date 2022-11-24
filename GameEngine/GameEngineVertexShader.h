#pragma once
#include <vector>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineShader.h"
#include "GameEngineDevice.h"

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineVertexShader : public GameEngineShader
{
	friend class  GameEngineRenderingPipeLine;
private:	// member Var
	ID3D11VertexShader* Shader_;

public:


	GameEngineVertexShader(); // default constructer ����Ʈ ������
	~GameEngineVertexShader(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineVertexShader(const GameEngineVertexShader& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineVertexShader(GameEngineVertexShader&& _other) noexcept = delete; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineVertexShader& operator=(const GameEngineVertexShader& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineVertexShader& operator=(const GameEngineVertexShader&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	bool Create(
		const std::string& _ShaderCode,
		const std::string& _EntryPoint,
		UINT _VersionHigh = 5,
		UINT _VersionLow = 0
	);

	bool Load(
		const std::string& _Path,
		const std::string& _EntryPoint,
		UINT _VersionHigh = 5,
		UINT _VersionLow = 0
	);

	bool StringCompile();

	bool FileCompile(const std::string& _Path);

	void SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) override;
	void SetTexture(const GameEngineTextureSetting* _Setting) override;
	void SetSampler(const GameEngineSamplerSetting* _Setting) override;
	void SetStructuredBuffer(const GameEngineStructuredBufferSetting* _Setting) override;

	void ReSetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) override;
	void ReSetTexture(const GameEngineTextureSetting* _Setting) override;
	void ReSetSampler(const GameEngineSamplerSetting* _Setting) override;
	void ReSetStructuredBuffer(const GameEngineStructuredBufferSetting* _Setting) override;

	void LayOutReset();

	void AddInputLayOut(
		const char* _SemanticName,
		unsigned int _Index,
		unsigned int _AlignedByteOffset,
		DXGI_FORMAT _Format,
		unsigned int _InputSlot,
		unsigned int _InstanceDataStepRate,
		D3D11_INPUT_CLASSIFICATION _inputClass
	);

	void CreateLayOut();

/// <summary>
/// ////////////////////////////// InputLayOutSettting
/// </summary>

private:
	ID3D11InputLayout* LayOut_;
	unsigned int LayOutOffset_;
	std::vector<std::string> SemanticName_;
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayoutDesc_;

	void LayOutCheck();

	void LayOutClear();


public:
	void InputLayOutSetting();
	void Setting();
};

