#pragma once
#include "GameEngineDevice.h"

// ���� : 
class GameEngineConstantBuffer;
class GameEngineConstantBufferManager
{
private:
	static GameEngineConstantBufferManager* Inst;

public:
	static GameEngineConstantBufferManager& GetInst()
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

private:	// member Var
	std::map<std::string, GameEngineConstantBuffer*> ResourcesMap;

public:
	// ���� ����� �ִ�.
	GameEngineConstantBuffer* Create(const std::string& _Name, const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo);

	GameEngineConstantBuffer* CreateAndFind(const std::string& _Name, const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo);

	// ���Ͽ��� �ε�
	GameEngineConstantBuffer* Load(const std::string& _Path);
	// �̸� ���� ����
	GameEngineConstantBuffer* Load(const std::string& _Name, const std::string& _Path);
	// ��Ͽ��� ã�´�.
	GameEngineConstantBuffer* Find(const std::string& _Name);

private:
	GameEngineConstantBufferManager(); // default constructer ����Ʈ ������
	~GameEngineConstantBufferManager(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineConstantBufferManager(const GameEngineConstantBufferManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineConstantBufferManager(GameEngineConstantBufferManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineConstantBufferManager& operator=(const GameEngineConstantBufferManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineConstantBufferManager& operator=(const GameEngineConstantBufferManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
};




