#version 330
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 volumeModelMatrix;

in vec3 vp;
in vec3 vc;
out vec4 fColor;
out vec4 fPosition;
out vec4 fXYZ;


void main () {
	gl_Position = projectionMatrix*viewMatrix*modelMatrix*vec4 (vp, 1.0);

	fColor = vec4(vc,01);

	fPosition = (volumeModelMatrix)*modelMatrix*vec4 (vp, 1.0);
}