#include "app.h"

#include <spdlog/spdlog.h>

int main() {
    try {
        Chemical::App::Start();
    } catch (const std::exception& e) {
        spdlog::critical("Application exception occurred: {}", e.what());
        return -1;
    }
    return 0; 
}
