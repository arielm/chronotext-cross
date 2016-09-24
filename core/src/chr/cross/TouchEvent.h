#pragma once

namespace chr
{
	struct TouchEvent
	{
		enum Kind
		{
			KIND_UNDEFINED,
			KIND_ADD,
			KIND_UPDATE,
			KIND_REMOVE,
		};

		float x;
		float y;
		uint32_t id;
		Kind kind = KIND_UNDEFINED;

		TouchEvent(float x, float y, uint32_t id, Kind kind)
		:
		x(x),
		y(y),
		id(id),
		kind(kind)
		{}

		TouchEvent(float x, float y, uint32_t id)
		:
		x(x),
		y(y),
		id(id)
		{}
	};
}
