#include "PreCompile.h"
#include "GameEngineFont.h"

IFW1Factory* GameEngineFont::FontFactory_ = nullptr;

GameEngineFont::GameEngineFont() 
	:FontWrapper_(nullptr)
{
}

GameEngineFont::~GameEngineFont() 
{
	// 
	if (nullptr !=  FontWrapper_)
	{
		FontWrapper_->Release();
		FontWrapper_ = nullptr;
	}

}

void GameEngineFont::Load(const std::string& _Path) 
{
	if (nullptr == FontFactory_)
	{
		if (S_OK != FW1CreateFactory(FW1_VERSION, &FontFactory_))
		{
			GameEngineDebug::MsgBoxError("폰트 로딩에 실패했습니다.");
			return;
		}
	}

	std::wstring wText = GameEngineString::StringToWStringReturn(_Path);

	if (S_OK != FontFactory_->CreateFontWrapper(GameEngineDevice::GetDevice(), wText.c_str(), &FontWrapper_))
	{
		GameEngineDebug::MsgBoxError("폰트 캐칭에 실패했습니다 랩핑이 되지 않았습니다.");
		return;
	}
}

void GameEngineFont::DrawFont(const std::string& _Text, float _Size, const float4& _Pos, const float4& _Color, unsigned int _Pivot /*= FW1_CENTER*/)
{
	std::wstring wText = GameEngineString::StringToWStringReturn(_Text);
	FontWrapper_->DrawString(GameEngineDevice::GetContext(), wText.c_str(), _Size, _Pos.x, _Pos.y, _Color.ColorToUint(), _Pivot);
}

void GameEngineFont::DrawFont(const std::string& _Text, float _Size, const float4& _ScreenMiddlePos, const float4& _RendererPos, const float4& _Color, unsigned int _Pivot /*= FW1_CENTER*/)
{
	std::wstring wText = GameEngineString::StringToWStringReturn(_Text);
	
	//스크린 중앙이 0,0 이라는 전제, x양수방향->화면 오른쪽/y양수방향->화면 위쪽
	//ex)렌더러 위치가 100, 100 이면 화면 가운데 기준 오른쪽위에 나오게 됩니다

	float4 Pos = {(_ScreenMiddlePos.x + _RendererPos.x),(_ScreenMiddlePos.y - _RendererPos.y)};
	FontWrapper_->DrawString(GameEngineDevice::GetContext(), wText.c_str(), _Size, Pos.x, Pos.y, _Color.ColorToUint(), _Pivot);
}