#include "Settings.h"

#include <fstream>
#include <sstream>

namespace settings
{
	namespace
	{
		Values     g_values;
		std::mutex g_saveLock;
		std::mutex g_valuesLock;   // the category switches: the page's thread writes, the game thread reads

		std::string_view Trim(std::string_view a_s)
		{
			while (!a_s.empty() && (a_s.front() == ' ' || a_s.front() == '\t')) a_s.remove_prefix(1);
			while (!a_s.empty() && (a_s.back() == ' ' || a_s.back() == '\t' || a_s.back() == '\r')) a_s.remove_suffix(1);
			return a_s;
		}

		std::string Lower(std::string a_s)
		{
			std::ranges::transform(a_s, a_s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
			return a_s;
		}

		std::filesystem::path ModuleFolder()
		{
			HMODULE self = nullptr;
			::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				reinterpret_cast<LPCWSTR>(&ModuleFolder), &self);
			wchar_t buf[MAX_PATH]{};
			const DWORD n = self ? ::GetModuleFileNameW(self, buf, MAX_PATH) : 0;
			if (n == 0 || n >= MAX_PATH) {
				return std::filesystem::path(L"OBSE") / L"Plugins";   // relative to the game's working folder
			}
			return std::filesystem::path(buf).parent_path();
		}

		// every key this plugin owns, as "section.key" -> its current value as INI text
		std::vector<std::pair<std::string, std::string>> Rows(const Values& a_v)
		{
			std::vector<std::pair<std::string, std::string>> out;
			for (std::size_t i = 0; i < kCategoryCount; ++i) {
				out.emplace_back(std::string("Categories.") + kCategoryKeys[i], a_v.categories[i] ? "1" : "0");
			}
			out.emplace_back("Log.uLogLevel", std::to_string(a_v.logLevel));
			return out;
		}
	}

	Values& Get() { return g_values; }

	bool IsOn(std::size_t a_index)
	{
		std::scoped_lock l(g_valuesLock);
		return a_index < kCategoryCount && g_values.categories[a_index];
	}

	void SetOn(std::size_t a_index, bool a_on)
	{
		std::scoped_lock l(g_valuesLock);
		if (a_index < kCategoryCount) {
			g_values.categories[a_index] = a_on;
		}
	}
	std::filesystem::path PluginFolder() { return ModuleFolder(); }
	std::filesystem::path IniPath() { return ModuleFolder() / L"Weightless.ini"; }

	void Load()
	{
		std::ifstream file(IniPath());
		if (!file.is_open()) {
			logger::info("settings: {} not found - the compiled defaults are in effect (they match the shipped INI)", IniPath().string());
			return;
		}
		std::unordered_map<std::string, std::string> entries;
		std::string line, section;
		while (std::getline(file, line)) {
			const auto t = Trim(line);
			if (t.empty() || t.front() == ';' || t.front() == '#') continue;
			if (t.front() == '[' && t.back() == ']') {
				section = Lower(std::string(Trim(t.substr(1, t.size() - 2))));
				continue;
			}
			const auto eq = t.find('=');
			if (eq == std::string_view::npos) continue;
			entries[section + "." + Lower(std::string(Trim(t.substr(0, eq))))] = std::string(Trim(t.substr(eq + 1)));
		}
		const auto get = [&](const std::string& a_key) -> const std::string* {
			const auto it = entries.find(Lower(a_key));
			return it != entries.end() ? &it->second : nullptr;
		};
		for (std::size_t i = 0; i < kCategoryCount; ++i) {
			if (const auto* v = get(std::string("Categories.") + kCategoryKeys[i])) {
				g_values.categories[i] = *v != "0" && Lower(*v) != "false";
			} else {
				logger::debug("settings: [Categories] {} not in the INI - compiled default {}", kCategoryKeys[i], kCategoryDefaults[i]);
			}
		}
		if (const auto* v = get("Log.uLogLevel")) g_values.logLevel = std::clamp(std::atoi(v->c_str()), 0, 6);
		std::string on;
		for (std::size_t i = 0; i < kCategoryCount; ++i) {
			if (g_values.categories[i]) on += (on.empty() ? "" : " ") + std::string(kCategoryKeys[i] + 1);
		}
		logger::info("settings loaded from {}: weightless [{}], log level {}", IniPath().string(), on, g_values.logLevel);
	}

	bool Save()
	{
		std::scoped_lock l(g_saveLock);
		const auto path = IniPath();
		std::vector<std::string> lines;
		bool crlf = true;
		{
			std::ifstream in(path, std::ios::binary);
			if (in.is_open()) {
				std::stringstream ss;
				ss << in.rdbuf();
				const std::string all = ss.str();
				crlf = all.find("\r\n") != std::string::npos || all.empty();
				std::string cur;
				for (char c : all) {
					if (c == '\n') {
						if (!cur.empty() && cur.back() == '\r') cur.pop_back();
						lines.push_back(cur);
						cur.clear();
					} else {
						cur.push_back(c);
					}
				}
				if (!cur.empty()) lines.push_back(cur);
			}
		}
		Values snapshot;
		{
			std::scoped_lock v(g_valuesLock);
			snapshot = g_values;
		}
		const auto rows = Rows(snapshot);
		std::vector<bool> written(rows.size(), false);
		std::string section;
		for (auto& ln : lines) {
			const auto t = Trim(ln);
			if (!t.empty() && t.front() == '[' && t.back() == ']') {
				section = Lower(std::string(Trim(t.substr(1, t.size() - 2))));
				continue;
			}
			if (t.empty() || t.front() == ';' || t.front() == '#') continue;
			const auto eq = t.find('=');
			if (eq == std::string_view::npos) continue;
			const std::string key = section + "." + Lower(std::string(Trim(t.substr(0, eq))));
			for (std::size_t i = 0; i < rows.size(); ++i) {
				if (Lower(rows[i].first) == key) {
					ln = rows[i].first.substr(rows[i].first.find('.') + 1) + "=" + rows[i].second;
					written[i] = true;
					break;
				}
			}
		}
		// keys the file did not have yet go at the end of their section (a new section at the end if needed)
		for (std::size_t i = 0; i < rows.size(); ++i) {
			if (written[i]) continue;
			const auto dot = rows[i].first.find('.');
			const std::string want = Lower(rows[i].first.substr(0, dot));
			const std::string text = rows[i].first.substr(dot + 1) + "=" + rows[i].second;
			std::size_t insertAt = lines.size();
			bool found = false;
			for (std::size_t j = 0; j < lines.size(); ++j) {
				const auto t = Trim(lines[j]);
				if (!t.empty() && t.front() == '[' && t.back() == ']') {
					if (found) {
						insertAt = j;
						break;
					}
					found = Lower(std::string(Trim(t.substr(1, t.size() - 2)))) == want;
				}
			}
			if (!found) {
				lines.push_back("");
				lines.push_back("[" + rows[i].first.substr(0, dot) + "]");
				insertAt = lines.size();
			} else {
				while (insertAt > 0 && Trim(lines[insertAt - 1]).empty()) --insertAt;
			}
			lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(insertAt), text);
		}
		std::ofstream out(path, std::ios::binary | std::ios::trunc);
		if (!out.is_open()) {
			logger::error("settings: {} could not be written", path.string());
			return false;
		}
		for (const auto& ln : lines) {
			out << ln << (crlf ? "\r\n" : "\n");
		}
		logger::debug("settings: saved to {}", path.string());
		return true;
	}
}
