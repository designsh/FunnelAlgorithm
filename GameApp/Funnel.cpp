#include "Precompile.h"
#include "Funnel.h"

std::list<float4> Funnel::PathOptimization(const float4& _StartPos, const float4& _EndPos, std::list<NaviCell*>& _MovePath)
{
	std::list<float4> ReturnPath;

	// ���Ź��� �������� ������ ����
	StartPos_ = _StartPos;
	EndPos_ = _EndPos;

	// ��Ż���� ���� ��
	LeftPortal_.clear();
	RightPortal_.clear();

	// A*�� �� �ִܰ�θ� ���� �ﰢ��(��)�� �����ϴ� Vertex�� �� ��
	// ���������� ������������ �Ǵ��Ͽ� ��� ����
	CreatePortalVertexList(_MovePath);

	// ������Ż, ��������Ż ����� �����Ǿ����Ƿ�
	// ������ �������� �򶧱⸦ �����Ͽ� �̵���� ����ȭ ����
	if (true == OptimizationStart(ReturnPath))
	{
		return ReturnPath;
	}

	return std::list<float4>();
}

void Funnel::CreatePortalVertexList(std::list<NaviCell*>& _MovePath)
{
	// ������Ż�� ��������Ż ��� �� ��Ż�� ���� �ۼ�
	std::list<NaviCell*>::iterator StartIter = _MovePath.begin();
	std::list<NaviCell*>::iterator EndIter = _MovePath.end();
	for (; StartIter != EndIter; )
	{
		// ��λ��� �ΰ��� ��(�ﰢ��)�� �����ϴ� ���� Get
		std::vector<GameEngineVertex> ShareVertex;
		NaviCell* FirstCell = (*StartIter);

		++StartIter;
		if (StartIter != EndIter)
		{
			ShareVertex = FirstCell->SearchShareVertex((*StartIter));

			// �ΰ��� ���� �����߽��� �����ϴ� ���⺤�� ����
			float4 FirstCellGravity = FirstCell->GetCenterToGravity();
			float4 SecondCellGravity = (*StartIter)->GetCenterToGravity();
			float4 FrontVector = SecondCellGravity - FirstCellGravity;

			// �����߽��� �����ϴ� ���⺤�� ���� �ΰ��� ������ �������� ���������� �Ǻ� �� ��Ͽ� ����
			for (int VertexIndex = 0; VertexIndex < static_cast<int>(ShareVertex.size()); ++VertexIndex)
			{
				// �������� ����Ǵ� ����
				float4 TargetVector = ShareVertex[VertexIndex].POSITION - FirstCellGravity;

				float4 Cross = float4::Cross3D(TargetVector, FrontVector).NormalizeReturn3D();
				float Dot = float4::Dot3D(float4(0.0f, 1.0f, 0.0f, 0.0f), Cross);
				if (Dot > 0.0f)
				{
					LeftPortal_.push_back(ShareVertex[VertexIndex].POSITION);
				}
				else if(Dot < 0.0f)
				{
					RightPortal_.push_back(ShareVertex[VertexIndex].POSITION);
				}
			}
		}
	}
}

