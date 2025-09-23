# SNES Development Snippets

This file documents reusable code patterns and techniques for SNES development in this project.

## Progression System Patterns

### Experience and Leveling

```c
// Grant experience from battle rewards
grantExperience(&playerProgression, enemyExperience);

// Check for automatic level-ups
if (canLevelUp(&playerProgression.stats)) {
    performLevelUp(&playerProgression.stats);
}

// Display level and experience
renderLevelDisplay(1, 1);  // Shows "Lv.X EXP:Y/Z"
```

### Equipment Management

```c
// Equip item from inventory
equipItem(&playerProgression, inventoryIndex, EQUIP_SLOT_WEAPON);

// Unequip item (adds back to inventory if space available)
unequipItem(&playerProgression, EQUIP_SLOT_WEAPON);

// Check if player can equip an item
if (canEquipItem(&playerProgression.stats, &equipmentDatabase[itemId])) {
    // Safe to equip
}
```

### Stat Calculations

```c
// Get current player stats (includes equipment bonuses)
u16 currentHp = playerProgression.stats.currentHp;
u16 maxHp = playerProgression.stats.maxHp;
u16 attack = playerProgression.stats.attack;
u16 defense = playerProgression.stats.defense;

// Calculate damage with equipment bonuses applied
u8 damage = calculateDamage(attackerAttack, defenderDefense);
```

### Progression UI

```c
// Render stat screen
renderStatScreen(1, 1);

// Render equipment menu
renderEquipmentMenu(1, 10);

// Render player inventory
renderPlayerInventory(1, 20);
```

### Initialization

```c
// Initialize progression system
initProgressionSystem();

// Reset to starting stats
resetProgressionState(&playerProgression);
```

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

## Dialogue System Patterns

### Dialogue Tree Structure

```c
// Define dialogue nodes with choices
DialogueNode dialogueNodes[] = {
    {
        .id = 0,
        .speaker = "Villager",
        .text = "Hello, traveler! What brings you here?",
        .choiceCount = 2,
        .choices = {
            {"I'm exploring.", 1, 1},
            {"I'm looking for adventure.", 2, 1}
        },
        .nextNodeId = 0
    },
    {
        .id = 1,
        .speaker = "Villager",
        .text = "Exploration! Be careful of the time anomalies.",
        .choiceCount = 1,
        .choices = {
            {"Time anomalies?", 3, 1}
        },
        .nextNodeId = 0
    }
};
```

### Dialogue Rendering

```c
void renderDialogue(void) {
    if (!isDialogueActive()) return;

    // Draw dialogue box
    drawDialogueBox();

    // Draw speaker name
    if (strlen(getCurrentSpeaker()) > 0) {
        consoleDrawText(DIALOGUE_BOX_X + 1, DIALOGUE_BOX_Y + 1,
                       getCurrentSpeaker());
    }

    // Draw text with typewriter effect
    drawDialogueText();

    // Draw choices if available
    if (getChoiceCount() > 0) {
        drawDialogueChoices();
    } else {
        // Draw continue prompt
        consoleDrawText(DIALOGUE_BOX_X + DIALOGUE_BOX_WIDTH - 3,
                       DIALOGUE_BOX_Y + DIALOGUE_BOX_HEIGHT - 2, ">>");
    }
}
```

### Dialogue Input Handling

```c
void updateDialogue(void) {
    if (!isDialogueActive()) return;

    u16 pad0 = padsCurrent(0);

    if (getChoiceCount() > 0) {
        // Choice selection mode
        if (pad0 & KEY_UP) {
            selectPreviousChoice();
        } else if (pad0 & KEY_DOWN) {
            selectNextChoice();
        } else if (pad0 & KEY_A) {
            confirmChoice();
        }
    } else {
        // Continue mode
        if (pad0 & KEY_A) {
            advanceDialogue();
        }
    }

    // Allow skipping text display
    if (pad0 & KEY_B) {
        skipDialogueText();
    }
}
```

### NPC Interaction Integration

```c
// In world navigation update
if (pad0 & KEY_A) {
    NPC* npc = getNPCAtPosition(playerCharacter.x, playerCharacter.y);
    if (npc && !isDialogueActive()) {
        // Find NPC index and start dialogue
        u8 npcId = getNPCIndex(npc);
        startDialogue(npcId);
    }
}
```

### Dialogue Usage Notes

- Initialize dialogue system with `initDialogueSystem()` after other systems
- Call `updateDialogue()` and `renderDialogue()` in the main game loop
- Dialogue boxes use console text coordinates (2,18) to (30,28) by default
- Text displays with typewriter effect (configurable speed 1-5)
- Player can skip text with B button, navigate choices with Up/Down, confirm with A
- Dialogue automatically prevents other input while active
