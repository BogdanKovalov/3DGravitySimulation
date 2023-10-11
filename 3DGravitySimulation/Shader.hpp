#pragma once
#ifndef _SHADER_H_
#define _SHADER_H_

typedef char GLchar;
typedef unsigned int GLuint;

class Shader {
public:
	GLuint program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use() const;
private:
	void CompileProgram(const GLchar* vertexShaderCode, const GLchar* fragmentShaderCode);
};
#endif //_SHADER_H_