#include "app.h"

#include <spdlog/spdlog.h>

int main() {
    try {
        Chemical::App app;
    } catch (const std::exception& e) {
        spdlog::critical("Application exception occurred: {}", e.what());
        throw e;
    }
    return 0; 
}
