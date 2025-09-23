# SNES Development Snippets

This file documents reusable code patterns and techniques for SNES development in this project.

## Fade In/Out Effects

To create smooth fade in or fade out effects for screen transitions:

### Fade Out (Dimming)
```c
// Start with brightness = 15 (full brightness)
// In your game loop:
if (fadeFrameCount % 4 == 0 && brightness > 0) {
    brightness--;
    setBrightness(brightness);
}
fadeFrameCount++;
if (brightness <= 0) {
    // Transition to next screen
}
```

### Fade In (Brightening)
```c
// Start with brightness = 0 (screen off)
// In your game loop:
if (fadeFrameCount % 4 == 0 && brightness < 15) {
    brightness++;
    setBrightness(brightness);
}
fadeFrameCount++;
if (brightness >= 15) {
    // Fade in complete, start normal operation
}
```

### Usage Notes
- Use a frame counter to control the fade speed (every 4 frames = ~15 frames total for full fade)
- Call `setScreenOn()` when starting the fade to ensure the screen is active
- Clear screen content during transitions using `clearScreenForTransition()`
- Reset `fadeFrameCount = 0` when starting a new fade
- Set initial `brightness` appropriately (0 for fade in, 15 for fade out)

## Battle System Patterns

### Random Encounter System

```c
#define ENCOUNTER_CHANCE 5  // 5% chance per movement

void checkRandomEncounter(int* currentScreen, int* fadeFrameCount, int* brightness) {
    // Only trigger encounters if not already in battle
    if (isBattleActive()) {
        return;
    }

    // Simple random encounter check
    u8 roll = rand() % 100;
    if (roll < ENCOUNTER_CHANCE) {
        // Determine enemy type based on player level
        EnemyType enemyType = ENEMY_TYPE_SLIME;
        if (playerCharacter.level >= 2) {
            enemyType = ENEMY_TYPE_GOBLIN;
        }
        if (playerCharacter.level >= 3) {
            enemyType = ENEMY_TYPE_ORC;
        }

        // Start the battle
        startBattle(enemyType);
        *currentScreen = SCREEN_BATTLE_FADEOUT;
        *fadeFrameCount = 0;
        *brightness = 15;
    }
}
```

### Battle UI Rendering

```c
void drawBattleUI(void) {
    char buffer[32];
    int i;

    // Clear UI area
    for (i = 18; i < 28; i++) {
        consoleDrawText(0, i, "                                        ");
    }

    // Player health bar
    consoleDrawText(1, 19, "PLAYER HP:");
    sprintf(buffer, "%d/%d", playerCharacter.health, playerCharacter.maxHealth);
    consoleDrawText(12, 19, buffer);

    // Draw health bar (10 characters wide)
    int healthPercent = (playerCharacter.health * 10) / playerCharacter.maxHealth;
    consoleDrawText(1, 20, "[");
    for (i = 0; i < 10; i++) {
        consoleDrawText(2 + i, 20, i < healthPercent ? "=" : " ");
    }
    consoleDrawText(12, 20, "]");

    // Time energy display
    consoleDrawText(1, 22, "TIME ENERGY:");
    sprintf(buffer, "%d/%d", playerCharacter.timeEnergy, playerCharacter.maxTimeEnergy);
    consoleDrawText(14, 22, buffer);

    // Battle menu
    consoleDrawText(1, 24, "A:ATTACK  B:DEFEND");
    consoleDrawText(1, 25, "X:TIME STOP  Y:SLOW");
    consoleDrawText(1, 26, "L:REWIND  R:ESCAPE");
}
```

### Time-Based Battle Abilities

```c
// Time Stop (15 energy, lasts 3 turns)
if (playerCharacter.timeEnergy >= TIME_STOP_ENERGY_COST) {
    setBattleAction(BATTLE_ACTION_TIME_STOP);
    playerCharacter.timeEnergy -= TIME_STOP_ENERGY_COST;
    currentBattle.timeStopActive = TIME_STOP_DURATION;
}

// Slow Motion (10 energy, lasts 2 turns)
if (playerCharacter.timeEnergy >= SLOW_MOTION_ENERGY_COST) {
    setBattleAction(BATTLE_ACTION_SLOW_MOTION);
    playerCharacter.timeEnergy -= SLOW_MOTION_ENERGY_COST;
    currentBattle.slowMotionActive = SLOW_MOTION_DURATION;
}

// Rewind (20 energy, battle state reversal)
if (playerCharacter.timeEnergy >= REWIND_ENERGY_COST) {
    setBattleAction(BATTLE_ACTION_REWIND);
    playerCharacter.timeEnergy -= REWIND_ENERGY_COST;
    rewindBattleTurn();
}
```
