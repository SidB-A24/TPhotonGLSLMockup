#version 330

uniform sampler2D u_rayTex;

out vec4 rayData;

void main()
{
    ivec2 currentTexelCoord = ivec2(gl_FragCoord.xy);

    vec4 currentRay = texelFetch(rayData, currentTexelCoord);

    //Stepping the ray. The actual head pos can be determined using origin + direction * distance.
    rayData = vec4(currentRay.xyz, currentRay.w + 1);
}