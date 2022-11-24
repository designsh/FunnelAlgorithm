#include "PreCompile.h"
#include "GameEngineVertexShader.h"
#include <GameEngineBase/GameEngineString.h>
#include "GameEngineShaderResHelper.h"

GameEngineVertexShader::GameEngineVertexShader() // default constructer 디폴트 생성자
	: GameEngineShader(ShaderType::VS)
{

}

GameEngineVertexShader::~GameEngineVertexShader() // default destructer 디폴트 소멸자
{
	if (nullptr != LayOut_)
	{
		LayOut_->Release();
		LayOut_ = nullptr;
	}

	if (nullptr != Shader_)
	{
		Shader_->Release();
		Shader_ = nullptr;
	}

	if (nullptr != CodeBlob_)
	{
		CodeBlob_->Release();
		CodeBlob_ = nullptr;
	}

}

bool GameEngineVertexShader::Create(
	const std::string& _ShaderCode,
	const std::string& _EntryPoint,
	UINT _VersionHigh,
	UINT _VersionLow
)
{
	SetVersion(_VersionHigh, _VersionLow);
	SetEntryPoint(_EntryPoint);
	SetCode(_ShaderCode);
	CreateVersion("vs");

	return StringCompile();
}

bool GameEngineVertexShader::Load(
	const std::string& _Path,
	const std::string& _EntryPoint,
	UINT _VersionHigh,
	UINT _VersionLow
)
{
	SetVersion(_VersionHigh, _VersionLow);
	SetEntryPoint(_EntryPoint);
	//SetCode(_ShaderCode);
	CreateVersion("vs");

	return FileCompile(_Path);
}

bool GameEngineVertexShader::FileCompile(const std::string& _Path) {

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif

	// 00000000 00000000 00000000 00000101

	// 행렬이 전치가 되서 들어가는것을 막아준다.
	// 전치가 기본이고 전치된걸 다시 
	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	// 결과물의 바이트코드

	ID3DBlob* ResultBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;

	// MultiByteToWideChar()

	std::wstring Path;
	GameEngineString::AnsiToUnicode(_Path, Path);

	if (S_OK != D3DCompileFromFile(
		Path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		EntryPoint_.c_str(),
		Version_.c_str(),
		Flag,
		0,
		&ResultBlob,
		&ErrorBlob))
	{
		std::string ErrorText = (char*)ErrorBlob->GetBufferPointer();

		GameEngineDebug::MsgBox(GetName() + " 컴파일 도중 에러가 발생했습니다.");
		GameEngineDebug::MsgBox(ErrorText);
		return false;
	}

	CodeBlob_ = ResultBlob;

	if (S_OK != GameEngineDevice::GetDevice()->CreateVertexShader(CodeBlob_->GetBufferPointer(), CodeBlob_->GetBufferSize(), nullptr, &Shader_))
	{
		GameEngineDebug::MsgBoxError("버텍스 버퍼 생성에 실패했습니다.");
		return false;
	}

	LayOutCheck();
	ResCheck();

	return true;
}


bool GameEngineVertexShader::StringCompile()
{

	unsigned int Flag = 0;

#ifdef _DEBUG
	Flag = D3D10_SHADER_DEBUG;
#endif

	// 00000000 00000000 00000000 00000101

	// 행렬이 전치가 되서 들어가는것을 막아준다.
	// 전치가 기본이고 전치된걸 다시 
	Flag |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	// 결과물의 바이트코드

	ID3DBlob* ResultBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;

	if (S_OK != D3DCompile(
		Code_.c_str(),
		Code_.size(),
		"",
		nullptr,
		nullptr,
		EntryPoint_.c_str(),
		Version_.c_str(),
		Flag,
		0,
		&ResultBlob,
		&ErrorBlob))
	{
		std::string ErrorText = (char*)ErrorBlob->GetBufferPointer();

		GameEngineDebug::MsgBox(GetName() + " 컴파일 도중 에러가 발생했습니다.");
		GameEngineDebug::MsgBox(ErrorText);
		return false;
	}

	CodeBlob_ = ResultBlob;

	if (S_OK != GameEngineDevice::GetDevice()->CreateVertexShader(CodeBlob_->GetBufferPointer(), CodeBlob_->GetBufferSize(), nullptr, &Shader_))
	{
		GameEngineDebug::MsgBoxError("버텍스 버퍼 생성에 실패했습니다.");
		return false;
	}

	LayOutCheck();
	ResCheck();

	return true;
}

