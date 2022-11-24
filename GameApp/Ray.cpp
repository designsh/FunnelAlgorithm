#include "PreCompile.h"
#include "Ray.h"

#include <GameEngineBase/GameEngineMath.h>

#include <GameEngine/GameEngineDevice.h>
#include <GameEngine/GameEngineCore.h>
#include <GameEngine/CameraComponent.h>
#include <GameEngine/GameEngineFBXMesh.h>
#include <GameEngine/GameEngineCollision.h>

#include "FloorMap.h"

bool Ray::IsPicked(const float4& _MousePos, float4& _PickedPos, GameEngineFBXRenderer* _Mesh)
{
    if (nullptr == _Mesh)
    {
        return IsColliderPicked(_MousePos, _PickedPos);
    }
    else
    {
        return IsMeshPicked(_Mesh, _MousePos, _PickedPos);
    }

    return false;
}

bool Ray::IsColliderPicked(const float4& _MousePos, float4& _PickedPos)
{
    // ������ ���念������ �̵�
    // ����� : ����������ġ, �����ǹ���
    if (false == RayAtViewSpace(_MousePos))
    {
        return false;
    }

    // ���� ������ ��� �浹ü ��� Get
    std::map<int, std::list<GameEngineCollision*>> AllList = GetLevel()->GetAllCollision();

    // �ش� ������ �����ϴ� �浹ü Ž��
    int GroupCnt = static_cast<int>(AllList.size());
    for (int Group = 0; Group < GroupCnt; ++Group)
    {
        std::list<GameEngineCollision*>::iterator StartIter = AllList[Group].begin();
        std::list<GameEngineCollision*>::iterator EndIter = AllList[Group].end();
        for (; StartIter != EndIter; ++StartIter)
        {
            // ������ ���������������� ���õ� �浹ü�� �����ϴ� ���������� �Ÿ�
            float Dist = 0.0f;
            if (true == (*StartIter)->BoundingToRayCollision((*StartIter)->GetCollisionType(), OriginPos_, Direction_, Dist))
            {
                // ������ ������ ��ǥ�� ��ȯ
                _PickedPos = OriginPos_ + (Direction_ * Dist);
                return true;
            }
        }
    }

    // �浹�ϴ� �浹ü�� ���������ʴٸ� ����
    return false;
}

bool Ray::IsMeshPicked(GameEngineFBXRenderer* _Mesh, const float4& _MousePos, float4& _PickedPos)
{
    // ������ ���念������ �̵�
    // ����� : ����������ġ, �����ǹ���
    if (false == RayAtViewSpace(_MousePos))
    {
        return false;
    }

    // Mesh�� ���ٸ� ó�� �Ұ�
    if (nullptr == _Mesh)
    {
        return false;
    }
    
    float Dist = 0.0f;

    // ���������� ������������ �Ÿ��� �̿��Ͽ� �ش� ��ǥ�� ��ȯ
    if (true == _Mesh->CheckIntersects(OriginPos_, Direction_, Dist))
    {
        _PickedPos = OriginPos_ + (Direction_ * Dist);
        return true;
    }

    return false;
}

NaviCell* Ray::IsPickedCell(GameEngineFBXRenderer* _Mesh, const float4& _MousePos, float4& _PickedPos)
{
    // ������ ���念������ �̵�
    // ����� : ����������ġ, �����ǹ���
    if (false == RayAtViewSpace(_MousePos))
    {
        return nullptr;
    }

    // Mesh�� ���ٸ� ó�� �Ұ�
    if (nullptr == _Mesh)
    {
        return nullptr;
    }

    float Dist = 0.0f;

    // ���������� ������������ �Ÿ��� �̿��Ͽ� �ش� ��ǥ�� ��ȯ
    std::vector<FbxMeshSet>& vecMeshMap = _Mesh->GetMesh()->GetAllMeshMap();
    std::vector<FbxExMeshInfo>& vecMeshInfos = _Mesh->GetMesh()->GetMeshInfos();

    for (int MeshNumber = 0; MeshNumber < static_cast<int>(vecMeshMap.size()); ++MeshNumber)
    {
        for (int MaterialNumber = 0; MaterialNumber < vecMeshInfos[MeshNumber].MaterialNum; ++MaterialNumber)
        {
            for (int FaceNumber = 0; FaceNumber < vecMeshInfos[MeshNumber].FaceNum; ++FaceNumber)
            {
                float4 V0 = vecMeshMap[MeshNumber].Vertexs[vecMeshMap[MeshNumber].Indexs[0][MaterialNumber][FaceNumber * 3 + 0]].POSITION * _Mesh->GetTransform()->GetTransformData().WorldWorld_;
                float4 V1 = vecMeshMap[MeshNumber].Vertexs[vecMeshMap[MeshNumber].Indexs[0][MaterialNumber][FaceNumber * 3 + 1]].POSITION * _Mesh->GetTransform()->GetTransformData().WorldWorld_;
                float4 V2 = vecMeshMap[MeshNumber].Vertexs[vecMeshMap[MeshNumber].Indexs[0][MaterialNumber][FaceNumber * 3 + 2]].POSITION * _Mesh->GetTransform()->GetTransformData().WorldWorld_;

                // �ش� �Լ��� Ray�� �����ϴ� ���� �ﰢ�� ���ο� �����ϴ��� �Ǵ��ϰ�, �� ������ ������ ��ġ��ǥ�� ��ȯ
                if (true == DirectX::TriangleTests::Intersects(OriginPos_.DirectVector, Direction_.DirectVector, V0.DirectVector, V1.DirectVector, V2.DirectVector, Dist))
                {
                    // �����ߴٸ� �� �������� ��ǥ�� ��ȯ�ϸ�
                    _PickedPos = OriginPos_ + (Direction_ * Dist);
                    return FloorMap::ptrFloorMap->GetNaviCellInfo(vecMeshMap[MeshNumber].Vertexs[vecMeshMap[MeshNumber].Indexs[0][MaterialNumber][FaceNumber * 3 + 0]].POSITION,
                        vecMeshMap[MeshNumber].Vertexs[vecMeshMap[MeshNumber].Indexs[0][MaterialNumber][FaceNumber * 3 + 1]].POSITION,
                        vecMeshMap[MeshNumber].Vertexs[vecMeshMap[MeshNumber].Indexs[0][MaterialNumber][FaceNumber * 3 + 2]].POSITION);
                }
            }
        }
    }

    return nullptr;
}

