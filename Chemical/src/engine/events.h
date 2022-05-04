#pragma once

#include <functional>
#include <vector>
class KeyPressDispatcher {

	inline static std::vector <std::function<void(int, int, int, int)>> subscribers;

public:

	inline static void subscribe(std::function<void(int, int, int, int)> func) {
		subscribers.emplace_back(func);
	}

	inline static void invoke(int key, int scancode, int action, int mods) {
		for (std::function<void(int, int, int, int)> a : subscribers) {
			a(key, scancode, action, mods);
		}
	}
};

class WindowResizeDispatcher {

	inline static std::vector <std::function<void(int, int)>> subscribers;

public:

	inline static void subscribe(std::function<void(int, int)> func) {
		subscribers.emplace_back(func);
	}

	inline static void invoke(int x, int y) {
		for (std::function<void(int, int)> a : subscribers) {
			a(x, y);
		}
	}
};

