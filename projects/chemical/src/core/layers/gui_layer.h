#pragma once

#include "core/application.h"

namespace Chemical {

	namespace Core {


		class GUILayer : public ApplicationLayer {
		private:

			void InitializeLayer(ApplicationData& appData) override;
			void UpdateLayer() override;
			void DestroyLayer() override;
		};
	}

}
