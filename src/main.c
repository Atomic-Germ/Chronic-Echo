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

// Include our player system
#include "player.h"

// Include our time manipulation system
#include "time_manipulation.h"

// Screen states
#define SCREEN_INTRO 0
#define SCREEN_FADEOUT 1
#define SCREEN_BLACK 2
#define SCREEN_TITLE 3
#define SCREEN_GAME 4
#define SCREEN_GAME_FADEOUT 5
#define SCREEN_TITLE_FADEOUT 6

//---------------------------------------------------------------------------------
// Screen clearing helper function
void clearScreenForTransition(void) {
    // Clear all console text by drawing spaces over the entire console area
    // Console is typically 32x28 characters, but we'll clear a safe area
    int y, x;
    for (y = 0; y < 32; y++) {
        for (x = 0; x < 32; x += 20) {  // Draw in chunks of 20 spaces
            consoleDrawText(x, y, "                    ");
        }
    }

    // Clear all sprites
    oamClear(0, 0);
    oamUpdate();

    // Wait for VBlank to ensure clearing takes effect
    WaitForVBlank();

    // Reset any other screen state as needed
    // (Add more clearing logic here as the game grows)
}

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

    // Initialize player character system
    initPlayerCharacter();

    // Initialize time manipulation system
    initPositionHistory();

    // Init background
    bgSetGfxPtr(0, 0x2000);
    bgSetMapPtr(0, 0x6800, SC_32x32);

    // Now Put in 16 color mode and disable Bgs except current
    setMode(BG_MODE1, 0);
    bgSetDisable(1);
    bgSetDisable(2);

    // Set initial brightness
    setBrightness(15);

    // Initialize screen state
    int currentScreen = SCREEN_INTRO;
    int introFrameCount = 0;
    int fadeFrameCount = 0;
    int blackFrameCount = 0;
    int brightness = 15;

    // Input state for time manipulation
    u16 previousPadState = 0;

    // Main game loop
    while (1) {
        switch (currentScreen) {
            case SCREEN_INTRO:
                // Intro screen: "Made with Copilot"
                if (introFrameCount == 0) {
                    consoleDrawText(8, 14, "Made with Copilot");
                    consoleDrawText(8, 16, "  and pvsneslib  ");
                    setScreenOn();
                }

                introFrameCount++;

                // Wait 2.5 seconds (150 frames at 60fps, 125 at 50fps)
                int frames_to_wait = (snes_fps == 60) ? 150 : 125;
                if (introFrameCount >= frames_to_wait) {
                    // Start fade out without clearing screen yet
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
                    // Clear screen during black screen transition
                    clearScreenForTransition();
                    currentScreen = SCREEN_BLACK;
                    blackFrameCount = 0;
                }
                break;

            case SCREEN_BLACK:
                // Brief black screen
                blackFrameCount++;
                if (blackFrameCount >= 30) { // ~0.5 seconds at 60fps
                    // Clear screen before title appears
                    clearScreenForTransition();
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
                    consoleDrawText(9, 10, "CHRONIC ECHOES");
                    consoleDrawText(10, 24, "PRESS START");
                    setScreenOn();
                }

                // Fade in title screen
                if (fadeFrameCount % 4 == 0 && brightness < 15) {
                    brightness++;
                    setBrightness(brightness);
                }

                // Check for start button to begin game
                if (padsCurrent(0) & KEY_START) {
                    // Start fade out before game
                    currentScreen = SCREEN_TITLE_FADEOUT;
                    fadeFrameCount = 0;
                }

                fadeFrameCount++;
                break;

            case SCREEN_TITLE_FADEOUT:
                // Fade out from title screen
                if (fadeFrameCount % 4 == 0 && brightness > 0) {
                    brightness--;
                    setBrightness(brightness);
                }

                fadeFrameCount++;
                if (brightness <= 0) {
                    // Clear screen before game starts
                    clearScreenForTransition();
                    currentScreen = SCREEN_GAME;
                    fadeFrameCount = 0;
                    brightness = 0;
                }
                break;

            case SCREEN_GAME:
                // Game screen - fade in then update and draw sprites
                if (fadeFrameCount == 0) {
                    // Initialize game content
                    updatePlayer();
                    drawPlayer();
                    setScreenOn();
                    brightness = 0;
                }

                // Fade in game screen
                if (fadeFrameCount % 4 == 0 && brightness < 15) {
                    brightness++;
                    setBrightness(brightness);
                }

                fadeFrameCount++;

                // Only handle game input after fade in is complete
                if (brightness >= 15) {
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
                        currentScreen = SCREEN_GAME_FADEOUT;
                        fadeFrameCount = 0;
                        brightness = 15;
                    }

                    // Handle time manipulation input
                    handleTimeManipulationInput(padsCurrent(0), previousPadState);
                }

                // Record current position for time manipulation
                recordCurrentPosition(playerCharacter.x, playerCharacter.y);

                // Update previous pad state for next frame
                previousPadState = padsCurrent(0);

                // Always update and draw sprites
                updatePlayer();
                drawPlayer();
                break;

            case SCREEN_GAME_FADEOUT:
                // Fade out from game screen
                if (fadeFrameCount % 4 == 0 && brightness > 0) {
                    brightness--;
                    setBrightness(brightness);
                }

                fadeFrameCount++;
                if (brightness <= 0) {
                    // Clear screen before title fades in
                    clearScreenForTransition();
                    currentScreen = SCREEN_TITLE;
                    fadeFrameCount = 0;
                    brightness = 0;
                }
                break;
        }

        // Wait for VBlank
        WaitForVBlank();
    }

    return 0;
}

