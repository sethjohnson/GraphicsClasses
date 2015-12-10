#version 330
in vec3 vp;
in vec3 vc;
out vec4 fColor;

void main () {
	gl_Position = vec4 (vp, 1.0);

	fColor = vec4(vc,01);
}