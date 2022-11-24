#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>

#pragma warning(push)
#pragma warning(disable:26495)
#include <fbxsdk.h>
#pragma comment(lib, "zlib-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "libfbxsdk-md.lib")
// #pragma comment(lib, "libfbxsdk.lib")
#pragma warning(pop)


class GameEngineFBX : public GameEngineObjectNameBase
{
public:
	GameEngineFBX();
	~GameEngineFBX();
	GameEngineFBX(const GameEngineFBX& _Other) = delete;
	GameEngineFBX(GameEngineFBX&& _Other) noexcept = delete;
	GameEngineFBX& operator=(const GameEngineFBX& _Other) = delete;
	GameEngineFBX& operator=(GameEngineFBX&& _Other) noexcept = delete;

public:
	void RecursiveAllNode(
		std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack,
		std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack,
		int _ParentReturn);

	void Reset();

public:
	inline fbxsdk::FbxNode* GetRootNode()
	{
		return RootNode;
	}

protected:
	bool CreateFBXSystemInitialize(const std::string& _Path);
	void FBXConvertScene();
	void RecursiveAllNode(fbxsdk::FbxNode* _Node,
		std::function<int(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _InitCallBack,
		std::function<void(fbxsdk::FbxNodeAttribute::EType, fbxsdk::FbxNode*, int)> _EndCallBack,
		int _ParentReturn);


	// 시작하려면 이녀석이 필요하다.
	fbxsdk::FbxManager*		Manager;
	fbxsdk::FbxIOSettings*	IOSetting;
	fbxsdk::FbxImporter*	Importer;
	fbxsdk::FbxScene*		Scene;
	fbxsdk::FbxNode*		RootNode;

	fbxsdk::FbxAMatrix	ConvertMatrix;
	fbxsdk::FbxAMatrix JointMatrix;

	fbxsdk::FbxVector4 AxisVector;

	// 기하 컨버트
	float4x4 FbxMatTofloat4x4(const fbxsdk::FbxAMatrix& _BaseTrans);
	fbxsdk::FbxAMatrix float4x4ToFbxAMatrix(const float4x4& _MATRIX);
	float4 FbxVecTofloat4(const fbxsdk::FbxVector4& _BaseVector);
	float4 FbxVecToTransform(const fbxsdk::FbxVector4& _BaseVector);
	float4 FbxQuaternionTofloat4(const fbxsdk::FbxQuaternion& _BaseVector);


private:

};

