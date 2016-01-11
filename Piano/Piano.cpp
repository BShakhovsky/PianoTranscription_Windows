#include "stdafx.h"
#include "Piano.h"

#include "MidiParser_Facade.h"
#include "MidiError.h"

#include "TrellisGraph_Facade.h"

#include "Keyboard.h"

#include "Sound_Facade.h"
#include "SoundError.h"

using std::vector; 
using std::string;
using std::wstring;
using std::shared_ptr;
using std::make_shared;

using boost::lexical_cast;
using boost::format;
using boost::wformat;
using boost::regex;

const auto szWindowClass(TEXT("MainWindow")); 
const auto ASPECT_RATIO(6);

HWND gControls(nullptr);
auto gDlgWidth(0);

shared_ptr<MidiParser_Facade> gMidi(nullptr);
Keyboard gKeyboard;
Sound_Facade gSound;

auto gIsPlaying(false);
UINT gTimerTick(USER_TIMER_MINIMUM);
DWORD gStart(0);
vector<size_t> gIndexes;
vector<size_t> gTracks;

inline BOOL About_OnInitDialog(HWND, HWND, LPARAM)
{
	return true;
}
inline void About_OnCommand(HWND hDlg, int id, HWND, UINT)
{
	if (id == IDOK || id == IDCANCEL) EndDialog(hDlg, id);
}
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hDlg, WM_INITDIALOG,	About_OnInitDialog);
		HANDLE_MSG(hDlg, WM_COMMAND,	About_OnCommand);
	default: return false;
	}
}

int PlayTrack(size_t trackNo, DWORD dwTime)
{
	auto result(0);

	for (; gIndexes.at(trackNo) < gMidi->GetNotes().at(trackNo).size()
			&& static_cast<time_t>(dwTime) - static_cast<time_t>(gStart
				+ gMidi->GetMilliSeconds().at(trackNo).at(gIndexes.at(trackNo))) >= 0;
		++gIndexes.at(trackNo))
	{
		gKeyboard.PressKey(gMidi->GetNotes().at(trackNo).at(gIndexes.at(trackNo)));
		gSound.AddNote(gMidi->GetNotes().at(trackNo).at(gIndexes.at(trackNo)));
		result = 1;
	}
	if (gIndexes.at(trackNo) < gMidi->GetNotes().at(trackNo).size() && (result
			&& static_cast<time_t>(gMidi->GetMilliSeconds().at(trackNo).at(gIndexes.at(trackNo)))
			- static_cast<time_t>(gMidi->GetMilliSeconds().at(trackNo).at(gIndexes.at(trackNo) - 1))
				<= gTimerTick
			|| static_cast<time_t>(dwTime + gTimerTick) - static_cast<time_t>(gStart
			+ gMidi->GetMilliSeconds().at(trackNo).at(gIndexes.at(trackNo))) >= 0))
		result = INT16_MIN;

	return result;
}
void CALLBACK OnTimer(HWND hWnd, UINT, UINT_PTR, DWORD dwTime)
{
	ScrollBar_SetPos(GetDlgItem(gControls, IDC_SCROLLBAR), static_cast<int>(dwTime - gStart), true);
	const auto seconds((dwTime - gStart) / 1'000), milliSec((dwTime - gStart) % 1'000);
	Edit_SetText(GetDlgItem(gControls, IDC_TIME), (wformat{ TEXT("Time %u:%02u:%02u") } %
		(seconds / 60) % (seconds % 60) % (milliSec / 10)).str().c_str());

	if (accumulate(gTracks.cbegin(), gTracks.cend(), 0,
		[dwTime](int val, size_t track) { return val + PlayTrack(track, dwTime); }) > 0)
	{
		InvalidateRect(hWnd, nullptr, false);
		gSound.Play();
	}
	if (all_of(gTracks.cbegin(), gTracks.cend(),
		[](size_t track) { return gIndexes.at(track) >= gMidi->GetNotes().at(track).size(); }))
	{
		FORWARD_WM_COMMAND(gControls, IDB_PLAY, GetDlgItem(gControls, IDB_PLAY), 0, SendMessage);
		fill(gIndexes.begin(), gIndexes.end(), 0);
	}
}

