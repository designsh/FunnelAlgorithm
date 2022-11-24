#pragma once
#include <GameEngine/MouseActor.h>



class PlayerController;
class Player;
class GameEngineCollision;
class GameMouse : public MouseActor
{
	friend PlayerController;
	friend Player;

public:
	GameMouse(); // default constructer 디폴트 생성자
	~GameMouse(); // default destructer 디폴트 소멸자
	GameMouse(const GameMouse& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameMouse(GameMouse&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자
	GameMouse& operator=(const GameMouse& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameMouse& operator=(const GameMouse&& _other) noexcept = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

protected:
	// Start() 에 들어갈 초기화 함수들
	void ImageInit();
	void CollisionInit();

	// Update() 에 들어가서 주기적으로 체크하는 함수들
	void InputCheck(float _DeltaTime); // 마우스 좌 우 클릭, 스크롤링을 체크합니다.
	void DebugUpdate(float _DeltaTime);


	GameEngineCollision* mouseCollision_;
	float4 XYBuffer_;
	void UpdateMove(float _DeltaTime);
	// 마우스 충돌체가 인 게임 내 바닥을 타고 다닐 수 있게 하는 함수(마우스의 XY 값을 인게임의 XZ 값으로 변환)

private:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	float4 savedPos_;
	// 마우스 이동 시 마우스의 좌표를 저장하는 버퍼





	void SaveCurPos()
	{
		savedPos_ = GameEngineInput::GetInst().GetMouse3DPos();
			//UIRenderer->GetTransform()->GetWorldPosition();
	}
	
public:
	const float4 GetSavedPos()
	{
		return savedPos_;
	}



private:
	// 피킹 변수와 함수들

	float4 pickingRayDirection_; // 광선의 방향
	float4 pickingRayOrigin_; // 광선의 원점
	bool Init(HINSTANCE _hinstance, HWND _hwnd, int _a, int _b);

	void Intersection(float _mouseX, float _mouseY);


	class Ray
	{
	public:
		Ray();
		~Ray();

	protected:
		float4 original_; // 광선의 출발 지점
		float4 direction_; // 광선의 방향(화면 안쪽으로 향하게)

	public:
		Ray RayAtViewSpace(int nScreenX, int nScreenY);
		// 디바이스에서 뷰포트와 투영행렬 얻어와 저장
		// 계산에 필요한 방향벡터 direction_ 을 계산
		// Z 값은? 1.0f?
		

		Ray RayAtWorldSpace(int nScreenX, int nScreenY);
		// 뷰포트, 투영행렬을 이요해 변환한 방향벡터를 얻어온다.
		// 행렬을 담기 위한 변수 2개를 생성 (matView, matInvView)
		// d3dDevice 를 이용해 matView 에 현재의 뷰행렬을 담는다.
		// matInvView행렬에 matView의 역행렬을 담는다.
		// 뷰의 역행렬을 이용해서 광선의 출발점을 변환한다.
		// ... 이거부터가 지금 난관임.
		// 
		// 
		// 뷰의 역행렬을 이용해서 방향벡터를 변환한다.
		// 방향벡터를 정규화(행렬안의 모든값이 1보다 작도록)한다.

		// 즉 뷰 스페이스가 적용되기 전의 좌표가 필요하다...


	};
public:
	


};

