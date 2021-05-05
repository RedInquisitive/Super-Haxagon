#ifndef SUPER_HAXAGON_SURFACE_HPP
#define SUPER_HAXAGON_SURFACE_HPP

#include "Core/Structs.hpp"

#include <vector>

namespace SuperHaxagon {
	class Surface {
	public:
		virtual ~Surface() = default;

		virtual void drawPolyAbsolute(const Color& color, const std::vector<Vec2f>& points);

		virtual Vec2f getScreenDim() const = 0;
		virtual void screenBegin() = 0;
		virtual void screenFinalize();
		virtual void screenSwitch();

		void setClearColor(const Color& color) {_clear = color;}
		void toScreenSpace(std::vector<Vec2f>& points) const;

	protected:
		Color _clear{};

	private:
		bool _needsClear = true;

	};
}

#endif //SUPER_HAXAGON_SURFACE_HPP
