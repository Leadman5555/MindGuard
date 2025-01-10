#include "ActionHandler.h"

BOOL ActionHandler::handleAction() const
{
    HANDLE hProccessSnapshot;
    PROCESSENTRY32 pe32;
    hProccessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProccessSnapshot == INVALID_HANDLE_VALUE) return FALSE;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProccessSnapshot, &pe32)) {
        CloseHandle(hProccessSnapshot);
        return FALSE;
    }
    int terminatedCount{ 0 };
    auto totalCount = (int) toTerminate.size();
    do {
        auto nameAsWCharArray = pe32.szExeFile;
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, nameAsWCharArray, -1, nullptr, 0, nullptr, nullptr);
        std::string executableName(bufferSize, 0);
        WideCharToMultiByte(CP_UTF8, 0, nameAsWCharArray, -1, &executableName[0], bufferSize, nullptr, nullptr);
        if (toTerminate.contains(executableName.substr(0, executableName.find_last_of('.')))) {
            if (terminateGivenProcess(pe32.th32ProcessID, 1) == 0)
                eventHandler.handleError(ERROR_CANNOT_CLOSE_PROCESS);
            terminatedCount++;
        }
    } while (terminatedCount < totalCount && Process32Next(hProccessSnapshot, &pe32));
    CloseHandle(hProccessSnapshot);
    return TRUE;
}

void ActionHandler::setValue(std::unordered_set<std::string, StringHash, std::equal_to<>>&& terminateOnAction)
{
    toTerminate = std::move(terminateOnAction);
}

BOOL ActionHandler::terminateGivenProcess(DWORD dwProcessId, UINT uExitCode) const
{
    DWORD dwNeededAccessLevel = PROCESS_TERMINATE;
    HANDLE hProcess = OpenProcess(dwNeededAccessLevel, FALSE, dwProcessId);
    if (hProcess == nullptr) return FALSE;
    BOOL result = TerminateProcess(hProcess, uExitCode);
    CloseHandle(hProcess);
    return result;
}
