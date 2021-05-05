#include "Core/Surface.hpp"

#include <cmath>

namespace SuperHaxagon {
	void Surface::drawPolyAbsolute(const Color&, const std::vector<Vec2f>&) {
		if (_needsClear) {
			this->screenBegin();
			_needsClear = false;
		}
	}

	void Surface::screenSwitch() {
		// By default do nothing since most platforms don't have two screens.
	}

	void Surface::screenFinalize() {
		_needsClear = true;
	}

	void Surface::toScreenSpace(std::vector<Vec2f>& points) const {
		// Transform to screen space and draw
		const auto screen = this->getScreenDim();
		for(auto& p : points) {
			// Fix aspect ratio
			if (screen.x > screen.y) {
				p.y *= screen.x / screen.y;
			} else {
				p.x *= screen.y / screen.x;
			}

			p.x = floor(((p.x + 1.0f) / 2.0f) * screen.x);
			p.y = floor(((-p.y + 1.0f) / 2.0f) * screen.y);
		}
	}
}