bool Funnel::OptimizationStart(std::list<float4>& _ReturnPath)
{
	// ������ġ ����
	_ReturnPath.push_back(StartPos_);

	// ��Ż �˻��� ����
	// 1. ��Ż�� �˻��� ������ġ ����
	float4 StartPoint = StartPos_;

	// 2. ���� ��Ż���� ������Ż�� ���Եȴٸ� ������Ż�� ���� ��Ż�� �ȴ�.
	float4 CheckCurLPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// ���� ��Ż�� ���� ����
	float4 CheckNextLPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// ���� ��Ż�� ���� ����
	float4 CheckCurRPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// ���� ��Ż�� ������ ����
	float4 CheckNextRPortal = float4(0.0f, 0.0f, 0.0f, 0.0f);		// ���� ��Ż�� ������ ����

	// 3. ���� ������Ż
	// -> ���� ��Ż�� ���������Ż ���ο� �����Ҷ� ������Ż ����
	int CurLPortalIndex = 0;
	int CurRPortalIndex = 0;

	// ������� �� �ڳ� ������ǥ Ž�� ����
	while(true)
	{
		// ��� ��Ż Ž���Ϸ�� ��� ���� �Ϸ�
		if (CurLPortalIndex >= static_cast<int>(LeftPortal_.size()) ||
			CurLPortalIndex + 1 >= static_cast<int>(LeftPortal_.size()) ||
			CurRPortalIndex >= static_cast<int>(RightPortal_.size()) ||
			CurRPortalIndex + 1 >= static_cast<int>(RightPortal_.size()))
		{
			break;
		}

		//============================================ ���� ��Ż �˻� ���� ���� ============================================//
		CheckCurLPortal = LeftPortal_[CurLPortalIndex];
		CheckNextLPortal = LeftPortal_[CurLPortalIndex + 1];

		float4 StartToCurLPortal = (CheckCurLPortal - StartPoint).NormalizeReturn3D();
		float4 StartToNextLPortal = (CheckNextLPortal - StartPoint).NormalizeReturn3D();
		float4 LPortalCross = float4::Cross3D(StartToNextLPortal, StartToCurLPortal).NormalizeReturn3D();
		float LPortalDot = float4::Dot3D(float4(0.0f, 1.0f, 0.0f, 0.0f), LPortalCross);

		//=========================================== ������ ��Ż �˻� ���� ���� ===========================================//
		CheckCurRPortal = RightPortal_[CurRPortalIndex];
		CheckNextRPortal = RightPortal_[CurRPortalIndex + 1];

		float4 StartToCurRPortal = (CheckCurRPortal - StartPoint).NormalizeReturn3D();
		float4 StartToNextRPortal = (CheckNextRPortal - StartPoint).NormalizeReturn3D();
		float4 RPortalCross = float4::Cross3D(StartToNextRPortal, StartToCurRPortal).NormalizeReturn3D();
		float RPortalDot = float4::Dot3D(float4(0.0f, 1.0f, 0.0f, 0.0f), RPortalCross);

		//============================================ ���� ��Ż �ڳ� ���� ���� ============================================//
		float4 LeftCornerCheck = float4::Cross3D(StartToNextRPortal, StartToCurLPortal).NormalizeReturn3D();
		float LeftCornerDot = float4::Dot3D(float4(0.0f, 1.0f, 0.0f, 0.0f), LeftCornerCheck);

		//=========================================== ������ ��Ż �ڳ� ���� ���� ===========================================//
		float4 RightCornerCheck = float4::Cross3D(StartToNextLPortal, StartToCurRPortal).NormalizeReturn3D();
		float RightCornerDot = float4::Dot3D(float4(0.0f, 1.0f, 0.0f, 0.0f), RightCornerCheck);

		//================================================= ��� ���� ���� ================================================//

		// Ž��������ġ(StartPoint)�� �޶����� ���
		// 1. Ž��������Ż�� ���ؿ�����Ż���� �����̰�, Ž����������Ż�� ���ؿ�������Ż���� �������� ���
		// 2. Ž��������Ż�� ���ؿ�����Ż���� �������̰�, Ž����������Ż�� ���ؿ�����Ż���� ������ ���
		// 3. Ž����������Ż�� ���ؿ�������Ż���� �����̰�, Ž��������Ż�� ���ؿ�������Ż���� �������� ���
		// -> 3���� ����� ���� �����ϰ� CurLPortalIndex, CurRPortalIndex�� �ִ�ġ���� �����ϸ鼭 Ž��

		// ���� 1 : �� �̻��� �����̵� �Ұ� ����
		//         1) Ž��������Ż�� ���ؿ�����Ż�� �����̰ų� ���ؿ�����Ż���� ������ ���
		//         2) Ž����������Ż�� ���ؿ�������Ż�� �����̰ų� ���ؿ�������Ż���� �������� ���
		if (LPortalDot >= 0.0f && RPortalDot <= 0.0f)
		{
			// �ΰ��� ���� �ε��� �� �� ���� �ε����� ������ ���� ������ ������ġ�� ����
			int CheckIndex = CurLPortalIndex < CurRPortalIndex ? CurLPortalIndex : CurRPortalIndex;

			// �� ��Ż�� ���� ���
			float4 Left = LeftPortal_[CheckIndex];
			float4 Right = RightPortal_[CheckIndex];
			float4 MidPoint = (Left + Right) * 0.5f;

			// Ž��������ġ ���� �� ��� �߰�
			StartPoint = MidPoint;
			_ReturnPath.push_back(StartPoint);

			// ���� Ž���� ���� �ε��� ����
			++CheckIndex;
			CurLPortalIndex = CheckIndex;
			CurRPortalIndex = CheckIndex;
			continue;
		}

		// ���� 2 : Ž��������Ż�� ���ؿ�����Ż���� �����ʿ� ��ġ
		if (LPortalDot <= 0.0f)
		{
			// ���ؿ����� ��Ż���� ���� ������Ż�� �����ʿ� ��ġ�ϰų� �����ϴٸ� �������ڳ� ����
			if (RightCornerDot <= 0.0f)
			{
				StartPoint = CheckCurRPortal;
				_ReturnPath.push_back(StartPoint);

				++CurRPortalIndex;
				CurLPortalIndex = CurRPortalIndex;
				continue;
			}

			// ���ؿ�����Ż�� Ž��������Ż�� ������ ��츦 �����ϸ� ���ᰡ��
			if (LPortalDot != 0.0f)
			{
				++CurLPortalIndex;

				//====================================== ������ ó���� ���� ���� ���� ======================================//
				CheckCurLPortal = LeftPortal_[CurLPortalIndex];
				StartToCurLPortal = (CheckCurLPortal - StartPoint).NormalizeReturn3D();
				LeftCornerCheck = float4::Cross3D(StartToNextRPortal, StartToCurLPortal).NormalizeReturn3D();
				LeftCornerDot = float4::Dot3D(float4(0.0f, 1.0f, 0.0f, 0.0f), LeftCornerCheck);
			}
		}

		// ���� 3 : Ž����������Ż�� ���ؿ�������Ż���� ���ʿ� ��ġ
		if (RPortalDot >= 0.0f)
		{
			// ���ؿ��� ��Ż���� ���� ��������Ż�� ���ʿ� ��ġ�ϰų� �����ϴٸ� �����ڳ� ����
			if (LeftCornerDot >= 0.0f)
			{
				StartPoint = CheckCurLPortal;
				_ReturnPath.push_back(StartPoint);

				++CurLPortalIndex;
				CurRPortalIndex = CurLPortalIndex;
				continue;
			}

			// ���ؿ�������Ż�� Ž����������Ż�� ������ ��츦 �����ϸ� ���ᰡ��
			if (RPortalDot != 0.0f)
			{
				++CurRPortalIndex;
			}
		}
	}

	// ��� ��οϼ��� ������ EndPos ����
	_ReturnPath.push_back(EndPos_);

	return true;
}

Funnel::Funnel()
	: StartPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
	, EndPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
{
}

Funnel::~Funnel()
{
}
