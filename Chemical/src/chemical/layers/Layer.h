#pragma once

namespace Chemical {

	namespace Layers {
		class Layer {

			Layer(const Layer&) = delete;
			Layer(Layer&&) = delete;

		protected:
			Layer() = default;

		public:
			virtual void OnUpdate() {};

			virtual ~Layer() {};

		};
	}


}

