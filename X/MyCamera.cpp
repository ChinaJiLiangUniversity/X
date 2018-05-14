#include "MyCamera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"

void MyCamera::ViewTranslate(const float trans_horizontal, const float trans_verticality)
{
	glm::vec3 right_ward = right_direction();
	glm::vec3 eye = eye_position();
	glm::vec3 lookdir = look_direction();
	glm::vec3 up = up_direction();

	float delta_xf = trans_horizontal;
	float delta_yf = trans_verticality;

	eye -= right_ward * delta_xf + up * delta_yf;
	set_look_at(eye, eye + lookdir, up);
}

void MyCamera::ViewScale(const float scale)
{
	glm::vec3 eye = eye_position();
	glm::vec3 lookdir = look_direction();
	glm::vec3 up = up_direction();

	glm::vec3 new_eye = eye + lookdir * (scale);
	set_look_at(new_eye, new_eye + lookdir, up);
}

void MyCamera::ViewRotate(const float angle_horizontal, const float angle_verticality, const glm::vec3 rotate_center /*= glm::vec3(0)*/)
{
	glm::vec3 eye = eye_position();
	glm::vec3 lookdir = look_direction();
	glm::vec3 up = up_direction();
	glm::vec3 right_ward = right_direction();

	float angleH = angle_horizontal;
	float angleV = angle_verticality;

	view_matrix_ *= glm::translate(rotate_center);
	view_matrix_ *= glm::rotate(angleH, glm::vec3(0, 1, 0));
	glm::vec3 v = glm::vec3(glm::affineInverse(view_matrix_)*glm::vec4(1, 0, 0, 0));
	view_matrix_ *= glm::rotate(angleV, v);
	view_matrix_ *= glm::translate(-rotate_center);
}

void MyCamera::set_look_at(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up)
{
	view_matrix_ = glm::lookAt(eye, lookat, up);
}

glm::vec3 MyCamera::eye_position()
{
	return glm::vec3(EyeToWorldSpace(glm::vec4(0, 0, 0, 1)));
}

glm::vec3 MyCamera::look_direction()
{
	return glm::normalize(glm::vec3(EyeToWorldSpace(glm::vec4(0, 0, -1, 0))));

}

glm::vec3 MyCamera::up_direction()
{
	return  glm::normalize(glm::vec3(EyeToWorldSpace(glm::vec4(0, 1, 0, 0))));

}

glm::vec3 MyCamera::right_direction()
{
	return glm::normalize(glm::vec3(EyeToWorldSpace(glm::vec4(1, 0, 0, 0))));

}

glm::vec4 MyCamera::EyeToWorldSpace(glm::vec4 eye_pt)
{
	return glm::inverse(view_matrix_)*eye_pt;

}
