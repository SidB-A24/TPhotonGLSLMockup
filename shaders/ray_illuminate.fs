#version 330

out vec4 FragColor;

in vec4 rayData;

float ray_distance(vec2 direction, float stepCount)
{
    direction *= stepCount;

    return pow(direction.x, 2) + pow(direction.y, 2);
}


void main()
{
    //TODO: write the draw calls
    //

    float dist = ray_distance(rayData.xy, rayData.w);

    float brightness =  abs(1 - dist/50);

    //pixelBrightness = vec4(1.0, 1.0, 1.0, brightness);
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}