#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "types.h"
#include "maracas.h"

#define H_IMPL
#include "shader.h"
#include "ttf.h"

void error_callback(int error, const char* description) {
	MRC_ERROR("GLFW(%d): %s", error, description);
}

int main() {
	if (!glfwInit()) {
		MRC_ABORT("GLFW: failed to initialize");
	}
	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	MRC_INFO("GLFW: initialized");

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);
	if (!window) {
		glfwTerminate();
		MRC_ABORT("GLFW: failed to create window");
	}
	MRC_INFO("GLFW: window created");

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		MRC_ABORT("GLEW: failed to initialize");
	}
	MRC_INFO("GLEW: initialized");

	u32 va;
	glCreateVertexArrays(1, &va);
	u32 vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	new_shader("test",
"layout (location = 0) in vec3 a_pos;\n"
"void main() {\n"
"    gl_Position = vec4(a_pos, 1.0);\n"
"}",
"out vec4 color;\n"
"void main() {\n"
"    color = vec4(1);\n"
"}");

	const char* FILE_NAME = "DejaVuSans.ttf";
	//const char* FILE_NAME = "Alkia.ttf";
	Glyph glyph = read_Font(FILE_NAME);
	(void)glyph;
	//debug_Glyph(&glyph);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	printf("good bye!\n");
}
