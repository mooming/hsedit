#include <iostream>
#include "../src/module/module_registry.h"

void test_module_registry() {
    std::cout << "  [test_module_registry] Module registry tests..." << std::endl;
    
    ModuleRegistry registry;
    ModuleInfo info;
    info.name = "test-module";
    info.version = "1.0";
    info.handles = {"TEST_CMD"};
    
    registry.register_module(info);
    
    if (registry.find_module("TEST_CMD") != nullptr) {
        std::cout << "  [PASS] find_module works" << std::endl;
    } else {
        std::cout << "  [FAIL] find_module failed" << std::endl;
    }
}
