#include "PreCompile.h"
#include "GameEngineConstantBuffer.h"
#include <GameEngineBase/GameEngineDebug.h>

GameEngineConstantBuffer::GameEngineConstantBuffer() 
	: Buffer_(nullptr),
	BufferData_(),
	ResData_(),
	BufferDesc_()
{
}

GameEngineConstantBuffer::~GameEngineConstantBuffer() 
{
	if (nullptr != Buffer_)
	{
		Buffer_->Release();
		Buffer_ = nullptr;
	}
}




void GameEngineConstantBuffer::Create(const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo)
{
	BufferDesc_ = _BufferDesc;

	BufferData_.ByteWidth = BufferDesc_.Size;
	BufferData_.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	BufferData_.CPUAccessFlags = 0;
	if (BufferData_.Usage == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		BufferData_.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	}

	BufferData_.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferData_.StructureByteStride = 0;
	BufferData_.MiscFlags = 0;

	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferData_, nullptr, &Buffer_))
	{
		GameEngineDebug::MsgBoxError("상수버퍼 생성에 오류가 있습니다." + GetName());
		return;
	}

	for (unsigned int i = 0; i < BufferDesc_.Variables; i++)
	{
		ID3D11ShaderReflectionVariable* Var = _VarInfo->GetVariableByIndex(i);
		D3D11_SHADER_VARIABLE_DESC VarDesc;
		Var->GetDesc(&VarDesc);
		VarInfos_.push_back(VarDesc);
	}
}

void GameEngineConstantBuffer::ChangeData(const void* _Data, size_t _Size) 
{
	if (BufferData_.ByteWidth != _Size)
	{
		GameEngineDebug::MsgBoxError("상수버퍼 세팅의 크기가 다릅니다.");
	}

	memset(&ResData_, 0, sizeof(ResData_));


	// 너무 자주 map을 호출하는것은 랜더링을 
	// 좀 느립니다.
	// 이제부터 Buffer_의 데이터가 바뀔꺼니 랜더링 정지해


	GameEngineDevice::GetContext()->Map(Buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &ResData_);
	// Cpu데이터의 전송을 알리는것이 알리는것이 map


	// 그래픽카드에서의 핸들혹은 주소라고 표현할수 있다
	// 여기에다가 데이터를 써주면 알아서 그래픽카드로 전송된다.


	// 요약 cpu에 존재하는 _Data 데이터를 BufferData_.ByteWidth만큼
	// 그래픽카드의 ResData_.pData로 전송해라.
	memcpy_s(ResData_.pData, BufferData_.ByteWidth, _Data, BufferData_.ByteWidth);

	
	
	// 다 전송했다고 이제 이 버퍼를 다시 사용해도 좋다라고 말하는게 unmap
	GameEngineDevice::GetContext()->Unmap(Buffer_, 0);
}