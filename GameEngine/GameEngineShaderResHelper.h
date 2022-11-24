#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "GameEngineRenderingSettingData.h"
#include "GameEngine/GameEngineShader.h"



class GameEngineRenderingPipeLine;
class GameEngineTexture;
class GameEngineSampler;
class GameEngineConstantBuffer;
class GameEngineStructuredBuffer;
// ���� :
class GameEngineShaderResHelper
{
	friend class CameraComponent;
	friend class  GameEngineRenderer;
	/////////////////////////////////// ���̴� ���� �����͵�

public:
	// constrcuter destructer
	GameEngineShaderResHelper();
	~GameEngineShaderResHelper();

	// delete Function
	//GameEngineShaderResHelper(const GameEngineShaderResHelper& _Other) = delete;
	// GameEngineShaderResHelper(GameEngineShaderResHelper&& _Other) noexcept = delete;
	//GameEngineShaderResHelper& operator=(const GameEngineShaderResHelper& _Other) = delete;
	//GameEngineShaderResHelper& operator=(GameEngineShaderResHelper&& _Other) noexcept = delete;

	void ShaderResourcesCheck(GameEngineRenderingPipeLine* _Shader);

	void ShaderResourcesCheck(GameEngineShader* _Shader);


	// �� �ּҰ��� ���������� ��� �������ִ� ��带 ���Ѵ�.
	// �ѹ� �������ָ� �־��� �ּҰ��� ��ġ�� �ı��Ǳ��������� �����е��� �Ű澵 �ʿ䰡 ����.

	bool IsConstantBuffer(const std::string& _SettingName);

	bool IsStructuredBuffer(const std::string& _SettingName);

	bool IsTextureSetting(const std::string& _SettingName);

	template<typename T>
	void SettingStructuredBufferLink(const std::string& _SettingName, const T& _Data)
	{
		std::string UpperName = GameEngineString::toupper(_SettingName);

		std::map<std::string, GameEngineStructuredBufferSetting>::iterator FindIter = AllStructuredBufferData_.find(UpperName);

		if (FindIter == AllStructuredBufferData_.end())
		{
			GameEngineDebug::MsgBoxError("�������� �ʴ� ����ȭ ���۸� �����Ϸ��� �߽��ϴ�." + UpperName);
			return;
		}

		GameEngineStructuredBufferSetting& SettingData = FindIter->second;

		SettingData.Mode_ = SettingMode::Link;
		SettingData.SettingDataSize_ = sizeof(_Data);
		SettingData.SettingData_ = reinterpret_cast<const char*>(&_Data);
	}

	void SettingStructuredBufferLink(const std::string& _SettingName, const void* _Data, size_t _Size)
	{
		std::string UpperName = GameEngineString::toupper(_SettingName);

		std::map<std::string, GameEngineStructuredBufferSetting>::iterator FindIter = AllStructuredBufferData_.find(UpperName);

		if (FindIter == AllStructuredBufferData_.end())
		{
			GameEngineDebug::MsgBoxError("�������� �ʴ� ����ȭ ���۸� �����Ϸ��� �߽��ϴ�." + UpperName);
			return;
		}

		GameEngineStructuredBufferSetting& SettingData = FindIter->second;

		SettingData.Mode_ = SettingMode::Link;
		SettingData.SettingDataSize_ = _Size;
		SettingData.SettingData_ = reinterpret_cast<const char*>(_Data);
	}

	template<typename T>
	void SettingConstantBufferLink(const std::string& _SettingName, const T& _Data) 
	{
		std::string UpperName = GameEngineString::toupper(_SettingName);

		std::map<std::string, GameEngineConstantBufferSetting>::iterator FindIter = AllConstantBufferData_.find(UpperName);

		if (FindIter == AllConstantBufferData_.end())
		{
			GameEngineDebug::MsgBoxError("�������� �ʴ� ������۸� �����Ϸ��� �߽��ϴ�." + UpperName);
			return;
		}

		GameEngineConstantBufferSetting& SettingData = FindIter->second;

		SettingData.Mode_ = SettingMode::Link;
		SettingData.SettingDataSize_ = sizeof(_Data);
		SettingData.SettingData_ = reinterpret_cast<const char*>(&_Data);
	}

	// �����Ҵ��� �ؼ� ���� ũ���� ���纻�� ����� ���ϴ�.
	// �ѹ� �������ָ� �������� ������ ������ �����Ǵ°̴ϴ�.
	template<typename T>
	void SettingConstantBufferSet(const std::string& _SettingName, const T& _Data)
	{
		std::string UpperName = GameEngineString::toupper(_SettingName);


		std::map<std::string, GameEngineConstantBufferSetting>::iterator FindIter = AllConstantBufferData_.find(UpperName);

		if (FindIter == AllConstantBufferData_.end())
		{
			GameEngineDebug::MsgBoxError("�������� �ʴ� ������۸� �����Ϸ��� �߽��ϴ�." + UpperName);
			return;
		}

		GameEngineConstantBufferSetting& SettingData = FindIter->second;

		// ���ʼ����̶�� �����ش�.
		//if (SettingData->Mode_ == SettingMode::MAX)
		//{
		//	SettingData->Clear();
		//}

		SettingData.Mode_ = SettingMode::Set;
		SettingData.SettingDataSize_ = sizeof(_Data);

		if (nullptr == SettingData.NewData_)
		{
			SettingData.NewData_ = new char[sizeof(_Data)];
		}

		memcpy_s(SettingData.NewData_, sizeof(_Data), &_Data, sizeof(_Data));
		SettingData.SettingData_ = SettingData.NewData_;
	}

	void SettingTexture(const std::string& _SettingName, const std::string& _ImageName);

	void SettingTexture(const std::string& _SettingName, GameEngineTexture* _Texture);


	void SettingSampler(const std::string& _SettingName, const std::string& _Name);

	void SettingSampler(const std::string& _SettingName, GameEngineSampler* _Value);

	void SettingStructuredBufferSetting(const std::string& _SettingName, GameEngineStructuredBuffer* _Buffer);

	void ReSet();
	void Setting();

protected:

private:
	std::map<std::string, GameEngineConstantBufferSetting> AllConstantBufferData_;
	std::map<std::string, GameEngineTextureSetting> AllTextureData_;
	std::map<std::string, GameEngineSamplerSetting> AllSamplerData_;
	std::map<std::string, GameEngineStructuredBufferSetting> AllStructuredBufferData_;

	void Clear();

};

