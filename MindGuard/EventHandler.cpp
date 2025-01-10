#include "EventHandler.h"

void EventHandler::setHandle(const HWND& hWndToSet)
{
	hWnd = hWndToSet;
}

void EventHandler::handleError(short errorCode) const
{
	MessageBoxW(hWnd, ERROR_MESSAGES[errorCode], L"Error occurred", MB_ICONERROR);
}

void EventHandler::handleCode(short code) const
{
	if (code == EVENT_SHOW_WINDOW) ShowWindow(hWnd, SW_SHOW);
}
