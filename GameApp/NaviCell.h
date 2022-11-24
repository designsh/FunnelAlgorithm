#pragma once
#include <GameEngine/EngineVertex.h>

// �з� : �ﰢ��(��)
// �뵵 : ��ã��˰��� ���̽�
// ���� : �޽��� �̷�� �ﰢ������ �� �Ѱ�
class NaviCell
{
public:
	inline int GetCellInfomationIndex() const
	{
		return Index_;
	}

	inline float4 GetCenterToGravity() const
	{
		return CenterOfGravity_;
	}

	inline std::vector<NaviCell*> GetAdjacentTriangles() const
	{
		return AdjacentTriangles_;
	}

public:
	void CreateNavigationCellInfo(int _Index, int _MeshIndex, int _FaceNumber, std::vector<GameEngineVertex> _VertexList, std::vector<UINT> _IndexList);
	void SearchAdjacentTriangles(NaviCell* _CompareNaviCell);

public:
	NaviCell* CompareVertices(const float4& _V0, const float4& _V1, const float4& _V2);
	bool CheckPointisIncludedIntheTriangle(const float4& _Position);

public:
	std::vector<GameEngineVertex> SearchShareVertex(NaviCell* _ShareCell);

protected:

private: // �� ������ ����
	void CenterOfGravityCalculation();

private: // ���� ���� �����ϴ� ���� Ž��(���ſ�)
	void StandingOntheCellCheck();

public:
	NaviCell();
	~NaviCell();

protected:
	NaviCell(const NaviCell& _other) = delete;
	NaviCell(NaviCell&& _other) noexcept = delete;

private:
	NaviCell& operator=(const NaviCell& _other) = delete;
	NaviCell& operator=(const NaviCell&& _other) = delete;

public:

protected:

private: // ���Ź޴� ����
	int Index_;											// ���� ��Ͽ� �ش��ϴ� Index(Ž����)
	int MeshIndex_;										// �ش� ���� �����ϴ� MeshIndex(Ž����)
	int FaceIndex_;										// �ش� ���� �ε���(Ž����)
	std::vector<GameEngineVertex> VertexList_;			// �ش� ���� �����ϴ� �������(3��)
	std::vector<UINT> IndexList_;						// �ش� ���� �����ϴ� �ε������

private: // �����߽� �� ������ ����
	float4 CenterOfGravity_;							// �ش� ��(�ﰢ��)�� �����߽�
	std::vector<NaviCell*> AdjacentTriangles_;		// �ش� ��(�ﰢ��)�� ������ �ﰢ���� �ε������
};

