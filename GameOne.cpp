#include <iostream>
#include <vector>
#include "raylib.h"
#include "raymath.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"

const int screenWidth = 1920;
const int screenHeight = 1440;

void PlayerMovement(Player&, float);
void Shoot(Player&, float);
void Kill(Player&, Game&);
void UpdateProjectiles(Player&, Game&, float);
void UpdateEnemies(Player&, Game&, float);
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "GameOne");
    SetTargetFPS(60);
    DisableCursor();

    Game thisGame = { 0, 1, 0, 0, 10 };
    Player player = { 100.0f, 0.0f, 30.0f, 0.0f, 0, 0, { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 70.0f, CAMERA_PERSPECTIVE } };
    float dt = 0.0;
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (thisGame.gameState == 0) {
            dt = GetFrameTime();
            PlayerMovement(player, dt);
            Shoot(player, dt);
            UpdateProjectiles(player, thisGame, dt);
            UpdateEnemies(player, thisGame, dt);

            if (thisGame.enemies.empty()) {
                if (thisGame.incrementRound()) EnableCursor();
                else thisGame.spawnEnemies();
            }

            if (player.health <= 0.0f) {
                Kill(player, thisGame);
            }
        }
        else if (thisGame.gameState == 1) {
            if (IsKeyPressed(KEY_SPACE)) {
                thisGame.gameState = 0;
                player.health = 100.0f;
                player.camera.position = Vector3Add(player.camera.position, { 0.0f, 50.0f, 0.0f });
                player.camera.target = Vector3Add(player.camera.target, { 0.0f, 50.0f, 0.0f });
                if (!IsCursorHidden()) {
                    DisableCursor();
                }
            }
        }
        else if (thisGame.gameState == 2){
            if (IsKeyPressed(KEY_SPACE)) {
                thisGame.gameState = 0;
                player.health = 100.0f;
                player.kills = 0;
                player.camera.position = { 0.0f, 10.0f, 10.0f };
                player.camera.target = { 0.0f, 0.0f, 0.0f };
                if (!IsCursorHidden()) {
                    DisableCursor();
                }
            }
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        if (thisGame.gameState == 0 || thisGame.gameState == 1) {
            BeginMode3D(player.camera);

            DrawCube({ 51.0f, 50.0f, 0.0f }, 0, 100, 102, { 158, 2, 13, 255 });
            DrawCube({ 0.0f, 50.0f, 51.0f }, 102, 100, 0, { 158, 2, 13, 255 });
            DrawCube({ -51.0f, 50.0f, 0.0f }, 0, 100, 102, { 158, 2, 13, 255 });
            DrawCube({ 0.0f, 50.0f, -51.0f }, 102, 100, 0, { 158, 2, 13, 255 });
            DrawPlane({ 0.0f, 0.0f, 0.0f }, { 104.0f, 104.0f }, { 158, 2, 13, 255 });
            DrawCube({ 0.0f, 100.0f, 0.0f }, 104, 0, 1040, { 158, 2, 13, 255 });

            for (auto const& p : player.projectiles) {
                if (p.time < 0.1f) {
                    Vector3 perpendicular = Vector3Add(Vector3Normalize(Vector3CrossProduct(p.direction, { 0.0f, 1.0f, 0.0f })), { 0.0f, -0.4f, 0.0f });
                    float t = p.direction.y > 0 ? 20.0f : std::min(20.0f, -p.position.y / p.direction.y);
                    Vector3 endPoint = p.hit ? p.hitPoint : Vector3Add(p.position, Vector3Scale(p.direction, t));
                    DrawCapsule(Vector3Add(p.position, perpendicular), endPoint, 0.05, 6, 6, PINK);
                }
                if (p.hit) {
                    DrawSphere(p.hitPoint, 0.5f, ORANGE);
                }
            }

            for (auto const& e : thisGame.enemies) {
                //DrawCube(e.position, 1, 1, 1, BLACK);
                DrawCapsule(Vector3Add(e.position, { 0.0f, e.size, 0.0f }), Vector3Add(e.position, { 0.0f, 3.0f * e.size, 0.0f}), e.size, 6, 6, BLACK);
                DrawBoundingBox(e.bound, GREEN);
            }

            DrawGrid(100, 1.0f);

            EndMode3D();

            DrawRectangle(GetScreenWidth() / 2 - 2, GetScreenHeight() / 2 - 20, 4, 40, GREEN);
            DrawRectangle(GetScreenWidth() / 2 - 20, GetScreenHeight() / 2 - 2, 40, 4, GREEN);

            DrawRectangle(18, GetScreenHeight() - 48, 704, 44, BLACK);
            if (player.health >= 0.0f) {
                DrawRectangle(20, GetScreenHeight() - 50, player.health * 7, 40, RED);
            }
            if (thisGame.gameState == 1) {
                
                DrawRectangle(GetScreenWidth() / 4, GetScreenHeight() / 4, GetScreenWidth() / 2, GetScreenHeight() / 2, RAYWHITE);
                DrawText(TextFormat("Level: %d", thisGame.level), GetScreenWidth() / 2 - (MeasureText(TextFormat("Level: %d", thisGame.level), 100) / 2), GetScreenHeight() / 2 - 100, 100, BLACK);

            }
        }
        else {
            DrawText("DIED LOL", GetScreenWidth() / 2 - (MeasureText("DIED LOL", 100) / 2), GetScreenHeight() / 2, 100, RED);
            DrawText(TextFormat("Kills: %d", player.kills), GetScreenWidth() / 2 - (MeasureText(TextFormat("Kills: %d", player.kills), 100) / 2), GetScreenHeight() / 2 + 100, 100, RED);
            
        }
        DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void PlayerMovement(Player& player, float deltaTime) {
    //rotation
    player.camera.target = Vector3Add(Vector3RotateByAxisAngle(Vector3Subtract(player.camera.target, player.camera.position), { 0.0f, 1.0f, 0.0f }, -GetMouseDelta().x * 0.5f * deltaTime), player.camera.position);
    float rotAngle = -GetMouseDelta().y * 0.5f * deltaTime;
    float camAngle = Vector3Angle(Vector3Subtract(player.camera.target, player.camera.position), {0.0f, 1.0f, 0.0f});
    if ((camAngle - rotAngle <= 0.1f) || (camAngle - rotAngle >= PI - 0.1f)) {
        rotAngle = 0.0f;
    }
    player.camera.target = Vector3Add(Vector3RotateByAxisAngle(Vector3Subtract(player.camera.target, player.camera.position), Vector3CrossProduct(Vector3Subtract(player.camera.target, player.camera.position), { 0.0f, 1.0f, 0.0f }), rotAngle), player.camera.position);
    
    //movement
    Vector3 Direction = Vector3Normalize(Vector3Subtract(player.camera.target, player.camera.position));
    Vector3 transformation = { 0.0f, 0.0f, 0.0f };
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S)) {
        transformation = Vector3Add(transformation, (Vector3Multiply(Direction, { player.moveSpeed * deltaTime, 0.0f, player.moveSpeed * deltaTime })));
        if (IsKeyDown(KEY_S)) transformation = Vector3Negate(transformation);
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_A)) {
        Vector3 transformationHorizontal = (Vector3Multiply(Vector3CrossProduct(Direction, { 0.0f, 1.0f, 0.0f }), { player.moveSpeed * deltaTime, 0.0f, player.moveSpeed * deltaTime }));
        if (IsKeyDown(KEY_A)) transformationHorizontal = Vector3Negate(transformationHorizontal);
        transformation = Vector3Add(transformation, transformationHorizontal);
    }
    Vector3 newPos = Vector3Add(player.camera.position, transformation);
    if (newPos.x > 50.0f || newPos.x < -50.0f) {
        transformation.x = 0.0f;
    }
    if (newPos.z > 50.0f || newPos.z < -50.0f) {
        transformation.z = 0.0f;
    }
    player.camera.position = Vector3Add(player.camera.position, transformation);
    player.camera.target = Vector3Add(player.camera.target, transformation);
    
    //Jumping
    if (player.camera.position.y < 1.0f) {
        player.camera.position.y = 1.0f;
        player.verticalVelocity = 0.0f;
    }

    if (player.camera.position.y == 1.0f && IsKeyDown(KEY_SPACE)) {
        player.verticalVelocity = 0.4f;
    }

    if (player.camera.position.y > 1.0f) {
        player.verticalVelocity -= 0.5f * deltaTime;
    }

    if (player.verticalVelocity != 0.0f) {
        player.camera.position.y += player.verticalVelocity;
        player.camera.target.y += player.verticalVelocity;
    }
}

