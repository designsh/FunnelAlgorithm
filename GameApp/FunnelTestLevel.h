#pragma once
#include <GameEngine/GameEngineLevel.h>

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineFBXWindow;
class SKySphereActor;
class FloorMap;
class Mouse;
class Yuki;
class TestActor;
class Wolf;
class FunnelTestLevel : public GameEngineLevel
{
public:

protected:

private:
	void LevelStart() override;
	void LevelUpdate(float _DeltaTime) override;
	void LevelChangeEndEvent(GameEngineLevel* _NextLevel) override;
	void LevelChangeStartEvent(GameEngineLevel* _PrevLevel) override;

private:
	void CreateActorLevel();

public:
	FunnelTestLevel();
	~FunnelTestLevel();

protected:
	FunnelTestLevel(const FunnelTestLevel& _other) = delete;
	FunnelTestLevel(FunnelTestLevel&& _other) noexcept = delete;

private:
	FunnelTestLevel& operator=(const FunnelTestLevel& _other) = delete;
	FunnelTestLevel& operator=(const FunnelTestLevel&& _other) = delete;

public:
protected:
private:
	GameEngineFBXWindow* FBXWindow;

private:
	SKySphereActor* Background_;
	FloorMap* FloorMap_;
	Mouse* InGameMouse_;
	Yuki* Yuki_;

private:
	Wolf* Wolf_;

private: // ���콺 ��ŷ�� ������Ʈ
	TestActor* TestBox_;
};

