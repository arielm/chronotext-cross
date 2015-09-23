#pragma once

#define GLM_SWIZZLE
#include <glm/glm.hpp>

struct Touch
{
	glm::vec2 pos;
	glm::vec2 prevPos;
	uint32_t id;
	double time;

	Touch(const glm::vec2 &pos, const glm::vec2 &prevPos, uint32_t id, double time)
	:
	pos(pos),
	prevPos(prevPos),
	id(id),
	time(time)
	{}
};
