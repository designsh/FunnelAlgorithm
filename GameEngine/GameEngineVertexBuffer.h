#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include <GameEngine/GameEngineDevice.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineVertexBuffer : public GameEngineObjectNameBase
{

public:	// member Var
	ID3D11Buffer* Buffer_;
	UINT Size_;
	UINT Count_;
	UINT Offset_;

	// 버퍼의 데이터
	// 버퍼의 크기
	// 버퍼의 개수
	D3D11_BUFFER_DESC BufferData_;

	// 초기값등의 데이터
	D3D11_SUBRESOURCE_DATA ResData_;


public:
	GameEngineVertexBuffer(); // default constructer 디폴트 생성자
	~GameEngineVertexBuffer(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineVertexBuffer(const GameEngineVertexBuffer& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineVertexBuffer(GameEngineVertexBuffer&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineVertexBuffer& operator=(const GameEngineVertexBuffer&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

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



