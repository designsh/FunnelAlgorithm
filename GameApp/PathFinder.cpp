#include "Precompile.h"
#include "PathFinder.h"

std::list<float4> PathFinder::SearchMovePath(const float4& _StartPos, const float4& _EndPos, NaviCell* _StartCell, NaviCell* _EndCell, int _Maximum)
{
	std::list<float4> ReturnPath;

	// 이동시작셀과 이동목표셀이 같다면 
	if (_StartCell == _EndCell)
	{
		// 시작위치 -> 클릭지점 경로 생성후 리턴
		ReturnPath.push_back(_StartPos);
		ReturnPath.push_back(_EndPos);
		return ReturnPath;
	}

	// 1. A*를 이용하여 최단경로를 탐색한 후
	std::list<NaviCell*> AStartPath;
	AStartPath = AStarMovePath(_StartCell, _EndCell, _Maximum);
	if (false == AStartPath.empty())
	{
		// 2. Funnel Algorithm을 이용하여 가시적인 최적직선경로 반환
		if (nullptr == Funnel_)
		{
			// 객체 생성
			Funnel_ = new Funnel();
		}

		ReturnPath = Funnel_->PathOptimization(_StartPos, _EndPos, AStartPath);
		if (false == ReturnPath.empty())
		{
			// 최적경로 생성완료
			return ReturnPath;
		}
	}

	return std::list<float4>();
}

std::list<NaviCell*> PathFinder::AStarMovePath(NaviCell* _StartCell, NaviCell* _EndCell, int _Maximum)
{
	// 초기화
	PoolCount_ = 0;
	NodePool_.clear();
	NodePool_.resize(_Maximum);
	OpenList_.clear();
	OpenKeys_.clear();
	CloseList_.clear();
	CloseKeys_.clear();

	// 시작노드 생성
	AStarNode* StartNode = CreateNode(_StartCell, _EndCell);
	OpenList_.insert(std::make_pair(StartNode->TotalLen_, StartNode));
	OpenKeys_.insert(StartNode->CellInfo_->GetCellInfomationIndex());

	// 열린노드 모두 소멸시까지 반복
	while (0 != OpenList_.size())
	{
		// 메모리 최대치 탐색이 일어났다면 경로를 찾지못함
		if (PoolCount_ >= NodePool_.size())
		{
			return std::list<NaviCell*>();
		}

		// 경로 탐색 시작
		std::vector<NaviCell*> AdjacentTri;
		AdjacentTri.clear();

		// 시작노드 설정 및 해당 노드에 인접한 삼각형정보 셋팅
		AStarNode* FirstNode = OpenList_.begin()->second;
		AdjacentTri = FirstNode->CellInfo_->GetAdjacentTriangles();

		// 열린노드 목록에서 해당 노드를 삭제
		OpenList_.erase(OpenList_.begin());
		OpenKeys_.erase(FirstNode->CellInfo_->GetCellInfomationIndex());

		// 닫힌노드 목록에 해당 노드 추가
		CloseList_.push_back(FirstNode);
		CloseKeys_.insert(FirstNode->CellInfo_->GetCellInfomationIndex());

		// 해당 노드에 인접한 노드갯수만큼 체크
		int AdjacentTriCount = static_cast<int>(AdjacentTri.size());
		for (int Tri = 0; Tri < AdjacentTriCount; ++Tri)
		{
			// 인접한 셀 Get
			NaviCell* CheckNode = AdjacentTri[Tri];

			// 열린노드목록에 해당 노드가 존재하면 Continue
			if (OpenKeys_.end() != OpenKeys_.find(CheckNode->GetCellInfomationIndex()))
			{
				continue;
			}

			// 닫힌노드목록에 해당 노드가 존재하면 Continue
			if (CloseKeys_.end() != CloseKeys_.find(CheckNode->GetCellInfomationIndex()))
			{
				continue;
			}

			// 위조건이 모두 아니라면 이동가능 노드로 판단
			AStarNode* NewNode = CreateNode(CheckNode, _EndCell, FirstNode);
			if (nullptr != NewNode)
			{
				// 노드 생성이 정상적이라면 
				NewNode->Parent_ = FirstNode;

				// 목표 노드에 도착했다면 이동경로 반환
				if (_EndCell == NewNode->CellInfo_)
				{
					return CalReturn(NewNode);
				}

				// 아니라면 열린목록에 추가
				OpenList_.insert(std::make_pair(NewNode->TotalLen_, NewNode));
				OpenKeys_.insert(NewNode->CellInfo_->GetCellInfomationIndex());
			}
		}
	}

	return std::list<NaviCell*>();
}

AStarNode* PathFinder::CreateNode(NaviCell* _StartCell, NaviCell* _EndCell, AStarNode* _ParentNode)
{
	// 최대 노드생성횟수 넘어가면 노드 생성불가
	if (PoolCount_ >= NodePool_.size())
	{
		return nullptr;
	}

	// 노드 생성
	AStarNode* NewNode = &NodePool_[PoolCount_++];
	NewNode->Reset();
	NewNode->CellInfo_ = _StartCell;
	NewNode->CalLen(_EndCell);

	return NewNode;
}

std::list<NaviCell*> PathFinder::CalReturn(AStarNode* _EndNode)
{
	// 목표지점의 노드 ~ 시작지점 노드를 순회하며 경로 생성
	std::list<NaviCell*> ReturnList;

	AStarNode* FindNode = _EndNode;
	while (nullptr != FindNode->Parent_)
	{
		ReturnList.push_front(FindNode->CellInfo_);

		// 부모노드로 이동
		FindNode = FindNode->Parent_;
	}

	NodePool_.clear();

	return ReturnList;
}

PathFinder::PathFinder()
	: PoolCount_(0)
	, Funnel_(nullptr)
{
}

PathFinder::~PathFinder()
{
	if (nullptr != Funnel_)
	{
		delete Funnel_;
		Funnel_ = nullptr;
	}
}
