/*---------------------------------------------------------------------------------


    Chronic Echo - SNES Game
    -- Title screen with intro sequence


---------------------------------------------------------------------------------*/
#include <snes.h>

extern char tilfont, palfont;

// Screen states
#define SCREEN_INTRO 0
#define SCREEN_TITLE 1

//---------------------------------------------------------------------------------
int main(void)
{
    // Initialize text console with our font
    consoleSetTextMapPtr(0x6800);
    consoleSetTextGfxPtr(0x3000);
    consoleSetTextOffset(0x0100);
    consoleInitText(0, 16 * 2, &tilfont, &palfont);

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
                    currentScreen = SCREEN_TITLE;
                    fadeFrameCount = 0;
                    brightness = 15;
                }
                break;

            case SCREEN_TITLE:
                // Title screen
                if (fadeFrameCount == 0) {
                    // Clear screen and draw title
                    consoleDrawText(1, 1, ""); // Clear any remaining text
                    consoleDrawText(10, 8, "CHRONIC ECHO");
                    consoleDrawText(12, 12, "PRESS START");
                    consoleDrawText(8, 16, "MADE WITH COPILOT");
                }

                // Fade in effect
                if (brightness < 15) {
                    brightness++;
                    setBrightness(brightness);
                }

                fadeFrameCount++;
                break;
        }

        // Wait for VBlank
        WaitForVBlank();
    }

    return 0;
}

