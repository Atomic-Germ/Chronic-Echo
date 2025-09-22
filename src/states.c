/*---------------------------------------------------------------------------------


    Chronic Echo - Game States Implementation
    -- Individual state implementations for the game state manager


---------------------------------------------------------------------------------*/
#include "state_manager.h"
#include "sprites.h"
#include <snes.h>

//---------------------------------------------------------------------------------
// Intro State - "Made with Copilot" screen
static void introInit(StateManager* manager) {
    manager->data.introFrameCount = 0;
}

static void introUpdate(StateManager* manager) {
    manager->data.introFrameCount++;
}

static void introDraw(StateManager* manager) {
    // Intro state doesn't need special drawing - text is already drawn
}

static void introCleanup(StateManager* manager) {
    // Cleanup will be handled by transition
}

static GameStateID introCheckTransition(StateManager* manager) {
    // Wait 2.5 seconds (150 frames at 60fps, 125 at 50fps)
    int frames_to_wait = (snes_fps == 60) ? 150 : 125;
    if (manager->data.introFrameCount >= frames_to_wait) {
        return STATE_FADEOUT;
    }
    return STATE_INTRO;
}

GameState* createIntroState(void) {
    static GameState state = {
        .id = STATE_INTRO,
        .name = "Intro",
        .init = introInit,
        .update = introUpdate,
        .draw = introDraw,
        .cleanup = introCleanup,
        .checkTransition = introCheckTransition
    };
    return &state;
}

//---------------------------------------------------------------------------------
// Fadeout State - Fade out from current screen
static void fadeoutInit(StateManager* manager) {
    manager->data.fadeFrameCount = 0;
    manager->data.brightness = 15;
}

static void fadeoutUpdate(StateManager* manager) {
    manager->data.fadeFrameCount++;
    if (manager->data.fadeFrameCount % 4 == 0 && manager->data.brightness > 0) {
        manager->data.brightness--;
        setBrightness(manager->data.brightness);
    }
}

static void fadeoutDraw(StateManager* manager) {
    // No special drawing needed
}

static void fadeoutCleanup(StateManager* manager) {
    // Cleanup handled by transition
}

static GameStateID fadeoutCheckTransition(StateManager* manager) {
    if (manager->data.brightness <= 0) {
        clearScreenForTransition();
        return STATE_BLACK;
    }
    return STATE_FADEOUT;
}

GameState* createFadeoutState(void) {
    static GameState state = {
        .id = STATE_FADEOUT,
        .name = "Fadeout",
        .init = fadeoutInit,
        .update = fadeoutUpdate,
        .draw = fadeoutDraw,
        .cleanup = fadeoutCleanup,
        .checkTransition = fadeoutCheckTransition
    };
    return &state;
}

//---------------------------------------------------------------------------------
// Black State - Brief black screen between transitions
static void blackInit(StateManager* manager) {
    manager->data.blackFrameCount = 0;
    // Ensure screen is black
    setBrightness(0);
    bgSetDisable(0);
}

static void blackUpdate(StateManager* manager) {
    manager->data.blackFrameCount++;
}

static void blackDraw(StateManager* manager) {
    // No drawing needed - screen is black
}

static void blackCleanup(StateManager* manager) {
    // Cleanup handled by transition
}

static GameStateID blackCheckTransition(StateManager* manager) {
    if (manager->data.blackFrameCount >= 30) { // ~0.5 seconds at 60fps
        clearScreenForTransition();
        return STATE_TITLE;
    }
    return STATE_BLACK;
}

GameState* createBlackState(void) {
    static GameState state = {
        .id = STATE_BLACK,
        .name = "Black",
        .init = blackInit,
        .update = blackUpdate,
        .draw = blackDraw,
        .cleanup = blackCleanup,
        .checkTransition = blackCheckTransition
    };
    return &state;
}

//---------------------------------------------------------------------------------
// Title State - Main title screen
static void titleInit(StateManager* manager) {
    manager->data.fadeFrameCount = 0;
    manager->data.brightness = 0;
    // Re-enable background layer 0 for title screen
    bgSetEnable(0);
    
    // Clear any remaining text from previous states
    clearScreenForTransition();
    
    consoleDrawText(9, 10, "CHRONIC ECHOES");
    consoleDrawText(10, 24, "PRESS START");
}

static void titleUpdate(StateManager* manager) {
    manager->data.fadeFrameCount++;

    // Fade in title screen
    if (manager->data.fadeFrameCount % 4 == 0 && manager->data.brightness < 15) {
        manager->data.brightness++;
        setBrightness(manager->data.brightness);
    }

    // Check for start button
    if (padsCurrent(0) & KEY_START) {
        return;  // Transition will be handled by checkTransition
    }
}

static void titleDraw(StateManager* manager) {
    // Title text is already drawn
}

