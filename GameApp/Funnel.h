#pragma once
#include "NaviCell.h"

// �з� : 
// �뵵 : A*�� �� ����� ������ ������θ� ����
// ���� : A*�� �� ���(������)�� ���Ź޾� �������� ������θ� ����Ͽ� ����
class Funnel
{
public:
	std::list<float4> PathOptimization(const float4& _StartPos, const float4& _EndPos, std::list<NaviCell*>& _MovePath);

protected:

private:
	void CreatePortalVertexList(std::list<NaviCell*>& _MovePath);
	bool OptimizationStart(std::list<float4>& _ReturnPath);

public:
	Funnel();
	~Funnel();

protected:
	Funnel(const Funnel& _other) = delete;
	Funnel(Funnel&& _other) noexcept = delete;

private:
	Funnel& operator=(const Funnel& _other) = delete;
	Funnel& operator=(const Funnel&& _other) = delete;

public:

protected:

private: // ��������
	float4 StartPos_;						// �̵�������ġ
	float4 EndPos_;							// �̵���ǥ��ġ

private: // ��Ż����
	std::vector<float4> LeftPortal_;		// �� ���� �����߽��� �����ϴ� ���� ���� ������ �������
	std::vector<float4> RightPortal_;		// �� ���� �����߽��� �����ϴ� ���� ���� �������� �������

private:
};

