#pragma once
class GameEngineString
{
private:
	GameEngineString(); // default constructer 디폴트 생성자
	virtual ~GameEngineString(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineString(const GameEngineString& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineString(GameEngineString&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineString& operator=(const GameEngineString& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineString& operator=(const GameEngineString&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	static std::string toupper(const std::string& _Text);

	static void AnsiToUnicode(const std::string& Text, std::wstring& _Out);
	static void UniCodeToUTF8(const std::wstring& _Text, std::string& _Out);
	static bool UniCodeToAnsi(const std::wstring& _Unicode, std::string& _Ansi);

	static void AnsiToUTF8(const std::string& Text, std::string& _Out);

	static bool UTF8ToAnsi(const std::string& _UTF8, std::string& _Ansi);
	static std::string UTF8ToAnsiReturn(const std::string& _UTF8);
	static bool UTF8ToUniCode(const std::string& _UTF8, std::wstring& _Unicode);

	static std::string AnsiToUTF8Return(const std::string& Text);
	static std::wstring StringToWStringReturn(const std::string& Text);


};

