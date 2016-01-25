#include "stdafx.h"
#include "MainWindow.h"
#include "Controls.h"
#include "About.h"

#include "Piano.h"
#include "Canvas.h"
#include "StdErrBuffer.h"

#include "MidiParser\MidiParser_Facade.h"
#include "MidiParser\MidiError.h"
#include "Keyboard.h"
#include "PianoSound\Sound_Facade.h"
#include "PianoSound\SoundError.h"

using namespace boost;

HWND MainWindow::hWndMain = nullptr;
int MainWindow::dlgWidth_ = 0;

BOOL MainWindow::OnCreate(const HWND hWnd, LPCREATESTRUCT)
{
	try
	{
		Piano::sound->Init(hWnd);
	}
	catch (const SoundError& e)
	{
		MessageBox(hWnd, (lexical_cast<String>(e.what())
			+ String(TEXT("\nThere will be no sound"))).c_str(), TEXT("Error"), MB_ICONHAND);
	}
	Controls::hDlgControls = CreateDialog(GetWindowInstance(hWnd),
		MAKEINTRESOURCE(IDD_CONTROLS), hWnd, Controls::Main);
	FORWARD_WM_COMMAND(hWnd, IDM_OPEN, nullptr, 0, SendMessage);
	ShowWindow(Controls::hDlgControls, SW_SHOWNORMAL);
	RECT rect{ 0 };
	GetWindowRect(Controls::hDlgControls, &rect);
	dlgWidth_ = rect.right - rect.left;
	return true;
}

void MainWindow::OnMove(const HWND hWnd, int, int)
{
	RECT rect{ 0 };
	GetWindowRect(hWnd, &rect);
	SetWindowPos(Controls::hDlgControls, HWND_TOP, (rect.left + rect.right - dlgWidth_) / 2,
		rect.bottom - 8, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
void MainWindow::OnSize(const HWND hWnd, UINT, const int cx, const int cy)
{
	Piano::keyboard->UpdateSize(hWnd, cx, cy);
	Piano::keyboard->ReleaseAllKeys();
}

void MainWindow_OnKey(HWND, UINT vk, BOOL, int, UINT)
{
	if (vk == VK_TAB) SetFocus(Controls::hDlgControls);
}

void MainWindow::OpenMidiFile(const HWND hWnd, const LPCTSTR fileName)
{
	using namespace std;

	Controls::Reset();
	StdErrBuffer errBuf;
	try
	{
		Piano::midi = make_unique<MidiParser_Facade>(fileName);
		Edit_SetText(Controls::midiLog, lexical_cast<String>(regex_replace(
			Piano::midi->GetLog() + "\nERRORS:\n" + (errBuf.Get().empty() ? "None" : errBuf.Get()),
				regex{ "\n" }, "\r\n").c_str()).c_str());

		for (size_t i(0); i < Piano::midi->GetTrackNames().size(); ++i)
			if (!Piano::midi->GetNotes().at(i).empty())
			{
				const auto aName(Piano::midi->GetTrackNames().at(i));
				String wName((Format{ TEXT("%d ") } % i).str()
					+ String(aName.cbegin(), aName.cend()));
				ComboBox_AddString(Controls::leftHand, wName.c_str());
				ComboBox_AddString(Controls::rightHand, wName.c_str());
				ListBox_AddString(Controls::trackList, wName.c_str());

				ComboBox_SetItemData(Controls::leftHand, ComboBox_GetCount(Controls::leftHand) - 1, i);
				ComboBox_SetItemData(Controls::rightHand, ComboBox_GetCount(Controls::rightHand) - 1, i);
				ListBox_SetItemData(Controls::trackList, ListBox_GetCount(Controls::trackList) - 1, i);
			}
		SendMessage(Controls::progressLeft, PBM_SETPOS, 0, 0);
		SendMessage(Controls::progressRight, PBM_SETPOS, 0, 0);

		Piano::tracks.clear();
		Piano::indexes.assign(Piano::midi->GetNotes().size(), 0);
		Piano::leftTrack.reset();
		Piano::rightTrack.reset();
		ZeroMemory(&Piano::hands, sizeof Piano::hands);

		ScrollBar_SetRange(Controls::scrollBar, 0, static_cast<int>(max_element(
			Piano::midi->GetMilliSeconds().cbegin(), Piano::midi->GetMilliSeconds().cend(),
			[](const vector<unsigned>& left, const vector<unsigned>& right)
			{
				return right.empty() ? false : left.empty() ? true : left.back() < right.back();
			}
		)->back()), false);
		ScrollBar_SetPos(Controls::scrollBar, 0, true);
		Button_Enable(Controls::playButton, true);
	}
	catch (const MidiError& e)
	{
		MessageBox(hWnd, lexical_cast<String>(e.what()).c_str(), TEXT("Error"), MB_ICONHAND);
		Edit_SetText(Controls::midiLog,
			lexical_cast<String>(regex_replace("Error opening MIDI file:\n"
				+ string(e.what()) + '\n' + errBuf.Get(), regex{ "\n" }, "\r\n").c_str()).c_str());
		Button_Enable(Controls::playButton, false);
	}
}
void MainWindow::OnDropFiles(HWND hWnd, HDROP hDrop)
{
	TCHAR fileName[MAX_PATH] = TEXT("");
	DragQueryFile(hDrop, 0, fileName, sizeof fileName / sizeof *fileName);
	OpenMidiFile(hWnd, fileName);
	DragFinish(hDrop);
}
void MainWindow::OnCommand(HWND hWnd, int id, HWND, UINT)
{
	switch (id)
	{
	case IDM_OPEN:
	{
		OPENFILENAME fileName{ sizeof fileName, hWnd };
		fileName.lpstrFilter = TEXT("MIDI files (*.mid)\0")	TEXT("*.mid*\0")
							TEXT("Karaoke files (*.kar)\0")	TEXT("*.kar*\0")
								TEXT("All files\0")			TEXT("*.*\0");
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
	}
}

void MainWindow::OnPaint(const HWND hWnd)
{
	Canvas canvas(hWnd);
	Piano::keyboard->Draw(canvas);
	Piano::keyboard->ReleaseAllKeys();
}

LRESULT CALLBACK MainWindow::WndProc(const HWND hWnd, const UINT message,
	const WPARAM wParam, const LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE,				OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY,			OnDestroy);

		HANDLE_MSG(hWnd, WM_WINDOWPOSCHANGING,	OnWindowPosChanging);
		HANDLE_MSG(hWnd, WM_MOVE,				OnMove);
	case WM_SIZING:								OnMove(hWnd, 0, 0); return FALSE; break;
		HANDLE_MSG(hWnd, WM_SIZE,				OnSize);

		HANDLE_MSG(hWnd, WM_KEYDOWN,			MainWindow_OnKey);

		HANDLE_MSG(hWnd, WM_DROPFILES,			OnDropFiles);
		HANDLE_MSG(hWnd, WM_COMMAND,			OnCommand);

		HANDLE_MSG(hWnd, WM_PAINT,				OnPaint);

	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}