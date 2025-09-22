/*---------------------------------------------------------------------------------


    Chronic Echo - Game State Manager
    -- Modular state management system for screen transitions and game logic


---------------------------------------------------------------------------------*/
#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <snes.h>

//---------------------------------------------------------------------------------
// Forward declarations
struct GameState;
struct StateManager;

//---------------------------------------------------------------------------------
// State IDs - unique identifiers for each game state
typedef enum {
    STATE_INTRO,
    STATE_FADEOUT,
    STATE_BLACK,
    STATE_TITLE,
    STATE_GAME,
    STATE_GAME_FADEOUT,
    STATE_TITLE_FADEOUT,
    STATE_COUNT  // Keep this last for array sizing
} GameStateID;

//---------------------------------------------------------------------------------
// State Data - data that persists across state transitions
typedef struct {
    int brightness;
    int fadeFrameCount;
    int introFrameCount;
    int blackFrameCount;
    // Add more persistent data as needed
} StateData;

//---------------------------------------------------------------------------------
// Game State structure - defines a single game state
typedef struct GameState {
    GameStateID id;
    const char* name;  // For debugging

    // State lifecycle functions
    void (*init)(struct StateManager* manager);     // Called when entering state
    void (*update)(struct StateManager* manager);   // Called every frame
    void (*draw)(struct StateManager* manager);     // Called to render state
    void (*cleanup)(struct StateManager* manager);  // Called when leaving state

    // Transition conditions
    GameStateID (*checkTransition)(struct StateManager* manager);  // Returns next state ID or current state
} GameState;

//---------------------------------------------------------------------------------
// State Manager - manages state transitions and execution
typedef struct StateManager {
    GameState* currentState;
    GameState* states[STATE_COUNT];
    StateData data;
    int initialized;
} StateManager;

//---------------------------------------------------------------------------------
// Global state manager instance
extern StateManager g_stateManager;

//---------------------------------------------------------------------------------
// State Manager API
void initStateManager(StateManager* manager);
void registerState(StateManager* manager, GameStateID id, GameState* state);
void changeState(StateManager* manager, GameStateID newStateID);
void updateStateManager(StateManager* manager);
void drawStateManager(StateManager* manager);

//---------------------------------------------------------------------------------
// State creation helpers
GameState* createIntroState(void);
GameState* createFadeoutState(void);
GameState* createBlackState(void);
GameState* createTitleState(void);
GameState* createGameState(void);
GameState* createGameFadeoutState(void);
GameState* createTitleFadeoutState(void);

//---------------------------------------------------------------------------------
// Utility functions
void clearScreenForTransition(void);

#endif // STATE_MANAGER_H