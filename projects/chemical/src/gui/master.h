#pragma once

namespace Chemical {

	class ChemicalEngine;

	class GUI {
	public:
		GUI(ChemicalEngine* engine);

		void Update(ChemicalEngine* engine);
	};
}
