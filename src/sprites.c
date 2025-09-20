/*---------------------------------------------------------------------------------


    Chronic Echos - Sprite Management System
    -- Player character and object sprite handling


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <snes/sprite.h>

// Include our header file
#include "sprites.h"

//---------------------------------------------------------------------------------
// Global player instance
Player player = {0};  // Initialize to zero

//---------------------------------------------------------------------------------
// Global projectile array
Projectile projectiles[MAX_PROJECTILES];

//---------------------------------------------------------------------------------
// Animation constants for compass sprite (32x32 = 4x4 tiles)
#define FRAMES_PER_DIRECTION 1  // Each direction uses 1 set of 4 tiles
#define TILE_OFFSET_PER_FRAME 4  // Each frame uses 4 tiles (2x2 for 16x16 sprite)

//---------------------------------------------------------------------------------
// Animation frame lookup table for 32x32 compass sprite
// The 32x32 sprite has 16 tiles arranged as 4x4 grid:
// Tiles 0-3: top row, 4-7: second row, 8-11: third row, 12-15: bottom row
// For directional arrows, we'll use different quadrants of the 4x4 sprite
const u8 animationTiles[4][4] = {
    // Right arrow: top-left quadrant (tiles 0,1,4,5)
    {0, 1, 4, 5},   // Frame 0: right arrow
    // Left arrow: top-right quadrant (tiles 2,3,6,7) - will flip horizontally
    {2, 3, 6, 7},   // Frame 1: left arrow
    // Up arrow: bottom-left quadrant (tiles 8,9,12,13)
    {8, 9, 12, 13}, // Frame 2: up arrow
    // Down arrow: bottom-right quadrant (tiles 10,11,14,15) - will flip vertically
    {10, 11, 14, 15} // Frame 3: down arrow
};

//---------------------------------------------------------------------------------
// Function declarations
void initSprites(void);
void initPlayer(void);
void updatePlayer(void);
void drawPlayer(void);
void movePlayer(s16 dx, s16 dy);
void initProjectiles(void);
void createProjectile(s16 x, s16 y, s16 vx, s16 vy);
void updateProjectiles(void);
void drawProjectiles(void);

//---------------------------------------------------------------------------------
void initSprites(void)
{
    // OAM is already initialized by oamInitGfxSet() in main.c
    // Just clear all sprites initially
    oamClear(0, 0);
}

//---------------------------------------------------------------------------------
void initPlayer(void)
{
    // Initialize player at center of screen with better positioning
    player.x = 120;  // Center X minus 16 for 32x32 sprite centering
    player.y = 104;  // Center Y minus 16 for 32x32 sprite centering
    player.vx = 0;
    player.vy = 0;
    player.facing = 0;  // Face right
    player.animationFrame = 0;
    player.spriteId = PLAYER_SPRITE_ID;
    player.active = 1;

    // Clear all 4 OAM entries for the 32x32 sprite
    oamClear(PLAYER_SPRITE_ID, 4);  // Clear 4 consecutive OAM entries

    // The actual OAM setup will be done in drawPlayer()
    oamUpdate();
}

//---------------------------------------------------------------------------------
void updatePlayer(void)
{
    s16 dx = 0;
    s16 dy = 0;

    // Handle input for movement
    if (pad_keys[0] & KEY_LEFT) {
        dx = -PLAYER_SPEED;
        player.facing = 1;  // Face left
    }
    if (pad_keys[0] & KEY_RIGHT) {
        dx = PLAYER_SPEED;
        player.facing = 0;  // Face right
    }
    if (pad_keys[0] & KEY_UP) {
        dy = -PLAYER_SPEED;
        player.facing = 2;  // Face up
    }
    if (pad_keys[0] & KEY_DOWN) {
        dy = PLAYER_SPEED;
        player.facing = 3;  // Face down
    }

    // Apply movement
    movePlayer(dx, dy);

    // No animation frame cycling for compass sprite - direction determines appearance
}

//---------------------------------------------------------------------------------
void movePlayer(s16 dx, s16 dy)
{
    // Store old position
    s16 oldX = player.x;
    s16 oldY = player.y;

    // Apply movement
    player.x += dx;
    player.y += dy;

    // Basic boundary checking (screen bounds for 32x32 sprite)
    if (player.x < 0) player.x = 0;
    if (player.x > 256 - 32) player.x = 256 - 32;  // 32 for 32x32 sprite
    if (player.y < 0) player.y = 0;
    if (player.y > 224 - 32) player.y = 224 - 32;  // 32 for 32x32 sprite

    // Position updates will be handled in drawPlayer()
}

//---------------------------------------------------------------------------------
void drawPlayer(void)
{
    // For 32x32 sprite (4x4 tiles), we need to use 4 OAM entries arranged in 2x2 grid
    // Each OAM entry handles one 16x16 quadrant of the 32x32 sprite

    // Get the correct tile offsets for current direction
    u8 tileOffset = animationTiles[player.facing][0];  // Base tile for this direction

    // Determine flip settings based on direction
    u8 flipX = (player.facing == 1) ? 1 : 0;  // Flip horizontally for left
    u8 flipY = (player.facing == 3) ? 1 : 0;  // Flip vertically for down

    // Set up the 4 OAM entries for the 2x2 grid of 16x16 sprites
    // Top-left quadrant
    oamSet(PLAYER_SPRITE_ID,
           player.x, player.y,           // Position
           3,                           // Priority (highest)
           flipX, flipY,                // Flipping
           tileOffset,                  // Tile offset
           0);                          // Palette
    oamSetEx(PLAYER_SPRITE_ID, OBJ_SMALL, OBJ_SHOW);

    // Top-right quadrant
    oamSet(PLAYER_SPRITE_ID + 1,
           player.x + 16, player.y,     // Position (16 pixels to the right)
           3,                           // Priority
           flipX, flipY,                // Flipping
           tileOffset + 1,              // Next tile
           0);                          // Palette
    oamSetEx(PLAYER_SPRITE_ID + 1, OBJ_SMALL, OBJ_SHOW);

    // Bottom-left quadrant
    oamSet(PLAYER_SPRITE_ID + 2,
           player.x, player.y + 16,     // Position (16 pixels down)
           3,                           // Priority
           flipX, flipY,                // Flipping
           tileOffset + 4,              // Tile 4 rows down
           0);                          // Palette
    oamSetEx(PLAYER_SPRITE_ID + 2, OBJ_SMALL, OBJ_SHOW);

    // Bottom-right quadrant
    oamSet(PLAYER_SPRITE_ID + 3,
           player.x + 16, player.y + 16, // Position (16 pixels right and down)
           3,                           // Priority
           flipX, flipY,                // Flipping
           tileOffset + 5,              // Next tile, 4 rows down
           0);                          // Palette
    oamSetEx(PLAYER_SPRITE_ID + 3, OBJ_SMALL, OBJ_SHOW);

    // Update OAM
    oamUpdate();
}

//---------------------------------------------------------------------------------
// Debug function to display player info
void debugPlayerInfo(void)
{
    consoleDrawText(0, 26, "PLAYER DEBUG:");
    consoleDrawText(0, 27, "X:     Y:     ");
    consoleDrawText(0, 28, "VX:    VY:    ");

    // Display coordinates (simple number display)
    char buffer[32];

    // X position
    sprintf(buffer, "%d", player.x);
    consoleDrawText(3, 27, buffer);

    // Y position
    sprintf(buffer, "%d", player.y);
    consoleDrawText(9, 27, buffer);

    // X velocity
    sprintf(buffer, "%d", player.vx);
    consoleDrawText(4, 28, buffer);

    // Y velocity
    sprintf(buffer, "%d", player.vy);
    consoleDrawText(10, 28, buffer);
}

//---------------------------------------------------------------------------------
void initProjectiles(void)
{
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        projectiles[i].active = 0;
        projectiles[i].spriteId = PLAYER_SPRITE_ID + 1 + i; // Reserve sprite IDs after player
    }
}

//---------------------------------------------------------------------------------
void createProjectile(s16 x, s16 y, s16 vx, s16 vy)
{
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        if (!projectiles[i].active) {
            projectiles[i].x = x;
            projectiles[i].y = y;
            projectiles[i].vx = vx;
            projectiles[i].vy = vy;
            projectiles[i].active = 1;

            // Set up the projectile's OAM entry
            // Use tile 1 (loaded at VRAM 0x4020) and palette 1 (loaded at CGram 144)
            oamSet(projectiles[i].spriteId,
                   projectiles[i].x, projectiles[i].y,
                   3,                           // Priority (highest)
                   0, 0,                        // No flipping
                   1,                           // Tile offset (tile 1)
                   1);                          // Palette (palette 1)

            oamSetEx(projectiles[i].spriteId, OBJ_SMALL, OBJ_SHOW);
            break;
        }
    }
}

//---------------------------------------------------------------------------------
void updateProjectiles(void)
{
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            // Update position
            projectiles[i].x += projectiles[i].vx;
            projectiles[i].y += projectiles[i].vy;

            // Check boundaries - deactivate if off screen
            if (projectiles[i].x < -PROJECTILE_WIDTH ||
                projectiles[i].x > 256 + PROJECTILE_WIDTH ||
                projectiles[i].y < -PROJECTILE_HEIGHT ||
                projectiles[i].y > 224 + PROJECTILE_HEIGHT) {
                projectiles[i].active = 0;
                oamSetVisible(projectiles[i].spriteId, OBJ_HIDE);
            } else {
                // Update sprite position
                oamSetXY(projectiles[i].spriteId, projectiles[i].x, projectiles[i].y);
            }
        }
    }
}

//---------------------------------------------------------------------------------
void drawProjectiles(void)
{
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        if (projectiles[i].active) {
            oamSetVisible(projectiles[i].spriteId, OBJ_SHOW);
        }
    }
}