BOOL Controls_OnInitDialog(HWND hDlg, HWND, LPARAM)
{
	Edit_SetText(GetDlgItem(hDlg, IDC_MIDI_LOG), TEXT("MIDI info and errors if any"));
	
	Edit_SetText(GetDlgItem(hDlg, IDC_TIME), TEXT("Time 0:00:00"));
	
	ComboBox_AddString(GetDlgItem(hDlg, IDC_LEFT_HAND), TEXT("None"));
	ComboBox_AddString(GetDlgItem(hDlg, IDC_RIGHT_HAND), TEXT("None"));
	ComboBox_SetCurSel(GetDlgItem(hDlg, IDC_LEFT_HAND), 0);
	ComboBox_SetCurSel(GetDlgItem(hDlg, IDC_RIGHT_HAND), 0);

	return true;
}

void RewindTracks()
{
	for (const auto& track : gTracks) gIndexes.at(track) = static_cast<size_t>(
		lower_bound(gMidi->GetMilliSeconds().at(track).cbegin(),
			gMidi->GetMilliSeconds().at(track).cend(), static_cast<unsigned>(
				ScrollBar_GetPos(GetDlgItem(gControls, IDC_SCROLLBAR))))
		- gMidi->GetMilliSeconds().at(track).cbegin());
}
void StretchScrollBar()
{
	ScrollBar_SetRange(GetDlgItem(gControls, IDC_SCROLLBAR), 0, static_cast<int>(gTracks.empty()
		? 0 : gMidi->GetMilliSeconds().at(*max_element(gTracks.cbegin(), gTracks.cend(),
			[](size_t left, size_t right) { return gMidi->GetMilliSeconds().at(left).back()
				< gMidi->GetMilliSeconds().at(right).back(); })).back()), false);
}
void Controls_OnHScroll(HWND, HWND hCtl, UINT code, int pos)
{
	const auto oldPos(ScrollBar_GetPos(hCtl));
	switch (code)
	{
	case SB_LEFT:			pos = 0;						break;
	case SB_RIGHT: ScrollBar_GetRange(hCtl, nullptr, &pos);	break;
	case SB_LINELEFT:		pos = oldPos - 1'000;			break;
	case SB_LINERIGHT:		pos = oldPos + 1'000;			break;
	case SB_PAGELEFT:		pos = oldPos - 10'000;			break;
	case SB_PAGERIGHT:		pos = oldPos + 10'000;			break;
	case SB_THUMBTRACK: case SB_THUMBPOSITION:
	{
		SCROLLINFO scrollInfo{ sizeof scrollInfo, SIF_TRACKPOS };
		GetScrollInfo(hCtl, SB_CTL, &scrollInfo);
		pos = scrollInfo.nTrackPos;
	}														break;
	case SB_ENDSCROLL:		pos = oldPos;					break;
	default: assert(!"Unhandled scroll bar message");
	}
	ScrollBar_SetPos(hCtl, pos, true);

	gStart += oldPos - pos;
	RewindTracks();
}
void Controls_OnCommand(HWND hDlg, int id, HWND hCtrl, UINT notifyCode)
{
	switch (id)
	{
	case IDB_PLAY:
	{
		const auto leftHand(GetDlgItem(hDlg, IDC_LEFT_HAND)),
			rightHand(GetDlgItem(hDlg, IDC_RIGHT_HAND));
		if (gIsPlaying)
		{
			KillTimer(GetParent(hDlg), 0);
			Button_SetText(hCtrl, TEXT("Play"));
			ComboBox_Enable(leftHand, true);
			ComboBox_Enable(rightHand, true);
			gIsPlaying = false;
		}
		else
		{
			gStart = GetTickCount() - ScrollBar_GetPos(GetDlgItem(hDlg, IDC_SCROLLBAR));
			SetTimer(GetParent(hDlg), 0, gTimerTick, OnTimer);
			Button_SetText(hCtrl, TEXT("Pause"));
			ComboBox_Enable(leftHand, false);
			ComboBox_Enable(rightHand, false);
			gIsPlaying = true;
		}
	}
	break;
	case IDC_TRACKS:
		if (notifyCode == LBN_SELCHANGE)
		{
			vector<int> items(static_cast<size_t>(ListBox_GetSelCount(hCtrl)), 0);
			ListBox_GetSelItems(hCtrl, items.size(), items.data());
			gTracks.clear();
			gTracks.reserve(items.size());
			for (const auto& item : items)
				gTracks.push_back(static_cast<size_t>(ListBox_GetItemData(hCtrl, item)));

			StretchScrollBar();
			RewindTracks();
		}
		break;
	}
}

