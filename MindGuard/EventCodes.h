#pragma once
#include <array>
#include <string>
#include <iostream>
#include "ResourceConstants.h"
constexpr short iERROR_COUNT = 15;
constexpr short ERROR_RESOURCE_FILE_DOES_NOT_EXIST = 0;
constexpr short ERROR_EMPTY_KEY_WORD = 1;
constexpr short ERROR_EMPTY_KEY_EXE = 2;
constexpr short ERROR_WORD_LENGTH_INVALID = 3;
constexpr short ERROR_CANNOT_SET_HOOK = 4;
constexpr short ERROR_INVALID_PROCESS_HANDLE = 5;
constexpr short ERROR_CANNOT_CLOSE_PROCESS = 6;
constexpr short ERROR_INVALID_JSON_STRUCTURE = 7;
constexpr short ERROR_INVALID_JSON_KEY_NAME = 8;
constexpr short ERROR_NO_VALUES_SELECTED = 9;
constexpr short ERROR_KEY_DOESNT_EXIST = 10;
constexpr short ERROR_SECRET_WORD_INVALID = 11;
constexpr short ERROR_MALFORMED_JSON = 12;
constexpr short ERROR_NESTED_JSON = 13;
constexpr short ERROR_INVALID_JSON_KEY = 14;
static const std::array<wchar_t const*, iERROR_COUNT> ERROR_MESSAGES = {
L"Resource files couldn't be loaded.",
L"One of the specified JSON keys for WORDS is empty (no values in array).",
L"One of the specified JSON keys for EXECUTABLES is empty (no values in array).",
L"Word length is invalid. Word needs to be between 3 and 15 charaters long. Use only letters and numbers",
L"Cannot set keyboard hook. Program privledges may not be high enough. Try running in ADMINISTRATOR mode.",
L"Cannot obtain valid process handle. Program privledges may not be high enough. Try running in ADMINISTRATOR mode.",
L"Cannot terminate process. Program privledges may not be high enough. Try running in ADMINISTRATOR mode.",
L"Malformed JSON structure. Make sure you have no dangling commas, quotes, braces and other special characters in the resource files.",
L"Malformed JSON structure - invalid JSON key. Up to 32 charaters long keys. Use letters and, if needed, numbers.",
L"No values selected. Seletect at least one WORD and EXECUTABLE resource.",
L"Malformed JSON structure - one of selected JSON keys doesn't exist. Do not modify the resource files while the program is running.",
L"Secret word is invalid. Word needs to be between 3 and 15 charaters long. Use only letters and numbers.",
L"Malformed JSON structure - string array is not an array. Make sure you have no dangling commas, quotes, braces and other special characters in the resource files. Make sure you are using correct braces [] for arrays.",
L"Malformed JSON structure - array values are not strings; nesting JSONs is forbidden. Make sure you have no dangling commas, quotes, braces and other special characters in the resource files. Make sure your array values are strings with in double quotes.",
L"Malformed JSON structure - most probably string key value is not a string. Using objects as keys is forbidden. If you want to use a number as a key, surround it with double quotes, for example: \"1\" or \"25\". Make sure you have no dangling commas, quotes, braces and other special characters in the resource files."};

constexpr short EVENT_SHOW_WINDOW = -1;