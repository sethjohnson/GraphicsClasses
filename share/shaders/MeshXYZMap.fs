#version 330
layout(location = 0) out vec4 color;
uniform vec3 boundingBox[2];

in vec4 fColor;
in vec4 fPosition;
in vec4 fXYZ;

float map(float value, float low1, float high1, float low2, float high2)
{
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

void main () {

    float X = map(fPosition.x,-0.5, 0.5, 0, 1);
    float Y = map(fPosition.y,-0.5, 0.5, 0, 1);
    float Z = map(fPosition.z,-0.5, 0.5, 0, 1);


    color = vec4(X,Y,Z,1);

}