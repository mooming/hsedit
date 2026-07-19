//
// Created by mooming on 18/07/2026.
//

#pragma once

#include <string>
#include <unordered_map>
#include <vector>


namespace hs::module
{

struct ModuleInfo final
{
	std::string name;
	std::string version;
	std::vector<std::string> handles;    // Commands this module handles
	std::vector<std::string> keybindings; // Key combinations
	std::vector<std::string> capabilities; // Advanced capabilities
};

class ModuleRegistry final
{
public:
	ModuleRegistry() = default;

	/// @brief Scan modules/ directory and register all modules
	[[nodiscard]] bool Scan(const std::string& modules_dir);

	/// @brief Register a module
	void RegisterModule(const ModuleInfo& info);

	/// @brief Find module that handles a command
	[[nodiscard]] ModuleInfo* FindModule(const std::string& command);

	/// @brief Get all registered modules
	[[nodiscard]] const std::vector<ModuleInfo>& GetModules() const;

private:
	std::vector<ModuleInfo> modules_;
	std::unordered_map<std::string, size_t> commandIndex_; // command -> module index
};

} // namespace hs::module