INT_PTR CALLBACK Controls(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hDlg, WM_INITDIALOG,	Controls_OnInitDialog);
		HANDLE_MSG(hDlg, WM_HSCROLL,	Controls_OnHScroll);
		HANDLE_MSG(hDlg, WM_COMMAND,	Controls_OnCommand);
	default: return false;
	}
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT)
{
	try
	{
		gSound.Init(hWnd);
	}
	catch (const SoundError& e)
	{
		MessageBox(hWnd, (lexical_cast<std::wstring>(e.what())
			+ wstring(TEXT("\nThere will be no sound"))).c_str(), TEXT("Error"), MB_ICONHAND);
	}
	gControls = CreateDialog(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_CONTROLS), hWnd, Controls);
	FORWARD_WM_COMMAND(hWnd, IDM_OPEN, nullptr, 0, SendMessage);
	ShowWindow(gControls, SW_SHOWNORMAL);
	RECT rect{ 0 };
	GetWindowRect(gControls, &rect);
	gDlgWidth = rect.right - rect.left;
	return true;
}
inline void OnDestroy(HWND)
{
	PostQuitMessage(0);
}

inline BOOL OnWindowPosChanging(HWND, LPWINDOWPOS pos)
{
	pos->cy = pos->cx / ASPECT_RATIO;
	return false;
}
void OnMove(HWND hWnd, int, int)
{
	RECT rect{ 0 };
	GetWindowRect(hWnd, &rect);
	SetWindowPos(gControls, HWND_TOP, (rect.left + rect.right - gDlgWidth) / 2, rect.bottom - 8,
		0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
inline void OnSize(HWND hWnd, UINT, int cx, int cy)
{
	gKeyboard.UpdateSize(hWnd, cx, cy);
	gKeyboard.ReleaseAllKeys();
}

void OpenMidiFile(HWND hWnd, LPCTSTR fileName)
{
	if (gIsPlaying)
		FORWARD_WM_COMMAND(gControls, IDB_PLAY, GetDlgItem(gControls, IDB_PLAY), 0, SendMessage);

	const auto midiLog(GetDlgItem(gControls, IDC_MIDI_LOG)),
		leftHand(GetDlgItem(gControls, IDC_LEFT_HAND)),
		rightHand(GetDlgItem(gControls, IDC_RIGHT_HAND)),
		trackList(GetDlgItem(gControls, IDC_TRACKS));
	ComboBox_ResetContent(leftHand);
	ComboBox_ResetContent(rightHand);
	ListBox_ResetContent(trackList);
	Controls_OnInitDialog(gControls, hWnd, 0);

	const auto scrollBar(GetDlgItem(gControls, IDC_SCROLLBAR)),
		playButton(GetDlgItem(gControls, IDB_PLAY));

	char errBuf[0xFF] = "";
	setvbuf(stderr, errBuf, _IOFBF, sizeof errBuf / sizeof *errBuf);
	string log;
	try
	{
		gMidi = make_shared<MidiParser_Facade>(fileName);

		Edit_SetText(midiLog, lexical_cast<wstring>(regex_replace(gMidi->GetLog() + "\nERRORS:\n"
				+ (*errBuf ? errBuf : "None"), regex{ "\n" }, "\r\n").c_str()).c_str());

		for (size_t i(0); i < gMidi->GetTrackNames().size(); ++i)
			if (!gMidi->GetNotes().at(i).empty())
			{
				const auto aName(gMidi->GetTrackNames().at(i));
				wstring wName((wformat{ TEXT("%d ") } % i).str()
					+ wstring(aName.cbegin(), aName.cend()));
				ComboBox_AddString(leftHand, wName.c_str());
				ComboBox_AddString(rightHand, wName.c_str());
				ListBox_AddString(trackList, wName.c_str());

				ComboBox_SetItemData(leftHand, ComboBox_GetCount(leftHand) - 1, i);
				ComboBox_SetItemData(rightHand, ComboBox_GetCount(rightHand) - 1, i);
				ListBox_SetItemData(trackList, ListBox_GetCount(trackList) - 1, i);
			}
		gTracks.clear();
		gIndexes.assign(gMidi->GetNotes().size(), 0);
		StretchScrollBar();
		ScrollBar_SetPos(scrollBar, 0, true);
		ScrollBar_Enable(scrollBar, true);
		Button_Enable(playButton, true);
	}
	catch (const MidiError& e)
	{
		MessageBox(hWnd, lexical_cast<std::wstring>(e.what()).c_str(), TEXT("Error"), MB_ICONHAND);
		Edit_SetText(midiLog, lexical_cast<wstring>(regex_replace("Error opening MIDI file:\n"
			+ string(e.what()) + '\n' + errBuf, regex{ "\n" }, "\r\n").c_str()).c_str());
		ScrollBar_Enable(scrollBar, false);
		Button_Enable(playButton, false);
	}
	setvbuf(stderr, nullptr, _IOFBF, 2);
}
void OnDropFiles(HWND hWnd, HDROP hDrop)
{
	TCHAR fileName[MAX_PATH] = TEXT("");
	DragQueryFile(hDrop, 0, fileName, sizeof fileName / sizeof *fileName);
	OpenMidiFile(hWnd, fileName);
	DragFinish(hDrop);
}
void OnCommand(HWND hWnd, int id, HWND, UINT)
{
	switch (id)
	{
	case IDM_OPEN:
	{
		OPENFILENAME fileName{ sizeof fileName, hWnd };
		fileName.lpstrFilter = TEXT("MIDI files (*.mid, *.midi)\0*.mid*\0All files\0*.*\0");
		TCHAR buf[MAX_PATH] = TEXT("");
		fileName.lpstrFile = buf;
		fileName.nMaxFile = sizeof buf / sizeof *buf;
		fileName.Flags = OFN_FILEMUSTEXIST;
		if (GetOpenFileName(&fileName)) OpenMidiFile(hWnd, fileName.lpstrFile);
	}
	break;
	case IDM_ABOUT:
		DialogBox(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}
}

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	const auto hdc(BeginPaint(hWnd, &ps));
	{
		gKeyboard.Draw(hdc);
		gKeyboard.ReleaseAllKeys();
	}
	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE,				OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY,			OnDestroy);

		HANDLE_MSG(hWnd, WM_WINDOWPOSCHANGING,	OnWindowPosChanging);
		HANDLE_MSG(hWnd, WM_MOVE,				OnMove);
	case WM_SIZING:								OnMove(hWnd, 0, 0); return FALSE; break;
		HANDLE_MSG(hWnd, WM_SIZE,				OnSize);

		HANDLE_MSG(hWnd, WM_DROPFILES,			OnDropFiles);
		HANDLE_MSG(hWnd, WM_COMMAND,			OnCommand);

		HANDLE_MSG(hWnd, WM_PAINT,				OnPaint);

	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

ATOM RegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex{ sizeof wcex };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LARGE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MENU);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
	return RegisterClassEx(&wcex);
}
inline BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	const auto hWnd(CreateWindowEx(WS_EX_ACCEPTFILES, szWindowClass, TEXT("Piano Fingers"),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
		HWND_DESKTOP, nullptr, hInstance, nullptr));
	if (!hWnd) return FALSE;

	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
#ifdef _DEBUG
	UNREFERENCED_PARAMETER(nCmdShow);
#else
	ShowWindow(hWnd, nCmdShow);
#endif
	UpdateWindow(hWnd);

	return TRUE;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
	RegisterClass(hInstance);
	if (!InitInstance(hInstance, nCmdShow)) return FALSE;

    const auto hAccelTable(LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MENU)));
	MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)
			&& !IsDialogMessage(gControls, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    return static_cast<int>(msg.wParam);
}