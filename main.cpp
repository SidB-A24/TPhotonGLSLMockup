#include <iostream>
#include <vector>

#include "raylib.h"

struct Light
{
    Vector2 pos;
    float brightness;
};

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    const int raysPerLight = 32;
    const int passCount = 50;

    InitWindow(screenWidth, screenHeight, "Hardware Raytracing Mockup");

    std::vector<Light> lights;

    Shader loadRaysShader = LoadShader(0, "shaders/ray_load.fs");
    Shader stepRaysShader = LoadShader(0, "shaders/ray_step.fs");
    Shader drawRaysShader = LoadShader("shaders/ray_illuminate.vs", "shaders/ray_illuminate.fs");

    RenderTexture2D rayTex = LoadRenderTexture(raysPerLight, lights.size());
    RenderTexture2D globalIllumationTexture = LoadRenderTexture(screenWidth , screenHeight);

    Mesh unitQuad = GenMeshPlane(1, 1, 1, 1);
    Material defaultMat = LoadMaterialDefault();


    int lightLocationPos = GetShaderLocation(loadRaysShader, "u_lightPos");
    int lightBrightnessPos = GetShaderLocation(loadRaysShader, "u_lightBrightness");
    int rayTexPos_draw = GetShaderLocation(drawRaysShader, "u_rayTex");
    int rayTexPos_step = GetShaderLocation(stepRaysShader, "u_rayTex");

    while (!WindowShouldClose())
    {

        //Ray texture.
        BeginTextureMode(rayTex);
        {
            //Adding rays onto the texture.
            ClearBackground(BLACK);
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
            SetShaderValueTexture(stepRaysShader, rayTexPos_step, rayTex.texture)
            SetShaderValueTexture(drawRaysShader, rayTexPos_draw, rayTex.texture);

            //Step the rays
            BeginTextureMode(rayTex);
            {
                BeginShaderMode(stepRaysShader);

                EndShaderMode();
            }
            EndTextureMode();



            //Draw the rays
            BeginTextureMode(globalIllumationTexture);
            {
                BeginShaderMode(drawRaysShader);

                //Calls vertex shader raysPerLight*lights times, so for all the rays.
                DrawMeshInstanced(unitQuad, defaultMat, nullptr, raysPerLight * (int)lights.size());

                EndShaderMode();
            }
            EndTextureMode();
        }




        BeginDrawing();
        {

        }
        EndDrawing();
    }

}



int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    const int raysPerLight = 32;
    const int numLights = 4;

    InitWindow(screenWidth, screenHeight, "TEngine Ray Spawn Boilerplate");

    // Load shaders from files
    Shader loadRaysShader = LoadShader(0, "loadRays.fs"); // fragment shader only
    Shader stepRaysShader = LoadShader(0, "stepRays.fs");

    // Create a floating-point texture to hold rays
    RenderTexture2D rayTex = LoadRenderTexture(raysPerLight, numLights);

    // Example uniforms
    Vector2 lightPos = { 400.0f, 300.0f };
    int uLightPosLoc = GetShaderLocation(loadRaysShader, "lightPos");
    int uResolutionLoc = GetShaderLocation(loadRaysShader, "resolution");

    float resolution[2] = { (float)raysPerLight, (float)numLights };

    SetShaderValue(loadRaysShader, uLightPosLoc, &lightPos, SHADER_UNIFORM_VEC2);
    SetShaderValue(loadRaysShader, uResolutionLoc, resolution, SHADER_UNIFORM_VEC2);

    while (!WindowShouldClose())
    {
        // Spawn rays into the texture
        BeginTextureMode(rayTex);
        ClearBackground(BLACK);
        BeginShaderMode(loadRaysShader);
        DrawRectangle(0, 0, raysPerLight, numLights, WHITE);
        EndShaderMode();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        // Just display the texture for debugging
        DrawTexture(rayTex.texture, 0, 0, WHITE);

        EndDrawing();
    }

    UnloadRenderTexture(rayTex);
    UnloadShader(loadRaysShader);
    UnloadShader(stepRaysShader);

    CloseWindow();

    return 0;
}
