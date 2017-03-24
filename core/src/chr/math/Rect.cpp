#include "chr/math/Rect.h"

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

		Rectf& Rectf::operator +=(const glm::vec2 &offset)
		{
			x1 += offset.x;
			y1 += offset.y;
			x2 += offset.x;
			y2 += offset.y;

			return *this;
		}

		Rectf& Rectf::operator -=(const glm::vec2 &offset)
		{
			x1 -= offset.x;
			y1 -= offset.y;
			x2 -= offset.x;
			y2 -= offset.y;

			return *this;
		}

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

		bool Rectf::isNull() const
		{
			return (x1 == x2) || (y1 == y2);
		}

		bool Rectf::contains(float x, float y) const
		{
			return (x >= x1) && (y >= y1) && (x < x2) && (y < y2);
		}

		void Rectf::inflate(const glm::vec2 amount)
		{
			x1 -= amount.x;
			y1 -= amount.y;

		  x2 += amount.x;
			y2 += amount.y;
		}

		Rectf Rectf::inflated(const glm::vec2 amount) const
		{
			return Rectf(x1 - amount.x, y1 - amount.y, (x2 - x1) + amount.x * 2, (y2 - y1) + amount.y * 2);
		}

		glm::vec2 Rectf::center() const
		{
			return glm::vec2(x1 + (x2 - x1) * 0.5f, y1 + (y2 - y1) * 0.5f);
		}

		float Rectf::distance(const glm::vec2 &point) const
		{
			float dx = 0, dy = 0;

			if (point.x < x1)
			{
				dx = x1 - point.x;
			}
			else if (point.x > x2)
			{
				dx = point.x - x2;
			}

			if (point.y < y1)
			{
				dy = y1 - point.y;
			}
			else if (point.y > y2)
			{
				dy = point.y - y2;
			}

			return sqrtf(dx * dx + dy * dy);
		}
	}
}
