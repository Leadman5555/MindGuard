#include "ActionDetector.h"

ActionDetector* ActionDetector::thisWrapper = nullptr;
HHOOK ActionDetector::hHook = nullptr;

void ActionDetector::setValues(
    std::unordered_set<std::string, StringHash, std::equal_to<>>&& wordValues,
    std::array<bool, iMAX_WORD_LENGTH + 1>&& lengthArray,
    size_t minLength,
    size_t maxLength
)
{
    forbiddenSet = std::move(wordValues);
    presentLength = std::move(lengthArray);
    minWordLength = minLength;
    maxWordLength = maxLength;
}

void ActionDetector::setSecretWord(std::string&& stopKeyWord)
{
    stopWord = std::move(stopKeyWord);
}

bool ActionDetector::startDetection()
{
    if (forbiddenSet.empty() || presentLength.empty()) return false;
    if (!stopWord.empty()) {
        size_t length = stopWord.length();
        forbiddenSet.insert(stopWord);
        if (minWordLength > length) minWordLength = length;
        else if (maxWordLength < length) maxWordLength = length;
        presentLength[length] = true;
    }
    bool wasSet = setHook();
    if (!wasSet) eventHandler.handleError(ERROR_CANNOT_SET_HOOK);
    return wasSet;
}

void ActionDetector::endDetection()
{
    UnhookWindowsHookEx(hHook);
}

LRESULT ActionDetector::KeyPressedWrapper(int nCode, WPARAM wParam, LPARAM lParam)
{
    return thisWrapper->KeyPressed(nCode, wParam, lParam);
}

LRESULT CALLBACK ActionDetector::KeyPressed(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        auto pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        if (wParam == WM_KEYDOWN) {
            const auto& code = pKeyboard->vkCode;
            if (code == 0x08) safePop();
            else if(code >= 0x30 && code <= 0x5A) {
                inputChain += (char)code;
                if (!isValid()) {
                    inputChain.clear();
                    if (!handler.handleAction()) {
                        eventHandler.handleError(ERROR_INVALID_PROCESS_HANDLE);
                        endDetection();
                    }
                }
            }
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

bool ActionDetector::setHook()
{
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyPressedWrapper, GetModuleHandle(nullptr), 0);
    return hHook != nullptr;
}

void ActionDetector::safePop()
{
    if (inputChain.length() > 0) inputChain.pop_back();
}

bool ActionDetector::isValid()
{
    size_t length{ inputChain.length() };
    for (size_t start{ (length > maxWordLength) ? (length - maxWordLength) : 0 }; start < length; start++)
    {
        size_t suggestedBound{ length - start };
        size_t bound{ (maxWordLength < suggestedBound) ? maxWordLength : suggestedBound };
        for (size_t len = minWordLength; len <= bound; len++) {
            if (!presentLength[len]) continue;
            if (forbiddenSet.contains(inputChain.substr(start, len))) {
                if (inputChain.substr(start, len) == stopWord) {
                    endDetection();
                    eventHandler.handleCode(EVENT_SHOW_WINDOW);
                    return true;
                }
                return false;
            }
        }
    }
    if (inputChain.length() >= iMAX_ALLOWED_MEM_SIZE) inputChain = inputChain.substr(iMAX_ALLOWED_MEM_SIZE - maxWordLength);
    return true;
}
