#pragma once
#include "GameEngineActor.h"
#include "CameraComponent.h"

class CameraComponent;
// Ό³Έν :
class CameraActor : public GameEngineActor
{
public:
	// constrcuter destructer
	CameraActor();
	~CameraActor();

	// delete Function
	CameraActor(const CameraActor& _Other) = delete;
	CameraActor(CameraActor&& _Other) noexcept = delete;
	CameraActor& operator=(const CameraActor& _Other) = delete;
	CameraActor& operator=(CameraActor&& _Other) noexcept = delete;

	CameraComponent* GetCamera() 
	{
		return Camera_;
	}

	float4x4 GetViewMatrix();
	float4x4 GetProjectionMatrix();

	bool IsFreeCameraMode();

	void FreeCameraModeSwitch();
	void FreeCameraModeOn();
	void FreeCameraModeOff();


protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	GameEngineTransform OriginTransform;
	float FreeCameraSpeed_;
	float RotSpeed_;
	bool IsFreeCameraMode_;
	ProjectionMode PrevCamMode_;
	CameraComponent* Camera_;



};

