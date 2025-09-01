#version 330

uniform sampler2D u_rayTex;
uniform vec2 u_GITextureSize;

out vec4 rayData;

void main()
{
    //int instanceID = gl_InstanceID;
    int textureWidth = textureSize(u_rayTex, 0).x;

    int row = gl_InstanceID % textureWidth;
    int column = gl_InstanceID - textureWidth * row;

    vec4 currentRay = texelFetch(u_rayTex, ivec2(column, row), 0);

    vec2 direction = vec2(cos(currentRay.z), sin(currentRay.z));
    vec2 rayHead = vec2(currentRay.x + direction.x * currentRay.w, currentRay.y + direction.y * currentRay.w);

    vec2 ndc;

    ndc.x = (rayHead.x / float(u_GITextureSize.x)) * 2.0 - 1.0;
    ndc.y = (rayHead.y / float(u_GITextureSize.y)) * 2.0 - 1.0;

    gl_Position = vec4(ndc.xy, 0.0, 1.0);
    rayData = currentRay;
}
