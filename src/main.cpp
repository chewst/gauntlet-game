#include "raylib.h"
#include "Grid.hpp"
#include "Player.hpp"
#include "TrappedCharacter.hpp"
#include <memory>

const int TILE_SIZE = 50;    // each tile is 50 × 50 pixels
const int GRID_WIDTH = 16;
const int GRID_HEIGHT = 10;
const int UI_HEIGHT = 60;    // top 60 pixels are reserved for the UI

// map with a border of walls, hazard tiles
Grid buildPeninsulaMap() {
    Grid grid(GRID_WIDTH, GRID_HEIGHT);

    for (int x = 0; x < GRID_WIDTH; ++x) {
        grid.setTile(x, 0, TileType::Wall);
        grid.setTile(x, GRID_HEIGHT - 1, TileType::Wall);
    }
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        grid.setTile(0, y, TileType::Wall);
        grid.setTile(GRID_WIDTH - 1, y, TileType::Wall);
    }

    grid.setTile(6, 4, TileType::Hazard);
    grid.setTile(7, 4, TileType::Hazard);
    grid.setTile(6, 5, TileType::Hazard);
    grid.setTile(7, 5, TileType::Hazard);

    grid.setTile(9, 2, TileType::Exit);

    return grid;
}

Color tileColor(TileType type) {
    switch (type) {
        case TileType::Wall:   return DARKGRAY;
        case TileType::Hazard: return MAROON;
        case TileType::Exit:   return GOLD;
        case TileType::Empty:
        default:                return LIGHTGRAY;
    }
}

bool isAdjacent(const Actor& a, const Actor& b) {
    const int dx = a.getX() - b.getX();
    const int dy = a.getY() - b.getY();
    return (dx * dx + dy * dy) <= 1;
}

void drawGrid(const Grid& grid) {
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            const int px = x * TILE_SIZE;
            const int py = y * TILE_SIZE + UI_HEIGHT;
            DrawRectangle(px, py, TILE_SIZE - 1, TILE_SIZE - 1, tileColor(grid.getTile(x, y)));
        }
    }
}

void drawActor(const Actor& actor) {
    // Center of this actor's tile, in pixels
    const int centerX = actor.getX() * TILE_SIZE + TILE_SIZE / 2;
    const int centerY = actor.getY() * TILE_SIZE + UI_HEIGHT + TILE_SIZE / 2;

    const int radius = TILE_SIZE / 2 - 6;
    DrawCircle(centerX, centerY, static_cast<float>(radius), actor.getColor());
}

int main() {
    InitWindow(GRID_WIDTH * TILE_SIZE, GRID_HEIGHT * TILE_SIZE + UI_HEIGHT, "Malaysia");
    SetTargetFPS(60);  // set the frame rate

    Grid level = buildPeninsulaMap();

    Player player(2, 2, DARKGREEN, "Malay");
    TrappedCharacter trapped(12, 5, RED, "Chinese");
    TrappedCharacter trapped2(6, 8, DARKBLUE, "Indian");

    int hp = 3;
    bool won = false;
    bool lost = false;

    while (!WindowShouldClose()) {
        if (!won && !lost) {
            int dx = 0;
            int dy = 0;
            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    dy = -1;
            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))  dy = 1;
            if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  dx = -1;
            if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) dx = 1;

            if (dx != 0 || dy != 0) {
                const int newX = player.getX() + dx;
                const int newY = player.getY() + dy;

                // if there is a trapped character, the tile is not walkable 
                const bool blockedByChinese = !trapped.isRescued() && trapped.getX() == newX && trapped.getY() == newY;
                const bool blockedByIndian  = !trapped2.isRescued() && trapped2.getX() == newX && trapped2.getY() == newY;

                if (level.isWalkable(newX, newY) && !blockedByChinese && !blockedByIndian) {
                    player.setPosition(newX, newY);

                    if (level.getTile(newX, newY) == TileType::Hazard) {
                        --hp;
                    }
                }
            }

            if (IsKeyPressed(KEY_E) && !trapped.isRescued() && isAdjacent(player, trapped)) {
                trapped.rescue();
            }

            if (IsKeyPressed(KEY_E) && !trapped2.isRescued() && isAdjacent(player, trapped2)) {
                trapped2.rescue();
            }

            if (hp <= 0) {
                lost = true;
            }
            if (trapped.isRescued() && trapped2.isRescued()
                && level.getTile(player.getX(), player.getY()) == TileType::Exit) {
                won = true;
            }
        }

        BeginDrawing();   
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("HP: %d", hp), 12, 12, 24, MAROON);
        DrawText(trapped.isRescued() ? "Chinese: rescued, walk together" : "Chinese: trapped, find and press E", 140, 12, 20, DARKGRAY);
        DrawText(trapped2.isRescued() ? "Indian: rescued, walk together" : "Indian: trapped, find and press E", 140, 36, 20, DARKGRAY);

        drawGrid(level);
        drawActor(player);

        if (!trapped.isRescued()) {
            drawActor(trapped);
        }

        if (!trapped2.isRescued()) {
            drawActor(trapped2);
        }

        if (won) {
            DrawText("United! You win.", GRID_WIDTH * TILE_SIZE / 2 - 140, GRID_HEIGHT * TILE_SIZE / 2 + UI_HEIGHT, 28, DARKGREEN);
        }
        if (lost) {
            DrawText("The journey ends here.", GRID_WIDTH * TILE_SIZE / 2 - 160, GRID_HEIGHT * TILE_SIZE / 2 + UI_HEIGHT, 28, MAROON);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}