#pragma once
#include <GameEngine/EngineVertex.h>

// 분류 : 삼각형(면)
// 용도 : 길찾기알고리즘 베이스
// 설명 : 메쉬를 이루는 삼각형집합 중 한개
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

private: // 셀 상세정보 생성
	void CenterOfGravityCalculation();

private: // 현재 셀에 존재하는 액터 탐색(갱신용)
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

private: // 수신받는 정보
	int Index_;											// 현재 목록에 해당하는 Index(탐색용)
	int MeshIndex_;										// 해당 면을 포함하는 MeshIndex(탐색용)
	int FaceIndex_;										// 해당 면의 인덱스(탐색용)
	std::vector<GameEngineVertex> VertexList_;			// 해당 면을 구성하는 정점목록(3개)
	std::vector<UINT> IndexList_;						// 해당 면을 구성하는 인덱스목록

private: // 무게중심 및 인접한 면목록
	float4 CenterOfGravity_;							// 해당 면(삼각형)의 무게중심
	std::vector<NaviCell*> AdjacentTriangles_;		// 해당 면(삼각형)과 인접한 삼각형의 인덱스목록
};

