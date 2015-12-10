#version 330
out vec4 frag_colour;
in vec4 fColor;

void main () {
	frag_colour = fColor;
}