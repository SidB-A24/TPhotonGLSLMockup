#version 330

out vec4 FragColor;

uniform vec2 u_lightLocation;
uniform float u_lightBrightness;

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

    //FragColor =  vec4(u_lightLocation, index, 1.0);
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
