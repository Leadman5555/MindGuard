#pragma once
#include "ActionDetector.h"
#include "ResourceLoader.h"
#include "WindowConstants.h"
#include "resource.h"
#include <CommCtrl.h>
#include <atlstr.h>
#include <shellapi.h>
#pragma comment(lib, "Comctl32.lib")
class MainWindow
{
public:
    MainWindow();
    ~MainWindow();
    static inline LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
        MainWindow* self = nullptr;
        if (msg == WM_NCCREATE) {
            auto createStruct = reinterpret_cast<CREATESTRUCT*>(lp);
            self = reinterpret_cast<MainWindow*>(createStruct->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
            self->hWnd = hWnd;
        }
        else {
            self = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        }
        if (self) {
            switch (msg) {
                case WM_CREATE: {
                    self->onCreate();
                    break;
                }
                case WM_SYSCOMMAND:
                    if ((wp & 0xFFF0) == SC_MINIMIZE) {
                        ShowWindow(hWnd, SW_HIDE);
                        Shell_NotifyIcon(NIM_ADD, &(self->nidTray));
                        return 0;
                    }
                    break;
                case WM_COMMAND: {
                    switch (LOWORD(wp))
                    {
                        case iWM_BUTTON_RUN_STOP_ID: {
                            auto handle = (HWND)lp;
                            wchar_t textBuffer[16];
                            GetWindowText(handle, textBuffer, std::size(textBuffer));
                            if (wcscmp(textBuffer, wsWM_BUTTON_RUN_TEXT) == 0) {
                                SetWindowText(handle, wsWM_BUTTON_STOP_TEXT);
                                self->onRunButtonClick();
                            }
                            else {
                                SetWindowText(handle, wsWM_BUTTON_RUN_TEXT);
                                self->onStopButtonClick();
                            }
                            break;
                        }
                        case iWM_CHECKBOX_STOP_WORD_ID: {
                            if (SendMessage((HWND)lp, BM_GETCHECK, 0, 0) == BST_CHECKED)
                                MessageBox(hWnd, wsWM_CHECKBOX_STOP_WORD_MSG, L"F enabled", MB_OK);
                        
                            break;
                        }
                    }
                    break;
                }
                case WM_SHOWWINDOW: {
                    if (wp == TRUE && self->isRunning){
                        SetWindowText(self->hRunStopButton, wsWM_BUTTON_RUN_TEXT);
                        self->onStopButtonClick();
                    }
                    break;
                }
                case iWM_TRAY_SHOW: {
                    if (lp == WM_LBUTTONDOWN || lp == WM_RBUTTONDOWN) {
                        bool trueState{ self->isRunning };
                        self->isRunning = false;
                        ShowWindow(hWnd, SW_SHOW);
                        self->isRunning = trueState;
                        ShowWindow(hWnd, SW_RESTORE);
                        DestroyIcon(self->nidTray.hIcon);
                        Shell_NotifyIcon(NIM_DELETE, &(self->nidTray));
                    }
                    break;
                }
                case WM_DESTROY: {
                    self->onDestroy();
                    DestroyIcon(self->nidTray.hIcon);
                    Shell_NotifyIcon(NIM_DELETE, &(self->nidTray));
                    PostQuitMessage(0);
                    return 0;
                }
            }
        }
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
    bool create(HINSTANCE hInstance);
private:
    HWND hWnd = nullptr;
    HWND hWordList = nullptr;
    HWND hExeList = nullptr;
    HWND hRunStopButton = nullptr;
    HWND hStopWordEdit = nullptr;
    HWND hStopWordCheckBox = nullptr;
    HWND hInfo = nullptr;
    NOTIFYICONDATA nidTray = { 0 };
    EventHandler* eventHandler = nullptr;
    ResourceLoader* loader = nullptr;
    ActionHandler* actionHandler = nullptr;
    ActionDetector* actionDetector = nullptr;
    bool isRunning = false;
    bool init() const;
    void onCreate();
    void onDestroy();
    void onRunButtonClick();
    void onStopButtonClick();
    void createListView(HWND& hListHandle, std::vector<std::string>& values, int atX, int atY, wchar_t columnHead[]);
    std::vector<std::string> fetchAllCheckedItems(const HWND& hListHandle) const;
    void createButton(HWND& hButtonHandle, int buttonId, const wchar_t* text, int atX, int atY);
    void createEdit(HWND& hEditHandle, int atX, int atY);
    void createCheckBox(HWND& hCheckBoxHandle, int checkBoxId, const wchar_t* text, int atX, int atY);
    void createStatic(HWND& hStaticHandle, int atX, int atY, const std::wstring& text);
};

