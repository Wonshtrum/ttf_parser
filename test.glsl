#ifdef VERTEX_STAGE
layout (location = 0) in vec2 a_pos1;
layout (location = 1) in vec2 a_pos2;
layout (location = 2) in vec2 a_pos3;
layout (location = 3) in float a_flags1;
layout (location = 4) in float a_flags2;
layout (location = 5) in float a_flags3;
out vec4 v_color;
out vec2 v_xy;

const vec2 foffsets[4] = vec2[](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));
void main() {
	int flags1 = int(a_flags1);
	int flags2 = int(a_flags2);
	int flags3 = int(a_flags3);
	vec2 offsets[4];
	vec2 xys[4] = vec2[](vec2(0, 0), vec2(1, 1), vec2(0.5, 0), vec2(1, 1));
	float z = 20;
	v_xy = vec2(0);
	if ((flags1 & 1) == 1 && (flags2 & 1) == 1) {
		vec2 d = a_pos1 - a_pos2;
		vec2 s = normalize(vec2(d.y, -d.x))*0.003;
		offsets = vec2[](z*a_pos1-s, z*a_pos1+s, z*a_pos2-s, z*a_pos2+s);
	} else if ((flags1 & 1) == 1) {
		vec2 pos3 = a_pos3;
		if ((flags3 & 1) == 0) {
			pos3 = (a_pos2+a_pos3)*0.5;
		}
		offsets = vec2[](z*a_pos1, z*pos3, z*a_pos2, z*pos3);
		v_xy = xys[gl_VertexID];
	} else if ((flags2 & 1) == 0 && (flags3 & 1) == 1) {
		vec2 pos1 = (a_pos1+a_pos2)*0.5;
		offsets = vec2[](z*pos1, z*a_pos3, z*a_pos2, z*a_pos3);
		v_xy = xys[gl_VertexID];
	} else {
		vec2 d = a_pos1 - a_pos2;
		vec2 s = normalize(vec2(d.y, -d.x))*0.003;
		offsets = vec2[](z*a_pos1-s, z*a_pos1+s, z*a_pos2-s, z*a_pos2+s);
	}
	gl_Position = vec4(offsets[gl_VertexID], 0.0, 1.0);

	int[4] flags = int[](flags1, flags3, flags2, 2);
	int on_curve = flags[gl_VertexID] & 1;
	int is_end = flags[0] >> 1 & 1;
	v_color = vec4(on_curve, gl_VertexID>1, 1.0, 1.2-(is_end));
}
#endif

#ifdef FRAGMENT_STAGE
in vec4 v_color;
in vec2 v_xy;
out vec4 color;
void main() {
	float x2 = v_xy.x * v_xy.x;
	float y  = v_xy.y;
	if (x2 > y-0.03 && x2 < y+0.03) {
		color = v_color;
	} else {
		color = vec4(1, 0, 0, 0);
	}
}
#endif
