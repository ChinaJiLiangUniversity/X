#ifndef MY_CAMERA_H
#define MY_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.1f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class MyCamera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	glm::vec3 new_up_;
	glm::vec3 new_right_;
	glm::vec3 new_target_;
	
	glm::mat4 view_matrix_;
	// Constructor with vectors
	MyCamera()
	{
		set_look_at(glm::vec3(0, 0.0, 1), glm::vec3(0, 0.0, 0), glm::vec3(0, 1, 0));
	}
	
	glm::mat4 GetViewMatrix()
	{
		return view_matrix_;
	}
	void set_look_at(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up);
	
	glm::vec3 eye_position();
	glm::vec3 look_direction();
	glm::vec3 up_direction();
	glm::vec3 right_direction();
	glm::vec4 EyeToWorldSpace(glm::vec4 eye_pt);

	
public:
	
	void ViewTranslate(const float trans_horizontal, const float trans_verticality);//相机移动
	void ViewScale(const float scale);//相机缩放
	void ViewRotate(const float angle_horizontal, const float angle_verticality, const glm::vec3 rotate_center = glm::vec3(0));//相机沿着点旋转


};
#endif

