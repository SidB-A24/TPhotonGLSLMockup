#version 330

uniform vec2 u_lightLocation;
uniform float u_lightBrightness;

out vec4 rayData;
//x, y -> rayOrigin
//z -> rayDirRadians
//w -> steps travelled.

vec2 random_direction(float angleRadians)
{
    return vec2(cos(angleRadians), sin(angleRadians));
}

void main()
{
    float index = gl_FragCoord.x;

    rayData =  vec4(u_lightLocation, index, 0);
}
