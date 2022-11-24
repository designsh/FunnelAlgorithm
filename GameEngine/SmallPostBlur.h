#pragma once
#include "GameEnginePostProcessRender.h"

struct SmallBlurData 
{
	int FilterStartX; // -2
	int FilterStartY; // -2
	int FilterEndX; // 2
	int FilterEndY; // 2
	int FilterCount;
	float FilterSum;
	float ImagePixelUVX;
	float ImagePixelUVY;
	float FilterPixelX;
	float FilterPixelY;
	float Temp0;
	float Temp1;
};


// Ό³Έν :
class SmallPostBlur : public GameEnginePostProcessRender
{
public:
	// constrcuter destructer
	SmallPostBlur();
	~SmallPostBlur();

	// delete Function
	SmallPostBlur(const SmallPostBlur& _Other) = delete;
	SmallPostBlur(SmallPostBlur&& _Other) noexcept = delete;
	SmallPostBlur& operator=(const SmallPostBlur& _Other) = delete;
	SmallPostBlur& operator=(SmallPostBlur&& _Other) noexcept = delete;
	void SetFilter(std::string _TextureName);

protected:
	void Effect(float _DeltaTime) override;
	void Initialize() override;


private:
	SmallBlurData  Data;

	GameEngineTexture* Filter;


};

