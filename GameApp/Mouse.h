#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 마우스
// 용도 : UI 상호 및 플레이어 이동
// 설명 : 
class GameEngineUIRenderer;
class GameEngineCollision;
class Ray;
class Mouse : public GameEngineActor
{
public:
	inline Ray* GetCurRay()
	{
		return Ray_;
	}

protected:
private:

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	Mouse();
	~Mouse();

protected:
	Mouse(const Mouse& _other) = delete;
	Mouse(Mouse&& _other) noexcept = delete;

private:
	Mouse& operator=(const Mouse& _other) = delete;
	Mouse& operator=(const Mouse&& _other) = delete;

public:
	static Mouse* MainMouse;

protected:

private:
	GameEngineUIRenderer* Renderer_;
	GameEngineCollision* Collider_;

private:
	Ray* Ray_;
};

