#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>

#include "types.h"
#include "maracas.h"

u32 compile_shader(const char* name, u32 type, const char* source);
u32 new_shader(const char* name, const char* vs_src, const char* fs_src);

#endif
