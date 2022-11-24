#pragma once
// 분류 :
// 용도 :
// 설명 :
class GameEnginePath
{
protected:	// member Var
	std::filesystem::path path_;

public:		
	GameEnginePath(); // default constructer 디폴트 생성자
	GameEnginePath(const std::filesystem::path& _path);
	~GameEnginePath(); // default destructer 디폴트 소멸자

public:		// delete constructer
	GameEnginePath(const GameEnginePath& _other); // default Copy constructer 디폴트 복사생성자
	GameEnginePath(GameEnginePath&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	GameEnginePath& operator=(const GameEnginePath& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEnginePath& operator=(const GameEnginePath&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	static std::string GetFileName(const std::string& _Path);
	std::string GetFileName();

	static bool IsExist(std::string Path);


	static std::string GetFileNameWithOutExtension(const std::string& _Path);
	std::string GetFileNameWithOutExtension();

	std::string GetExtension();


public:		//member Func
	// 유요한 경로인가 아닌가를 판단하는 함수.
	bool IsExist();

	std::string GetFullPath();

};

