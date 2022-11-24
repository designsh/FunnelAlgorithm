#pragma once
#include "GameEngineBase\GameEngineObjectNameBase.h"
#include <vector>
#include "imgui.h"

// Ό³Έν :
class GameEngineGUIWindow;
class GameEngineGUI
{
	friend class GameEngineCore;
	friend class GameEngineLevel;
private:
	static GameEngineGUI* Inst_;

public:
	inline static GameEngineGUI* GetInst()
	{
		return Inst_;
	}

	static void Destroy()
	{
		if (nullptr != Inst_)
		{
			delete Inst_;
			Inst_ = nullptr;
		}
	}


public:
	template<typename WindowType>
	WindowType* CreateGUIWindow(const std::string& _Name)
	{
		WindowType* NewWindow = new WindowType();

		NewWindow->SetName(_Name);

		GameEngineGUIWindow* ParentType = NewWindow;
		ParentType->Start();
		Windows_.push_back(NewWindow);

		return NewWindow;
	}

	GameEngineGUIWindow* FindGUIWindow(const std::string& _Name);

	template<typename ConvertType>
	ConvertType* FindGUIWindowConvert(const std::string& _Name)
	{
		return dynamic_cast<ConvertType*>(FindGUIWindow(_Name));
	}


	std::list<GameEngineGUIWindow*> FindGUIWindowForList(const std::string& _Name);

protected:


private:
	std::list<GameEngineGUIWindow*> Windows_;

	void Initialize();
	void GUIRenderStart();
	void GUIRenderEnd();

	// constrcuter destructer
	GameEngineGUI();
	~GameEngineGUI();

	// delete Function
	GameEngineGUI(const GameEngineGUI& _Other) = delete;
	GameEngineGUI(GameEngineGUI&& _Other) noexcept = delete;
	GameEngineGUI& operator=(const GameEngineGUI& _Other) = delete;
	GameEngineGUI& operator=(GameEngineGUI&& _Other) noexcept = delete;

};



class GameEngineGUIWindow : public GameEngineObjectNameBase
{
	friend GameEngineGUI;

public:
	virtual void Start() {};

	void Begin()
	{
		ImGui::Begin(GetName().c_str(), &GetIsUpdateRef(), Style_);
	}

	virtual void OnGUI() = 0;

	void End()
	{
		ImGui::End();
	}

	// constrcuter destructer
	GameEngineGUIWindow();
	~GameEngineGUIWindow();

protected:
	int Style_;


private:

	// delete Function
	GameEngineGUIWindow(const GameEngineGUIWindow& _Other) = delete;
	GameEngineGUIWindow(GameEngineGUIWindow&& _Other) noexcept = delete;
	GameEngineGUIWindow& operator=(const GameEngineGUIWindow& _Other) = delete;
	GameEngineGUIWindow& operator=(GameEngineGUIWindow&& _Other) noexcept = delete;

};