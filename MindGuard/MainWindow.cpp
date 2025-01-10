#include "MainWindow.h"

MainWindow::MainWindow() : eventHandler{ new EventHandler() } {
    loader = new ResourceLoader(sPATH_WORD, sPATH_EXE, *eventHandler);
    actionHandler = new ActionHandler(*eventHandler);
    actionDetector = new ActionDetector(iMIN_WORD_LENGTH, iMAX_WORD_LENGTH, *actionHandler, *eventHandler);
}

MainWindow::~MainWindow()
{
    onDestroy();
}

bool MainWindow::create(HINSTANCE hInstance)
{
    WNDCLASSW mainWindow = { 0 };
    mainWindow.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    mainWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
    mainWindow.hInstance = hInstance;
    mainWindow.lpszClassName = wsWINDOW_CLASS_NAME;
    mainWindow.lpfnWndProc = &MainWindow::WindowProc;
    mainWindow.style = CS_HREDRAW | CS_VREDRAW;
    mainWindow.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUARD_ICON));
    if (!RegisterClassW(&mainWindow)) return false;
    hWnd = CreateWindowW(wsWINDOW_CLASS_NAME, wsWINDOW_MAIN_NAME,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, wmWidth, wmHeight,
        nullptr, nullptr,
        hInstance,
        this);
    if (hWnd == nullptr) return false;
    if (!loader->assertExists()) return false;
    eventHandler->setHandle(hWnd);
    ZeroMemory(&nidTray, sizeof(nidTray));
    nidTray.cbSize = sizeof(NOTIFYICONDATA);
    nidTray.hWnd = hWnd;
    nidTray.uID = iWM_TRAY_ID;
    nidTray.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nidTray.uCallbackMessage = iWM_TRAY_SHOW;
    nidTray.hIcon = (HICON)LoadImage(
        hInstance,
        MAKEINTRESOURCE(IDI_GUARD_ICON),
        IMAGE_ICON,
        16, 16,
        LR_DEFAULTCOLOR
    );
    wcscpy_s(nidTray.szTip, wsWINDOW_MAIN_NAME);
    return true;
}

bool MainWindow::init() const
{
    if (!loader->assertExists()) {
        eventHandler->handleError(ERROR_RESOURCE_FILE_DOES_NOT_EXIST);
        return false;
    }
    return true;
}

void MainWindow::onCreate()
{
    std::vector<std::string> wordKeys;
    std::vector<std::string> exeKeys;
    if (!loader->loadAvaliableKeys(wordKeys, exeKeys)) {
        PostQuitMessage(0);
        return;
    }

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    wchar_t columnHeadWord[] = L"Word Keys";
    wchar_t columnHeadExe[] = L"Executable Keys";
    createListView(hWordList, wordKeys, listViewX, listViewY, columnHeadWord);
    int lv2Y = listViewY + listViewHeight + PADDING;
    createListView(hExeList,exeKeys, listViewX, lv2Y, columnHeadExe);

    int chBY = lv2Y + listViewHeight + PADDING;
    createEdit(hStopWordEdit, secretWordX, chBY);
    createCheckBox(hStopWordCheckBox, iWM_CHECKBOX_STOP_WORD_ID, wsWM_CHECKBOX_STOP_WORD_TEXT, secretWordX + secretWordCheckBoxWidth + PADDING, chBY);
    
    int rBY = chBY + secretCheckBoxHeight + PADDING;
    createButton(hRunStopButton, iWM_BUTTON_RUN_STOP_ID, wsWM_BUTTON_RUN_TEXT, listViewX, rBY);

    createStatic(hInfo, listViewX + listViewWidth + PADDING, listViewY, wsWM_INFO_TEXT);
}

void MainWindow::onDestroy()
{
    if (eventHandler) delete eventHandler;
    if (loader) delete loader;
    if (actionDetector) delete actionDetector;
    if (actionHandler) delete actionHandler;
    eventHandler = nullptr;
    actionDetector = nullptr;
    loader = nullptr;
    actionHandler = nullptr;
}

