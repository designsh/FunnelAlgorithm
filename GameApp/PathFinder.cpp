#include "Precompile.h"
#include "PathFinder.h"

std::list<float4> PathFinder::SearchMovePath(const float4& _StartPos, const float4& _EndPos, NaviCell* _StartCell, NaviCell* _EndCell, int _Maximum)
{
	std::list<float4> ReturnPath;

	// �̵����ۼ��� �̵���ǥ���� ���ٸ� 
	if (_StartCell == _EndCell)
	{
		// ������ġ -> Ŭ������ ��� ������ ����
		ReturnPath.push_back(_StartPos);
		ReturnPath.push_back(_EndPos);
		return ReturnPath;
	}

	// 1. A*�� �̿��Ͽ� �ִܰ�θ� Ž���� ��
	std::list<NaviCell*> AStartPath;
	AStartPath = AStarMovePath(_StartCell, _EndCell, _Maximum);
	if (false == AStartPath.empty())
	{
		// 2. Funnel Algorithm�� �̿��Ͽ� �������� ����������� ��ȯ
		if (nullptr == Funnel_)
		{
			// ��ü ����
			Funnel_ = new Funnel();
		}

		ReturnPath = Funnel_->PathOptimization(_StartPos, _EndPos, AStartPath);
		if (false == ReturnPath.empty())
		{
			// ������� �����Ϸ�
			return ReturnPath;
		}
	}

	return std::list<float4>();
}

std::list<NaviCell*> PathFinder::AStarMovePath(NaviCell* _StartCell, NaviCell* _EndCell, int _Maximum)
{
	// �ʱ�ȭ
	PoolCount_ = 0;
	NodePool_.clear();
	NodePool_.resize(_Maximum);
	OpenList_.clear();
	OpenKeys_.clear();
	CloseList_.clear();
	CloseKeys_.clear();

	// ���۳�� ����
	AStarNode* StartNode = CreateNode(_StartCell, _EndCell);
	OpenList_.insert(std::make_pair(StartNode->TotalLen_, StartNode));
	OpenKeys_.insert(StartNode->CellInfo_->GetCellInfomationIndex());

	// ������� ��� �Ҹ�ñ��� �ݺ�
	while (0 != OpenList_.size())
	{
		// �޸� �ִ�ġ Ž���� �Ͼ�ٸ� ��θ� ã������
		if (PoolCount_ >= NodePool_.size())
		{
			return std::list<NaviCell*>();
		}

		// ��� Ž�� ����
		std::vector<NaviCell*> AdjacentTri;
		AdjacentTri.clear();

		// ���۳�� ���� �� �ش� ��忡 ������ �ﰢ������ ����
		AStarNode* FirstNode = OpenList_.begin()->second;
		AdjacentTri = FirstNode->CellInfo_->GetAdjacentTriangles();

		// ������� ��Ͽ��� �ش� ��带 ����
		OpenList_.erase(OpenList_.begin());
		OpenKeys_.erase(FirstNode->CellInfo_->GetCellInfomationIndex());

		// ������� ��Ͽ� �ش� ��� �߰�
		CloseList_.push_back(FirstNode);
		CloseKeys_.insert(FirstNode->CellInfo_->GetCellInfomationIndex());

		// �ش� ��忡 ������ ��尹����ŭ üũ
		int AdjacentTriCount = static_cast<int>(AdjacentTri.size());
		for (int Tri = 0; Tri < AdjacentTriCount; ++Tri)
		{
			// ������ �� Get
			NaviCell* CheckNode = AdjacentTri[Tri];

			// ��������Ͽ� �ش� ��尡 �����ϸ� Continue
			if (OpenKeys_.end() != OpenKeys_.find(CheckNode->GetCellInfomationIndex()))
			{
				continue;
			}

			// ��������Ͽ� �ش� ��尡 �����ϸ� Continue
			if (CloseKeys_.end() != CloseKeys_.find(CheckNode->GetCellInfomationIndex()))
			{
				continue;
			}

			// �������� ��� �ƴ϶�� �̵����� ���� �Ǵ�
			AStarNode* NewNode = CreateNode(CheckNode, _EndCell, FirstNode);
			if (nullptr != NewNode)
			{
				// ��� ������ �������̶�� 
				NewNode->Parent_ = FirstNode;

				// ��ǥ ��忡 �����ߴٸ� �̵���� ��ȯ
				if (_EndCell == NewNode->CellInfo_)
				{
					return CalReturn(NewNode);
				}

				// �ƴ϶�� ������Ͽ� �߰�
				OpenList_.insert(std::make_pair(NewNode->TotalLen_, NewNode));
				OpenKeys_.insert(NewNode->CellInfo_->GetCellInfomationIndex());
			}
		}
	}

	return std::list<NaviCell*>();
}

AStarNode* PathFinder::CreateNode(NaviCell* _StartCell, NaviCell* _EndCell, AStarNode* _ParentNode)
{
	// �ִ� ������Ƚ�� �Ѿ�� ��� �����Ұ�
	if (PoolCount_ >= NodePool_.size())
	{
		return nullptr;
	}

	// ��� ����
	AStarNode* NewNode = &NodePool_[PoolCount_++];
	NewNode->Reset();
	NewNode->CellInfo_ = _StartCell;
	NewNode->CalLen(_EndCell);

	return NewNode;
}

std::list<NaviCell*> PathFinder::CalReturn(AStarNode* _EndNode)
{
	// ��ǥ������ ��� ~ �������� ��带 ��ȸ�ϸ� ��� ����
	std::list<NaviCell*> ReturnList;

	AStarNode* FindNode = _EndNode;
	while (nullptr != FindNode->Parent_)
	{
		ReturnList.push_front(FindNode->CellInfo_);

		// �θ���� �̵�
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
