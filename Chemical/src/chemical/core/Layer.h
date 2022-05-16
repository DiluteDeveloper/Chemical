#pragma once

namespace Chemical {

	namespace Core {
		class Layer {

			Layer(const Layer&) = delete;
			Layer(Layer&&) = delete;

		protected:
			Layer() = default;

		public:
			virtual void OnUpdate() {};

			virtual ~Layer() = default;

		};
	}


}

