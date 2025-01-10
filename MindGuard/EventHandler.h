#pragma once
#include "EventCodes.h"
#include <Windows.h>
class EventHandler
{
public:
	explicit EventHandler() = default;
	void setHandle(const HWND& hWndToSet);
	void handleError(short errorCode) const;
	void handleCode(short code) const;
private:
	HWND hWnd = nullptr;
};

