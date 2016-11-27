#ifndef LOCALIZATION_H
#define LOCALIZATION_H
#include <string>
#include <unordered_map>
#include <vector>
class Localization
{
private:
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> keys_;
	std::vector<std::string> availableLanguages_;
public:
	const std::vector<std::__cxx11::string> &availableLanguages() const;
	Localization(const std::string &path);
	~Localization();
	void addLang(const std::string &key, const std::string &name, const std::string &value);
	bool removeLang(const std::string &key, const std::string &name);
	const std::string &operator()(const std::string &key, const std::string &name);

	const std::string &tr(const std::string &key, const std::string &name);
};

#endif // LOCALIZATION_H

