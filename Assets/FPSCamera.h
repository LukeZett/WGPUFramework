#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

struct CameraMovement {
	float forwardSpeed = 1;
	float sideWaysMultiplier = 1;
	float backwardMultiplier = 1;

	float forward = 0;
	float right = 0;
};


class FPSCamera
{
private:
	glm::vec3 looking_dir = { 1, 0, 0 };
	glm::vec3 movement_dir = { 1, 0, 0 };
	glm::vec3 up_dir = { 0, 1, 0 };
	float fov = 70;
	float aspect_ratio = 16 / 9;
	float z_near = 0.01;
	float z_far = 100;
	float horizontalAngle = 0;
	float verticalAngle = 0;
	CameraMovement movement;

public:
	glm::vec3 position = { 0, 1, 0 };

	inline float GetAngleHorizontal()
	{
		return horizontalAngle;
	}
	inline float GetAngleVertical()
	{
		return verticalAngle;
	}
	inline glm::vec3& GetLookVector()
	{
		return looking_dir;
	}

	inline void SetLookVector(const glm::vec3& vector)
	{
		looking_dir = vector;
	}

	inline glm::vec3& GetMovementVector()
	{
		return movement_dir;
	}

	inline void SetMovementVector(const glm::vec3& vector)
	{
		movement_dir = vector;
	}



	inline float GetSpeed()
	{
		return movement.forwardSpeed;
	}

	inline void SetSpeed(float speed)
	{
		movement.forwardSpeed = speed;
	}

	inline glm::vec3& GetPos()
	{
		return position;
	}

	inline void SetAspectRatio(float ratio)
	{
		aspect_ratio = ratio;
	}

	inline float GetFoV()
	{
		return fov;
	}

	inline float GetNearPlane()
	{
		return z_near;
	}

	inline void SetNearPlane(float z)
	{
		z_near = z;
	}

	inline float GetFarPlane()
	{
		return z_far;
	}

	inline void SetFarPlane(float z)
	{
		z_far = z;
	}

	inline glm::mat4x4 GetPerspective()
	{
		return glm::perspective(fov, aspect_ratio, z_near, z_far);
	}

	inline glm::mat4x4 GetView()
	{
		return glm::lookAt(position, position + looking_dir, up_dir);
	}
	inline void UpdateDirection()
	{
		looking_dir = {
				cos(verticalAngle) * sin(horizontalAngle),
				sin(verticalAngle),
				cos(verticalAngle) * cos(horizontalAngle)
		};
		movement_dir = looking_dir;
	}

	inline void UpdateMovementDirection(float up, float right)
	{
		movement_dir = glm::normalize(glm::vec3(
			cos(up) * sin(right),
			sin(up),
			cos(up) * cos(right)
		));
	}


	inline void TurnTo(float up, float right)
	{
		verticalAngle = up;
		horizontalAngle = right;
		UpdateDirection();
	}

	inline void Turn(float deltaUp, float deltaRight, float sensitivity)
	{
		verticalAngle += deltaUp * sensitivity;
		horizontalAngle += deltaRight * sensitivity;
		UpdateDirection();
	}

	inline void UpdateForwardMovement(float forward)
	{
		movement.forward = forward;
	}

	inline void UpdateSidewaysMovement(float right)
	{
		movement.right = right;
	}

	inline void Move(float deltaTime)
	{
		position += movement_dir * movement.forward * movement.forwardSpeed * deltaTime;
		position += glm::cross(movement_dir, up_dir) * movement.forwardSpeed * movement.sideWaysMultiplier * movement.right * deltaTime;
	}

	inline void MoveVector(glm::vec3& offset)
	{
		position += offset;
	}

	inline void MoveTo(glm::vec3& pos)
	{
		position = pos;
	}
};