static void titleCleanup(StateManager* manager) {
    // Cleanup handled by transition
}

static GameStateID titleCheckTransition(StateManager* manager) {
    if (padsCurrent(0) & KEY_START) {
        return STATE_TITLE_FADEOUT;
    }
    return STATE_TITLE;
}

GameState* createTitleState(void) {
    static GameState state = {
        .id = STATE_TITLE,
        .name = "Title",
        .init = titleInit,
        .update = titleUpdate,
        .draw = titleDraw,
        .cleanup = titleCleanup,
        .checkTransition = titleCheckTransition
    };
    return &state;
}

//---------------------------------------------------------------------------------
// Title Fadeout State - Fade out from title screen
static void titleFadeoutInit(StateManager* manager) {
    manager->data.fadeFrameCount = 0;
    manager->data.brightness = 15;
}

static void titleFadeoutUpdate(StateManager* manager) {
    manager->data.fadeFrameCount++;
    if (manager->data.fadeFrameCount % 4 == 0 && manager->data.brightness > 0) {
        manager->data.brightness--;
        setBrightness(manager->data.brightness);
    }
}

static void titleFadeoutDraw(StateManager* manager) {
    // No special drawing
}

static void titleFadeoutCleanup(StateManager* manager) {
    // Cleanup handled by transition
}

static GameStateID titleFadeoutCheckTransition(StateManager* manager) {
    if (manager->data.brightness <= 0) {
        clearScreenForTransition();
        return STATE_GAME;
    }
    return STATE_TITLE_FADEOUT;
}

GameState* createTitleFadeoutState(void) {
    static GameState state = {
        .id = STATE_TITLE_FADEOUT,
        .name = "TitleFadeout",
        .init = titleFadeoutInit,
        .update = titleFadeoutUpdate,
        .draw = titleFadeoutDraw,
        .cleanup = titleFadeoutCleanup,
        .checkTransition = titleFadeoutCheckTransition
    };
    return &state;
}

//---------------------------------------------------------------------------------
// Game State - Main gameplay
static void gameInit(StateManager* manager) {
    manager->data.fadeFrameCount = 0;
    manager->data.brightness = 0;

    // Initialize game content
    updatePlayer();
    drawPlayer();
}

static void gameUpdate(StateManager* manager) {
    manager->data.fadeFrameCount++;

    // Fade in game screen
    if (manager->data.fadeFrameCount % 4 == 0 && manager->data.brightness < 15) {
        manager->data.brightness++;
        setBrightness(manager->data.brightness);
    }

    // Only handle game input after fade in is complete
    if (manager->data.brightness >= 15) {
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
            return;  // Transition handled by checkTransition
        }
    }

    // Always update and draw sprites
    updatePlayer();
}

static void gameDraw(StateManager* manager) {
    drawPlayer();
}

static void gameCleanup(StateManager* manager) {
    // Cleanup handled by transition
}

static GameStateID gameCheckTransition(StateManager* manager) {
    if (padsCurrent(0) & KEY_B) {
        return STATE_GAME_FADEOUT;
    }
    return STATE_GAME;
}

GameState* createGameState(void) {
    static GameState state = {
        .id = STATE_GAME,
        .name = "Game",
        .init = gameInit,
        .update = gameUpdate,
        .draw = gameDraw,
        .cleanup = gameCleanup,
        .checkTransition = gameCheckTransition
    };
    return &state;
}

//---------------------------------------------------------------------------------
// Game Fadeout State - Fade out from game screen
static void gameFadeoutInit(StateManager* manager) {
    manager->data.fadeFrameCount = 0;
    manager->data.brightness = 15;
}

static void gameFadeoutUpdate(StateManager* manager) {
    manager->data.fadeFrameCount++;
    if (manager->data.fadeFrameCount % 4 == 0 && manager->data.brightness > 0) {
        manager->data.brightness--;
        setBrightness(manager->data.brightness);
    }
}

static void gameFadeoutDraw(StateManager* manager) {
    // No special drawing
}

static void gameFadeoutCleanup(StateManager* manager) {
    // Cleanup handled by transition
}

static GameStateID gameFadeoutCheckTransition(StateManager* manager) {
    if (manager->data.brightness <= 0) {
        clearScreenForTransition();
        return STATE_TITLE;
    }
    return STATE_GAME_FADEOUT;
}

GameState* createGameFadeoutState(void) {
    static GameState state = {
        .id = STATE_GAME_FADEOUT,
        .name = "GameFadeout",
        .init = gameFadeoutInit,
        .update = gameFadeoutUpdate,
        .draw = gameFadeoutDraw,
        .cleanup = gameFadeoutCleanup,
        .checkTransition = gameFadeoutCheckTransition
    };
    return &state;
}