#include "MyCreateShader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

MyCreateShader::MyCreateShader(std::string _vertex_shader_name, std::string _frag_shader_name, std::string _geometry_shader_path)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(_vertex_shader_name.c_str());
		fShaderFile.open(_frag_shader_name);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (_geometry_shader_path.c_str() != nullptr)
		{
			gShaderFile.open(_geometry_shader_path.c_str());
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	//checkCompileErrors(fragment, "FRAGMENT");
	// if geometry shader is given, compile geometry shader
	unsigned int geometry;
	if (_geometry_shader_path.c_str() != nullptr)
	{
		const char * gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		//checkCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex);
	glAttachShader(program_id, fragment);
	if (_geometry_shader_path.c_str() != nullptr)
		glAttachShader(program_id, geometry);
	glLinkProgram(program_id);
	//checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (_geometry_shader_path.c_str() != nullptr)
		glDeleteShader(geometry);
}

MyCreateShader::~MyCreateShader()
{

}

void MyCreateShader::use()
{
	{
		glUseProgram(program_id);
	}
}

void MyCreateShader::setBool(const std::string &name, bool value) const
{
	{
		glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
	}
}

void MyCreateShader::setInt(const std::string &name, int value) const
{
	{
		glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
	}
}

void MyCreateShader::setFloat(const std::string &name, float value) const
{
	{
		glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
	}
}

void MyCreateShader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	{
		glUniform2fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
	}
}

void MyCreateShader::setVec2(const std::string &name, float x, float y) const
{
	{
		glUniform2f(glGetUniformLocation(program_id, name.c_str()), x, y);
	}

}

void MyCreateShader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	{
		glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
	}
}

void MyCreateShader::setVec3(const std::string &name, float x, float y, float z) const
{
	{
		glUniform3f(glGetUniformLocation(program_id, name.c_str()), x, y, z);
	}
}

void MyCreateShader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	{
		glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
	}
}

void MyCreateShader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	{
		glUniform4f(glGetUniformLocation(program_id, name.c_str()), x, y, z, w);
	}
}

void MyCreateShader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	{
		glUniformMatrix2fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
}

void MyCreateShader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	{
		glUniformMatrix3fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
}

void MyCreateShader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	{
		glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
}
