#pragma once
#include <GameEngine/GameEngineCore.h>

// 분류 : 
// 용도 : 
// 설명 : 
class UserGame : public GameEngineCore
{
public:
	// 락이 걸리지 않는다.
	// 락이 걸리는데 가벼운 락이 걸려서 빠르다.
	// 운영체제가 관리해주는 cpu 수준의 락을 거는데 이게 빠르다.
	// 64바이트
	static std::atomic<int> LoadingFolder;

	static bool IsServer_;

private:	// member Var
public:
	UserGame(); // default constructer 디폴트 생성자
	~UserGame(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	UserGame(const UserGame& _other) = delete; // default Copy constructer 디폴트 복사생성자
	UserGame(UserGame&& _other) noexcept; // 

private:		//delete operator
	UserGame& operator=(const UserGame& _other) = delete; // default Copy operator 디폴트 대입 연산자
	UserGame& operator=(const UserGame&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	virtual void Initialize() override;
	virtual void ResourcesLoad() override;
	virtual void Release() override;

	// void TextureLoad();

	virtual float4 StartWindowPos() {
		return { 0, 0 };
	}
	virtual float4 StartWindowSize() {
		return { 1280, 720 };
	}
};

