#include "PreCompile.h"
#include "EngineVertex.h"
#include <GameEngineBase/GameEngineDebug.h>

void GameEngineLayOut::AddInputLayOut(
	const char* _SemanticName,
	unsigned int _Index,
	unsigned int _AlignedByteOffset,
	DXGI_FORMAT _Format,
	unsigned int _InputSlot,
	unsigned int _InstanceDataStepRate,
	D3D11_INPUT_CLASSIFICATION _inputClass
) 
{
	SemanticName_.push_back(_SemanticName);

	D3D11_INPUT_ELEMENT_DESC LayOutDesc = { 0, };

	// https://docs.microsoft.com/ko-kr/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics
	//                         "POSTION"
	LayOutDesc.SemanticName = _SemanticName;
	//                          0
	LayOutDesc.SemanticIndex = _Index;
	//                 DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT
	LayOutDesc.Format = _Format;
	//                        0 신경쓰지마
	LayOutDesc.InputSlot = _InputSlot;
	//                                  신경쓰지마
	LayOutDesc.InstanceDataStepRate = _InstanceDataStepRate;
	//                               16
	LayOutDesc.AlignedByteOffset = LayOutOffset_;

	LayOutOffset_ += _AlignedByteOffset;

	InputLayoutDesc_.push_back(LayOutDesc);
}

ID3D11InputLayout* GameEngineLayOut::CreateLayOut(GameEngineShader* _Shader) 
{
	ID3D11InputLayout* LayOut_;

	if (
		S_OK != GameEngineDevice::GetInst().GetDevice()->CreateInputLayout
		(
			&InputLayoutDesc_[0],
			static_cast<unsigned int>(InputLayoutDesc_.size()),
			_Shader->CodeBlob_->GetBufferPointer(),
			_Shader->CodeBlob_->GetBufferSize(),
			&LayOut_
		)
		)
	{
		GameEngineDebug::MsgBoxError("인풋레이아웃 생성에 실패했습니다.");
		return nullptr;
	}

	return LayOut_;
}

GameEngineLayOut GameEngineVertex::LayOut;