#pragma once
#include <Windows.h>
#include <iostream>
#include <unordered_set>
#include "StringHash.h"
#include <process.h>
#include <TlHelp32.h>
#include <tchar.h>
#include "EventHandler.h"
class ActionHandler
{
public:
	explicit ActionHandler(const EventHandler& eventHandler):
		eventHandler{eventHandler} {}
	BOOL handleAction() const;
	void setValue(std::unordered_set<std::string, StringHash, std::equal_to<>>&& terminateOnAction);
private:
	BOOL terminateGivenProcess(DWORD dwProcessId, UINT uExitCode) const;
	std::unordered_set<std::string, StringHash, std::equal_to<>> toTerminate;
	const EventHandler& eventHandler;
};

