#include "PreCompile.h"
#include "GameEngineWindow.h"
#include <GameEngineBase/GameEngineDebug.h>
#include <GameEngineBase/GameEngineTime.h>
#include <GameEngineBase/GameEngineSound.h>
#include <iostream>


std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> GameEngineWindow::MessageCallBack_ = nullptr;
// 포인터형 싱글톤
GameEngineWindow* GameEngineWindow::Inst = new GameEngineWindow();

bool WindowOn = true;

LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
    if (nullptr != GameEngineWindow::MessageCallBack_)
    {
        if (0 != GameEngineWindow::MessageCallBack_(_hWnd, _message, _wParam, _lParam))
        {
            return true;
        }
    }

    //if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    //    return true;

    switch (_message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        // HDC 윈도우에 픽셀에 색깔을 바꿀수 있는 권한이입니다.
        // 윈도우창이 그려지는 2차원 배열에 접근할수 있는 권한입니다.
        HDC hdc = BeginPaint(_hWnd, &ps);
        EndPaint(_hWnd, &ps);
        break;
    }
    case WM_SIZE:
    {
        return DefWindowProc(_hWnd, _message, _wParam, _lParam);
        break;
    }
    case WM_DESTROY:
    {
        WindowOn = false;
        break;
    }
    default:
        return DefWindowProc(_hWnd, _message, _wParam, _lParam);
    }

    return 0;
}

GameEngineWindow::GameEngineWindow() 
    : className_("")
    , windowTitle_("")
    , windowhandle_(nullptr)
    , hInstance_(nullptr)
    , devicecontext_(nullptr)
{
}

GameEngineWindow::~GameEngineWindow() 
{
    if (nullptr != windowhandle_)
    {
        DestroyWindow(windowhandle_);
        windowhandle_ = nullptr;
    }
}

// constructer destructer
//member Func
int GameEngineWindow::CreateMainWindowClass()
{
    hInstance_ = GetModuleHandle(NULL);

    if (nullptr == hInstance_)
    {
        GameEngineDebug::AssertFalse();
        return 0;
    }

    className_ = "DEF";

    WNDCLASSEXA wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW; // 화면 재갱신 옵션
    wcex.lpfnWndProc = WndProc; // <= 내일 설명
    wcex.cbClsExtra = 0; // 신경안써도 됨
    wcex.cbWndExtra = 0; // 신경안써도 됨.
    wcex.hInstance = hInstance_;
    wcex.hIcon = nullptr;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT3));
    wcex.hCursor = nullptr;//LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);

    wcex.lpszMenuName = nullptr;//MAKEINTRESOURCEW(IDC_WINDOWSPROJECT3);
    wcex.lpszClassName = className_.c_str();
    wcex.hIconSm = nullptr;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // 아래의 함수의 내용이 
    return RegisterClassExA(&wcex);
}

void GameEngineWindow::CreateMainWindow(const std::string& _titlename, const float4& _size, const float4& _pos)
{
    if (0 == CreateMainWindowClass())
    {
        GameEngineDebug::MsgBoxError("윈도우 클래스 등록에 실패했습니다.");
        return;
    }

    if (nullptr == hInstance_)
    {
        GameEngineDebug::AssertFalse();
        return;
    }

    if ("" == className_)
    {
        GameEngineDebug::AssertFalse();
        return;
    }

    // setlocale(LC_ALL, "");

    windowTitle_ = _titlename;
    windowhandle_ = nullptr;
    windowhandle_ = CreateWindowA(className_.c_str(), "TEST", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance_, nullptr);

    if (0 == windowhandle_)
    {
        GameEngineDebug::AssertFalse();
        return;
    }

    SetWindowTextA(windowhandle_, windowTitle_.c_str());
    ShowWindow(windowhandle_, SW_SHOW);
    SetSizeAndPos(_size, _pos);
    UpdateWindow(windowhandle_);


    devicecontext_ = ::GetDC(windowhandle_);
    return;
}

void GameEngineWindow::SetSizeAndPos(const float4& _size, const float4& _pos)
{
    size_ = _size;
    pos_ = _pos;

    //          1920          
    RECT Rc = { 0, 0, _size.ix(), _size.iy() };
    // 내가 넣어준 렉트에 타이틀바와 메뉴등의 사이즈가 들어간 녀석으로 만들어주세요.
    AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, false);

    // 0넣으면 그냥 보통 기본이다.
    SetWindowPos(windowhandle_, nullptr, _pos.ix(), _pos.iy(), Rc.right - Rc.left, Rc.bottom - Rc.top, 0);
}

void GameEngineWindow::Loop(void(*_loopFunc)()) 
{
    MSG msg;
    while (WindowOn)
    {
        if (0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {

            if (nullptr == _loopFunc)
            {
                GameEngineDebug::AssertFalse();
                return;
            }

            _loopFunc();

            // Window메세지가 있든 없든 돌아간다.
            if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg); // WNDPROC
            }


        }
        else 
        {

            if (nullptr == _loopFunc)
            {
                GameEngineDebug::AssertFalse();
                return;
            }

            _loopFunc();
        }
    }
}



bool GameEngineWindow::IsWindowRangeOut(const float4& _Pos) 
{
    if (0 > _Pos.x)
    {
        return true;
    }

    if (0 > _Pos.y)
    {
        return true;
    }

    if (_Pos.x > GetSize().x)
    {
        return true;
    }

    if (_Pos.y > GetSize().y)
    {
        return true;
    }

    return false;
}