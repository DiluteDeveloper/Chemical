#include <chemical_api/test.h>

namespace ApplicationAPI {
	void on_main_begin() {
		std::cout << "Hello! main just begun!" << std::endl;

		std::cout << ChemicalAPI::make_charp() << std::endl;
		std::cout << ChemicalAPI::make_string() << std::endl;
		std::cout << ChemicalAPI::size<uint8_t>() << std::endl;

		std::cout << "NEW LINE" << std::endl;
	}

	void on_engine_begin() {
		std::cout << "Hello! engine just begun!" << std::endl;

		std::cout << ChemicalAPI::make_charp() << std::endl;
		std::cout << ChemicalAPI::make_string() << std::endl;
		std::cout << ChemicalAPI::size<uint8_t>() << std::endl;

		std::cout << "NEW LINE" << std::endl;
	}

	void on_scene_begin() {
		std::cout << "Hello! scene just begun!" << std::endl;

		std::cout << ChemicalAPI::make_charp() << std::endl;
		std::cout << ChemicalAPI::make_string() << std::endl;
		std::cout << ChemicalAPI::size<uint8_t>() << std::endl;

		std::cout << "NEW LINE" << std::endl;
	}
}

