#include "camera.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, float aspect_ratio, float z_near, float z_far) :
	Camera(eye,at, up, aspect_ratio, z_near, z_far, glm::pi<float>() / 4)
{
	
}

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, float aspect_ratio, float z_near, float z_far, float fovy) :
	eye_(eye),
	at_(at),
	up_(up),
	aspect_ratio_(aspect_ratio),
	fovy_(fovy),
	z_near_(z_near),
	z_far_(z_far)
{

}

Camera::~Camera()
{
	
}

void Camera::SetEye(const glm::vec3& eye)
{
	eye_ = eye;
}

void Camera::SetAt(const glm::vec3& at)
{
	at_ = at;
}

void Camera::SetUp(const glm::vec3& up)
{
	up_ = up;
}

const glm::vec3& Camera::GetEye() const
{
	return eye_;
}

const glm::vec3& Camera::GetAt() const
{
	return at_;
}

const glm::vec3& Camera::GetUp() const
{
	return up_;
}

void Camera::SetNear(float z_near)
{
	z_near_ = z_near;
}

void Camera::SetFar(float z_far)
{
	z_far_ = z_far;
}

void Camera::SetFovy(float fovy)
{
	fovy_ = fovy;
}

void Camera::SetAspectRatio(float aspect_ratio)
{
	aspect_ratio_ = aspect_ratio;
}

float Camera::GetNear() const
{
	return z_near_;
}

float Camera::GetFar() const
{
	return z_far_;
}

float Camera::GetFovy() const
{
	return fovy_;
}

float Camera::GetAspectRatio() const
{
	return aspect_ratio_;
}

glm::mat4 Camera::GetViewTransform() const
{
	return glm::lookAt(eye_, at_, up_);
}

glm::mat4 Camera::GetProjectionTransform() const
{
	return glm::perspective(fovy_, aspect_ratio_, z_near_, z_far_);
}