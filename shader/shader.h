#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

class Shader
{
private:
	unsigned int shader_program;
public:
	Shader(std::string vert_file, std::string frag_file);
	void use() {
		glUseProgram(shader_program);
	}
};