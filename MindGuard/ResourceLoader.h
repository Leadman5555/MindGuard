#pragma once
#include <string>
#include <string_view>
#include <unordered_set>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <string_view>
#include "StringHash.h"
#include "ResourceConstants.h"
#include "EventHandler.h"

using json = nlohmann::json;
class ResourceLoader
{
public:
	explicit ResourceLoader(
		const std::string& wordResourcePath,
		const std::string& exeResourcePath,
		const EventHandler& eventHandler) :
		wordResourcePath{ wordResourcePath },
		exeResourcePath{ exeResourcePath },
		eventHandler{ eventHandler } {}
	bool loadSubResources(const std::vector<std::string>& subResourceKeys,
		size_t& minLength,
		size_t& maxLength,
		std::unordered_set<std::string, StringHash, std::equal_to<>>& values,
		std::array<bool, iMAX_WORD_LENGTH + 1>& presentLength) const;
	bool loadTerminationSets(
		const std::vector<std::string>& subSetKeys,
		std::unordered_set<std::string, StringHash, std::equal_to<>>& values) const;
	bool loadAvaliableKeys(std::vector<std::string>& wordKeyValues, std::vector<std::string>& exeKeyValues) const;
	bool assertExists() const;
	bool checkValidWord(const std::string_view& wordToCheck) const;
private:
	const std::string wordResourcePath;
	const std::string exeResourcePath;
	const EventHandler& eventHandler;
	json loadJSON(const std::string& forKey, const std::string& forPath) const;
	bool loadKeysToVector(std::vector<std::string>& values, const std::string& forPath) const;
};

