#version 330

layout(location = 0) in vec3 in_position;
layout(location = 3) in vec2 UV_in;
 out vec2 UV;

void main()
{
    gl_Position = vec4(in_position,1);
	UV = UV_in;
}