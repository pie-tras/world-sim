#pragma once

namespace maths {
	static glm::vec2 lerp(const glm::vec2& start, const glm::vec2& end, const float t) {
		return start * (1.0f - t) + end * t;
	}
}