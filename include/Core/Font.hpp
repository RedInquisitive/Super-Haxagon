#ifndef SUPER_HAXAGON_FONT_HPP
#define SUPER_HAXAGON_FONT_HPP

#include "Vector.hpp"

#include <string>

namespace SuperHaxagon {
	enum class Alignment {
		LEFT,
		CENTER,
		RIGHT
	};

	struct Color;

	class Font {
	public:
		Font() = default;
		Font(Font&) = delete;
		virtual ~Font() = default;

		virtual void setScale(float scale) = 0;
		virtual float getHeight() const = 0;
		virtual float getWidth(const std::string& str) const = 0;
		virtual void draw(const Color& color, const Vec2f& position, Alignment alignment, const std::string& text) = 0;
	};
}

#endif //SUPER_HAXAGON_FONT_HPP
