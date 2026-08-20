#include "raylib.h"
#include "Grid.hpp"
#include "Player.hpp"
#include "TrappedCharacter.hpp"

#include "Enemy.hpp"
#include "Tiger.hpp"
#include "Trap.hpp"
#include <memory>
#include <vector>
#include <ctime>

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

    grid.setTile(6, 3, TileType::Hazard);
    grid.setTile(13, 3, TileType::Hazard);
    grid.setTile(3, 6, TileType::Hazard);
    grid.setTile(10, 7, TileType::Hazard);

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

void drawTexture(Texture2D texture, int gridX, int gridY) {
    // convert grid coordinates to screen pixel coordinates
    const int centerX = gridX * TILE_SIZE + TILE_SIZE / 2;
    const int centerY = gridY * TILE_SIZE + UI_HEIGHT + TILE_SIZE / 2;

    const float destSize = TILE_SIZE * 0.85f;  // make it slightly smaller than the tile

    Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };  // use whole image
    Rectangle dest   = { (float)centerX, (float)centerY, destSize, destSize }; // where and how large to draw it
    Vector2 origin   = { destSize / 2.0f, destSize / 2.0f };  // set the image's center as its pivot

    // drawtexturepro allow to resize with the gridsize
    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
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

    srand( time(NULL) );  // use the current time to generate seed

    Texture2D malayTexture = LoadTexture("assets/malay.png");
    Texture2D chineseTexture = LoadTexture("assets/chinese.png");
    Texture2D indianTexture = LoadTexture("assets/indian.png");
    Texture2D tigerTexture = LoadTexture("assets/tiger.png");
    Texture2D trapTexture = LoadTexture("assets/trap.png");

    Grid level = buildPeninsulaMap();

    Player player(2, 2, DARKGREEN, "Malay");

    std::vector<TrappedCharacter> trappedCharacters = {
        TrappedCharacter(13, 5, RED, "Chinese"),
        TrappedCharacter(5, 7, BLUE, "Indian")
    };

    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Tiger>(4, 4, 10, ORANGE, "Tiger"));
    enemies.push_back(std::make_unique<Tiger>(11, 3, 8, ORANGE, "Tiger2", true)); // allow vertical

    Trap trap(GRID_WIDTH, GRID_HEIGHT);

    bool won = false;
    bool lost = false;

    // for text display
    std::string damageMessage;
    int damageMessageTimer = 0;   // counts down frames; message shows while > 0

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
                        damageMessage = "Ouch! Hazard hurt you! HP is now " + std::to_string(player.getHp());
                        damageMessageTimer = 90;   // roughly 1.5 seconds at 60 FPS
                    }

                    // enemies take their turn right after the player moves
                    for (auto& enemy : enemies) {
                        enemy->takeTurn(level);
                    }
                    
                    trap.update(level);

                    // check for enemy contact once, after everyone has moved this turn
                    for (const auto& enemy : enemies) {
                        if (isAdjacent(player, *enemy)) {
                            player.takeDamage(1);
                            damageMessage = enemy->getName() + " hurt you! HP is now " + std::to_string(player.getHp());
                            damageMessageTimer = 90;
                        }
                    }

                    if (trap.isActiveAt(newX, newY)) {
                        player.takeDamage(1);
                        damageMessage = "You stepped into a trap! HP is now " + std::to_string(player.getHp());
                        damageMessageTimer = 90;
                    }
                }
            }

            if (IsKeyPressed(KEY_E)) {
                for (auto& character : trappedCharacters) {
                    if (!character.isRescued() && isAdjacent(player, character)) {
                        character.rescue();
                        player.increaseMaxHp(1);
                        damageMessage = "Unity gives strength! " + character.getName()
                            + " joined. Max HP is now " + std::to_string(player.getHp());
                        damageMessageTimer = 90;
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

        DrawText(TextFormat("HP: %d/%d", player.getHp(), player.getMaxHp()), 12, 12, 24, MAROON);
        drawStatusText(trappedCharacters);

        drawGrid(level);
        if (trap.isActive()) {
            drawTexture(trapTexture, trap.getX(), trap.getY());
        }

        drawExitMarker(9, 2);    // same as the exit tile
        drawTexture(malayTexture, player.getX(), player.getY());

        for (const auto& enemy : enemies) {
            drawTexture(tigerTexture, enemy->getX(), enemy->getY()); 
        }

        if (damageMessageTimer > 0) {
            --damageMessageTimer;

            const int fontSize = 20;
            const int textWidth = MeasureText(damageMessage.c_str(), fontSize);

            const int margin = 12;
            const int textX = GRID_WIDTH * TILE_SIZE - textWidth - margin;
            const int textY = GRID_HEIGHT * TILE_SIZE + UI_HEIGHT - fontSize - margin;

            // small background box so the text stays readable over any tile colour
            DrawRectangle(textX - 8, textY - 4, textWidth + 16, fontSize + 8, Fade(BLACK, 0.5f));
            DrawText(damageMessage.c_str(), textX, textY, fontSize, WHITE);
        }

        for (const auto& character : trappedCharacters) {
            if (!character.isRescued()) {
                const Texture2D& texture = (character.getName() == "Chinese") ? chineseTexture : indianTexture;
                drawTexture(texture, character.getX(), character.getY());
            }
        }

        if (won) {
            DrawText("United! You win.", GRID_WIDTH * TILE_SIZE / 2 - 140, GRID_HEIGHT * TILE_SIZE / 2 + UI_HEIGHT, 28, DARKGREEN);
        }
        if (lost) {
            DrawText("The journey ends here.", GRID_WIDTH * TILE_SIZE / 2 - 160, GRID_HEIGHT * TILE_SIZE / 2 + UI_HEIGHT, 28, DARKBLUE);
        }

        EndDrawing();
    }

    UnloadTexture(malayTexture);
    UnloadTexture(chineseTexture);
    UnloadTexture(indianTexture);
    UnloadTexture(tigerTexture);
    UnloadTexture(trapTexture);

    CloseWindow();
    return 0;
}