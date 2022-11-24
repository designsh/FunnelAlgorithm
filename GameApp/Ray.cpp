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
    // 광선을 월드영역으로 이동
    // 결과값 : 광선시작위치, 광선의방향
    if (false == RayAtViewSpace(_MousePos))
    {
        return false;
    }

    // 현재 레벨의 모든 충돌체 목록 Get
    std::map<int, std::list<GameEngineCollision*>> AllList = GetLevel()->GetAllCollision();

    // 해당 광선과 교차하는 충돌체 탐색
    int GroupCnt = static_cast<int>(AllList.size());
    for (int Group = 0; Group < GroupCnt; ++Group)
    {
        std::list<GameEngineCollision*>::iterator StartIter = AllList[Group].begin();
        std::list<GameEngineCollision*>::iterator EndIter = AllList[Group].end();
        for (; StartIter != EndIter; ++StartIter)
        {
            // 광선의 시작지점에서부터 선택된 충돌체와 교차하는 지점까지의 거리
            float Dist = 0.0f;
            if (true == (*StartIter)->BoundingToRayCollision((*StartIter)->GetCollisionType(), OriginPos_, Direction_, Dist))
            {
                // 교차한 지점의 좌표를 반환
                _PickedPos = OriginPos_ + (Direction_ * Dist);
                return true;
            }
        }
    }

    // 충돌하는 충돌체가 존재하지않다면 실패
    return false;
}

bool Ray::IsMeshPicked(GameEngineFBXRenderer* _Mesh, const float4& _MousePos, float4& _PickedPos)
{
    // 광선을 월드영역으로 이동
    // 결과값 : 광선시작위치, 광선의방향
    if (false == RayAtViewSpace(_MousePos))
    {
        return false;
    }

    // Mesh가 없다면 처리 불가
    if (nullptr == _Mesh)
    {
        return false;
    }
    
    float Dist = 0.0f;

    // 교차성공시 교차점까지의 거리를 이용하여 해당 좌표를 반환
    if (true == _Mesh->CheckIntersects(OriginPos_, Direction_, Dist))
    {
        _PickedPos = OriginPos_ + (Direction_ * Dist);
        return true;
    }

    return false;
}

NaviCell* Ray::IsPickedCell(GameEngineFBXRenderer* _Mesh, const float4& _MousePos, float4& _PickedPos)
{
    // 광선을 월드영역으로 이동
    // 결과값 : 광선시작위치, 광선의방향
    if (false == RayAtViewSpace(_MousePos))
    {
        return nullptr;
    }

    // Mesh가 없다면 처리 불가
    if (nullptr == _Mesh)
    {
        return nullptr;
    }

    float Dist = 0.0f;

    // 교차성공시 교차점까지의 거리를 이용하여 해당 좌표를 반환
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

                // 해당 함수는 Ray가 교차하는 점이 삼각형 내부에 존재하는지 판단하고, 그 교차한 지점의 위치좌표를 반환
                if (true == DirectX::TriangleTests::Intersects(OriginPos_.DirectVector, Direction_.DirectVector, V0.DirectVector, V1.DirectVector, V2.DirectVector, Dist))
                {
                    // 교차했다면 그 교차점의 좌표를 반환하며
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
    // 현재 광선과 제일처음 충돌하는 충돌체를 탐색
    if (false == RayAtViewSpace(_MousePos))
    {
        return nullptr;
    }

    // 현재 레벨의 모든 충돌체 목록 Get
    std::map<int, std::list<GameEngineCollision*>> AllList = GetLevel()->GetAllCollision();

    // 해당 광선과 교차하는 충돌체 탐색
    int GroupCnt = static_cast<int>(AllList.size());
    for (int Group = 0; Group < GroupCnt; ++Group)
    {
        std::list<GameEngineCollision*>::iterator StartIter = AllList[Group].begin();
        std::list<GameEngineCollision*>::iterator EndIter = AllList[Group].end();
        for (; StartIter != EndIter; ++StartIter)
        {
            // 광선의 시작지점에서부터 선택된 충돌체와 교차하는 지점까지의 거리
            float Dist = 0.0f;
            if (true == (*StartIter)->BoundingToRayCollision((*StartIter)->GetCollisionType(), OriginPos_, Direction_, Dist))
            {
                // 교차한 지점의 좌표를 반환
                _PickedPos = OriginPos_ + (Direction_ * Dist);
                return (*StartIter)->GetActor();
            }
        }
    }

    return nullptr;
}

bool Ray::RayAtViewSpace(float _MousePosX, float _MousePosY)
{
    // 2D 환경에서의 마우스 클릭지점을 이용하여
    // 3D 환경에서의 마우스 좌표와 현재 카메라가 바라보는 방향을 계산한다.
    OriginPos_ = float4(0.f, 0.f, 0.f, 0.f);

    // 1. 광선을 뷰포트영역 -> 투영영역
    UINT ViewPortNo = 1;
    D3D11_VIEWPORT ViewPort_ = {};
    GameEngineDevice::GetInst().GetContext()->RSGetViewports(&ViewPortNo, &ViewPort_);
    float PointX = ((2.0f * _MousePosX) / ViewPort_.Width) - 1.0f;
    float PointY = (((2.0f * _MousePosY) / ViewPort_.Height) - 1.0f) * -1.0f;
    float PointZ = 1.0f;

    // 2. 광선을 투영영역 -> 뷰영역
    float4x4 ProjMat = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
    PointX = PointX / ProjMat._11;
    PointY = PointY / ProjMat._22;

    // 3. 광선을 뷰영역 -> 월드영역
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
    // 2D 환경에서의 마우스 클릭지점을 이용하여
    // 3D 환경에서의 마우스 좌표와 현재 카메라가 바라보는 방향을 계산한다.
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