void MainWindow::onRunButtonClick()
{
    bool wasSet{ false };
    if (SendMessage(hStopWordCheckBox, BM_GETCHECK, 0, 0) == BST_CHECKED) {
        wchar_t textBuffer[32];
        GetWindowText(hStopWordEdit, textBuffer, std::size(textBuffer));
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, textBuffer, -1, nullptr, 0, nullptr, nullptr);
        std::string secretWord(bufferSize, 0);
        WideCharToMultiByte(CP_UTF8, 0, textBuffer, -1, &secretWord[0], bufferSize, nullptr, nullptr);
        secretWord = secretWord.c_str();
        if (!loader->checkValidWord(secretWord)) {
            SetWindowText(hRunStopButton, wsWM_BUTTON_RUN_TEXT);
            eventHandler->handleError(ERROR_SECRET_WORD_INVALID);
            return;
        }
        std::transform(secretWord.begin(), secretWord.end(), secretWord.begin(), [](unsigned char c) { return std::toupper(c); });
        actionDetector->setSecretWord(std::move(secretWord));
        wasSet = true;
    }

    std::unordered_set<std::string, StringHash, std::equal_to<>> loadedExeSet;
    auto fetchedSelectedExeList = fetchAllCheckedItems(hExeList);
    if (fetchedSelectedExeList.empty()) {
        SetWindowText(hRunStopButton, wsWM_BUTTON_RUN_TEXT);
        eventHandler->handleError(ERROR_NO_VALUES_SELECTED);
        return;
    }
    if (!loader->loadTerminationSets(fetchedSelectedExeList, loadedExeSet)) return;
    std::unordered_set<std::string, StringHash, std::equal_to<>> loadedWordSet;
    auto fetchedSelectedWordList = fetchAllCheckedItems(hWordList);
    if (fetchedSelectedWordList.empty()) {
        SetWindowText(hRunStopButton, wsWM_BUTTON_RUN_TEXT);
        eventHandler->handleError(ERROR_NO_VALUES_SELECTED);
        return;
    }
    std::array<bool, iMAX_WORD_LENGTH + 1> loadedLengthArray = { false };
    size_t loadedMinSize{ iMIN_WORD_LENGTH };
    size_t loadedMaxSize{ iMAX_WORD_LENGTH };
    if (!loader->loadSubResources(fetchedSelectedWordList, loadedMinSize,
        loadedMaxSize, loadedWordSet, loadedLengthArray)) return;

    actionHandler->setValue(std::move(loadedExeSet));
    actionDetector->setValues(std::move(loadedWordSet), std::move(loadedLengthArray),
        loadedMinSize, loadedMaxSize);
    if (!actionDetector->startDetection()) {
        SetWindowText(hRunStopButton, wsWM_BUTTON_RUN_TEXT); return;
    }
    if (wasSet) ShowWindow(hWnd, SW_HIDE);
    isRunning = true;
    MessageBoxW(hWnd, L"Detection started", L"Detection has started", MB_ICONINFORMATION);
}

void MainWindow::onStopButtonClick()
{
    actionDetector->endDetection();
    isRunning = false;
}

void MainWindow::createListView(HWND& hListHandle, std::vector<std::string>& values, int atX, int atY, wchar_t columnHead[])
{
    hListHandle = CreateWindowEx(0, WC_LISTVIEW, nullptr,
        WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS | WS_BORDER,
        atX, atY, listViewWidth, listViewHeight,
        hWnd, nullptr,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
    ListView_SetExtendedListViewStyle(hListHandle, LVS_EX_CHECKBOXES);
    LVCOLUMN lvCol = { 0 };
    lvCol.mask = LVCF_TEXT | LVCF_WIDTH;
    lvCol.pszText = columnHead;
    lvCol.cx = listViewWidth;
    ListView_InsertColumn(hListHandle, 0, &lvCol);
    LVITEM lvItem = { 0 };
    lvItem.mask = LVIF_TEXT;
    for (int i{ 0 }; i < values.size(); i++) {
        lvItem.pszText = CStringW(CA2W(values[i].c_str(), CP_UTF8)).GetBuffer();
        lvItem.iItem = i;
        ListView_InsertItem(hListHandle, &lvItem);
    }
}

std::vector<std::string> MainWindow::fetchAllCheckedItems(const HWND& hListHandle) const
{
    auto totalCount = ListView_GetItemCount(hListHandle);
    std::vector<std::string> checkedItems;
    for (int i{ 0 }; i < totalCount; i++) {
        if (ListView_GetCheckState(hListHandle, i)) {
            wchar_t buffer[iMAX_ALLOWED_KEY_SIZE+1];
            ListView_GetItemText(hListHandle, i, 0, buffer, std::size(buffer));
            int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
            std::string keyName(bufferSize, 0);
            WideCharToMultiByte(CP_UTF8, 0, buffer, -1, &keyName[0], bufferSize, nullptr, nullptr);
            checkedItems.push_back(std::move(keyName));
        }
    }
    return checkedItems;
}

void MainWindow::createButton(HWND& hButtonHandle, int buttonId, const wchar_t* text, int atX, int atY)
{
    hButtonHandle = CreateWindowEx(0, WC_BUTTON, text, 
        WS_VISIBLE | WS_CHILD | WS_TABSTOP,
        atX, atY, buttonWidth, buttonHeight,
        hWnd, (HMENU)buttonId,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
    if (!hButtonHandle) PostQuitMessage(0);
}

void MainWindow::createEdit(HWND& hEditHandle, int atX, int atY)
{
    hEditHandle = CreateWindowEx(0, WC_EDIT, L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        atX, atY, secretWordCheckBoxWidth,  secretCheckBoxHeight,
        hWnd, nullptr,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
    if (!hEditHandle) PostQuitMessage(0);
}

void MainWindow::createCheckBox(HWND& hCheckBoxHandle, int checkBoxId, const wchar_t* text, int atX, int atY)
{
    hCheckBoxHandle = CreateWindow(WC_BUTTON, text,
        WS_TABSTOP | WS_VISIBLE | WS_CHILD| BS_AUTOCHECKBOX,
        atX, atY, secretWordCheckBoxWidth + 70, secretCheckBoxHeight,
        hWnd, (HMENU)checkBoxId,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
    if (!hCheckBoxHandle) PostQuitMessage(0);
}

void MainWindow::createStatic(HWND& hStaticHandle, int atX, int atY, const std::wstring& text)
{
    hStaticHandle = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, text.c_str(),
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_READONLY | ES_MULTILINE,
        atX, atY, infoWidth,infoHeight,
        hWnd, nullptr,
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
    if (!hStaticHandle) PostQuitMessage(0);
}
