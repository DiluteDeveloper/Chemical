#pragma once

#include <functional>

namespace Chemical {
	namespace OpenGL {

		// must be used for all opengl functions
		// handles dispatching opengl functions at predictable times
		class Handler {

		public:

			template<typename Function, typename... Arguments>
			void SubmitFunctionExecution(Function func, Arguments... parameters) {
				functions.emplace_back(std::bind(func, parameters...));

			};

			void Dispatch() {
				for (auto& f : functions) {
					f();
				
				}

				functions.clear();

			}

		private:
			std::vector<std::function<void()>> functions;
		};
	}
}
