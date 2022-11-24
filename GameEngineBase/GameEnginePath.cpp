#include "PreCompile.h"
#include "GameEnginePath.h"


// Static Var
// Static Func

// constructer destructer
GameEnginePath::GameEnginePath()
	: path_("")
{
}

GameEnginePath::GameEnginePath(const std::filesystem::path& _path)
	: path_(_path)
{

}

GameEnginePath::~GameEnginePath()
{
}


GameEnginePath::GameEnginePath(const GameEnginePath& _other)
	: path_(_other.path_)
{

}

GameEnginePath::GameEnginePath(GameEnginePath&& _other) noexcept
{
}

//member Func

bool GameEnginePath::IsExist()
{
	return std::filesystem::exists(path_);
}

std::string GameEnginePath::GetFileName(const std::string& _Path)
{
	std::filesystem::path NewPath = _Path;
	return NewPath.filename().string();
}

std::string GameEnginePath::GetFullPath()
{
	return path_.string();
}

std::string GameEnginePath::GetFileNameWithOutExtension(const std::string& _Path)
{
	GameEnginePath Path = std::filesystem::path(_Path);
	return Path.GetFileNameWithOutExtension();
}


std::string GameEnginePath::GetFileName() 
{
	return path_.filename().string();
}

std::string GameEnginePath::GetFileNameWithOutExtension()
{
	std::string Ex = path_.extension().string();
	std::string FileName = path_.filename().string();
	return FileName.erase(FileName.size() - Ex.size(), Ex.size());
}

std::string GameEnginePath::GetExtension() 
{
	return path_.extension().string();
}

bool GameEnginePath::IsExist(std::string _Path)
{
	return 	std::filesystem::exists(_Path);
}