void Shoot(Player& player, float dt) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (player.shotCooldown <= 0.0f) {
            player.projectiles.push_back(Projectile(player.camera.position, Vector3Normalize(Vector3Subtract(player.camera.target, player.camera.position)), 55));
            player.shotCooldown = 0.2f;
            player.shots++;
        }
    }
    player.shotCooldown -= dt;
}

void Kill(Player& player, Game& thisGame) {
    thisGame.gameState = 2;
    thisGame.enemies.clear();
    player.projectiles.clear();
    thisGame.enemyCount = 0;
    player.shots = 0;
    player.shotCooldown = 0.0f;
    thisGame.level = 1;
    thisGame.round = 0;
    if (IsCursorHidden()) {
        EnableCursor();
    }
}

void UpdateProjectiles(Player& player, Game& thisGame, float dt) {
    for (auto it = player.projectiles.begin(); it != player.projectiles.end(); ) {
        it->UpdateTime(dt);
        float minDistance = 99999999;
        int closestIndex = -1;
        for (int i = 0; i < thisGame.enemies.size(); i++) {
            RayCollision rayCollision = GetRayCollisionBox(it->collisionRay, thisGame.enemies.at(i).bound);
            if (rayCollision.hit && Vector3Distance(rayCollision.point, player.camera.position) < minDistance) {
                minDistance = Vector3Distance(rayCollision.point, player.camera.position);
                closestIndex = i;
                it->hitPoint = rayCollision.point;
            }
        }
        if (closestIndex >= 0 && !it->hit) {
            it->hit = true;
            thisGame.enemies.at(closestIndex).health -= it->damage;
            if (thisGame.enemies.at(closestIndex).health <= 0.0f) {
                thisGame.enemies.erase(thisGame.enemies.begin() + closestIndex);
                player.kills++;
            }
        }

        if (it->time > 0.4f) {
            it = player.projectiles.erase(it);
        }
        else {
            ++it;
        }
    }
}
void UpdateEnemies(Player& player, Game& thisGame, float dt) {
    for (auto& e : thisGame.enemies) {
        if (Vector3Distance(e.position, player.camera.position) > 3.0f) {
            e.UpdatePosition(player.camera.position, dt);
        }
        else {
            player.health -= e.Attack(dt);
        }
    }
}