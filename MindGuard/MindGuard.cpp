#include <Windows.h>
#include "MainWindow.h"

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPWSTR arg,_In_ int nCmdShow) {
    MainWindow mainWindow;
    if (!mainWindow.create(hInst)) return -1;
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}