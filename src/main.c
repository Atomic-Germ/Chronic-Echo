/*---------------------------------------------------------------------------------


    Chronic Echos Title Screen Demo
    -- GitHub Copilot


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <snes/input.h>

extern char tilfont, palfont;

//---------------------------------------------------------------------------------
// Game States
typedef enum {
    GAME_STATE_TITLE,
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSE,
    GAME_STATE_GAME_OVER
} GameState;

//---------------------------------------------------------------------------------
// Global game state
GameState currentState = GAME_STATE_TITLE;
GameState previousState = GAME_STATE_TITLE; // Track state changes

//---------------------------------------------------------------------------------
// Function declarations
void initGame(void);
void clearScreen(void);
void handleTitleScreen(void);
void handleMenu(void);
void handleGameplay(void);
void handlePause(void);
void handleGameOver(void);

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
        }

        // Handle current game state
        switch (currentState)
        {
            case GAME_STATE_TITLE:
                handleTitleScreen();
                break;
            case GAME_STATE_MENU:
                handleMenu();
                break;
            case GAME_STATE_PLAYING:
                handleGameplay();
                break;
            case GAME_STATE_PAUSE:
                handlePause();
                break;
            case GAME_STATE_GAME_OVER:
                handleGameOver();
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
    // Initialize text console with our font
    consoleSetTextVramBGAdr(0x6800);
    consoleSetTextVramAdr(0x3000);
    consoleSetTextOffset(0x0100);
    consoleInitText(0, 16 * 2, &tilfont, &palfont);

    // Init background
    bgSetGfxPtr(0, 0x2000);
    bgSetMapPtr(0, 0x6800, SC_32x32);

    // Now Put in 16 color mode and disable Bgs except current
    setMode(BG_MODE1, 0);
    bgSetDisable(1);
    bgSetDisable(2);

    // Turn on the screen
    setScreenOn();
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
    // Draw the title screen
    consoleDrawText(8, 8, "CHRONIC ECHOS");
    consoleDrawText(4, 12, "TIME MANIPULATION RPG");
    consoleDrawText(2, 16, "FOR SUPER NINTENDO ENTERTAINMENT SYSTEM");
    consoleDrawText(6, 20, "PRESS START TO BEGIN");

    // Check for START button press to transition to menu
    if (pad_keysdown[0] & KEY_START)
    {
        currentState = GAME_STATE_MENU;
    }
}

//---------------------------------------------------------------------------------
void handleMenu(void)
{
    // Draw menu
    consoleDrawText(10, 8, "MAIN MENU");
    consoleDrawText(8, 12, "1. NEW GAME");
    consoleDrawText(8, 14, "2. LOAD GAME");
    consoleDrawText(8, 16, "3. SETTINGS");
    consoleDrawText(8, 18, "4. QUIT");

    // Check for menu selections
    if (pad_keysdown[0] & KEY_A)
    {
        // For now, any selection goes to gameplay
        // TODO: Implement proper menu selection logic
        currentState = GAME_STATE_PLAYING;
    }

    // Check for B button to go back to title
    if (pad_keysdown[0] & KEY_B)
    {
        currentState = GAME_STATE_TITLE;
    }
}

//---------------------------------------------------------------------------------
void handleGameplay(void)
{
    // Show gameplay placeholder
    consoleDrawText(10, 10, "GAMEPLAY");
    consoleDrawText(8, 12, "Time manipulation RPG");
    consoleDrawText(6, 14, "coming soon...");
    consoleDrawText(8, 16, "PRESS START TO PAUSE");

    // Check for START to pause
    if (pad_keysdown[0] & KEY_START)
    {
        currentState = GAME_STATE_PAUSE;
    }
}

//---------------------------------------------------------------------------------
void handlePause(void)
{
    // Draw pause screen
    consoleDrawText(12, 10, "PAUSED");
    consoleDrawText(8, 12, "PRESS START TO RESUME");
    consoleDrawText(10, 14, "PRESS B TO QUIT");

    // Check for START to resume
    if (pad_keysdown[0] & KEY_START)
    {
        currentState = GAME_STATE_PLAYING;
    }

    // Check for B to go back to menu
    if (pad_keysdown[0] & KEY_B)
    {
        currentState = GAME_STATE_MENU;
    }
}

//---------------------------------------------------------------------------------
void handleGameOver(void)
{
    // Draw game over screen
    consoleDrawText(10, 10, "GAME OVER");
    consoleDrawText(8, 12, "PRESS START TO CONTINUE");
    consoleDrawText(10, 14, "PRESS B TO MENU");

    // Check for START to restart
    if (pad_keysdown[0] & KEY_START)
    {
        currentState = GAME_STATE_PLAYING;
    }

    // Check for B to go back to menu
    if (pad_keysdown[0] & KEY_B)
    {
        currentState = GAME_STATE_MENU;
    }
}