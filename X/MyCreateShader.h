#ifndef MY_CREATE_SHADER_H
#define  MY_CREATE_SHADER_H
#include <string>
using namespace std;
#include <istream>
#include <gl/glew.h>
#include "gl/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyCreateShader
{
public:
	MyCreateShader(std::string _vertex_shader_name, std::string _frag_shader_name, std::string _geometry_shader_path);
	~MyCreateShader();

public:
	unsigned int program_id;
	void use();
	
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const;
	
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const;
	
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const;
	
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w);
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const;

};

#endif  //MY_CREATE_SHADER_H