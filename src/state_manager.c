/*---------------------------------------------------------------------------------


    Chronic Echo - Game State Manager Implementation
    -- Modular state management system implementation


---------------------------------------------------------------------------------*/
#include "state_manager.h"
#include <string.h>

//---------------------------------------------------------------------------------
// Global state manager instance
StateManager g_stateManager;

//---------------------------------------------------------------------------------
// Screen clearing helper function
void clearScreenForTransition(void) {
    // Clear the console tilemap at 0x6800 (32x32 tiles = 2048 bytes)
    // Create a buffer with zeros to fill the tilemap
    static u8 zeroBuffer[2048] = {0};
    dmaFillVram(zeroBuffer, 0x6800, 2048);

    // Clear all sprites
    oamClear(0, 0);
    oamUpdate();

    // Reset any other screen state as needed
    // (Add more clearing logic here as the game grows)
}

//---------------------------------------------------------------------------------
// Initialize the state manager
void initStateManager(StateManager* manager) {
    memset(manager, 0, sizeof(StateManager));
    memset(&manager->data, 0, sizeof(StateData));
    manager->data.brightness = 15;  // Start with full brightness
    manager->initialized = 1;
}

//---------------------------------------------------------------------------------
// Register a state with the manager
void registerState(StateManager* manager, GameStateID id, GameState* state) {
    if (id < STATE_COUNT) {
        manager->states[id] = state;
    }
}

//---------------------------------------------------------------------------------
// Change to a new state
void changeState(StateManager* manager, GameStateID newStateID) {
    if (newStateID >= STATE_COUNT || !manager->states[newStateID]) {
        return;  // Invalid state
    }

    // Cleanup current state
    if (manager->currentState && manager->currentState->cleanup) {
        manager->currentState->cleanup(manager);
    }

    // Change to new state
    manager->currentState = manager->states[newStateID];

    // Initialize new state
    if (manager->currentState->init) {
        manager->currentState->init(manager);
    }
}

//---------------------------------------------------------------------------------
// Update the current state
void updateStateManager(StateManager* manager) {
    if (!manager->currentState) {
        return;
    }

    // Check for state transitions
    GameStateID nextState = manager->currentState->checkTransition(manager);
    if (nextState != manager->currentState->id) {
        changeState(manager, nextState);
        return;
    }

    // Update current state
    if (manager->currentState->update) {
        manager->currentState->update(manager);
    }
}

//---------------------------------------------------------------------------------
// Draw the current state
void drawStateManager(StateManager* manager) {
    if (!manager->currentState) {
        return;
    }

    if (manager->currentState->draw) {
        manager->currentState->draw(manager);
    }
}