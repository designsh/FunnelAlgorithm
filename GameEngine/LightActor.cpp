#include "PreCompile.h"
#include "LightActor.h"
#include "GameEngineLightComponent.h"

LightActor::LightActor() 
{
}

LightActor::~LightActor() 
{
}


void LightActor::Start() 
{
	Light_ = CreateTransformComponent<GameEngineLightComponent>(GetTransform());
}

void LightActor::Update(float _DeltaTime) 
{

}