void GameEngineVertexShader::AddInputLayOut(
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

	//struct GameEngineVertex
	//{
	//public:
	//	// 포지션
	//	// 텍스처 비율
	//	float4 Postion1; POSTION 1 0
	//	float4 Postion0; POSTION 0 16
	//  float4 Postion2; POSTION 2 32 
	//	float4 Color; COLOR 0 48
	//}


	// 
}

void GameEngineVertexShader::CreateLayOut()
{
	if (nullptr != LayOut_)
	{
		LayOut_->Release();
		LayOut_ = nullptr;
	}

	if (
		S_OK != GameEngineDevice::GetInst().GetDevice()->CreateInputLayout
		(
			&InputLayoutDesc_[0],
			static_cast<unsigned int>(InputLayoutDesc_.size()),
			CodeBlob_->GetBufferPointer(),
			CodeBlob_->GetBufferSize(),
			&LayOut_
		)
		)
	{
		GameEngineDebug::MsgBoxError("인풋레이아웃 생성에 실패했습니다.");
		return;
	}
}

void GameEngineVertexShader::LayOutCheck()
{
	LayOutClear();

	if (nullptr == CodeBlob_)
	{
		return;
	}

	// 내가 쉐이더에서 사용한 변수 함수들 인자들 그 이외의 상수버퍼등등등등의 모든
	// 정보를 알고 있는 녀석
	// 이 사람 쉐이더에서 행렬1개사용함
	ID3D11ShaderReflection* CompilInfo;

	if (S_OK != D3DReflect
	(
		CodeBlob_->GetBufferPointer(),
		CodeBlob_->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		reinterpret_cast<void**>(&CompilInfo)
	)
		)
	{
		// 뭔가 코드가 이상함.
		GameEngineDebug::MsgBoxError("쉐이더 컴파일 정보를 얻어오지 못했습니다.");
		return;
	}

	D3D11_SHADER_DESC Info;

	CompilInfo->GetDesc(&Info);

	std::string Name = "";
	int PrevIndex = 0;

	// [포지션]
	// [유비]
	// [노말]
	for (unsigned int i = 0; i < Info.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC Input = { 0, };
		CompilInfo->GetInputParameterDesc(i, &Input);

		DXGI_FORMAT Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;

		// 타입이 정수인지 실수인지 부호가 있는지 없는지
		D3D_REGISTER_COMPONENT_TYPE Reg = Input.ComponentType;

		// float4
		// 1 1 1 1

		// float3
		// 0 1 1 1

		unsigned int ParameterSize = 0;

		switch (Input.Mask)
		{
			// 1개짜리
		case 1:
			ParameterSize = 4;
			switch (Reg)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				break;
			case D3D_REGISTER_COMPONENT_UINT32:
				// unsigned int형 정보라는 뜻
				Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				Format = DXGI_FORMAT::DXGI_FORMAT_R32_SINT;
				// int형 정보라는 뜻
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
				// float형 정보라는 뜻
				break;
			default:
				break;
			}
			break;
		case 3:
			ParameterSize = 8;
			switch (Reg)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				break;
			case D3D_REGISTER_COMPONENT_UINT32:
				// unsigned int형 정보라는 뜻
				Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_SINT;
				// int형 정보라는 뜻
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
				// float형 정보라는 뜻
				break;
			default:
				break;
			}
			break;
		case 7:
			ParameterSize = 12;
			switch (Reg)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				break;
			case D3D_REGISTER_COMPONENT_UINT32:
				// unsigned int형 정보라는 뜻
				Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_SINT;
				// int형 정보라는 뜻
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
				// float형 정보라는 뜻
				break;
			default:
				break;
			}
			break;
		case 15:
			ParameterSize = 16;
			switch (Reg)
			{
			case D3D_REGISTER_COMPONENT_UNKNOWN:
				break;
			case D3D_REGISTER_COMPONENT_UINT32:
				// unsigned int형 정보라는 뜻
				Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT;
				break;
			case D3D_REGISTER_COMPONENT_SINT32:
				Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_SINT;
				// int형 정보라는 뜻
				break;
			case D3D_REGISTER_COMPONENT_FLOAT32:
				Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
				// float형 정보라는 뜻
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		std::string NextName = Input.SemanticName;

		if (Name == "")
		{
			Name = NextName;
			++PrevIndex;
		}
		else
		{
			if (Name == NextName)
			{
				if (PrevIndex != Input.SemanticIndex)
				{
					GameEngineDebug::MsgBoxError("시맨틱의 인덱스 순서가 잘못되었습니다 오름차순이 아닙니다..");
					return;
				}

				++PrevIndex;
				// 저 순서가 어때야 합니까?
			}
			else
			{
				Name = NextName;
				PrevIndex = 0;
			}
		}

		// 이것도 안된다.
		//Postion0;
		//Postion3;

		if (DXGI_FORMAT::DXGI_FORMAT_UNKNOWN == Format)
		{
			GameEngineDebug::MsgBoxError("쉐이더 인풋 파라미터 타입이 정상적이지 않습니다.");
		}

		AddInputLayOut(Input.SemanticName, Input.SemanticIndex, ParameterSize, Format, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA);
	}

	CreateLayOut();
}

