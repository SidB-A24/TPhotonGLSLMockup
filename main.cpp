#include <iostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"

struct Light
{
    Vector2 pos;
    float brightness;
};

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    const int raysPerLight = 256;
    const int passCount = 50;

    InitWindow(screenWidth, screenHeight, "Hardware Raytracing Mockup");

    std::vector<Light> lights;

    lights.push_back(Light{ Vector2(100, 100), 100 });

    Shader loadRaysShader = LoadShader(nullptr, "shaders/ray_load.fs");
    Shader stepRaysShader = LoadShader(nullptr, "shaders/ray_step.fs");
    Shader drawRaysShader = LoadShader("shaders/ray_illuminate.vs", "shaders/ray_illuminate.fs");

    RenderTexture2D rayTex = LoadRenderTexture(raysPerLight, (int)lights.size());
    RenderTexture2D globalIllumationTexture = LoadRenderTexture(screenWidth , screenHeight);

    Mesh unitQuad = GenMeshPlane(1, 1, 1, 1);
    Material defaultMat = LoadMaterialDefault();

    Rectangle rectangle(0, 0, screenWidth, screenHeight);

    Matrix identity = MatrixIdentity();
    std::vector<Matrix> transforms(raysPerLight*lights.size(), identity);

    int lightLocationPos = GetShaderLocation(loadRaysShader, "u_lightPos");
    int lightBrightnessPos = GetShaderLocation(loadRaysShader, "u_lightBrightness");
    int rayTexPos_draw = GetShaderLocation(drawRaysShader, "u_rayTex");
    int rayTexPos_step = GetShaderLocation(stepRaysShader, "u_rayTex");
    int giTexResPos = GetShaderLocation(drawRaysShader, "u_GITextureSize");

    float res[2] = {(float)globalIllumationTexture.texture.width, (float)globalIllumationTexture.texture.height};
    SetShaderValue(drawRaysShader, giTexResPos, res, SHADER_UNIFORM_VEC2);


    while (!WindowShouldClose())
    {

        //Ray texture.
        BeginTextureMode(rayTex);
        {
            //Adding rays onto the texture.
            BeginShaderMode(loadRaysShader);
            for (int i = 0; i < lights.size(); i++)
            {
                Light light = lights[i];
                float pos[2] = {light.pos.x, light.pos.y};
                SetShaderValue(loadRaysShader, lightLocationPos, pos, SHADER_UNIFORM_VEC2);
                SetShaderValue(loadRaysShader, lightBrightnessPos, &light.brightness, SHADER_UNIFORM_FLOAT);


                DrawRectangle(0, i, raysPerLight, 1, WHITE);
            }
            EndShaderMode();

        }
        EndTextureMode();

        for (int i =0; i < passCount; i++)
        {
            //Setting uniform rayTex for access during draw, step.

            SetShaderValueTexture(stepRaysShader, rayTexPos_step, rayTex.texture);

            //Step the rays
            BeginTextureMode(rayTex);
            {
                BeginShaderMode(stepRaysShader);
                DrawRectangle(0, 0, raysPerLight, (int)lights.size(), WHITE);
                EndShaderMode();
            }
            EndTextureMode();

            SetShaderValueTexture(drawRaysShader, rayTexPos_draw, rayTex.texture);

            //Draw the rays
            BeginTextureMode(globalIllumationTexture);
            {
                BeginShaderMode(drawRaysShader);

                //Calls vertex shader raysPerLight*lights times, so for all the rays.
                DrawMeshInstanced(unitQuad, defaultMat, transforms.data(), raysPerLight * (int)lights.size());

                EndShaderMode();
            }
            EndTextureMode();
        }




        BeginDrawing();
        {

        ClearBackground(PINK);
        DrawTextureRec(
        rayTex.texture,
        Rectangle(screenWidth/2, screenHeight/2, rayTex.texture.width, rayTex.texture.height),
        Vector2{0, 0},
        WHITE
        );
        }
        EndDrawing();
    }

    UnloadShader(loadRaysShader);
    UnloadShader(stepRaysShader);
    UnloadShader(drawRaysShader);
    UnloadRenderTexture(rayTex);
    UnloadRenderTexture(globalIllumationTexture);
}



