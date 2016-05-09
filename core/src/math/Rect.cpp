
#include "math/Rect.h"

namespace chr
{
  namespace math
  {
		Rectf::Rectf(float left, float top, float width, float height)
		:
		x1(left),
		y1(top),
		x2(left + width),
		y2(top + height)
		{}

		Rectf::Rectf(const glm::vec2 &upperLeft, const glm::vec2 &lowerRight)
		:
		x1(upperLeft.x),
		y1(upperLeft.y),
		x2(lowerRight.x),
		y2(lowerRight.y)
		{}

		float Rectf::width() const
		{
			return x2 - x1;
		}

		float Rectf::height() const
		{
			return y2 - y1;
		}

		glm::vec2 Rectf::size() const
		{
			return glm::vec2(x2 - x1, y2 - y1);
		}

		glm::vec2 Rectf::x1y1() const
		{
			return glm::vec2(x1, y1);
		}

		glm::vec2 Rectf::x2y1() const
		{
			return glm::vec2(x2, y1);
		}

		glm::vec2 Rectf::x2y2() const
		{
			return glm::vec2(x2, y2);
		}

		glm::vec2 Rectf::x1y2() const
		{
			return glm::vec2(x1, y2);
		}
	}
}
