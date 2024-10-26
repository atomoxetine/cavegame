#include "cavegame.h"
#include "../lib/raylib/raylib.h"
#include "../lib/raylib/rcamera.h"
#include "player.h"
#include "world.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float Vec3Dist(const Vector3 a, const Vector3 b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

void DrawDebug(Player *player);

// ------------------------------------------

int main(void) {
    srand(time(NULL));

    InitWindow(SCREEN_W, SCREEN_H, "logemi's cavegame");
    SetTargetFPS(60);

    Image spongeImage = LoadImage("assets/sponge.png");
    Texture2D spongeTexture = LoadTextureFromImage(spongeImage);

    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model spongeModel = LoadModelFromMesh(cube);
    spongeModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = spongeTexture;

    InitWorld();
    Player player = InitPlayer(5, WORLD_Y - 2, 5);

    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        // ---- Update ----

        PlayerUpdateStandingState(&player);
        PlayerDoWalk(&player);
        PlayerDoMouseLook(&player);
        PlayerUpdatePos(&player);
        PlayerUpdateTargetBlock(&player);
        PlayerHandleInteractWithTargetBlock(&player);

        // ---- Draw ----

        BeginDrawing();

        ClearBackground(SKYBLUE);
        Camera camera = PlayerGetCamera(&player);

        BeginMode3D(camera);

        RenderWorld(player.targetBlock, &camera, &spongeModel);

        EndMode3D();

        // Crosshair
        DrawRectangleRec((Rectangle){(float)SCREEN_W / 2 - 7,
                                     (float)SCREEN_H / 2 - 2, 14, 4},
                         WHITE);
        DrawRectangleRec((Rectangle){(float)SCREEN_W / 2 - 2,
                                     (float)SCREEN_H / 2 - 7, 4, 14},
                         WHITE);

#ifdef DEBUG
        DrawDebug(&player);
#endif

        EndDrawing();
    }

    UnloadTexture(spongeTexture);
    CloseWindow();

    return 0;
}

// ------------------------------------------

void DrawDebug(Player *player) {
    DrawFPS(10, 10);

    // Draw info boxes
    DrawRectangle(600, 5, 195, 120, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(600, 5, 195, 120, BLUE);

    DrawText("Camera status:", 610, 15, 10, BLACK);
    DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", player->pos.x,
                        player->pos.y, player->pos.z),
             610, 60, 10, BLACK);
    DrawText(TextFormat("- Rotation: (%06.3f, %06.3f)", player->lookRot.x,
                        player->lookRot.y),
             610, 75, 10, BLACK);
}
