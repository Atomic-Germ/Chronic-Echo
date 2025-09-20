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
// Animation constants for 64x64 sprite (8x8 grid of 8x8 tiles)
#define FRAMES_PER_DIRECTION 1  // Each direction uses 1 set of 16 tiles
#define TILE_OFFSET_PER_FRAME 16  // Each frame uses 16 tiles (4x4 for 32x32 sprite)

//---------------------------------------------------------------------------------
// Animation frame lookup table for 64x64 compass sprite
// The 64x64 sprite has 64 tiles arranged as 8x8 grid:
// For a 32x32 sprite, we use 4x4 tiles (16 tiles) from different quadrants
const u8 animationTiles[4][16] = {
    // Right arrow: top-left quadrant (32x32) - tiles 0-3,8-11,16-19,24-27
    {0,1,8,9, 2,3,10,11, 16,17,24,25, 18,19,26,27},  // 16 tiles for 4 OAM entries
    // Left arrow: top-right quadrant (32x32) - tiles 4-7,12-15,20-23,28-31 (will flip)
    {4,5,12,13, 6,7,14,15, 20,21,28,29, 22,23,30,31},
    // Up arrow: bottom-left quadrant (32x32) - tiles 32-35,40-43,48-51,56-59
    {32,33,40,41, 34,35,42,43, 48,49,56,57, 50,51,58,59},
    // Down arrow: bottom-right quadrant (32x32) - tiles 36-39,44-47,52-55,60-63 (will flip)
    {36,37,44,45, 38,39,46,47, 52,53,60,61, 54,55,62,63}
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
    // Initialize player at center of screen
    player.x = 120;  // Center X
    player.y = 104;  // Center Y
    player.vx = 0;
    player.vy = 0;
    player.facing = 0;  // Face right
    player.animationFrame = 0;
    player.spriteId = PLAYER_SPRITE_ID;
    player.active = 1;

    // Clear the OAM entry for the sprite
    oamClear(PLAYER_SPRITE_ID, 1);  // Clear 1 OAM entry

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

    // Basic boundary checking (screen bounds for 16x16 sprite)
    if (player.x < 0) player.x = 0;
    if (player.x > 256 - 16) player.x = 256 - 16;  // 16 for 16x16 sprite
    if (player.y < 0) player.y = 0;
    if (player.y > 224 - 16) player.y = 224 - 16;  // 16 for 16x16 sprite

    // Position updates will be handled in drawPlayer()
}

//---------------------------------------------------------------------------------
void drawPlayer(void)
{
    // Simple 16x16 sprite - use tile 0 for now (first tile in sprite sheet)
    u8 tileIndex = 0;
    
    // Set sprite position and properties
    oamSet(PLAYER_SPRITE_ID, player.x, player.y, 3, 0, 0, tileIndex, 0);
    oamSetEx(PLAYER_SPRITE_ID, OBJ_SMALL, OBJ_SHOW);
    
    // Update OAM
    oamUpdate();
    
    // Debug output
    char buffer[32];
    sprintf(buffer, "SPRITE: X=%d Y=%d", player.x, player.y);
    consoleDrawText(0, 21, buffer);
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