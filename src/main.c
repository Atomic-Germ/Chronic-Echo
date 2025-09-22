/*---------------------------------------------------------------------------------


    Chronic Echo - SNES Game
    -- Title screen with intro sequence


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <snes/sprite.h>

extern char tilfont, palfont;
extern char sprites_simple, sprites_simple_pal;

// Include our sprite system
#include "sprites.h"

// Include the state manager
#include "state_manager.h"

//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
int main(void)
{
    // Initialize text console with our font
    consoleSetTextMapPtr(0x6800);
    consoleSetTextGfxPtr(0x3000);
    consoleSetTextOffset(0x0100);
    consoleInitText(0, 16 * 2, &tilfont, &palfont);

    // Explicitly load font graphics into VRAM
    dmaCopyVram(&tilfont, 0x3000, sizeof(tilfont));

    // Initialize sprites
    initSprites();
    initPlayer();

    // Init background
    bgSetGfxPtr(0, 0x2000);
    bgSetMapPtr(0, 0x6800, SC_32x32);

    // Now Put in 16 color mode and disable Bgs except current
    setMode(BG_MODE1, 0);
    bgSetEnable(0);
    bgSetDisable(1);
    bgSetDisable(2);

    // Initialize the state manager
    initStateManager(&g_stateManager);

    // Register all game states
    registerState(&g_stateManager, STATE_INTRO, createIntroState());
    registerState(&g_stateManager, STATE_FADEOUT, createFadeoutState());
    registerState(&g_stateManager, STATE_BLACK, createBlackState());
    registerState(&g_stateManager, STATE_TITLE, createTitleState());
    registerState(&g_stateManager, STATE_GAME, createGameState());
    registerState(&g_stateManager, STATE_GAME_FADEOUT, createGameFadeoutState());
    registerState(&g_stateManager, STATE_TITLE_FADEOUT, createTitleFadeoutState());

    // Start with intro state
    changeState(&g_stateManager, STATE_INTRO);

    // Enable the screen now that everything is set up
    setScreenOn();

    // Draw the intro text directly for testing
    consoleDrawText(8, 14, "Made with Copilot");
    consoleDrawText(8, 16, "  and pvsneslib  ");

    // Main game loop
    while (1) {
        // Update the current state (handles transitions automatically)
        updateStateManager(&g_stateManager);

        // Draw the current state
        drawStateManager(&g_stateManager);

        // Wait for VBlank
        WaitForVBlank();
    }

    return 0;
}

