#pragma once
#include <GameEngine/GameEngineCore.h>

// �з� : 
// �뵵 : 
// ���� : 
class UserGame : public GameEngineCore
{
public:
	// ���� �ɸ��� �ʴ´�.
	// ���� �ɸ��µ� ������ ���� �ɷ��� ������.
	// �ü���� �������ִ� cpu ������ ���� �Ŵµ� �̰� ������.
	// 64����Ʈ
	static std::atomic<int> LoadingFolder;

	static bool IsServer_;

private:	// member Var
public:
	UserGame(); // default constructer ����Ʈ ������
	~UserGame(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	UserGame(const UserGame& _other) = delete; // default Copy constructer ����Ʈ ���������
	UserGame(UserGame&& _other) noexcept; // 

private:		//delete operator
	UserGame& operator=(const UserGame& _other) = delete; // default Copy operator ����Ʈ ���� ������
	UserGame& operator=(const UserGame&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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

