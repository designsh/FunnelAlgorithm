#pragma once
#include "GameEnginePath.h"
// �з� :
// �뵵 :
// ���� :

class GameEngineFile;
class GameEngineDirectory : public GameEnginePath
{
public:
	GameEngineDirectory(); // default constructer ����Ʈ ������
	GameEngineDirectory(const std::string& Path); // default constructer ����Ʈ ������
	~GameEngineDirectory(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	GameEngineDirectory(const GameEngineDirectory& _other); // default Copy constructer ����Ʈ ���������
	GameEngineDirectory(GameEngineDirectory&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	GameEngineDirectory& operator=(const GameEngineDirectory& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineDirectory& operator=(const GameEngineDirectory&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	std::string DirectoryName();

public:		//member Func
	// C: D:���� Ȯ��
	bool IsRoot();
	// �θ������� �̵�
	void MoveParent();

	// �θ������� _DirName�� ���� ������ �̵�
	bool MoveParent(const std::string& _DirName);

	// �ڽ� ������ _DirName���� �̵�
	bool MoveChild(const std::string& _DirName);

	GameEngineDirectory& operator/(const std::string& _DirName)
	{
		MoveChild(_DirName);
		return *this;
	}

public:
	// �� ������ + �����̸� ��θ� �������ִ� �Լ�
	std::string PathToPlusFileName(const std::string& _FileName);

	// * �� �ǹ�
	// A* ��� �̸�
	// ABVBBB ��
	// CAFDSA �ȵ�
	// .
	// * ��� Ȯ����
	std::vector<GameEngineFile> GetAllFile(const std::string& _filter = "*");

	std::vector<GameEngineDirectory> GetAllDirectory(const std::string& _filter = "");

	std::vector<GameEngineDirectory> GetAllDirectoryRecursive(const std::string& _filter = "");
};

