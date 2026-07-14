#include "module_registry.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

bool ModuleRegistry::scan(const std::string& modules_dir) {
    std::cout << "[ModuleRegistry] Scanning " << modules_dir << "..." << std::endl;
    
    if (!fs::exists(modules_dir)) {
        std::cerr << "[ModuleRegistry] Directory not found: " << modules_dir << std::endl;
        return false;
    }
    
    for (const auto& entry : fs::directory_iterator(modules_dir)) {
        if (entry.is_regular_file() && !entry.path().extension().empty()) {
            std::cout << "[ModuleRegistry] Found: " << entry.path().filename() << std::endl;
            // TODO: Run module --reveal and parse response
        }
    }
    
    return true;
}

void ModuleRegistry::register_module(const ModuleInfo& info) {
    std::cout << "[ModuleRegistry] Registering: " << info.name << " v" << info.version << std::endl;
    
    size_t idx = modules_.size();
    modules_.push_back(info);
    
    // Index commands
    for (const auto& cmd : info.handles) {
        command_index_[cmd] = idx;
    }
}

ModuleInfo* ModuleRegistry::find_module(const std::string& command) {
    auto it = command_index_.find(command);
    if (it != command_index_.end()) {
        return &modules_[it->second];
    }
    return nullptr;
}

const std::vector<ModuleInfo>& ModuleRegistry::get_modules() const {
    return modules_;
}
