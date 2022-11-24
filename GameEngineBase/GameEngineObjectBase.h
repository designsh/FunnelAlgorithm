#pragma once

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineObjectBase
{
private:	// member Var
	// true��� �ϴ¼��� �޸������� �ı��ɰ̴ϴ�.
	bool isDeath_; // �״±���� �ƴմϴ�. �׾����� Ȯ���Ҽ� �ִ±��
	bool isUpdate_; // �̰� ������Ʈ�Ǵ� ����� �ƴϰ� ������Ʈ�� �ؾ�
	bool isDebug_; // ����� ��� �̰� true�� �س��� f9 �ɾ� ������ 
	int Order_;
	bool isOrigin_; // ����� ��� �̰� true�� �س��� f9 �ɾ� ������ 
	GameEngineObjectBase* parent_;

public:
	bool IsNull()
	{
		return nullptr == this;
	}

	GameEngineObjectBase* GetParent()
	{
		return parent_;
	}

	void SetParent(GameEngineObjectBase* _parent)
	{
		parent_ = _parent;
	}

public:
	bool IsDeath()
	{
		if (nullptr == parent_)
		{
			return isDeath_;
		}

		return true == isDeath_ || parent_->isDeath_;
	}

	bool IsUpdate()
	{
		if (nullptr == parent_)
		{
			return false == IsDeath() && isUpdate_;
		}

		return false == IsDeath() &&  // ���� ���׾���
			true == isUpdate_ &&  // ���� �����ְ�
			parent_->isUpdate_; // �θ� �����־��
	}

	void CloneOn()
	{
		isOrigin_ = false;
	}

	bool IsClone()
	{
		return isOrigin_ == false;
	}


	void Death()
	{
		isDeath_ = true;
	}

	void OnOffChange()
	{
		isUpdate_ = !isUpdate_;
	}

	void On()
	{
		isUpdate_ = true;
	}

	void Off()
	{
		isUpdate_ = false;
	}

	void DebugOn()
	{
		isDebug_ = true;
	}

	void DebugOff()
	{
		isDebug_ = false;
	}

	bool IsDebugCheck()
	{
		if (true == isDebug_)
		{
			int a = 0;
		}

		return isDebug_;
	}

	void SetOrder(int _Order)
	{
		Order_ = _Order;
	}

	int GetOrder()
	{
		return Order_;
	}

	bool& GetIsUpdateRef()
	{
		return isUpdate_; // �θ� �����־��
	}


public:
	GameEngineObjectBase(); // default constructer ����Ʈ ������
	virtual ~GameEngineObjectBase(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineObjectBase(const GameEngineObjectBase& _other); // default Copy constructer ����Ʈ ���������
	GameEngineObjectBase(GameEngineObjectBase&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineObjectBase& operator=(const GameEngineObjectBase& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineObjectBase& operator=(const GameEngineObjectBase&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
};

