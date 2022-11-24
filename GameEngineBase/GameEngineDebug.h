#pragma once
#include <string>
// #include <WinSock2.h>

// 분류 : 디버거
// 용도 : 메모리관리 메세지 출력 로그
// 설명 : 모든 디버깅 관련 기능은 여기에 모아놓는다.
class GameEngineDebug
{ 
private:	// member Var

public:
	static void LeakCheckOn(int Check = -1);

protected:		
	GameEngineDebug(); // default constructer 디폴트 생성자
	~GameEngineDebug(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineDebug(const GameEngineDebug& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineDebug(GameEngineDebug&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineDebug& operator=(const GameEngineDebug& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineDebug& operator=(const GameEngineDebug&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
	static void MsgBoxError(const std::string& _Text);
	static void AssertFalse();
	static void OutPutDebugString(const std::string& _Text);
	static void MsgBox(const std::string& _Ptr);
	static void GetLastErrorPrint()
	{
		DWORD error = GetLastError();
		char* message = nullptr;

		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			nullptr,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&message,
			0,
			nullptr);

		if (nullptr != message)
		{
			std::string Text = "Code : ";
			Text += std::to_string(error);
			Text += " Message : ";
			Text += message;

			MsgBoxError(Text);
			// 운영체제가 준건 해제헤야 한다.
			// 서버면 더더더더더더욱
			// 실행중 발생하는 릭이 가장 위험하다.
			// 힙이 비대해지기 시작합니다.
			// 램을 초과하면서 터진다.
			LocalFree(message);
		}
	}

};

