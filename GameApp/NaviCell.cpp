#include "Precompile.h"
#include "NaviCell.h"

void NaviCell::CreateNavigationCellInfo(int _Index, int _MeshIndex, int _FaceIndex, std::vector<GameEngineVertex> _VertexList, std::vector<UINT> _IndexList)
{
	// �⺻���� ����
	Index_ = _Index;
	MeshIndex_ = _MeshIndex;
	FaceIndex_ = _FaceIndex;
	VertexList_ = _VertexList;
	IndexList_ = _IndexList;

	// ���Ź��� 3���� �������� �����߽� ���
	CenterOfGravityCalculation();
}

void NaviCell::SearchAdjacentTriangles(NaviCell* _CompareNaviCell)
{
	// 2���� ������ �����ϴ� ���� ������ ������ �Ǵ�
	int SharedVerticesCount = 0;
	int ThisVertexSize = static_cast<int>(VertexList_.size());
	int CompareVertexSize = static_cast<int>(_CompareNaviCell->VertexList_.size());
	for (int ThisVertex = 0; ThisVertex < ThisVertexSize; ++ThisVertex)
	{
		for (int CompareVertex = 0; CompareVertex < CompareVertexSize; ++CompareVertex)
		{
			if (VertexList_[ThisVertex].POSITION == _CompareNaviCell->VertexList_[CompareVertex].POSITION)
			{
				++SharedVerticesCount;
				break;
			}
		}

		// 2���� ������ �����Ҷ� ������ ������ �Ǵ�
		if (2 == SharedVerticesCount)
		{
			AdjacentTriangles_.push_back(_CompareNaviCell);
			break;
		}
	}
}

NaviCell* NaviCell::CompareVertices(const float4& _V0, const float4& _V1, const float4& _V2)
{
	std::vector<float4> CompareVertexList;
	CompareVertexList.push_back(_V0);
	CompareVertexList.push_back(_V1);
	CompareVertexList.push_back(_V2);

	// ������ ���ؽ��� ��� �����ϰ� �ִٸ� �� �ڽ��� ��ȯ
	int ThisVertexSize = static_cast<int>(VertexList_.size());
	int SameVertexCnt = 0;
	for (int ThisVertexNumber = 0; ThisVertexNumber < ThisVertexSize; ++ThisVertexNumber)
	{
		for (int CompareVertexNumber = 0; CompareVertexNumber < static_cast<int>(CompareVertexList.size()); ++CompareVertexNumber)
		{
			if (VertexList_[ThisVertexNumber].POSITION == CompareVertexList[CompareVertexNumber])
			{
				++SameVertexCnt;
				break;
			}
		}
	}

	if (3 == SameVertexCnt)
	{
		return this;
	}

	return nullptr;
}

bool NaviCell::CheckPointisIncludedIntheTriangle(const float4& _Position)
{
	// �ش� �ﰢ���� ���ο� ����(_Position)�� �����ϸ� true ��ȯ
	// Inside�� Zero�ΰ�� �𼭸��� ���� ����
	float4 Inside = DirectX::Internal::PointOnPlaneInsideTriangle(_Position.DirectVector, VertexList_[0].POSITION.DirectVector, VertexList_[1].POSITION.DirectVector, VertexList_[2].POSITION.DirectVector);
	if (Inside != float4(0.0f, 0.0f, 0.0f, 0.0f))
	{
		return true;
	}

	return false;
}

std::vector<GameEngineVertex> NaviCell::SearchShareVertex(NaviCell* _ShareCell)
{
	std::vector<GameEngineVertex> ReturnVertex;

	int VertexListSize = static_cast<int>(VertexList_.size());
	for (int i = 0; i < VertexListSize; ++i)
	{
		for (int j = 0; j < static_cast<int>(_ShareCell->VertexList_.size()); ++j)
		{
			if (VertexList_[i].POSITION == _ShareCell->VertexList_[j].POSITION)
			{
				ReturnVertex.push_back(VertexList_[i]);
				break;
			}
		}
	}

	if (2 == ReturnVertex.size())
	{
		return ReturnVertex;
	}

	return std::vector<GameEngineVertex>();
}

void NaviCell::CenterOfGravityCalculation()
{
	float4 Vertex0 = VertexList_[0].POSITION;
	float4 Vertex1 = VertexList_[1].POSITION;
	float4 Vertex2 = VertexList_[2].POSITION;

	//CenterOfGravity_ = Vertex0;
	CenterOfGravity_.x = (Vertex0.x + Vertex1.x + Vertex2.x) / 3.0f;
	CenterOfGravity_.y = (Vertex0.y + Vertex1.y + Vertex2.y) / 3.0f;
	CenterOfGravity_.z = (Vertex0.z + Vertex1.z + Vertex2.z) / 3.0f;
	CenterOfGravity_.w = 1.0f;

	// 220819 SJH �ּ�ó��
	//CenterOfGravity_ = DirectX::XMVectorBaryCentric(Vertex0.DirectVector, Vertex1.DirectVector, Vertex2.DirectVector, 1.0f, 2.0f);
}

void NaviCell::StandingOntheCellCheck()
{
}

NaviCell::NaviCell()
	: Index_(-1)
	, MeshIndex_(-1)
	, FaceIndex_(-1)
	, CenterOfGravity_(float4::ZERO)
{
}

NaviCell::~NaviCell()
{
}
