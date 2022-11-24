#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineDebugRenderData.h"
#include "GameEngineLightComponent.h"
#include "DeferredCalLightEffect.h"
#include "DeferredMerge.h"
#include "Enums.h"

// 투영 타입
enum class ProjectionMode
{
	Perspective,
	Orthographic
};

// 분류 : 카메라 컴포넌트
// 용도 : 
// 설명 : 
class GameEngineRendererBase;
class CameraComponent : public GameEngineTransformComponent
{
	friend class CameraActor;
	friend class GameEngineLevel;
	friend class GameEngineRenderer;

private:	// member Var
	ProjectionMode				ProjectionMode_;			// 
	float						FovAngleY_;					// 
	float4						CamSize_;					// 
	float						NearZ_;						// 
	float						FarZ_;						// 
	float						ZoomValue;

private:
	LightsData LightData_;
	std::list<GameEngineLightComponent*> Lights_;
	std::map<int, std::list<GameEngineRendererBase*>> RendererList_;

private:
	int DebugRenderCount_;
	float ZoomValue_;
	std::vector<GameEngineDebugRenderData> DebugVector_;
	GameEngineRenderTarget* CameraBufferTarget_;

	GameEngineRenderTarget* CameraForwardTarget_;

	GameEngineRenderTarget* CameraDeferredGBufferTarget;
	GameEngineRenderTarget* CameraDeferredLightTarget;
	GameEngineRenderTarget* CameraDeferredTarget_;

public:
	CameraComponent();
	~CameraComponent();

protected:		// delete constructer
	CameraComponent(const CameraComponent& _other) = delete;
	CameraComponent(CameraComponent&& _other) noexcept = delete;

private:		//delete operator
	CameraComponent& operator=(const CameraComponent& _other) = delete;
	CameraComponent& operator=(const CameraComponent&& _other) = delete;

private:
	void ClearCameraTarget();
	void CameraTransformUpdate();
	void Render(float _DeltaTime);
	void DebugRender();
	void ReleaseRenderer();

	void NextLevelMoveRenderer(CameraComponent* _NextCamera, GameEngineActor* _Actor);

public:
	inline const LightsData& GetLightData() const
	{
		return LightData_;
	}

	inline GameEngineRenderTarget* GetCameraDeferredTarget()
	{
		return CameraDeferredTarget_;
	}

	inline GameEngineRenderTarget* GetCameraDeferredGBufferTarget()
	{
		return CameraDeferredGBufferTarget;
	}

	inline GameEngineRenderTarget* GetCameraDeferredLightTarget()
	{
		return CameraDeferredLightTarget;
	}

	inline GameEngineRenderTarget* GetCameraForwardTarget_()
	{
		return CameraForwardTarget_;
	}

	inline GameEngineRenderTarget* GetCameraRenderTarget()
	{
		return CameraBufferTarget_;
	}

	inline float GetZoomValue()
	{
		return ZoomValue_;
	}

	inline float GetFar() 
	{
		return FarZ_;
	}

	inline const LightsData& LightData() const
	{
		return LightData_;
	}

	inline void SetFov(float _fovAngle)
	{
		FovAngleY_ = _fovAngle;
	}

	DeferredCalLightEffect CalLightEffect;
	DeferredMerge DeferredMergeEffect;

public:
	void CameraZoomReset();
	void CameraZoomSetting(float _Value);

public:
	void SetProjectionMode(ProjectionMode _ProjectionMode);
	void PushRenderer(int _Order, GameEngineRendererBase* _Renderer);
	void PushLight(GameEngineLightComponent* _Light);

public:
	void PushDebugRender(GameEngineTransform* _Trans, CollisionType _Type, float4 _Color = float4::GREEN);
	void ChangeRendererGroup(int _Group, GameEngineRendererBase* _Renderer);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
};

