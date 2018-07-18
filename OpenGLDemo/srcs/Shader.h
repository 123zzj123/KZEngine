#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
using namespace std;

class Shader
{
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void use();
	void setInt(const std::string &name, int value) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
private:
	void CheckCompileErrors(unsigned int shader, std::string type);
};

#endif
