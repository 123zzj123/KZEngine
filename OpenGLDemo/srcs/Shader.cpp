#include "Shader.h"

const string VERTEXSHADER = "Vertex Shader";
const string FRAGMENTSHADER = "Fragment Shader";
const string GEOMETRYSHADER = "Geometry Shader";
const string PROGRAM = "Program";

Shader::Shader(const char * vertexPath, const char * fragmentPath, const char * geometryPath)
{
	string vertex_str_code;
	string fragment_str_code;
	string geometry_str_code;
	ifstream vertex_filestream;
	ifstream fragment_filestream;
	ifstream geometry_filestream;
	vertex_filestream.exceptions(ifstream::failbit | ifstream::badbit);
	fragment_filestream.exceptions(ifstream::failbit | ifstream::badbit);
	geometry_filestream.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		stringstream vertex_sstream, fragment_sstream;
		vertex_filestream.open(vertexPath);
		fragment_filestream.open(fragmentPath);
		vertex_sstream << vertex_filestream.rdbuf();
		fragment_sstream << fragment_filestream.rdbuf();
		vertex_filestream.close();
		fragment_filestream.close();
		vertex_str_code = vertex_sstream.str();
		fragment_str_code = fragment_sstream.str();

		if (geometryPath) {
			stringstream geometry_sstream;
			geometry_filestream.open(geometryPath);
			geometry_sstream << geometry_filestream.rdbuf();
			geometry_filestream.close();
			geometry_str_code = geometry_sstream.str();
		}
	}
	catch (ifstream::failure e)
	{
		cout << "fail to read shader file" << endl;
	}
	const char* vertex_code = vertex_str_code.c_str();
	const char* fragment_code = fragment_str_code.c_str();
	uint32_t vertex, fragment, geometry;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_code, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, VERTEXSHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_code, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, FRAGMENTSHADER);

	if (geometryPath) {
		const char* geometry_code = geometry_str_code.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometry_code, NULL);
		glCompileShader(geometry);
		CheckCompileErrors(geometry, GEOMETRYSHADER);
	}

	id_ = glCreateProgram();
	glAttachShader(id_, vertex);
	glAttachShader(id_, fragment);
	if (geometryPath) {
		glAttachShader(id_, geometry);
	}
	glLinkProgram(id_);
	CheckCompileErrors(id_, PROGRAM);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath) {
		glDeleteShader(geometry);
	}
}

void Shader::use() {
	glUseProgram(id_);
}

void Shader::SetInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(id_, name.c_str()), 1, &value[0]);
}


void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::CheckCompileErrors(unsigned int shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != PROGRAM) {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::Shader Compilation Error in " << type << "\n" << infoLog << "\n" << endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::Program Compilation Error in " << type << "\n" << infoLog << "\n" << endl;
		}
	}
}
