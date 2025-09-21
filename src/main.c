/*---------------------------------------------------------------------------------


    Chronic Echo - SNES Game
    -- Title screen with intro sequence


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <snes/sprite.h>

extern char tilfont, palfont;
extern char sprites_new, sprites_new_pal;

// Include our sprite system
#include "sprites.h"

// Screen states
#define SCREEN_INTRO 0
#define SCREEN_FADEOUT 1
#define SCREEN_BLACK 2
#define SCREEN_TITLE 3
#define SCREEN_GAME 4

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
    bgSetDisable(1);
    bgSetDisable(2);

    // Initialize screen state
    int currentScreen = SCREEN_INTRO;
    int introFrameCount = 0;
    int fadeFrameCount = 0;
    int blackFrameCount = 0;
    int brightness = 15;

    // Main game loop
    while (1) {
        switch (currentScreen) {
            case SCREEN_INTRO:
                // Intro screen: "Made with Copilot"
                if (introFrameCount == 0) {
                    consoleDrawText(8, 14, "Made with Copilot");
                    setScreenOn();
                }

                introFrameCount++;

                // Wait 2.5 seconds (150 frames at 60fps, 125 at 50fps)
                int frames_to_wait = (snes_fps == 60) ? 150 : 125;
                if (introFrameCount >= frames_to_wait) {
                    currentScreen = SCREEN_FADEOUT;
                    fadeFrameCount = 0;
                    brightness = 15;
                }
                break;

            case SCREEN_FADEOUT:
                // Fade out from intro screen
                if (fadeFrameCount % 4 == 0 && brightness > 0) {
                    brightness--;
                    setBrightness(brightness);
                }

                fadeFrameCount++;
                if (brightness <= 0) {
                    currentScreen = SCREEN_BLACK;
                    blackFrameCount = 0;
                }
                break;

            case SCREEN_BLACK:
                // Brief black screen
                blackFrameCount++;
                if (blackFrameCount >= 30) { // ~0.5 seconds at 60fps
                    currentScreen = SCREEN_TITLE;
                    fadeFrameCount = 0;
                    brightness = 0;
                }
                break;

            case SCREEN_TITLE:
                // Title screen
                if (fadeFrameCount == 0) {
                    // Clear the area where "Made with Copilot" was displayed
                    consoleDrawText(8, 14, "                    ");
                    consoleDrawText(10, 12, "CHRONIC ECHO");
                    consoleDrawText(8, 16, "PRESS START");
                    setScreenOn();
                }

                // Fade in title screen
                if (fadeFrameCount % 4 == 0 && brightness < 15) {
                    brightness++;
                    setBrightness(brightness);
                }

                // Check for start button to begin game
                if (padsCurrent(0) & KEY_START) {
                    currentScreen = SCREEN_GAME;
                    // Clear text and prepare for game
                    consoleDrawText(8, 14, "                    ");
                    consoleDrawText(10, 12, "                    ");
                    consoleDrawText(8, 16, "                    ");
                }

                fadeFrameCount++;
                break;

            case SCREEN_GAME:
                // Game screen - update and draw sprites
                updatePlayer();
                drawPlayer();

                // Handle input for player movement
                if (padsCurrent(0) & KEY_LEFT) {
                    movePlayer(-2, 0);
                }
                if (padsCurrent(0) & KEY_RIGHT) {
                    movePlayer(2, 0);
                }
                if (padsCurrent(0) & KEY_UP) {
                    movePlayer(0, -2);
                }
                if (padsCurrent(0) & KEY_DOWN) {
                    movePlayer(0, 2);
                }

                // Press B to return to title
                if (padsCurrent(0) & KEY_B) {
                    currentScreen = SCREEN_TITLE;
                    fadeFrameCount = 0;
                    brightness = 15;
                }
                break;
        }

        // Wait for VBlank
        WaitForVBlank();
    }

    return 0;
}

