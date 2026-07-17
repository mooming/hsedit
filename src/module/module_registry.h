#ifndef MODULE_REGISTRY_H
#define MODULE_REGISTRY_H

#include <string>
#include <vector>
#include <unordered_map>

struct ModuleInfo {
    std::string name;
    std::string version;
    std::vector<std::string> handles;  // Commands this module handles
    std::vector<std::string> keybindings;  // Key combinations
    std::vector<std::string> capabilities;  // Advanced capabilities
};

class ModuleRegistry {
public:
    ModuleRegistry() = default;
    
    // Scan modules/ directory and register all modules
    bool scan(const std::string& modules_dir);
    
    // Register a module
    void register_module(const ModuleInfo& info);
    
    // Find module that handles a command
    ModuleInfo* find_module(const std::string& command);
    
    // Get all registered modules
    const std::vector<ModuleInfo>& get_modules() const;
    
private:
    std::vector<ModuleInfo> modules_;
    std::unordered_map<std::string, size_t> command_index_;  // command -> module index
};

#endif  // MODULE_REGISTRY_H
