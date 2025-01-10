#pragma once
#include <string>
constexpr const wchar_t* wsWINDOW_CLASS_NAME = L"MindGuardWindow";
constexpr const wchar_t* wsWINDOW_MAIN_NAME = L"MindGuard";
constexpr int iWM_BUTTON_RUN_STOP_ID = 1;
constexpr const wchar_t*  wsWM_BUTTON_RUN_TEXT= L"Run guard";
constexpr const wchar_t* wsWM_BUTTON_STOP_TEXT = L"Stop guard";
constexpr int iWM_CHECKBOX_STOP_WORD_ID = 2;
constexpr const wchar_t* wsWM_CHECKBOX_STOP_WORD_TEXT = L"Run with escape word";
constexpr const wchar_t* wsWM_CHECKBOX_STOP_WORD_MSG =L"Secret word is enabled. The window will hide during detection.";
constexpr int iWM_TRAY_ID = 3;
constexpr int iWM_TRAY_SHOW = 4;
static const std::wstring wsWM_INFO_TEXT = L"\t\t\t\tMIND GUARD\r\nAdditional information:\r\n"
L"\t1. Do I need Windows to run it?\r\n If you didn't have Windows 10 or 11 already, you wouldn't be reading it. But yes, it works only on Windows. It should run without problems on the older versions of Windows too.\r\n"
L"\t2. Do I need to disable antiviruses and/or Windows Defender to run it?\r\nNo. It doesn't carry any malicous code and thus will not trigger any antivirus reaction.\r\n"
L"\t3. How to use the app?\r\nWord sets pointed to by selected word keys will become tracked input. Should multiple sets be selected, a set union will be performed. If a series of characters, letters (case insensetive) or numbers, is entered and it matches one of the words signal will be sent. Backspace key works.\r\n"
L"When signal is sent, all of the executables which names are pointed to by selected exe keys will be terminated. In case multiple exe sets are selected, a set union is also performed.\r\n"
L"\t4. How do I set my own data sets?\r\nUpdate the data in resource files.'words_resource.json' is for word sets, 'executables_resource.json' for exe sets.\r\nEnter set name as a JSON string key and names under that key in json string array format.\r\nDo not change the file directory (same parent folder as MindGuard.exe) or names of the .json files. Good practice is to use only letters and numbers for key names. Entered words are checked automatically.\r\nWhen creating your own data sets you don't need to use white-spaces, new lines or indentations - it's simply more readable that way. The important characters are the quotes, brackets and commas.\r\nJSONs nested more that one level deep are not supported.\r\n"
L"\t5. What does 'secret word' do?\r\nBy default, when minimized, the app hides in system tray (up-arrow icon, left of your desktop clock). However, you can hide it completely by defining a secret word and checking the box.\r\nWhen run with a secret word, the app will be hidden during detection. If the secret word is typed, the app will show itself again and stop detecting.\r\nYou can always find the running app in the TaskMgr process list as 'MindGuard.exe'.\r\n"
L"\t6. I ran the app and got an error message, what should I do?\r\nAny common error that could happen of is handled correctly - the error message tells you what failed and gives a suggestion on what to change.\r\nIf the error text is a garbage value, it's most likely an internal Windows error. Good luck then!\r\n"
L"\t7. Is the app resource intensive?\r\nNo. When running detection, even if you add hundreds of words, it will still use less system resources than opened Notepad and a lot less than a single browser tab. You will never even notice that the app is running in background. As for disk size, it's equally negligible - app weights merely around 150 KB, thats more than 15 times less than a single average photo.\r\n"
L"\t8. Does the app require ADMIN level privledge to run?\r\nYes. It needs to be ether run as Admin or on an ADMIN user account. If the latter, you do not have to click 'Run as adminstrator' although it's still encouraged.\r\n"
L"\t9. What is this program written in?\r\nThe programm is written in C++20, but mostly uses C-Style Windows and Shell APIs to work. As such using a new C++ standard may seem pointless and probably is.\r\n"
L"\t10. Why are JSON files used as the storage format?\r\nBecause they seem to be the best compromise between offering the most flexiblity, unneeded scalability and being idiot-proof. Working with .txt files is a bother and other options are not so user-friendly.\r\n"
L"\t11. Why are the words limited to 15 characters?\r\nYou are unlikely to need a longer one. Remember that it matches the prefix, after all. It's mainly a limit to keep it lighting fast. A rather surprising fact for most people would be that the detection would work faster with thousands of words sharing the same length than just a dozen of them with all having different length; Not that anyone would notice any difference.\r\n"
L"\t12. What is the most beneficaial way of using this program?\r\nIt can help you keep focused on what you should be. If you find yourself distracted and absentmindedly typing in 'youtube.com', 'tiktok' in browser or 'steam' in the search bar, then you can use this application to keep yourself from doing that.\r\n"
L"\t13. What is the most nefarious way of using this program?\r\nFinding a personal account of someone with Admin level prviledge, setting parametrs for automatic console start (without the UI), setting a facade shortcut with an underlying batch script (linking it as a DLL - advanced version) that runs this program hidden in the background. Add as forbidden words their passwords to various sites. In addition, blacklist all words that someone may use while seeking help on self-closing applications and viruses. Now they cannot login anywhere or seek help online. It probably would work on someone even with a higher degree of computer literacy, at least for a while.\r\n"
L"\t13. Why is the UI so atrocious?\r\nBecause it's made in native Windows API. It's incredibly efficient, fast and lightweight - that app weights merely around 150 KB, thats more than 15 times less than a single average photo.\r\nAlso, your computer is very happy to render it - it feels like home to it. You should also be happy to see it, after all, aren't shapes so... rectangular and grey?";

static const std::string sPATH_WORD = "words_resource.json";
static const std::string sPATH_EXE = "executables_resource.json";

constexpr int wmWidth = 800;
constexpr int wmHeight = 780;
constexpr int secretCheckBoxHeight = 50;
constexpr int secretWordX = 10;
constexpr int secretWordCheckBoxWidth= 100;
constexpr int listViewX = 10;
constexpr int listViewY = 10;
constexpr int listViewWidth = 200;
constexpr int listViewHeight = 300;
constexpr int buttonWidth = 150;
constexpr int buttonHeight = 50;
constexpr int PADDING = 5;
constexpr int infoWidth = wmWidth - (listViewX + PADDING + listViewWidth + 5 * PADDING);
constexpr int infoHeight = listViewHeight + listViewHeight + PADDING;