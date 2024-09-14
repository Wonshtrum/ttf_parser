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

char CHAR = 0;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	(void)scancode;
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			return;
		}
		char c = 0;
		if (key >= 32 && key <= 126) {
			if ((mods & 1) == 0 && key >= 'A' && key <= 'Z') {
				c = key - 'A' + 'a';
			} else {
				c = key;
			}
		}
		CHAR = c;
	}
}

u32* triangulate(Glyph* g) {
	u32 end = g->contours[0];
	Point* points = g->points;
	for (int i=0; i<end; i++) {
		points[i];
	}
}

int main(int argc, char** argv) {
	Font font;
	if (argc > 1) {
		font = read_Font(argv[1]);
	} else {
		font = read_Font("DejaVuSans.ttf");
	}
	debug_Font(&font);

	if (!glfwInit()) {
		MRC_ABORT("GLFW: failed to initialize");
	}
	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	MRC_INFO("GLFW: initialized");

	GLFWwindow* window = glfwCreateWindow(480, 480, "Hello OpenGL", NULL, NULL);
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

	glfwSetKeyCallback(window, key_callback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	char* shader_src = (void*)read_file("test.glsl").ptr;
	MRC_WARN("%s", shader_src);
	u32 shader_test = new_shader("test", shader_src, shader_src);
	free(shader_src);
	glUseProgram(shader_test);

	u32 va;
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	u32 vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	for (int i=0; i<3; i++) {
		size_t offset = i*sizeof(Point);
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, 2, GL_SHORT, GL_TRUE, sizeof(Point), (void*)offset);
		glVertexAttribDivisor(i, 1);
		glEnableVertexAttribArray(i+3);
		glVertexAttribPointer(i+3, 1, GL_BYTE, GL_FALSE, sizeof(Point), (void*)(offset+offsetof(Point, flags)));
		glVertexAttribDivisor(i+3, 1);
	}

	int n_points = 0;
	char c = 0;
	while (!glfwWindowShouldClose(window)) {
		if (c != CHAR) {
			c = CHAR;
			Glyph glyph = get_Glyph(&font, c);
			debug_Glyph(&glyph);
			n_points = vector_len(glyph.points);
			glBufferData(GL_ARRAY_BUFFER, vector_size(glyph.points), glyph.points, GL_STATIC_DRAW);
			free_Glyph(&glyph);
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, n_points-1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	free_Font(&font);
	printf("good bye!\n");
}
