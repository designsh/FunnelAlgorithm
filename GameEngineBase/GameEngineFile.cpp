 #include "PreCompile.h"
#include "GameEngineFile.h"
#include "GameEngineDebug.h"


// Static Var
// Static Func

// constructer destructer
GameEngineFile::GameEngineFile()
	: OpenMode(""), fileHandle_(nullptr)
{
}

GameEngineFile::GameEngineFile(const std::filesystem::path& _Path) 
	: GameEnginePath(_Path), fileHandle_(nullptr)
{

}

GameEngineFile::GameEngineFile(const std::string& _Path)
	: fileHandle_(nullptr)
{
	path_ = _Path;
	if (false == IsExist())
	{
		GameEngineDebug::AssertFalse();
	}
}

GameEngineFile::GameEngineFile(const std::string& _Path, const std::string& _Mode)
{
	path_ = _Path;

	// 오픈하고 익시트
	Open(_Mode);

	if (false == IsExist())
	{
		GameEngineDebug::AssertFalse();
	}
}

GameEngineFile::~GameEngineFile()
{
	Close();
}

GameEngineFile::GameEngineFile(GameEngineFile&& _other) noexcept
	: GameEnginePath(_other), fileHandle_(nullptr)
{
}

//member Func

void GameEngineFile::Open(const std::string& _Mode) 
{
	OpenMode = _Mode;
	fopen_s(&fileHandle_, path_.string().c_str(), _Mode.c_str());
	if (nullptr == fileHandle_)
	{
		GameEngineDebug::AssertFalse();
	}
}

void GameEngineFile::Close() 
{
	if (nullptr != fileHandle_)
	{
		fclose(fileHandle_);
		fileHandle_ = nullptr;
	}
}

void GameEngineFile::Write(const void* _Data, size_t _Size) 
{
	// 쓰기용으로 파일을 열지 않고
	// 왜 쓰려고 하는냐에 대한 예외처리입니다.
	if (OpenMode[0] != 'w')
	{
		GameEngineDebug::AssertFalse();
		return;
	}

	// 열지도 않은 파일을 사용하려고 했다.
	if (nullptr == fileHandle_)
	{
		GameEngineDebug::AssertFalse();
		return;
	}

	fwrite(_Data, _Size, 1, fileHandle_);
}

void GameEngineFile::Read(void* _Buffer, size_t _BufferSize, size_t _DataSize)
{
	// 쓰기용으로 파일을 열지 않고
// 왜 쓰려고 하는냐에 대한 예외처리입니다.
	if (OpenMode[0] != 'r')
	{
		GameEngineDebug::AssertFalse();
		return;
	}

	// 열지도 않은 파일을 사용하려고 했다.
	if (nullptr == fileHandle_)
	{
		GameEngineDebug::AssertFalse();
		return;
	}

	fread_s(_Buffer, _BufferSize, _DataSize, 1, fileHandle_);

}

void GameEngineFile::Write(const std::string& _Data)
{
	// 크기를 저장해줘야 합니다.
	// string은? 크기가 일정한 데이터를 가지고 있나요?
	int Size = static_cast<int>(_Data.size());
	Write(&Size, sizeof(int));
	Write(_Data.c_str(), _Data.size());
}

void GameEngineFile::Write(const int& _Data)
{
	Write(&_Data, sizeof(int));
}

void GameEngineFile::Write(const unsigned int& _Data)
{
	Write(&_Data, sizeof(unsigned int));
}

void GameEngineFile::Write(const double& _Data)
{
	Write(&_Data, sizeof(double));
}


void GameEngineFile::Write(const float4& _Data)
{
	Write(&_Data, sizeof(float4));
}

void GameEngineFile::Write(const float4x4& _Data)
{
	Write(&_Data, sizeof(float4x4));
}

void GameEngineFile::Write(const float& _Data)
{
	Write(&_Data, sizeof(float));
}

void GameEngineFile::Read(std::string& _Data)
{
	int Size = 0;
	Read(&Size, sizeof(int), sizeof(int));
	_Data.resize(Size);
	Read(&_Data[0], Size, Size);
}
void GameEngineFile::Read(int& _Data)
{
	Read(&_Data, sizeof(int), sizeof(int));
}

void GameEngineFile::Read(double& _Data)
{
	Read(&_Data, sizeof(double), sizeof(double));
}


void GameEngineFile::Read(unsigned int& _Data)
{
	Read(&_Data, sizeof(unsigned int), sizeof(unsigned int));
}

void GameEngineFile::Read(float& _Data)
{
	Read(&_Data, sizeof(float), sizeof(float));
}

void GameEngineFile::Read(float4& _Data)
{
	Read(&_Data, sizeof(float4), sizeof(float4));
}

void GameEngineFile::Read(float4x4& _Data)
{
	Read(&_Data, sizeof(float4x4), sizeof(float4x4));
}


uintmax_t GameEngineFile::GetFileSize() 
{
	return std::filesystem::file_size(path_);
}

std::string GameEngineFile::GetString() 
{
	std::string AllString = std::string();
	AllString.resize(GetFileSize());
	Read(&AllString[0], AllString.size(), AllString.size());
	return AllString;
}

std::string GameEngineFile::FileName() 
{
	return path_.filename().string();
}