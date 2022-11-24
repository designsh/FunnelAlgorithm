#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include <GameEngine/GameEngineDevice.h>

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineVertexBuffer : public GameEngineObjectNameBase
{

public:	// member Var
	ID3D11Buffer* Buffer_;
	UINT Size_;
	UINT Count_;
	UINT Offset_;

	// ������ ������
	// ������ ũ��
	// ������ ����
	D3D11_BUFFER_DESC BufferData_;

	// �ʱⰪ���� ������
	D3D11_SUBRESOURCE_DATA ResData_;


public:
	GameEngineVertexBuffer(); // default constructer ����Ʈ ������
	~GameEngineVertexBuffer(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineVertexBuffer(const GameEngineVertexBuffer& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineVertexBuffer(GameEngineVertexBuffer&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

	void Create(const void* _Data, size_t _Datas, size_t _Count, D3D11_USAGE _Usage);

public:
	template<typename VertexType>
	void Create(const std::vector<VertexType>& _Datas, D3D11_USAGE _Usage)
	{
		InputLayoutDesc_ = &(VertexType::LayOut.InputLayoutDesc_);

		Create(reinterpret_cast<const void*>(&_Datas[0]), sizeof(VertexType), _Datas.size(), _Usage);
	}

	void Setting();


public:
	std::vector<D3D11_INPUT_ELEMENT_DESC>* InputLayoutDesc_;
};



