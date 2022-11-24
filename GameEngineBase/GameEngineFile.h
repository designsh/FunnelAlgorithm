#pragma once
#include "GameEnginePath.h"
#include "GameEngineMath.h"
#include <iostream>

// �з� :
// �뵵 :
// ���� :
class GameEngineFile : public GameEnginePath
{
private:
	FILE* fileHandle_;
	std::string OpenMode;

public:		
	GameEngineFile(); // default constructer ����Ʈ ������

	GameEngineFile(const std::filesystem::path& _Path); // default constructer ����Ʈ ������

	GameEngineFile(const std::string& _Path); // default constructer ����Ʈ ������

	GameEngineFile(const std::string& _Path, const std::string& _Mode); // default constructer ����Ʈ ������

	~GameEngineFile(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	GameEngineFile(const GameEngineFile& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineFile(GameEngineFile&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	GameEngineFile& operator=(const GameEngineFile& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineFile& operator=(const GameEngineFile&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
	void Open(const std::string& _Mode);
	void Close();

	uintmax_t GetFileSize();

	std::string FileName();

public:
	void Write(const void* _Data, size_t _Size);


	// ���ø����� ����� �����Ҷ��� ���Ƽ�
	// ���� ����ڽ��ϴ�.
	// ��¥ ũ�Ⱑ �Ⱥ��� �ڷ����鸸 �����Ұ̴ϴ�.
	// �ʿ��ϸ� ���� ���弼��.
	// short �����ϴ°� ����� 
	void Write(const std::string& _Data);
	void Write(const int& _Data);
	void Write(const float& _Data);
	void Write(const unsigned int& _Data);
	void Write(const double& _Data);
	void Write(const float4& _Data);
	void Write(const float4x4& _Data);

	template<typename DataType>
	void Write(const DataType& _Data)
	{
		_Data.Write(this);
	}

	template<typename DataType>
	void Write(const std::vector<DataType>& _Data)
	{
		int Size = static_cast<int>(_Data.size());
		Write(&Size, sizeof(int));


		//if (0 != Size)
		//{
		//	// Write(&_Data[0], Size * sizeof(DataType));
		//	Write(&_Data[0], Size * sizeof(DataType));
		//}
		//for (size_t i = 0; i < _Data.size(); i++)
		//{
		//	Write(&_Data[i], sizeof(_Data[i]));
		//}

		for (size_t i = 0; i < _Data.size(); i++)
		{
			Write(_Data[i]);
		}
	}

	template<typename DataType>
	void Write(const std::list<DataType>& _Data)
	{
		int Size = static_cast<int>(_Data.size());
		Write(&Size, sizeof(int));

		// ���ø����� �ٸ� ���ø� ����Ҷ��� ���� �� �𸣹Ƿ�
		// typename �ٿ���� �մϴ�.
		typename std::list<DataType>::iterator Start = _Data.begin();
		typename std::list<DataType>::iterator End = _Data.end();

		for (; Start != End; ++Start)
		{
			Write(*Start);
		}
	}

	template<typename KeyType, typename DataType>
	void Write(const std::map<KeyType, DataType>& _Data)
	{
		int Size = static_cast<int>(_Data.size());
		Write(&Size, sizeof(int));

		typename std::map<KeyType, DataType>::iterator Start = _Data.begin();
		typename std::map<KeyType, DataType>::iterator End = _Data.end();

		for (; Start != End; ++Start)
		{
			Write(Start->first);
			Write(Start->second);
		}
	}



	// ũ�Ⱑ ���ϴ� �ڷ����� �������� �����ϴ�.
	void Write(const size_t& _Data) = delete;

	template<typename DataType>
	void Read(DataType& _Data)
	{
		_Data.Read(this);
	}

	void Read(void* _Buffer, size_t _BufferSize, size_t _Size);
	void Read(std::string& _Data);
	void Read(int& _Data);
	void Read(float& _Data);
	void Read(unsigned int& _Data);
	void Read(double& _Data);
	void Read(float4& _Data);
	void Read(float4x4& _Data);

	template<typename DataType>
	void Read(std::vector<DataType>& _Data)
	{
		int Size = 0;
		Read(&Size, sizeof(int), sizeof(int));

		for (int i = 0; i < Size; i++)
		{
			DataType Buffer;
			Read(Buffer);
			_Data.push_back(Buffer);
		}
	}

	template<typename DataType>
	void Read(std::list<DataType>& _Data)
	{
		int Size = 0;
		Read(&Size, sizeof(int), sizeof(int));

		for (int i = 0; i < Size; i++)
		{
			DataType Buffer;
			Read(Buffer);
			_Data.push_back(Buffer);
		}
	}

	template<typename KeyType, typename DataType>
	void Read(std::map<KeyType, DataType>& _Data)
	{
		int Size = 0;
		Read(&Size, sizeof(int), sizeof(int));

		for (int i = 0; i < Size; i++)
		{
			KeyType KeyBuffer;
			Read(KeyBuffer);
			DataType ValueBuffer;
			Read(ValueBuffer);

			_Data.insert(std::map<KeyType, DataType>::value_type(KeyBuffer, ValueBuffer));
		}
	}

	std::string GetString();
};