void GameEngineVertexShader::LayOutClear()
{
	if (nullptr != LayOut_)
	{
		LayOut_->Release();
	}

	LayOut_ = nullptr;
	InputLayoutDesc_.clear();
	SemanticName_.clear();
	LayOutOffset_ = 0;
}

void GameEngineVertexShader::InputLayOutSetting()
{
	if (nullptr == LayOut_)
	{
		GameEngineDebug::MsgBoxError("쉐이더 인풋 파라미터가 존재하지 않습니다.");
	}

	GameEngineDevice::GetInst().GetContext()->IASetInputLayout(LayOut_);
}

void GameEngineVertexShader::Setting()
{
	GameEngineDevice::GetInst().GetContext()->VSSetShader(Shader_, nullptr, 0);
}

void GameEngineVertexShader::SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting)
{
	GameEngineDevice::GetContext()->VSSetConstantBuffers(_Setting->SettingIndex_, 1, &_Setting->Res_->GetBuffer());
}

void GameEngineVertexShader::SetTexture(const GameEngineTextureSetting* _Setting)
{
	GameEngineDevice::GetContext()->VSSetShaderResources(_Setting->SettingIndex_, 1, _Setting->Res_->GetShaderResourcesView());
}

void GameEngineVertexShader::SetSampler(const GameEngineSamplerSetting* _Setting)
{
	GameEngineDevice::GetContext()->VSSetSamplers(_Setting->SettingIndex_, 1, _Setting->Res_->GetSamplerState());
}

void GameEngineVertexShader::SetStructuredBuffer(const GameEngineStructuredBufferSetting* _Setting) 
{
	GameEngineDevice::GetContext()->VSSetShaderResources(_Setting->SettingIndex_, 1, _Setting->Res_->GetShaderResourcesView());
}


void GameEngineVertexShader::ReSetConstantBuffers(const GameEngineConstantBufferSetting* _Setting)
{
	static ID3D11Buffer* const ReSetting[16] = { nullptr };

	// 한번에 여러개 세팅가능합니다.
	GameEngineDevice::GetContext()->VSSetConstantBuffers(_Setting->SettingIndex_, 1, ReSetting);
}

void GameEngineVertexShader::ReSetTexture(const GameEngineTextureSetting* _Setting)
{
	static ID3D11ShaderResourceView* ReSetting[16] = { nullptr };

	GameEngineDevice::GetContext()->VSSetShaderResources(_Setting->SettingIndex_, 1, ReSetting);
}

void GameEngineVertexShader::ReSetSampler(const GameEngineSamplerSetting* _Setting)
{
	static ID3D11SamplerState* const ReSetting[16] = { nullptr };

	GameEngineDevice::GetContext()->VSSetSamplers(_Setting->SettingIndex_, 1, ReSetting);
}

void GameEngineVertexShader::ReSetStructuredBuffer(const GameEngineStructuredBufferSetting* _Setting)
{
	static ID3D11ShaderResourceView* ReSetting[16] = { nullptr };

	GameEngineDevice::GetContext()->VSSetShaderResources(_Setting->SettingIndex_, 1, ReSetting);
}

void GameEngineVertexShader::LayOutReset()
{
	SemanticName_.clear();
	InputLayoutDesc_.clear();
}