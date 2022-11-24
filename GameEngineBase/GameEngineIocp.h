#pragma once
#include <Windows.h>
#include <process.h>

// ���� :
class GameEngineIocp
{
public:
	// constrcuter destructer
	GameEngineIocp()
		: IocpHandle(nullptr)
	{
		// CreateIoCompletionPort�� 2���� �뵵�� ���ȴ�.
		// ��������¿��� ����Ҽ� �ִ°͵� �ƴմϴ�.
		// �����带 �ڵ鸵
		// ���� ���������� �����带 �����ϰ� ������
		// ��������°� ���Ͽ����� ������� �ʰ�
		// ��������� == ��������̱� ������
		// ���� ������ ������ ����.
		// �ٵ� ������ ����� ������
		// �����̾߱⸦ �Ұž�.
		// ?????? �׷��Ը� ���� �ִ°� �ƴϴ�.
		// ���� �������� ������ ��� ������ �� �𸣰ڰ�.
		// �����带 ����ϴٰ�.
		// iocp��°� �˰Եư� �����带 �����ϴµ��� ����Ҽ� �ִٴ°� �˰ԵǾ���.

		// ������ �����带 �ھ� ������ŭ ����̴ϴ�.
		// IocpHandle = CreateIoCompletionPort(IocpHandle, NULL, NULL, ThreadCount);
	}	
	
	~GameEngineIocp() {}



	bool Start(int ThreadCount = 0)
	{
		IocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, ThreadCount);
		if (nullptr == IocpHandle)
		{
			// ġ������ ����.
			return false;
		}

		return true;
	}

	// ������ �ȿ��� ������ ������� �մϴ�.
	BOOL WaitforWork(DWORD& lpNumberOfBytesTransferred, ULONG_PTR& lpCompletionKey, LPOVERLAPPED& lpOverlapped, DWORD dwMilliseconds = INFINITE)
	{
		// ��������� == �������

		//HANDLE CompletionPort, �� �����带 �����ϴ� IOCP
		//LPDWORD lpNumberOfBytesTransferred, ������� Ȥ�� ��������� �ϰ� �ִٸ� 
		//                                    ���� ��Ʃ�� ���� �������� xxx����Ʈ ��ŭ �޾Ҿ��.
		//                                    ���� ���� 3�Ⱑ¥�� ������ 200�ް� �о����

		//PULONG_PTR lpCompletionKey, // ���� �ѱ涧 8����Ʈ ������ �ѱ�� �ִ�.

		//LPOVERLAPPED* lpOverlapped, // ��������� �Ҷ� ������� ���� �񵿱� ����� ����.
									  // �̳༮�� ��¥ ���� �Ⱦ��̴ϴ�.

		//DWORD dwMilliseconds      // �̰� �󸶳� ��ٸ��ų�.
		//                          INFINITY�� ������ ��¥ ���� ���������� ������ ��ٸ�.

		return GetQueuedCompletionStatus(IocpHandle, &lpNumberOfBytesTransferred, &lpCompletionKey, &lpOverlapped, dwMilliseconds);

		// � �����忡�� �� �Լ��� �����Ű��
		// ���� ���涧���� .
	}

	// WorkParameter �ƹ��ų� ��������� �������ش�.
	// -1�� ����.
	BOOL Makework(DWORD _WorkParameter, void* _Ptr)
	{
		return PostQueuedCompletionStatus(IocpHandle, _WorkParameter, reinterpret_cast<ULONG_PTR>(_Ptr), nullptr);
	}


protected:

private:
	HANDLE IocpHandle;
};

