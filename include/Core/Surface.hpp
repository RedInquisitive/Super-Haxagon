#ifndef SUPER_HAXAGON_SURFACE_HPP
#define SUPER_HAXAGON_SURFACE_HPP

#include "Core/Structs.hpp"

#include <vector>

namespace SuperHaxagon {
	class Surface {
	public:
		virtual ~Surface() = default;

		virtual void drawPolyAbsolute(const Color& color, const std::vector<Vec2f>& points) = 0;

		virtual Vec2f getScreenDim() const = 0;
		virtual void screenBegin() = 0;
		virtual void screenFinalize() = 0;
		virtual void screenSwap();

		void toScreenSpace(std::vector<Vec2f>& points) const;
	};
}

#endif //SUPER_HAXAGON_SURFACE_HPP
