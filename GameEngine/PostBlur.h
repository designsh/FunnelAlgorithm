#pragma once
#include "GameEnginePostProcessRender.h"

struct BlurData 
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
class PostBlur : public GameEnginePostProcessRender
{
public:
	// constrcuter destructer
	PostBlur();
	~PostBlur();

	// delete Function
	PostBlur(const PostBlur& _Other) = delete;
	PostBlur(PostBlur&& _Other) noexcept = delete;
	PostBlur& operator=(const PostBlur& _Other) = delete;
	PostBlur& operator=(PostBlur&& _Other) noexcept = delete;
	void SetFilter(std::string _TextureName);

protected:
	void Effect(float _DeltaTime) override;
	void Initialize() override;


private:
	BlurData Data;

	GameEngineTexture* Filter;


};

