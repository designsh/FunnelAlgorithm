#include "PreCompile.h"
#include "GameEngineRenderWindow.h"
#include <iostream>
#include "GameEngineImageShotWindow.h"

GameEngineRenderWindow::GameEngineRenderWindow() 
{
}

GameEngineRenderWindow::~GameEngineRenderWindow() 
{
}

void GameEngineRenderWindow::OnGUI()
{
	static int Index = 0;

	for (size_t i = 0; i < DrawRenderTarget_.size(); i++)
	{
		size_t Count =  DrawRenderTarget_[i].Target->GetShaderResourcesViewCount();

		for (size_t TextureCount = 0; TextureCount < Count; TextureCount++)
		{
			ImGui::Text(GameEngineString::AnsiToUTF8Return(DrawRenderTarget_[i].Name).c_str());
			ImTextureID Id = reinterpret_cast<ImTextureID>(DrawRenderTarget_[i].Target->GetShaderResourcesView(TextureCount));
			float4 Size = DrawRenderTarget_[i].Size_;

			if (true == ImGui::ImageButton(Id, { Size.x, Size.y }))
			{
				for (auto& Window : ImageShot_)
				{
					if (true == Window->IsUpdate())
					{
						continue;
					}

					Window->On();
					Window->SetView(DrawRenderTarget_[i].Target->GetShaderResourcesView(TextureCount), DrawRenderTarget_[i].Target->GetTextureSize(TextureCount));
					return;
				}

				GameEngineImageShotWindow* ImageWindow = GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineImageShotWindow>("ImageShot" + std::to_string(ImageShot_.size()));
				ImageWindow->SetView(DrawRenderTarget_[i].Target->GetShaderResourcesView(TextureCount), DrawRenderTarget_[i].Target->GetTextureSize(TextureCount));
				ImageShot_.push_back(ImageWindow);
			}
		}

	}
}