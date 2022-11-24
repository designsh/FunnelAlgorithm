#include "PreCompile.h"
#include "FloorMap.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngine/GameEngineCollision.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "NaviCell.h"
#include "Yuki.h"
#include "Wolf.h"

FloorMap* FloorMap::ptrFloorMap = nullptr;

GameEngineFBXMesh* FloorMap::GetFloorMapMesh()
{
	if (nullptr == NaviMeshRenderer_)
	{
		return nullptr;
	}

	return NaviMeshRenderer_->GetMesh();
}

GameEngineFBXRenderer* FloorMap::GetFloorMapRenderer()
{
	return NaviMeshRenderer_;
}

NaviCell* FloorMap::GetNaviCellInfo(const float4& _Vertex0, const float4& _Vertex1, const float4& _Vertex2)
{
	int CellInfoSize = static_cast<int>(NavigationCellInfos_.size());
	for (int CellNumber = 0; CellNumber < CellInfoSize; ++CellNumber)
	{
		if(nullptr != NavigationCellInfos_[CellNumber]->CompareVertices(_Vertex0, _Vertex1, _Vertex2))
		{
			return NavigationCellInfos_[CellNumber];
		}
	}

	return nullptr;
}

NaviCell* FloorMap::SearchCurrentPosToNaviCell(const float4& _Position)
{
	// 위치좌표를 수신받아 해당 좌표가 포함된 삼각형(셀)을 찾아내서 반환
	for (int CellNum = 0; CellNum < static_cast<int>(NavigationCellInfos_.size()); ++CellNum)
	{
		if (true == NavigationCellInfos_[CellNum]->CheckPointisIncludedIntheTriangle(_Position))
		{
			return NavigationCellInfos_[CellNum];
		}
	}

	return nullptr;
}

bool FloorMap::MoveFacePath(const float4& _StartPos, const float4& _EndPos, NaviCell* _StartCell, NaviCell* _TargetCell, std::list<float4>& _MovePath)
{
	if (nullptr == PathFinder_)
	{
		PathFinder_ = new PathFinder();
	}

	// 경로 탐색 시작
	// 1. A*로 최단거리 경로를 얻어낸 후
	// 2. 해당 경로를 최적화작업(Funnel Algorithm)으로 최적의 경로를 반환
	_MovePath = PathFinder_->SearchMovePath(_StartPos, _EndPos, _StartCell, _TargetCell, static_cast<int>(NavigationCellInfos_.size()) * 300);
	if (false == _MovePath.empty())
	{
		return true;
	}

	return false;
}

void FloorMap::Start()
{
	// FBX Files 경로 지정
	GameEngineDirectory Directory;
	Directory.MoveParent("FunnelAlgorithm");
	Directory.MoveChild("Resources");
	Directory.MoveChild("FBX");
	Directory.MoveChild("YSJ");

	//=============================================== NaviMesh Load & Create NaviCell Info
	std::string NaviMeshName = "Bg_NaviMesh.fbx";

	// Mesh Load
	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(Directory.PathToPlusFileName(NaviMeshName)))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName(NaviMeshName));
		Mesh->CreateRenderingBuffer();
	}

	// Create Renderer
	NaviMeshRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	NaviMeshRenderer_->SetFBXMesh(NaviMeshName, "TextureDeferredLight");

	for (UINT i = 0; i < NaviMeshRenderer_->GetRenderSetCount(); i++)
	{
		NaviMeshRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Green.png");
		NaviMeshRenderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerNone");
		//NaviMeshRenderer_->GetRenderSet(i).PipeLine_->SetRasterizer("EngineBaseRasterizerWireFrame");
	}

	// 네비게이션 셀정보 생성
	CreateAllNaviCellInfo();

	// NavigationCellInfos_의 랜덤한 삼각형을 선택하여 플레이어 위치좌표(랜덤한 삼각형의 무게중심) 셋팅
	if (nullptr != Yuki::MainPlayer)
	{
		GameEngineRandom Random;
		int RandomFace = Random.RandomInt(0, static_cast<int>(NavigationCellInfos_.size()) - 1);
		Yuki::MainPlayer->Initialize(NavigationCellInfos_[RandomFace], NavigationCellInfos_[RandomFace]->GetCenterToGravity());
	}

	// NavigationCellInfos_의 랜덤한 삼각형을 선택하여 몬스터 위치좌표(랜덤한 삼각형의 무게중심) 셋팅
	if (nullptr != Wolf::MainWolf)
	{
		GameEngineRandom Random;
		int RandomFace = Random.RandomInt(0, static_cast<int>(NavigationCellInfos_.size()) - 1);
		Wolf::MainWolf->Initialize(NavigationCellInfos_[RandomFace], NavigationCellInfos_[RandomFace]->GetCenterToGravity());
	}

