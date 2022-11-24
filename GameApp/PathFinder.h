#pragma once
#include "Funnel.h"

// A* 노드 정보
class NaviCell;
class AStarNode
{
public:
	NaviCell* CellInfo_;
	AStarNode* Parent_;
	float PathLen_;
	float EndLen_;
	float TotalLen_;

public:
	void CalLen(NaviCell* _CellInfo)
	{
		// 부모노드가 없는경우 : 시작노드일때
		if (nullptr == Parent_)
		{
			PathLen_ = 0.0f;
		}
		// 부모노드가 존재하는경우 : 탐색노드일때
		else
		{
			// 무게중심기준 거리 측정
			PathLen_ = Parent_->PathLen_ + (Parent_->CellInfo_->GetCenterToGravity() - CellInfo_->GetCenterToGravity()).Len3D();
		}

		EndLen_ = (_CellInfo->GetCenterToGravity() - CellInfo_->GetCenterToGravity()).Len3D();
		TotalLen_ = PathLen_ + EndLen_;
	}

	void Reset()
	{
		memset(this, 0x00, sizeof(*this));
	}
};

// 용도 : A* 로 최단경로를 구한 후 Funnel Algorithm을 통해 최적경로를 반환
class Funnel;
class PathFinder
{
public:
	std::list<float4> SearchMovePath(const float4& _StartPos, const float4& _EndPos, NaviCell* _StartCell, NaviCell* _EndCell, int _Maximum = 10000);

protected:

private: // A*
	std::list<NaviCell*> AStarMovePath(NaviCell* _StartCell, NaviCell* _EndCell, int _Maximum = 1000);
	AStarNode* CreateNode(NaviCell* _StartCell, NaviCell* _EndCell, AStarNode* _ParentNode = nullptr);
	std::list<NaviCell*> CalReturn(AStarNode* _EndNode);

public:
	PathFinder();
	~PathFinder();

protected:		// delete constructer
	PathFinder(const PathFinder& _other) = delete;
	PathFinder(PathFinder&& _other) noexcept = delete;

private:		//delete operator
	PathFinder& operator=(const PathFinder& _other) = delete;
	PathFinder& operator=(const PathFinder&& _other) = delete;

public:

protected:

private: // 노드생성
	int PoolCount_;										// 최대 노드생성횟수(고정)
	std::vector<AStarNode> NodePool_;				// 생성노드목록

private: // 열린노드목록
	std::multimap<float, AStarNode*> OpenList_;		// (거리, 열린노드) 목록
	std::set<int> OpenKeys_;							// 열린노드 Key

private: // 닫힌노드목록
	std::list<AStarNode*> CloseList_;				// 닫힌노드목록
	std::set<int> CloseKeys_;							// 닫힌노드 Key

private:
	Funnel* Funnel_;								// 깔때기알고리즘객체
};

