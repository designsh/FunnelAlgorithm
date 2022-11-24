#pragma once

enum class CollisionGroup
{
	None = 0,
	MousePointer,
	Player,
	PlayerGround,
	PlayerSight,
	PlayerAttack,
	MonsterSight,
	Monster,
	MonsterAttack,
	Item,
	ItemBox,
	UI,

	//���� �߰�
	Unit,


	MAX = 999

};

enum class ObjectType
{
	Player_,
	Monster_,
	Item_


};

#pragma region SJH
enum class CharacterType // ĳ���� Ÿ��
{
	NONE = -1,
	AI,
	MAIN,
	MAX
};

enum class JobType // ����Ÿ��
{
	NONE = -1,
	YUKI,		// ��Ű(�⺻)
	FIORA,		// �ǿ���
	ZAHIR,		// ������
	NADINE,		// ����
				// ....
				MAX
};

#pragma endregion


enum class ItemType
{
	None,
	Equipment,
	Useable,
	Misc
};