//=============================================== NaviColMesh Load
	std::string NaviColMeshName = "NaviCol.fbx";

	if (nullptr == GameEngineFBXMeshManager::GetInst().Find(Directory.PathToPlusFileName(NaviColMeshName)))
	{
		GameEngineFBXMesh* Mesh = GameEngineFBXMeshManager::GetInst().Load(Directory.PathToPlusFileName(NaviColMeshName));
		Mesh->CreateRenderingBuffer();
	}

	// Create Renderer
	NaviColMeshRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	NaviColMeshRenderer_->SetFBXMesh(NaviColMeshName, "TextureDeferredLight");

	for (UINT i = 0; i < NaviColMeshRenderer_->GetRenderSetCount(); i++)
	{
		NaviColMeshRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Red.png");
	}
}

void FloorMap::CreateAllNaviCellInfo()
{
	// 해당 네비게이션 메쉬의 정보를 Get
	std::vector<FbxExMeshInfo>& AllMeshInfo = NaviMeshRenderer_->GetMesh()->GetMeshInfos();
	std::vector<FbxMeshSet>& AllMeshMap = NaviMeshRenderer_->GetMesh()->GetAllMeshMap();

	// 해당 Navigation FBX File이 가지는 모든 메쉬를 탐색
	for (int MeshNumber = 0; MeshNumber < static_cast<int>(AllMeshInfo.size()); ++MeshNumber)
	{
		for (int MaterialNumber = 0; MaterialNumber < AllMeshInfo[MeshNumber].MaterialNum; ++MaterialNumber)
		{
			// 3개의 정점을 연결하는 인덱스목록이 존재 -> 면(삼각형) 1개를 이룸
			for (int FaceNumber = 0; FaceNumber < AllMeshInfo[MeshNumber].FaceNum; ++FaceNumber)
			{
				std::vector<GameEngineVertex> VertexList;
				std::vector<UINT> IndexList;
				for (int VertexNumber = 0; VertexNumber < 3; ++VertexNumber)
				{
					UINT VertexIndex = AllMeshMap[MeshNumber].Indexs[0][MaterialNumber][FaceNumber * 3 + VertexNumber];
					GameEngineVertex Vertex = AllMeshMap[MeshNumber].Vertexs[VertexIndex];
					VertexList.push_back(Vertex);
					IndexList.push_back(VertexIndex);
				}

				NaviCell* NewCellInfo = new NaviCell();
				NewCellInfo->CreateNavigationCellInfo(static_cast<int>(NavigationCellInfos_.size()), MeshNumber, FaceNumber, VertexList, IndexList);
				NavigationCellInfos_.push_back(NewCellInfo);
			}
		}
	}

	// 모든 면을 검사하여 인접한 면을 찾아내서 정보에 저장
	FindAdjacentFaces();
}

void FloorMap::FindAdjacentFaces()
{
	int CellInfoSize = static_cast<int>(NavigationCellInfos_.size());
	for (int CurCellNumber = 0; CurCellNumber < CellInfoSize; ++CurCellNumber)
	{
		for (int CompareCellNumber = 0; CompareCellNumber < CellInfoSize; ++CompareCellNumber)
		{
			// 단, 현재 기준면과 상대의 기준면이 동일한 면이라면 무시
			if (CurCellNumber == CompareCellNumber)
			{
				continue;
			}

			// 기준면의 인접한 면 탐색
			NavigationCellInfos_[CurCellNumber]->SearchAdjacentTriangles(NavigationCellInfos_[CompareCellNumber]);
		}
	}
}

void FloorMap::Update(float _DeltaTime)
{
}

FloorMap::FloorMap()
	: NaviMeshRenderer_(nullptr)
	, NaviColMeshRenderer_(nullptr)
	, PathFinder_(nullptr)
{
}

FloorMap::~FloorMap()
{
	for (int CellInfoNum = 0; CellInfoNum < static_cast<int>(NavigationCellInfos_.size()); ++CellInfoNum)
	{
		delete NavigationCellInfos_[CellInfoNum];
		NavigationCellInfos_[CellInfoNum] = nullptr;
	}
	NavigationCellInfos_.clear();

	if (nullptr != PathFinder_)
	{
		delete PathFinder_;
		PathFinder_ = nullptr;
	}
}
