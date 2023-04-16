#include <iostream>

namespace ApplicationAPI {
	void on_main_begin() {
		std::cout << "Hello! main just begun!" << std::endl;

	}

	void on_engine_begin() {
		std::cout << "Hello! engine just begun!" << std::endl;

	}

	void on_scene_begin() {
		std::cout << "Hello! scene just begun!" << std::endl;

	}
}

