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
	unsigned int id_;
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void Use();
	void SetInt(const std::string &name, int value) const;
	void Shader::SetFloat(const std::string &name, float value) const;
	void SetVec3(const std::string &name, const glm::vec3 &value) const;
	void SetMat4(const std::string &name, const glm::mat4 &mat) const;
private:
	void CheckCompileErrors(unsigned int shader, std::string type);
};

#endif
