#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"
#include "ThirdParty/Include/DirectXTex/DirectXTex.h"

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineTexture;
class GameEngineFolderTexture : public GameEngineObjectNameBase
{
public:
	void Load(const std::string& _Path);

	GameEngineFolderTexture();
	~GameEngineFolderTexture();

	GameEngineTexture* GetTextureIndex(int _Index);

	inline int GetTextureCount() 
	{
		return static_cast<int>(AllTexture_.size());
	}

protected:		// delete constructer

private:		//delete operator
	GameEngineFolderTexture(const GameEngineFolderTexture& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineFolderTexture(GameEngineFolderTexture&& _other) noexcept = delete; // default RValue Copy constructer ����Ʈ RValue ���������
	GameEngineFolderTexture& operator=(const GameEngineFolderTexture& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineFolderTexture& operator=(const GameEngineFolderTexture&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

	std::vector<GameEngineTexture*> AllTexture_;
};

