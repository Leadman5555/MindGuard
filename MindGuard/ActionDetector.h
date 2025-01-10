#pragma once
#include <Windows.h>
#include <unordered_set>
#include "StringHash.h"
#include "ResourceConstants.h"
#include <array>
#include "ActionHandler.h"
#include <iostream>
class ActionDetector
{
public:
	ActionDetector(
		size_t minWordLength,
		size_t maxWordLength,
		const ActionHandler& actionHandler,
		const EventHandler& eventHandler) :
		minWordLength{ minWordLength },
		maxWordLength{ maxWordLength },
		handler{actionHandler},
		eventHandler{eventHandler}
	{
		thisWrapper = this;
	}
	ActionDetector(const ActionDetector& other) :
		forbiddenSet(other.forbiddenSet),
		presentLength(other.presentLength),
		minWordLength(other.minWordLength),
		maxWordLength(other.maxWordLength),
		handler(other.handler),
		eventHandler(other.eventHandler) {}
	~ActionDetector() {
		thisWrapper = nullptr;
	}
	void setValues(
		std::unordered_set<std::string, StringHash, std::equal_to<>>&& wordValues,
		std::array<bool, iMAX_WORD_LENGTH + 1>&& lengthArray,
		size_t minLength,
		size_t maxLength
	);
	void setSecretWord(std::string&& stopKeyWord);
	bool startDetection();
	void endDetection();
private:
	
	static ActionDetector* thisWrapper;
	static HHOOK hHook;
	static LRESULT CALLBACK KeyPressedWrapper(int nCode, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK KeyPressed(int nCode, WPARAM wParam, LPARAM lParam);
	bool setHook();
	void safePop();
	bool isValid();
	std::string inputChain;
	std::unordered_set<std::string, StringHash, std::equal_to<>> forbiddenSet;
	std::array<bool, iMAX_WORD_LENGTH+1> presentLength;
	size_t minWordLength;
	size_t maxWordLength;
	std::string stopWord;
	const ActionHandler& handler;
	const EventHandler& eventHandler;
};
