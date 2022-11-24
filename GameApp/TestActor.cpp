#include "PreCompile.h"
#include "TestActor.h"

#include <GameEngine/GameEngineRenderer.h>
#include <GameEngine/GameEngineCollision.h>

void TestActor::Start()
{
	BoxRenderer_ = CreateTransformComponent<GameEngineRenderer>();
	BoxRenderer_->SetRenderingPipeLine("ColorSphere");
	BoxRenderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", float4(1.0f, 0.0f, 1.0f, 1.0f));

	BoxCollider_ = CreateTransformComponent<GameEngineCollision>();
}

void TestActor::Update(float _deltaTime)
{
#ifdef _DEBUG
	if (nullptr != BoxCollider_)
	{
		GetLevel()->PushDebugRender(BoxCollider_->GetTransform(), CollisionType::Sphere3D);
	}
#endif // _DEBUG

}

TestActor::TestActor()
	: BoxRenderer_(nullptr)
	, BoxCollider_(nullptr)
{
}

TestActor::~TestActor()
{
}

void TestActor::SetResultColor(const float4& _Color)
{
	if (nullptr != BoxRenderer_)
	{
		BoxRenderer_->ShaderHelper.SettingConstantBufferLink("ResultColor", _Color);
	}
}
