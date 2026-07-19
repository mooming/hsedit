//
// Created by mooming on 18/07/2026.
//

#include "ModuleRegistry.h"

#include <filesystem>
#include <iostream>


namespace fs = std::filesystem;
namespace hs::module
{

[[nodiscard]] bool ModuleRegistry::Scan(const std::string& modules_dir)
{
	std::cout << "[ModuleRegistry] Scanning " << modules_dir << "..." << std::endl;

	if (!fs::exists(modules_dir))
	{
		std::cerr << "[ModuleRegistry] Directory not found: " << modules_dir << std::endl;
		return false;
	}

	for (const auto& entry : fs::directory_iterator(modules_dir))
	{
		if (entry.is_regular_file() && !entry.path().extension().empty())
		{
			std::cout << "[ModuleRegistry] Found: " << entry.path().filename() << std::endl;
			// TODO: Run module --reveal and parse response
		}
	}

	return true;
}

void ModuleRegistry::RegisterModule(const ModuleInfo& info)
{
	std::cout << "[ModuleRegistry] Registering: " << info.name << " v" << info.version << std::endl;

	const size_t idx = modules_.size();
	modules_.push_back(info);

	// Index commands
	for (const auto& cmd : info.handles)
	{
		commandIndex_[cmd] = idx;
	}
}

[[nodiscard]] ModuleInfo* ModuleRegistry::FindModule(const std::string& command)
{
	const auto it = commandIndex_.find(command);
	if (it != commandIndex_.end())
	{
		return &modules_[it->second];
	}

	return nullptr;
}

[[nodiscard]] const std::vector<ModuleInfo>& ModuleRegistry::GetModules() const
{
	return modules_;
}

} // namespace hs::module
