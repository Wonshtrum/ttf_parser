all:
	gcc gl.c -Wall -Wextra -lglfw -lGLEW -lGL -o gl
	gcc ttf_test.c -Wall -Wextra -o ttf
