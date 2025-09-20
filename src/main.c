/*---------------------------------------------------------------------------------


    Chronic Echos Title Screen Demo
    -- GitHub Copilot


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <snes/input.h>

// Include our sprite management system
#include "sprites.h"

extern char tilfont, palfont;

// New graphics data
extern char tileset, tileset_pal, tileset_pal_end;
extern char sprites_new, sprites_new_pal, sprites_new_pal_end;

//---------------------------------------------------------------------------------
// Game States
typedef enum {
    GAME_STATE_TITLE,
    GAME_STATE_PLAYING
} GameState;

//---------------------------------------------------------------------------------
// Global game state
GameState currentState = GAME_STATE_TITLE;
GameState previousState = GAME_STATE_TITLE; // Track state changes

//---------------------------------------------------------------------------------
// Function declarations
void initGame(void);
void initBackground(void);
void clearScreen(void);
void handleTitleScreen(void);
void handleGameplay(void);

//---------------------------------------------------------------------------------
int main(void)
{
    // Initialize the game
    initGame();

    // Main game loop - state machine
    while (1)
    {
        // Check if state changed and redraw screen if needed
        if (currentState != previousState)
        {
            clearScreen();
            previousState = currentState;

            // Handle sprite initialization and visibility based on game state
            if (currentState == GAME_STATE_PLAYING)
            {
                // Initialize player when entering gameplay
                if (player.spriteId != PLAYER_SPRITE_ID) {  // Only initialize once
                    initPlayer();
                }
                // Show all 4 OAM entries for the 32x32 sprite
                oamSetVisible(PLAYER_SPRITE_ID, OBJ_SHOW);
                oamSetVisible(PLAYER_SPRITE_ID + 1, OBJ_SHOW);
                oamSetVisible(PLAYER_SPRITE_ID + 2, OBJ_SHOW);
                oamSetVisible(PLAYER_SPRITE_ID + 3, OBJ_SHOW);
            }
            else
            {
                // Hide all 4 OAM entries for the 32x32 sprite
                oamSetVisible(PLAYER_SPRITE_ID, OBJ_HIDE);
                oamSetVisible(PLAYER_SPRITE_ID + 1, OBJ_HIDE);
                oamSetVisible(PLAYER_SPRITE_ID + 2, OBJ_HIDE);
                oamSetVisible(PLAYER_SPRITE_ID + 3, OBJ_HIDE);
            }
            oamUpdate();
        }

        // Handle current game state
        switch (currentState)
        {
            case GAME_STATE_TITLE:
                handleTitleScreen();
                break;
            case GAME_STATE_PLAYING:
                handleGameplay();
                break;
        }

        // Wait for VBlank
        WaitForVBlank();
    }

    return 0;
}

//---------------------------------------------------------------------------------
void initGame(void)
{
    // Initialize text console with our font - use BG2 to avoid conflict with BG1 tilemap
    consoleSetTextMapPtr(0x7000);  // Changed from 0x6800 to avoid conflict
    consoleSetTextGfxPtr(0x3000);
    consoleSetTextOffset(0x0100);
    consoleInitText(2, 16 * 2, &tilfont, &palfont);  // Use BG2 for console

    // Init background - use BG1 for tilemap
    bgSetGfxPtr(1, 0x2000);  // BG1 for tilemap graphics
    bgSetMapPtr(1, 0x6800, SC_32x32);  // BG1 for tilemap data

    // Now Put in 16 color mode and enable BG1 (tilemap) and BG2 (console) and sprites
    setMode(BG_MODE1, BG1_ENABLE | BG1_TSIZE16x16 | BG2_ENABLE | OBJ_ENABLE);
    bgSetDisable(3);  // Disable BG3, keep BG2 enabled for console

    // Load background tileset graphics and palette to BG1
    dmaCopyVram(&tileset, 0x2000, 8192);  // Load tileset to VRAM 0x2000 (256 tiles * 32 bytes each)
    dmaCopyCGram(&tileset_pal, 0, (&tileset_pal_end - &tileset_pal));  // Load tileset palette to CGram 0

    // Load sprite graphics and palette using the new compass sprite
    // Keep as 16x16 size but we'll handle the 32x32 sprite with multiple OAM entries
    oamInitGfxSet(&sprites_new, 2048, &sprites_new_pal, (&sprites_new_pal_end - &sprites_new_pal), 0, 0x4000, OBJ_SIZE16_L32);

    // Initialize sprite system
    initSprites();

    // Initialize player character at a more visible position (only for gameplay)
    // Note: Player will be initialized when entering gameplay state
    // initPlayer();

    // Initialize projectile system
    initProjectiles();

    // Turn on the screen with sprites enabled
    setScreenOn();
}

//---------------------------------------------------------------------------------
void initBackground(void)
{
    // Create a simple 32x32 tilemap using our tileset
    // We'll create a checkerboard pattern with different colored tiles
    u16 tilemap[32 * 32];  // 32x32 tilemap

    int x, y;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 32; x++) {
            // Create a simple pattern using different tile indices
            u16 tileIndex = ((x + y) % 16);  // Cycle through first 16 tiles
            u16 tileAttributes = 0;  // No flip, palette 0, priority 0

            tilemap[y * 32 + x] = tileIndex | tileAttributes;
        }
    }

    // Copy tilemap to VRAM for BG1
    dmaCopyVram(tilemap, 0x6800, sizeof(tilemap));
}

//---------------------------------------------------------------------------------
void clearScreen(void)
{
    // Clear the screen by drawing spaces over the text area
    // This is a simple way to clear since consoleClear() doesn't exist
    int y, x;
    for (y = 0; y < 30; y++) {  // Clear 30 rows
        for (x = 0; x < 32; x++) {  // Clear 32 columns
            consoleDrawText(x, y, " ");
        }
    }
}

//---------------------------------------------------------------------------------
void handleTitleScreen(void)
{
    // Initialize background if not already done
    static bool backgroundInitialized = false;
    if (!backgroundInitialized) {
        initBackground();
        backgroundInitialized = true;
    }

    // Draw the title screen
    consoleDrawText(8, 8, "CHRONIC ECHOS");
    consoleDrawText(4, 12, "TIME MANIPULATION RPG");
    consoleDrawText(2, 16, "FOR SUPER NINTENDO ENTERTAINMENT SYSTEM");
    consoleDrawText(6, 20, "PRESS START TO BEGIN");

    // Add copyright
    consoleDrawText(4, 24, "(C) 2025 NOBODY");
    consoleDrawText(6, 25, "ALL RIGHTS RESERVED");

    // Check for START button press to transition to gameplay
    if (pad_keysdown[0] & KEY_START)
    {
        currentState = GAME_STATE_PLAYING;
    }
}

//---------------------------------------------------------------------------------
void handleGameplay(void)
{
    // Update player character
    updatePlayer();

    // Update projectiles
    updateProjectiles();

    // Handle shooting
    if (pad_keysdown[0] & KEY_A) {
        // Shoot in the direction the player is facing
        s16 vx = 0, vy = 0;
        switch (player.facing) {
            case 0: vx = PROJECTILE_SPEED; break;  // Right
            case 1: vx = -PROJECTILE_SPEED; break; // Left
            case 2: vy = -PROJECTILE_SPEED; break; // Up
            case 3: vy = PROJECTILE_SPEED; break;  // Down
        }
        createProjectile(player.x + PLAYER_WIDTH/2, player.y + PLAYER_HEIGHT/2, vx, vy);
    }

    // Draw player sprite
    drawPlayer();

    // Draw projectiles
    drawProjectiles();

    // Display gameplay HUD
    consoleDrawText(0, 0, "CHRONIC ECHOS - DEMO");
    consoleDrawText(0, 1, "D-Pad: Move  A: Shoot");
    consoleDrawText(0, 2, "START: Return to Title");

    // Show debug information
    debugPlayerInfo();

    // Check for START to return to title
    if (pad_keysdown[0] & KEY_START)
    {
        currentState = GAME_STATE_TITLE;
    }
}