GameEngineActor* Ray::IsPickedTarget(const float4& _MousePos, float4& _PickedPos)
{
    // ���� ������ ����ó�� �浹�ϴ� �浹ü�� Ž��
    if (false == RayAtViewSpace(_MousePos))
    {
        return nullptr;
    }

    // ���� ������ ��� �浹ü ��� Get
    std::map<int, std::list<GameEngineCollision*>> AllList = GetLevel()->GetAllCollision();

    // �ش� ������ �����ϴ� �浹ü Ž��
    int GroupCnt = static_cast<int>(AllList.size());
    for (int Group = 0; Group < GroupCnt; ++Group)
    {
        std::list<GameEngineCollision*>::iterator StartIter = AllList[Group].begin();
        std::list<GameEngineCollision*>::iterator EndIter = AllList[Group].end();
        for (; StartIter != EndIter; ++StartIter)
        {
            // ������ ���������������� ���õ� �浹ü�� �����ϴ� ���������� �Ÿ�
            float Dist = 0.0f;
            if (true == (*StartIter)->BoundingToRayCollision((*StartIter)->GetCollisionType(), OriginPos_, Direction_, Dist))
            {
                // ������ ������ ��ǥ�� ��ȯ
                _PickedPos = OriginPos_ + (Direction_ * Dist);
                return (*StartIter)->GetActor();
            }
        }
    }

    return nullptr;
}

bool Ray::RayAtViewSpace(float _MousePosX, float _MousePosY)
{
    // 2D ȯ�濡���� ���콺 Ŭ�������� �̿��Ͽ�
    // 3D ȯ�濡���� ���콺 ��ǥ�� ���� ī�޶� �ٶ󺸴� ������ ����Ѵ�.
    OriginPos_ = float4(0.f, 0.f, 0.f, 0.f);

    // 1. ������ ����Ʈ���� -> ��������
    UINT ViewPortNo = 1;
    D3D11_VIEWPORT ViewPort_ = {};
    GameEngineDevice::GetInst().GetContext()->RSGetViewports(&ViewPortNo, &ViewPort_);
    float PointX = ((2.0f * _MousePosX) / ViewPort_.Width) - 1.0f;
    float PointY = (((2.0f * _MousePosY) / ViewPort_.Height) - 1.0f) * -1.0f;
    float PointZ = 1.0f;

    // 2. ������ �������� -> �俵��
    float4x4 ProjMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
    PointX = PointX / ProjMat._11;
    PointY = PointY / ProjMat._22;

    // 3. ������ �俵�� -> ���念��
    float4x4 ViewMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
    float4x4 InverseViewMat = ViewMat.InverseReturn();
    Direction_.x = (PointX * InverseViewMat._11) + (PointY * InverseViewMat._21) + (PointZ * InverseViewMat._31);
    Direction_.y = (PointX * InverseViewMat._12) + (PointY * InverseViewMat._22) + (PointZ * InverseViewMat._32);
    Direction_.z = (PointX * InverseViewMat._13) + (PointY * InverseViewMat._23) + (PointZ * InverseViewMat._33);
    Direction_.w = 0.0f;
    Direction_.Normalize3D();

    OriginPos_.x = InverseViewMat._41;
    OriginPos_.y = InverseViewMat._42;
    OriginPos_.z = InverseViewMat._43;
    OriginPos_.w = 0.0f;

    return true;
}

bool Ray::RayAtViewSpace(float4 _MouseClickPos)
{
    // 2D ȯ�濡���� ���콺 Ŭ�������� �̿��Ͽ�
    // 3D ȯ�濡���� ���콺 ��ǥ�� ���� ī�޶� �ٶ󺸴� ������ ����Ѵ�.
    float MousePosX = _MouseClickPos.x;
    float MousePosY = _MouseClickPos.y;

    return RayAtViewSpace(MousePosX, MousePosY);
}

void Ray::Start()
{
}

void Ray::Update(float _DeltaTime)
{
}

Ray::Ray()
    : OriginPos_(float4::ZERO)
    , Direction_(float4::ZERO)
{
}

Ray::~Ray()
{
}
