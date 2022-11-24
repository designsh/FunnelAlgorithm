#pragma once
#include <GameEngine/GameEngineActor.h>

enum class Yuki_State
{
	COM_ARRIVE,
	COM_BOXOPEN,
	COM_COLLECT,
	COM_COLLECT_STONE,
	COM_COLLECT_WATER,
	COM_COLLECT_WOOD,
	COM_CRAFTFOOD,
	COM_CRAFTMETAL,
	COM_DANCE,
	COM_DEATH,
	COM_DOWN_DEAD,
	COM_DOWN_RUN,
	COM_DOWN_START,
	COM_DOWN_WAIT,
	COM_DOWNDEAD,
	COM_FISHING,
	COM_INSTALL_TRAP,
	COM_OPERATE,
	COM_REST_END,
	COM_REST_LOOP,
	COM_REST_START,
	COM_RESURRECT,
	COM_RUN,
	COM_SKILL04,
	COM_SKILL04_END,
	COM_WAIT,
	MAX
};

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class GameEngineCollision;
class NaviCell;
class Yuki : public GameEngineActor
{
public: // 스폰위치 셋팅
	void Initialize(NaviCell* _CurNaviCell, const float4& _InitPos);

public: // 이동관련
	void MoveStart(NaviCell* _TargetNaviCell, const float4& _MoveTargetPos);

public: // 기타액션
	void Detect();
	void Attack();
	void SkillAttack();
	void Block();
	void GetHit();

protected:

private:
	void InputKeyStateCheck(float _DeltaTime);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	Yuki();
	~Yuki();

protected:
	Yuki(const Yuki& _other) = delete;
	Yuki(Yuki&& _other) noexcept = delete;

private:
	Yuki& operator=(const Yuki& _other) = delete;
	Yuki& operator=(const Yuki&& _other) = delete;

public:
	static Yuki* MainPlayer;

protected:

private:
	GameEngineFBXMesh* BaseMesh_;
	GameEngineFBXRenderer* AnimRenderer_;

private:
	GameEngineCollision* BodyCollider_;
	GameEngineCollision* AttackCollider_;

#pragma region 애니메이션 관리관련
private:
	std::vector<std::string> AnimNameList_;	// 현재 캐릭터가 사용하는 애니메이션 이름 목록
	int CurAnimationIndex_;					// 현재 캐릭터 애니메이션 인덱스

private:
	Yuki_State CurState_;					// 현재 캐릭터 상태

private:
#pragma endregion

#pragma region 이동관련
private:
	NaviCell* CurNaviCell_;				// 현재 플레이어가 위치하고 있는 삼각형
	NaviCell* TargetNaviCell_;			// 현재 플레이어가 이동하려는 최종목표지점의 위치한 삼각형
	bool MoveStart_;						// 이동경로 생성 완료시 On

private:
	std::list<float4> MovePath_;			// 이동경로

private:
	float4 MoveStartPos_;					// 이동경로의 시작 위치
	float4 MoveEndPos_;						// 이동경로의 목표 위치
	float MoveSpeed_;						// 이동속도
#pragma endregion

private:
	GameEngineActor* CurTarget_;			// 현재 선택된 타겟
	bool BattleMode_;						// 현재 지정된 타겟과 전투모드 활성화
};

