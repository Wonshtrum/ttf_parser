all:
	gcc gl.c shader.c -Wall -Wextra -lglfw -lGLEW -lGL -o gl
	gcc ttf.c -Wall -Wextra -o ttf
