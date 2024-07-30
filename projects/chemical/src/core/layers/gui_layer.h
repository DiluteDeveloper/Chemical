#pragma once

#include "core/application.h"

namespace Chemical {

	namespace Core {


		class GUILayer : protected ApplicationLayer {
		private:
			using ApplicationLayer::ApplicationLayer;
			friend ApplicationData;

			void InitializeLayer() override;
			void UpdateLayer() override;
			void DestroyLayer() override;
		};
	}

}
