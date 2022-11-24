#pragma once
#include <GameEngine/GameEngineActor.h>

#include "PathFinder.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineCollision;
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class NaviCell;
class FloorMap : public GameEngineActor
{
public:
	static FloorMap* ptrFloorMap;

public:
	GameEngineFBXMesh* GetFloorMapMesh();
	GameEngineFBXRenderer* GetFloorMapRenderer();

public:
	NaviCell* GetNaviCellInfo(const float4& _Vertex0, const float4& _Vertex1, const float4& _Vertex2);

public:
	NaviCell* SearchCurrentPosToNaviCell(const float4& _Position);
	bool MoveFacePath(const float4& _StartPos, const float4& _EndPos, NaviCell* _StartCell, NaviCell* _TargetCell, std::list<float4>& _MovePath);

protected:

private:
	void CreateAllNaviCellInfo();
	void FindAdjacentFaces();

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	FloorMap();
	~FloorMap();

protected:
	FloorMap(const FloorMap& _other) = delete;
	FloorMap(FloorMap&& _other) noexcept = delete;

private:
	FloorMap& operator=(const FloorMap& _other) = delete;
	FloorMap& operator=(const FloorMap&& _other) = delete;

public:

protected:

private:
	GameEngineFBXRenderer* NaviMeshRenderer_;					// NaviMesh(실질적인 네비게이션)
	GameEngineFBXRenderer* NaviColMeshRenderer_;				// NaviMesh Col(벽 or ..)

private:
	std::vector<NaviCell*> NavigationCellInfos_;			// NavigationCellInfos_[FaceNumber]

private:
	PathFinder* PathFinder_;								// A* 경로탐색
};

