#include "raylib.h"
#include "Grid.hpp"
#include "Player.hpp"
#include "TrappedCharacter.hpp"

#include "Enemy.hpp"
#include "Tiger.hpp"
#include <memory>
#include <vector>

const int TILE_SIZE = 50;    // each tile is 50 x 50 pixels
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

bool allRescued(const std::vector<TrappedCharacter>& characters) {
    for (const auto& character : characters) {
        if (!character.isRescued()) {
            return false;
        }
    }
    return true;
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

void drawExitMarker(int gridX, int gridY) {
    const float centerX = gridX * TILE_SIZE + TILE_SIZE / 2.0f;
    const float centerY = gridY * TILE_SIZE + UI_HEIGHT + TILE_SIZE / 2.0f;

    // DrawRing(center, innerRadius, outerRadius, startAngle, endAngle, segments, color);
    DrawRing({centerX, centerY}, 10.0f, 20.0f, 0.0f, 360.0f, 32, ORANGE);
}

void drawStatusText(const std::vector<TrappedCharacter>& characters) {
    int lineY = 12;
    for (const auto& character : characters) {
        const std::string status = character.getName() +
            (character.isRescued() ? ": rescued, walk together" : ": trapped, find and press E");
        DrawText(status.c_str(), 140, lineY, 20, DARKGRAY);
        lineY += 24;
    }
}

int main() {
    InitWindow(GRID_WIDTH * TILE_SIZE, GRID_HEIGHT * TILE_SIZE + UI_HEIGHT, "Malaysia");
    SetTargetFPS(60);  // set the frame rate

    Grid level = buildPeninsulaMap();

    Player player(2, 2, DARKGREEN, "Malay");

    std::vector<TrappedCharacter> trappedCharacters = {
        TrappedCharacter(12, 5, RED, "Chinese"),
        TrappedCharacter(5, 7, DARKBLUE, "Indian")
    };

    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Tiger>(4, 4, 10, ORANGE, "Tiger"));

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

                // a tile with an un-rescued trapped character on it is not walkable
                bool blocked = false;
                for (const auto& character : trappedCharacters) {
                    if (!character.isRescued() && character.getX() == newX && character.getY() == newY) {
                        blocked = true;
                        break;
                    }
                }

                if (level.isWalkable(newX, newY) && !blocked) {
                    player.setPosition(newX, newY);

                    if (level.getTile(newX, newY) == TileType::Hazard) {
                        player.takeDamage(1);
                    }

                    // enemies take their turn right after the player moves
                    for (auto& enemy : enemies) {
                        enemy->takeTurn(level, player.getX(), player.getY());
                    }

                    // check for enemy contact once, after everyone has moved this turn
                    for (const auto& enemy : enemies) {
                        if (isAdjacent(player, *enemy)) {
                            player.takeDamage(1);
                        }
                    }
                }
            }

            if (IsKeyPressed(KEY_E)) {
                for (auto& character : trappedCharacters) {
                    if (!character.isRescued() && isAdjacent(player, character)) {
                        character.rescue();
                    }
                }
            }

            if (!player.isAlive()) {
                lost = true;
            }
            if (allRescued(trappedCharacters)
                && level.getTile(player.getX(), player.getY()) == TileType::Exit) {
                won = true;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(TextFormat("HP: %d", player.getHp()), 12, 12, 24, MAROON);
        drawStatusText(trappedCharacters);

        drawGrid(level);
        drawExitMarker(9, 2);    // same as the exit tile
        drawActor(player);

        for (const auto& character : trappedCharacters) {
            if (!character.isRescued()) {
                drawActor(character);
            }
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