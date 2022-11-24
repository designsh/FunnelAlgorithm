#pragma once
#include "GameEngineRendererBase.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

class RenderSet 
{
public:
	GameEngineShaderResHelper* ShaderHelper;
	GameEngineRenderingPipeLine* PipeLine_;
	int Index;
	std::vector<float4x4> BoneData;
	bool isRender = true;
};

class FbxExAniData;
class GameEngineFBXRenderer;
class GameEngineFBXMesh;
class GameEngineFBXAnimation;
class FBXAnimation
{
public:
	GameEngineFBXMesh* Mesh;
	GameEngineFBXAnimation* Animation;
	FbxExAniData* PixAniData;
	GameEngineFBXRenderer* ParentRenderer;

	float CurFrameTime;
	float FrameTime;
	UINT CurFrame;
	UINT End;
	UINT Start;
	bool isLoop_;

	void Init(int _Index, bool _isLoop = true);
	void Update(float _DeltaTime);
	void ResetFrame();
};

// ���� : �ϳ��� ���� ������ ǥ���մϴ�.
class CameraComponent;
class GameEngineLevel;
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineRenderingPipeLine;
class GameEngineFBXRenderer : public GameEngineRendererBase
{
	friend FBXAnimation;

private:
	friend GameEngineLevel;
	friend CameraComponent;

public:
	// constrcuter destructer
	GameEngineFBXRenderer();
	~GameEngineFBXRenderer();

	// delete Function
	GameEngineFBXRenderer(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer(GameEngineFBXRenderer&& _Other) noexcept = delete;
	GameEngineFBXRenderer& operator=(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer& operator=(GameEngineFBXRenderer&& _Other) noexcept = delete;

	// virtual void SetRenderGroup(int _Order);

	// �̳༮�� ���⿡ ����ִ� Mesh�� fbx�Դϴ�.
	void SetFBXMesh(const std::string& _Value, std::string _PipeLine);

	void SetFBXMeshRenderSet(const std::string& _Value, std::string _PipeLine, int _MeshIndex);

	size_t GetRenderSetCount()
	{
		return RenderSets.size();
	}

	RenderSet& GetRenderSet(unsigned int _Index)
	{
		return RenderSets[_Index];
	}


	bool CheckIntersects(const float4& _Position, const float4& _Direction, float& _Distance);

	bool CheckMeshToPointCollision(const float4& _Position, const float4& _Direction, 
		const float _Range);

public:
	inline GameEngineFBXMesh* GetMesh()
	{
		return FBXMesh;
	}

	inline std::vector<std::string> GetAnimationNameList()
	{
		std::vector<std::string> ReturnNameList;

		for (auto AnimName : Animations)
		{
			ReturnNameList.push_back(AnimName.first);
		}

		return ReturnNameList;
	}

protected:
	void Start() override;

	void Render(float _DeltaTime, bool _IsDeferred) override;

private:
	std::vector<RenderSet> RenderSets;

	GameEngineFBXMesh* FBXMesh;

	void Update(float _DeltaTime) override;


/////////////////////////////// �ִϸ��̼�
	// FBXAnimation

public:
	void CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName, int _AnimationIndex = 0, bool _isLoop = true);
	void ChangeFBXAnimation(const std::string& _AnimationName, bool _bForceChange = false);

private:

	std::map<std::string, FBXAnimation*> Animations;
	FBXAnimation* CurAnimation;


};

