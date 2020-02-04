#ifndef PLANAR_REFLECTION_CAMERA
#define PLANAR_REFLECTION_CAMERA

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, float aspect_ratio, float z_near, float z_far, float fovy);
	Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, float aspect_ratio, float z_near, float z_far);
	virtual ~Camera();

	void SetEye(const glm::vec3& eye);
	void SetAt(const glm::vec3& at);
	void SetUp(const glm::vec3& up);
	
	const glm::vec3& GetEye() const;
	const glm::vec3& GetAt() const;
	const glm::vec3& GetUp() const;
	
	void SetNear(float z_zear);
	void SetFar(float z_far);
	void SetFovy(float fovy);
	void SetAspectRatio(float aspect_ratio);

	float GetNear() const;
	float GetFar() const;
	float GetFovy() const;
	float GetAspectRatio() const;

	glm::mat4 GetViewTransform() const;
	glm::mat4 GetProjectionTransform() const;

private:
	glm::vec3 eye_;
	glm::vec3 up_;
	glm::vec3 at_;

	float fovy_;
	float z_near_;
	float z_far_;
	float aspect_ratio_;
};

#endif