#include "PreCompile.h"
#include "GameEngineShader.h"
#include "GameEngineConstantBufferManager.h"
#include "GameEngineResourcesManager.h"
#include <GameEngineBase/GameEngineFile.h>

void GameEngineShader::AutoCompile(GameEngineFile& ShaderFile)
{
	ShaderFile.Open("rt");

	std::string FileName = ShaderFile.GetFileNameWithOutExtension();
	std::string AllCode = ShaderFile.GetString();

	if (std::string::npos != AllCode.find(FileName + "_VS"))
	{
		GameEngineVertexShader* Ptr = GameEngineVertexShaderManager::GetInst().Load(FileName + "_VS", ShaderFile.GetFullPath(), FileName + "_VS");
	}

	if (std::string::npos != AllCode.find(FileName + "_PS"))
	{
		GameEnginePixelShader* Ptr = GameEnginePixelShaderManager::GetInst().Load(FileName + "_PS", ShaderFile.GetFullPath(), FileName + "_PS");

		int Count = Ptr->GetOutPutSize("SV_Target");

		if (Count > 1 && AllCode.find("DeferredOutPut"))
		{
			Ptr->IsDeferred_ = true;
		}

		

	}
}

GameEngineShader::GameEngineShader(ShaderType _Type)
	: VersionHigh_(5)
	, VersionLow_(0)
	, Type_(_Type)
{
}

GameEngineShader::~GameEngineShader() 
{
}


void GameEngineShader::SetVersion(UINT _VersionHigh, UINT _VersionLow)
{
	VersionHigh_ = _VersionHigh;
	VersionLow_ = _VersionLow;
}

void GameEngineShader::CreateVersion(const std::string& _ShaderType)
{
	Version_ = "";
	Version_ += _ShaderType + "_";
	Version_ += std::to_string(VersionHigh_);
	Version_ += "_";
	Version_ += std::to_string(VersionLow_);
}

void GameEngineShader::SetCode(const std::string& _Code)
{
	Code_ = _Code;
}

void GameEngineShader::SetEntryPoint(const std::string& _EntryPoint)
{
	EntryPoint_ = _EntryPoint;
}

void GameEngineShader::ResCheck() 
{

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

	D3D11_SHADER_INPUT_BIND_DESC ResInfo;

	for (unsigned int i = 0; i < Info.BoundResources; i++)
	{
		CompilInfo->GetResourceBindingDesc(i, &ResInfo);

		//LPCSTR                      Name;           // Name of the resource
		//D3D_SHADER_INPUT_TYPE       Type;           // Type of resource (e.g. texture, cbuffer, etc.)
		//UINT                        BindPoint;      // Starting bind point
		//UINT                        BindCount;      // Number of contiguous bind points (for arrays)
		//UINT                        uFlags;         // Input binding flags
		//D3D_RESOURCE_RETURN_TYPE    ReturnType;     // Return type (if texture) 
		//D3D_SRV_DIMENSION           Dimension;      // Dimension (if texture) // 3차원 텍스처
		//UINT                        NumSamples;     // Number of samples (0 if not MS texture)

		std::string Name = GameEngineString::toupper(ResInfo.Name);
		unsigned int BindPoint = ResInfo.BindPoint;
		D3D_SHADER_INPUT_TYPE Type = ResInfo.Type;

		switch (Type)
		{
		case D3D_SIT_CBUFFER:
		{
			ID3D11ShaderReflectionConstantBuffer* Buffer = CompilInfo->GetConstantBufferByName(ResInfo.Name);

			D3D11_SHADER_BUFFER_DESC BufferDesc;
			Buffer->GetDesc(&BufferDesc);

			GameEngineConstantBuffer* NewBuffer = GameEngineConstantBufferManager::GetInst().CreateAndFind(Name, BufferDesc, Buffer);

			if (BufferDesc.Size != NewBuffer->GetBufferSize())
			{
				GameEngineDebug::MsgBoxError("구조가 다른 상수버퍼가 존재합니다.");
				return;
			}

			GameEngineConstantBufferSetting Setting;
			Setting.SetName(Name);
			Setting.SettingIndex_ = ResInfo.BindPoint;
			Setting.Res_ = NewBuffer;

			ConstantBuffers_.insert(std::make_pair(Name, Setting));
			break;
		}
		case D3D_SIT_SAMPLER:
		{
			
			if (nullptr == GameEngineSamplerManager::GetInst().Find(Name))
			{
				D3D11_SAMPLER_DESC Smp_Decs = {};

				memset(&Smp_Decs, 0, sizeof(D3D11_SAMPLER_DESC));

				// 뭉개라.

				std::string CheckName = GameEngineString::toupper(Name);

				Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

				if (std::string::npos != CheckName.find("POINT"))
				{
					Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
				}
				else if (std::string::npos != CheckName.find("LINEAR"))
				{
					Smp_Decs.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				}

				Smp_Decs.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
				Smp_Decs.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
				Smp_Decs.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

				Smp_Decs.MipLODBias = 0.0f;
				Smp_Decs.MaxAnisotropy = 1;
				Smp_Decs.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
				Smp_Decs.MinLOD = -FLT_MAX;
				Smp_Decs.MaxLOD = FLT_MAX;
				// Smp_Decs.BorderColor;
				// Smp_Decs.MaxAnisotropy;
				GameEngineSamplerManager::GetInst().Create(Name, Smp_Decs);
			}
			
			GameEngineSampler* NewRes = GameEngineSamplerManager::GetInst().Find(Name);

			GameEngineSamplerSetting Setting;
			Setting.SetName(Name);
			Setting.SettingIndex_ = ResInfo.BindPoint;
			Setting.Res_ = NewRes;

			Samplers_.insert(std::make_pair(Name, Setting));
			break;
		}

		case D3D_SIT_TEXTURE:
		{
			GameEngineTextureSetting Setting;
			Setting.SetName(Name);
			Setting.SettingIndex_ = ResInfo.BindPoint;
			Setting.Res_ = GameEngineTextureManager::GetInst().Find("NotSettingError.png");

			Textures_.insert(std::make_pair(Name, Setting));
			break;
		}
		case D3D_SIT_STRUCTURED:
		{
			// 이름으로 구분할수 있나요 없나요?
			// GameEngineStructuredBuffer* NewBuffer = new GameEngineStructuredBuffer(); // GameEngineStructuredBufferManager::GetInst().CreateAndFind(Name, BufferDesc, Buffer);

			GameEngineStructuredBufferSetting Setting;
			Setting.SetName(Name);
			Setting.SettingIndex_ = ResInfo.BindPoint;
			Setting.Res_ = nullptr;

			StructuredBuffers_.insert(std::make_pair(Name, Setting));
			break;
		}
		default:
			GameEngineDebug::MsgBoxError("처리하지 못하는 타입의 쉐이더 리소스가 발견되었습니다");
			break;
		}
	}

	for (unsigned int i = 0; i < Info.OutputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC Desc;
		CompilInfo->GetOutputParameterDesc(i, &Desc);

	 	std::string Name = GameEngineString::toupper(Desc.SemanticName);

		OutPutMap[Name].push_back(Desc);
	}

}