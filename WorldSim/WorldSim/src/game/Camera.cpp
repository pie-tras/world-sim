#include "Camera.h"

void Camera::move(const glm::vec2& target, const float t) {
	position = maths::lerp(position, target, t);
}

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::scale(view, glm::vec3(zoom));
	view = glm::translate(view, glm::vec3(-position, 0.0f));
	return view;
}

void Camera::setPosition(const glm::vec2& pos) {
	position = pos;
}

glm::vec2 Camera::getPosition() {
	return position;
}

void Camera::setZoom(float zoom) {
	this->zoom = glm::exp(zoom);
}

float Camera::getZoom() {
	return zoom;
}