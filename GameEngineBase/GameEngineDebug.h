#pragma once
#include <string>
// #include <WinSock2.h>

// �з� : �����
// �뵵 : �޸𸮰��� �޼��� ��� �α�
// ���� : ��� ����� ���� ����� ���⿡ ��Ƴ��´�.
class GameEngineDebug
{ 
private:	// member Var

public:
	static void LeakCheckOn(int Check = -1);

protected:		
	GameEngineDebug(); // default constructer ����Ʈ ������
	~GameEngineDebug(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineDebug(const GameEngineDebug& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineDebug(GameEngineDebug&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineDebug& operator=(const GameEngineDebug& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineDebug& operator=(const GameEngineDebug&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

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
			// �ü���� �ذ� ������� �Ѵ�.
			// ������ ��������������
			// ������ �߻��ϴ� ���� ���� �����ϴ�.
			// ���� ��������� �����մϴ�.
			// ���� �ʰ��ϸ鼭 ������.
			LocalFree(message);
		}
	}

};

