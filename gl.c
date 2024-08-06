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

int main() {
	//Font font = read_Font("DejaVuSans.ttf");
	Font font = read_Font("vtks.ttf");
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

	u32 shader_test = new_shader("test",
"layout (location = 0) in vec2 a_pos;\n"
"layout (location = 1) in vec2 a_pos2;\n"
"layout (location = 2) in float a_end;\n"
"out vec4 v_color;"
//"const vec2 foffsets[6] = vec2[](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 0), vec2(0, 1), vec2(1, 1));\n"
"const vec2 foffsets[4] = vec2[](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));\n"
"void main() {\n"
"	vec2 d = a_pos - a_pos2;\n"
"	vec2 s = normalize(vec2(d.y, -d.x))*0.003;\n"
"	float z = 20;\n"
"	vec2 offsets[4] = vec2[](z*a_pos-s, z*a_pos+s, z*a_pos2-s, z*a_pos2+s);\n"
"	gl_Position = vec4(offsets[gl_VertexID], 0.0, 1.0);\n"
//"	gl_Position = vec4(a_pos*12+foffsets[gl_VertexID]*0.05, 0.0, 1.0);\n"
"	v_color = vec4(gl_InstanceID == 0, gl_VertexID<2, 1.0, 1.2-a_end);\n"
//"	v_color = vec4(a_skip);\n"
"}",
"in vec4 v_color;\n"
"out vec4 color;\n"
"void main() {\n"
"	color = v_color;\n"
"}");
	glUseProgram(shader_test);

	u32 va;
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);

	u32 vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_SHORT, GL_TRUE, sizeof(Point), 0);
	glVertexAttribDivisor(0, 1);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_SHORT, GL_TRUE, sizeof(Point), (void*)(sizeof(Point)));
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_BYTE, GL_FALSE, sizeof(Point), (void*)(2*sizeof(i16)));
	glVertexAttribDivisor(2, 1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
