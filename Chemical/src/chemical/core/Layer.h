#pragma once

#include <iostream>

namespace Chemical {

	namespace Core {
		class Layer {

			Layer(const Layer&) = delete;
			Layer(Layer&&) = delete;

		protected:
			Layer() {
				std::cout << "Layer constructor called." << std::endl;
			}

		public:
			virtual void OnUpdate() = 0;

			virtual ~Layer()
			{
				std::cout << "Layer destructor called." << std::endl;
			}

		};
	}


}

