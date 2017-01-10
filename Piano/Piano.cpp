#include "stdafx.h"
#include "Piano.h"
#include "MainWindow.h"
#include "Controls.h"
#include "PianoKeyboard\IKeyboard.h"
#include <vld.h>

using namespace std;

vector<vector<map<int16_t, float>>> Piano::notes = vector<vector<map<int16_t, float>>>();
vector<vector<pair<unsigned, unsigned>>> Piano::milliSeconds
	= vector<vector<pair<unsigned, unsigned>>>();

shared_ptr<IKeyboard> Piano::keyboard = nullptr;

vector<size_t> Piano::indexes = vector<size_t>();
vector<size_t> Piano::tracks = vector<size_t>();
shared_ptr<size_t> Piano::leftTrack = nullptr;
shared_ptr<size_t> Piano::rightTrack = nullptr;

vector<vector<vector<string>>> Piano::fingersLeft = vector<vector<vector<string>>>();
vector<vector<vector<string>>> Piano::fingersRight = vector<vector<vector<string>>>();


ATOM Piano::MyRegisterClass()
{
	WNDCLASSEXW wcex{ sizeof wcex };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWindow::WndProc;
	wcex.hInstance = MainWindow::hInstance;
	wcex.hIcon = LoadIcon(MainWindow::hInstance, MAKEINTRESOURCE(IDI_LARGE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MENU);
	wcex.lpszClassName = szWindowClass_;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
	return RegisterClassEx(&wcex);
}

inline BOOL Piano::InitInstance(const int nCmdShow)
{
	MainWindow::hWndMain = CreateWindowEx(WS_EX_ACCEPTFILES, szWindowClass_, windowTitle,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
		HWND_DESKTOP, nullptr, MainWindow::hInstance, nullptr);
		if (!MainWindow::hWndMain) return FALSE;

		ShowWindow(MainWindow::hWndMain, SW_SHOWMAXIMIZED);
#ifdef _DEBUG
		UNREFERENCED_PARAMETER(nCmdShow);
#else
		ShowWindow(MainWindow::hWndMain, nCmdShow);
#endif
		UpdateWindow(MainWindow::hWndMain);

		return TRUE;
}

int Piano::Main(const int nCmdShow)
{
	MyRegisterClass();
	if (!InitInstance(nCmdShow)) return FALSE;

    const auto hAccelTable(LoadAccelerators(MainWindow::hInstance, MAKEINTRESOURCE(IDC_MENU)));
	MSG msg;

#ifdef _DEBUG
	const auto before(VLDGetLeaksCount());
#endif

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(MainWindow::hWndMain, hAccelTable, &msg) &&
			!IsDialogMessage(Controls::hDlgControls, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (typeid(*keyboard) == typeid(Keyboard3D)) keyboard->Update();
		else assert("Wrong keyboard class" && typeid(*keyboard) == typeid(Keyboard2D));
	}

#ifdef _DEBUG
	const auto after(VLDGetLeaksCount());
#endif
//	assert("Memory leaks detected" && before == after);

	return static_cast<int>(msg.wParam);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
	MainWindow::hInstance = hInstance;
	return Piano::Main(nCmdShow);
}