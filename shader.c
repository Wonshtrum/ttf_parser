#include "shader.h"

u32 compile_shader(const char* name, u32 type, const char* source) {
	u32 id = glCreateShader(type);
	const char* sources[3] = {
		"#version 330 core\n",
		type == GL_VERTEX_SHADER ? "#define VERTEX_STAGE\n" : "#define FRAGMENT_STAGE\n",
		source
	};
	glShaderSource(id, 3, sources, NULL);
	glCompileShader(id);
	i32 result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		i32 length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = alloca(length*sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		glDeleteShader(id);
		MRC_ERROR("Failed to compile %s shader %s:\n%s", (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"), name, message);
	}
	return id;
}

u32 new_shader(const char* name, const char* vs_src, const char* fs_src) {
	u32 id = glCreateProgram();
	u32 vs = compile_shader(name, GL_VERTEX_SHADER, vs_src);
	u32 fs = compile_shader(name, GL_FRAGMENT_SHADER, fs_src);
	glAttachShader(id, vs);
	glAttachShader(id, fs);
	glLinkProgram(id);
	glDeleteShader(vs);
	glDeleteShader(fs);
	i32 result;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		i32 length;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = alloca(length*sizeof(char));
		glGetProgramInfoLog(id, length, &length, message);
		glDeleteProgram(id);
		MRC_ERROR("Failed to link shader %s:\n%s", name, message);
	}
	glValidateProgram(id);
	glGetProgramiv(id, GL_VALIDATE_STATUS, &result);
	if (result == GL_FALSE) {
		i32 length;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = alloca(length*sizeof(char));
		glGetProgramInfoLog(id, length, &length, message);
		glDeleteProgram(id);
		MRC_ERROR("Failed to validate shader %s:\n%s", name, message);
	}
	MRC_INFO("Successfully compiled shader %s", name);
	return id;
}
