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

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class GameEngineCollision;
class NaviCell;
class Yuki : public GameEngineActor
{
public: // ������ġ ����
	void Initialize(NaviCell* _CurNaviCell, const float4& _InitPos);

public: // �̵�����
	void MoveStart(NaviCell* _TargetNaviCell, const float4& _MoveTargetPos);

public: // ��Ÿ�׼�
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

#pragma region �ִϸ��̼� ��������
private:
	std::vector<std::string> AnimNameList_;	// ���� ĳ���Ͱ� ����ϴ� �ִϸ��̼� �̸� ���
	int CurAnimationIndex_;					// ���� ĳ���� �ִϸ��̼� �ε���

private:
	Yuki_State CurState_;					// ���� ĳ���� ����

private:
#pragma endregion

#pragma region �̵�����
private:
	NaviCell* CurNaviCell_;				// ���� �÷��̾ ��ġ�ϰ� �ִ� �ﰢ��
	NaviCell* TargetNaviCell_;			// ���� �÷��̾ �̵��Ϸ��� ������ǥ������ ��ġ�� �ﰢ��
	bool MoveStart_;						// �̵���� ���� �Ϸ�� On

private:
	std::list<float4> MovePath_;			// �̵����

private:
	float4 MoveStartPos_;					// �̵������ ���� ��ġ
	float4 MoveEndPos_;						// �̵������ ��ǥ ��ġ
	float MoveSpeed_;						// �̵��ӵ�
#pragma endregion

private:
	GameEngineActor* CurTarget_;			// ���� ���õ� Ÿ��
	bool BattleMode_;						// ���� ������ Ÿ�ٰ� ������� Ȱ��ȭ
};

