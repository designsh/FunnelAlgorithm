#pragma once
#include "Funnel.h"

// A* ��� ����
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
		// �θ��尡 ���°�� : ���۳���϶�
		if (nullptr == Parent_)
		{
			PathLen_ = 0.0f;
		}
		// �θ��尡 �����ϴ°�� : Ž������϶�
		else
		{
			// �����߽ɱ��� �Ÿ� ����
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

// �뵵 : A* �� �ִܰ�θ� ���� �� Funnel Algorithm�� ���� ������θ� ��ȯ
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

private: // ������
	int PoolCount_;										// �ִ� ������Ƚ��(����)
	std::vector<AStarNode> NodePool_;				// ���������

private: // ���������
	std::multimap<float, AStarNode*> OpenList_;		// (�Ÿ�, �������) ���
	std::set<int> OpenKeys_;							// ������� Key

private: // ���������
	std::list<AStarNode*> CloseList_;				// ���������
	std::set<int> CloseKeys_;							// ������� Key

private:
	Funnel* Funnel_;								// �򶧱�˰���ü
};

