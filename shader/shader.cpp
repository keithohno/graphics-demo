#include "shader.h"

Shader::Shader(std::string vert_file, std::string frag_file) {

	// read shader code from files
	std::ifstream vert_ifs(vert_file);
	std::ifstream frag_ifs(frag_file);

	// check that shader files were found
	if (!vert_ifs || !frag_ifs) {
		std::cout << "vert shader found: " << (bool)vert_ifs << std::endl;
		std::cout << "frag shader found: " << (bool)frag_ifs << std::endl;
	}

	// read shader code to strings
	std::stringstream buf;
	buf << vert_ifs.rdbuf();
	std::string vert_shader_str = buf.str();
	char const* vert_shader_source = vert_shader_str.c_str();
	buf.str("");
	buf.clear();
	buf << frag_ifs.rdbuf();
	std::string frag_shader_str = buf.str();
	char const* frag_shader_source = frag_shader_str.c_str();

	// compile shaders
	unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vert_shader_source, NULL);
	glCompileShader(vert_shader);
	unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
	glCompileShader(frag_shader);

	// create shader program
	// stored as object field as an unsigned int
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader);
	glAttachShader(shader_program, frag_shader);
	glLinkProgram(shader_program);

	// check errors
	int success;
	char log[512];
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(frag_shader, 512, NULL, log);
		std::cout << "[ERROR] Failed to compile vert shader: " << log << std::endl;
	}
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vert_shader, 512, NULL, log);
		std::cout << "[ERROR] Failed to compile frag shader: " << log << std::endl;
	}

	// delete old vert/frag shaders
	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);

	// use shaders
	glUseProgram(shader_program);

}