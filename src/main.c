/*---------------------------------------------------------------------------------


    Minimal SNES Test
    -- Basic console text test


---------------------------------------------------------------------------------*/
#include <snes.h>

extern char tilfont, palfont;

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

    // Display centered text at middle of screen
    consoleDrawText(8, 14, "Made with Copilot");

    // Turn screen on
    setScreenOn();

    // Wait for 2.5 seconds (150 frames at 60fps, 125 at 50fps)
    int frames_to_wait = (snes_fps == 60) ? 150 : 125;
    int i;
    for (i = 0; i < frames_to_wait; i++) {
        WaitForVBlank();
    }

    // Fade out gradually
    int brightness;
    for (brightness = 15; brightness >= 0; brightness--) {
        setBrightness(brightness);
        // Wait a few frames between brightness changes for smooth fade
        int j;
        for (j = 0; j < 4; j++) {
            WaitForVBlank();
        }
    }

    // Main loop (screen will be black now)
    while (1) {
        // Just wait for VBlank
        WaitForVBlank();
    }

    return 0;
}

