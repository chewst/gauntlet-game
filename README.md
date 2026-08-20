# United Malaysia
 
A small grid-based survival game built with the theme of Malaysia Day 
 
## What the game is and how it connects to Malaysia Day
 
You play as a Malay, who starts on the Peninsula map. There are two other characters which are Chinese and Indian that are trapped elsewhere on the map. Walk up to a trapped character and press `E` to free them; each rescue will permanently raises your max HP
 
Along the way, there are two patrolling Tiger and a randomly-appearing Trap that will cause deduction of your HP. Gather all characters and find your way to the final destination, a gold tile with a ring
 
## Building and running
 
Requires **raylib 5.5**

### dependencies

```bash
# dependencies
sudo apt update
sudo apt install 
build-essential git libasound2-dev libx11-dev \
    libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev \
    libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev

 
# raylib itself, pinned to 5.5
git clone --branch 5.5 --depth 1 https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```
 
Control the player by: `WASD` or arrow keys to move, `E` to rescue an adjacent trapped character
 
## Library choice
 
raylib as it provides a window, input, and 2D drawing working quickly, which mattered given the jam's timeframe. 
 
The window/game loop (InitWindow, BeginDrawing/EndDrawing), primitive drawing (DrawRectangle,DrawCircle, DrawRing). texture loading and scaled drawing (LoadTexture, DrawTexturePro), text rendering (DrawText) and keyboard input polling (IsKeyPressed).
 
Everything else is ours: the grid and tile system, all game state (HP, rescue status, trap timers), the turn structure, enemy movement and every class in src.
 
## Where Week 1-7 shows up
 
- **Const-correctness**: read-only methods are marked `const`
  throughout (`Grid::getTile`, `Actor::getX`, `Player::getHp`, etc.), and functions that only read an object take it by `const&`(`isAdjacent(const Actor&, const Actor&)`, `drawGrid(const Grid&)`).
- **Ownership / RAII**: enemies are owned via `std::vector<std::unique_ptr<Enemy>>`, each Tiger is automatically destroyed when the vector goes out of scope, no need to manual delete anywhere. Textures are loaded
  once at startup and explicitly unloaded once at shutdown via `LoadTexture`/`UnloadTexture` per asset
- **Class design / polymorphism**: `Actor` is the base for anything living on the grid; `Player` and `TrappedCharacter` are derived from it directly. Besides, `Enemy` derives from it as an abstract base with `takeTurn` as  pure virtual amd `Tiger` as its concrete implementation. `main.cpp`'s enemy-turn loop calls `takeTurn` polymorphically through `Enemy&`/`Enemy*` without knowing the concrete type
- **STL containers**: `std::vector<TrappedCharacter>` for rescuable characters, `std::vector<std::unique_ptr<Enemy>>` for enemies, `std::vector<std::vector<TileType>>` inside `Grid` for the tiles
- **Algorithms**: `Trap` uses rejection sampling (repeatedly
  generating a random tile and checking it against the grid until an open floor tile is found) to place itself. 
- **Tests**: not yet implemented 

## What we cut, and what we'd do with more time
 
- **The Borneo map:** Originally planned as a second map reached
  after reuniting the Peninsula characters, with Sarawak and Sabah
  characters to find there. 
- **A real pathfinding enemy:**  a `Guard` enemy that chased the player using BFS over the grid (reusing the `neighbours` traversal idea from the Day 6 exercise), with a detection radius so it only gave chase when the player was nearby
- **Catch2 tests.** Time ran out before I got to do this

## Assets
 
All character images, the tiger and the trap icon were AI-generated images. 