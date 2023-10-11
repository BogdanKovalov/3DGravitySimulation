#include "Shader.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit);

	try {
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);

		std::stringstream vertexShaderStream, fragmentShaderStream;
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure exception) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << exception.what() << std::endl;
	}
	CompileProgram(vertexCode.c_str(), fragmentCode.c_str());

}

void Shader::CompileProgram(const GLchar* vertexShaderCode, const GLchar* fragmentShaderCode) {
	GLint success;
	GLchar infoLog[512];

	GLuint vertexProgram = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexProgram, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexProgram);
	glGetShaderiv(vertexProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_ERROR: " << infoLog << std::endl;
	}

	GLuint fragmentProgram = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentProgram, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentProgram);
	glGetShaderiv(fragmentProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_ERROR: " << infoLog << std::endl;
	}

	program = glCreateProgram();
	glAttachShader(program, vertexProgram);
	glAttachShader(program, fragmentProgram);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED: " << infoLog << std::endl;
	}

	glDeleteShader(vertexProgram);
	glDeleteShader(fragmentProgram);
}

void Shader::Use() const {
	glUseProgram(program);
}