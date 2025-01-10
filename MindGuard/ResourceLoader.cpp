#include "ResourceLoader.h"

bool ResourceLoader::loadSubResources(
	const std::vector<std::string>& subResourceKeys,
	size_t& minLength,
	size_t& maxLength,
	std::unordered_set<std::string, StringHash, std::equal_to<>>& values,
	std::array<bool, iMAX_WORD_LENGTH + 1>& presentLength) const
{
	for (const auto& key : subResourceKeys) {
		json data = loadJSON(key, wordResourcePath);
		if (data == nullptr) {
			eventHandler.handleError(ERROR_KEY_DOESNT_EXIST);
			return false;
		}
		else if (data.empty()) {
			eventHandler.handleError(ERROR_EMPTY_KEY_WORD);
			return false;
		}
		for (const auto& value : data) {
			auto s = value.get<std::string>();
			if (!checkValidWord(s)) {
				eventHandler.handleError(ERROR_WORD_LENGTH_INVALID); 
				return false;
			}
			std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
			presentLength[s.length()] = true;
			values.insert(std::move(s));
		}
	}
	for (size_t i{ iMIN_WORD_LENGTH }; i < iMAX_WORD_LENGTH; i++) if (presentLength[i]) { minLength = i; break; }
	for (size_t i{ iMAX_WORD_LENGTH - 1}; i >= iMIN_WORD_LENGTH; i--) if (presentLength[i]) { maxLength = i; break; }
	return true;
}

bool ResourceLoader::loadTerminationSets(
	const std::vector<std::string>& subSetKeys,
	std::unordered_set<std::string, StringHash, std::equal_to<>>& values) const
{
	for (const auto& key : subSetKeys) {
		json data = loadJSON(key, exeResourcePath);
		if (data == nullptr) {
			eventHandler.handleError(ERROR_KEY_DOESNT_EXIST);
			return false;
		}
		else if (data.empty()) {
			eventHandler.handleError(ERROR_EMPTY_KEY_EXE);
			return false;
		}
		else if (!data.is_array()) {
			eventHandler.handleError(ERROR_MALFORMED_JSON);
			return false;
		}
		for (const auto& value : data) {
			if (!value.is_string()) {
				eventHandler.handleError(ERROR_NESTED_JSON);
				return false;
			}
			values.insert(value.get<std::string>());
		}
		
	}
	return true;
}

bool ResourceLoader::loadAvaliableKeys(std::vector<std::string>& wordKeyValues, std::vector<std::string>& exeKeyValues) const
{
	return loadKeysToVector(wordKeyValues, wordResourcePath) && loadKeysToVector(exeKeyValues, exeResourcePath);
}

bool ResourceLoader::assertExists() const
{
	std::ifstream canOpenWord(wordResourcePath);
	if (!canOpenWord.is_open()) return false;
	canOpenWord.close();
	std::ifstream canOpenExe(exeResourcePath);
	if (!canOpenExe.is_open()) return false;
	canOpenExe.close();
	return true;
}

json ResourceLoader::loadJSON(const std::string& forKey, const std::string& forPath) const
{
	std::ifstream file(forPath);
	if(!file.is_open()) return nullptr;
	json data;
	try {
		file >> data;
		file.close();
		auto cKey = forKey.c_str();
		if (!data.contains(cKey)) return nullptr;
		return data[cKey];
	}
	catch (json::type_error&) {
		file.close();
		eventHandler.handleError(ERROR_INVALID_JSON_STRUCTURE);
		return nullptr;
	}
}

bool ResourceLoader::loadKeysToVector(std::vector<std::string>& values, const std::string& forPath) const
{
	std::ifstream file(forPath);
	if (!file.is_open()) {
		eventHandler.handleError(ERROR_RESOURCE_FILE_DOES_NOT_EXIST);
		return false;
	}
	json data;
	try {
		file >> data;
	}
	catch (json::parse_error&) {
		eventHandler.handleError(ERROR_INVALID_JSON_KEY);
		file.close();
		return false;
	}
	file.close();
	for (auto it = data.begin(); it != data.end(); it++) {
		std::string key{ it.key() };
		if (key.length() > iMAX_ALLOWED_KEY_SIZE) {
			eventHandler.handleError(ERROR_INVALID_JSON_KEY_NAME);
			return false;
		}
		values.push_back(std::move(key));
	}
	return true;
}

bool ResourceLoader::checkValidWord(const std::string_view& wordToCheck) const
{
	size_t length = wordToCheck.length();
	return ((length >= iMIN_WORD_LENGTH && length <= iMAX_WORD_LENGTH) &&
		std::all_of(wordToCheck.begin(), wordToCheck.end(),
		[](const char& c) {return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }));
}
