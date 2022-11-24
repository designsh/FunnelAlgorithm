#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : ���콺
// �뵵 : UI ��ȣ �� �÷��̾� �̵�
// ���� : 
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

