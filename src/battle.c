/*---------------------------------------------------------------------------------


    Chronic Echo - Battle System Implementation
    -- Turn-based combat with time manipulation mechanics


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <string.h>
#include <stdio.h>

#include "battle.h"
#include "player.h"  // For player character access

//---------------------------------------------------------------------------------
// Global battle instance
Battle currentBattle = {0};

//---------------------------------------------------------------------------------
// Enemy definitions array
static Enemy enemyDefinitions[6];

//---------------------------------------------------------------------------------
// Function prototypes
static void processPlayerAttack(void);
static void processPlayerDefend(void);
static void processEnemyTurn(void);
static void updateTimeEffects(void);
static void checkBattleEndConditions(void);

//---------------------------------------------------------------------------------
// Initialize battle system
void initBattleSystem(void) {
    currentBattle.state = BATTLE_STATE_IDLE;
    initEnemyDefinitions();
}

//---------------------------------------------------------------------------------
// Initialize enemy definitions
void initEnemyDefinitions(void) {
    // Slime
    enemyDefinitions[ENEMY_TYPE_SLIME].type = ENEMY_TYPE_SLIME;
    strcpy(enemyDefinitions[ENEMY_TYPE_SLIME].name, "SLIME");
    enemyDefinitions[ENEMY_TYPE_SLIME].maxHealth = 20;
    enemyDefinitions[ENEMY_TYPE_SLIME].attack = 8;
    enemyDefinitions[ENEMY_TYPE_SLIME].defense = 2;
    enemyDefinitions[ENEMY_TYPE_SLIME].experience = 10;
    enemyDefinitions[ENEMY_TYPE_SLIME].level = 1;
    enemyDefinitions[ENEMY_TYPE_SLIME].spriteId = 0;

    // Goblin
    enemyDefinitions[ENEMY_TYPE_GOBLIN].type = ENEMY_TYPE_GOBLIN;
    strcpy(enemyDefinitions[ENEMY_TYPE_GOBLIN].name, "GOBLIN");
    enemyDefinitions[ENEMY_TYPE_GOBLIN].maxHealth = 35;
    enemyDefinitions[ENEMY_TYPE_GOBLIN].attack = 12;
    enemyDefinitions[ENEMY_TYPE_GOBLIN].defense = 4;
    enemyDefinitions[ENEMY_TYPE_GOBLIN].experience = 20;
    enemyDefinitions[ENEMY_TYPE_GOBLIN].level = 2;
    enemyDefinitions[ENEMY_TYPE_GOBLIN].spriteId = 1;

    // Orc
    enemyDefinitions[ENEMY_TYPE_ORC].type = ENEMY_TYPE_ORC;
    strcpy(enemyDefinitions[ENEMY_TYPE_ORC].name, "ORC");
    enemyDefinitions[ENEMY_TYPE_ORC].maxHealth = 60;
    enemyDefinitions[ENEMY_TYPE_ORC].attack = 18;
    enemyDefinitions[ENEMY_TYPE_ORC].defense = 8;
    enemyDefinitions[ENEMY_TYPE_ORC].experience = 35;
    enemyDefinitions[ENEMY_TYPE_ORC].level = 3;
    enemyDefinitions[ENEMY_TYPE_ORC].spriteId = 2;

    // Skeleton
    enemyDefinitions[ENEMY_TYPE_SKELETON].type = ENEMY_TYPE_SKELETON;
    strcpy(enemyDefinitions[ENEMY_TYPE_SKELETON].name, "SKELETON");
    enemyDefinitions[ENEMY_TYPE_SKELETON].maxHealth = 45;
    enemyDefinitions[ENEMY_TYPE_SKELETON].attack = 15;
    enemyDefinitions[ENEMY_TYPE_SKELETON].defense = 6;
    enemyDefinitions[ENEMY_TYPE_SKELETON].experience = 25;
    enemyDefinitions[ENEMY_TYPE_SKELETON].level = 2;
    enemyDefinitions[ENEMY_TYPE_SKELETON].spriteId = 3;

    // Dragon
    enemyDefinitions[ENEMY_TYPE_DRAGON].type = ENEMY_TYPE_DRAGON;
    strcpy(enemyDefinitions[ENEMY_TYPE_DRAGON].name, "DRAGON");
    enemyDefinitions[ENEMY_TYPE_DRAGON].maxHealth = 120;
    enemyDefinitions[ENEMY_TYPE_DRAGON].attack = 25;
    enemyDefinitions[ENEMY_TYPE_DRAGON].defense = 15;
    enemyDefinitions[ENEMY_TYPE_DRAGON].experience = 80;
    enemyDefinitions[ENEMY_TYPE_DRAGON].level = 5;
    enemyDefinitions[ENEMY_TYPE_DRAGON].spriteId = 4;

    // Boss
    enemyDefinitions[ENEMY_TYPE_BOSS].type = ENEMY_TYPE_BOSS;
    strcpy(enemyDefinitions[ENEMY_TYPE_BOSS].name, "TIME LORD");
    enemyDefinitions[ENEMY_TYPE_BOSS].maxHealth = 200;
    enemyDefinitions[ENEMY_TYPE_BOSS].attack = 35;
    enemyDefinitions[ENEMY_TYPE_BOSS].defense = 20;
    enemyDefinitions[ENEMY_TYPE_BOSS].experience = 150;
    enemyDefinitions[ENEMY_TYPE_BOSS].level = 8;
    enemyDefinitions[ENEMY_TYPE_BOSS].spriteId = 5;
}

//---------------------------------------------------------------------------------
// Create enemy from type
Enemy createEnemy(EnemyType type) {
    Enemy enemy = enemyDefinitions[type];
    enemy.health = enemy.maxHealth; // Reset health to max
    return enemy;
}

//---------------------------------------------------------------------------------
// Start a battle with specified enemy type
void startBattle(EnemyType enemyType) {
    if (currentBattle.state != BATTLE_STATE_IDLE) {
        return; // Battle already active
    }

    currentBattle.state = BATTLE_STATE_ENCOUNTER;
    currentBattle.enemy = createEnemy(enemyType);
    currentBattle.turnCount = 0;
    currentBattle.playerAction = BATTLE_ACTION_ATTACK;
    currentBattle.isPlayerTurn = 1; // Player goes first
    currentBattle.timeStopActive = 0;
    currentBattle.slowMotionActive = 0;
    currentBattle.battleResult = 0;
}

//---------------------------------------------------------------------------------
// Update battle state
void updateBattle(void) {
    switch (currentBattle.state) {
        case BATTLE_STATE_IDLE:
            // No battle active
            break;

        case BATTLE_STATE_ENCOUNTER:
            // Handle encounter animation/transition
            // For now, immediately go to active battle
            currentBattle.state = BATTLE_STATE_ACTIVE;
            break;

        case BATTLE_STATE_ACTIVE:
            // Active battle - handle turns and mechanics
            if (currentBattle.isPlayerTurn) {
                // Wait for player input
            } else {
                // Process enemy turn
                processEnemyTurn();
                currentBattle.isPlayerTurn = 1;
            }

            // Update time-based effects
            updateTimeEffects();
            break;

        case BATTLE_STATE_VICTORY:
        case BATTLE_STATE_DEFEAT:
        case BATTLE_STATE_ESCAPE:
            // Battle ended - wait for player to acknowledge
            break;
    }
}

//---------------------------------------------------------------------------------
// End current battle
void endBattle(void) {
    currentBattle.state = BATTLE_STATE_IDLE;
    // Reset time manipulation effects
    currentBattle.timeStopActive = 0;
    currentBattle.slowMotionActive = 0;
}

//---------------------------------------------------------------------------------
// Check if battle is currently active
u8 isBattleActive(void) {
    return currentBattle.state != BATTLE_STATE_IDLE;
}

//---------------------------------------------------------------------------------
// Set player battle action
void setBattleAction(u8 action) {
    if (currentBattle.state != BATTLE_STATE_ACTIVE || !currentBattle.isPlayerTurn) {
        return;
    }

    currentBattle.playerAction = action;
}

//---------------------------------------------------------------------------------
// Process battle turn
void processBattleTurn(void) {
    if (currentBattle.state != BATTLE_STATE_ACTIVE) {
        return;
    }

    currentBattle.turnCount++;

    switch (currentBattle.playerAction) {
        case BATTLE_ACTION_ATTACK:
            processPlayerAttack();
            break;
        case BATTLE_ACTION_DEFEND:
            processPlayerDefend();
            break;
        case BATTLE_ACTION_TIME_STOP:
            applyTimeStop();
            break;
        case BATTLE_ACTION_SLOW_MOTION:
            applySlowMotion();
            break;
        case BATTLE_ACTION_REWIND:
            rewindBattleTurn();
            break;
        case BATTLE_ACTION_ESCAPE:
            if (attemptEscape()) {
                currentBattle.state = BATTLE_STATE_ESCAPE;
            } else {
                // Failed escape - enemy gets free turn
                processEnemyTurn();
            }
            break;
    }

    // Check battle end conditions
    checkBattleEndConditions();

    // Switch turns
    currentBattle.isPlayerTurn = !currentBattle.isPlayerTurn;
}

//---------------------------------------------------------------------------------
// Process player attack
void processPlayerAttack(void) {
    u8 damage = calculateDamage(playerCharacter.attack, currentBattle.enemy.defense);
    if (damage > currentBattle.enemy.health) {
        currentBattle.enemy.health = 0;
    } else {
        currentBattle.enemy.health -= damage;
    }
}

//---------------------------------------------------------------------------------
// Process player defend
void processPlayerDefend(void) {
    // Defending increases defense for this turn
    // Enemy attack will be reduced
    // This is handled in enemy attack calculation
}

//---------------------------------------------------------------------------------
// Process enemy turn
void processEnemyTurn(void) {
    u8 damage = calculateDamage(currentBattle.enemy.attack, playerCharacter.defense);
    if (damage > playerCharacter.health) {
        playerCharacter.health = 0;
    } else {
        playerCharacter.health -= damage;
    }
}

//---------------------------------------------------------------------------------
// Calculate damage with defense reduction
u8 calculateDamage(u16 attackerAttack, u16 defenderDefense) {
    if (attackerAttack <= defenderDefense) {
        return 1; // Minimum damage
    }
    return attackerAttack - defenderDefense;
}

//---------------------------------------------------------------------------------
// Attempt to escape battle
u8 attemptEscape(void) {
    u8 escapeChance = ESCAPE_CHANCE_BASE;

    // Modify escape chance based on various factors
    // For now, simple random chance
    u8 roll = rand() % 100;
    return roll < escapeChance;
}

//---------------------------------------------------------------------------------
// Apply time stop effect
void applyTimeStop(void) {
    if (playerCharacter.timeEnergy < TIME_STOP_ENERGY_COST) {
        return; // Not enough energy
    }

    playerCharacter.timeEnergy -= TIME_STOP_ENERGY_COST;
    currentBattle.timeStopActive = TIME_STOP_DURATION;
    // Time stop prevents enemy from acting for several turns
}

//---------------------------------------------------------------------------------
// Apply slow motion effect
void applySlowMotion(void) {
    if (playerCharacter.timeEnergy < SLOW_MOTION_ENERGY_COST) {
        return; // Not enough energy
    }

    playerCharacter.timeEnergy -= SLOW_MOTION_ENERGY_COST;
    currentBattle.slowMotionActive = SLOW_MOTION_DURATION;
    // Slow motion reduces enemy attack damage
}

//---------------------------------------------------------------------------------
// Rewind battle turn
void rewindBattleTurn(void) {
    if (playerCharacter.timeEnergy < REWIND_ENERGY_COST) {
        return; // Not enough energy
    }

    // Rewind restores some health and gives another turn
    u16 healAmount = 10;
    if (playerCharacter.health + healAmount > playerCharacter.maxHealth) {
        playerCharacter.health = playerCharacter.maxHealth;
    } else {
        playerCharacter.health += healAmount;
    }

    playerCharacter.timeEnergy -= REWIND_ENERGY_COST;
    // Give player another turn
    currentBattle.isPlayerTurn = 1;
}

//---------------------------------------------------------------------------------
// Update time-based effects
void updateTimeEffects(void) {
    if (currentBattle.timeStopActive > 0) {
        currentBattle.timeStopActive--;
        // Skip enemy turn while time stop is active
        if (currentBattle.timeStopActive > 0 && !currentBattle.isPlayerTurn) {
            currentBattle.isPlayerTurn = 1;
        }
    }

    if (currentBattle.slowMotionActive > 0) {
        currentBattle.slowMotionActive--;
    }
}

//---------------------------------------------------------------------------------
// Check battle end conditions
void checkBattleEndConditions(void) {
    if (playerCharacter.health <= 0) {
        currentBattle.state = BATTLE_STATE_DEFEAT;
    } else if (currentBattle.enemy.health <= 0) {
        currentBattle.state = BATTLE_STATE_VICTORY;
        // Award experience
        playerCharacter.experience += currentBattle.enemy.experience;
        levelUpPlayerCharacter();
    }
}

//---------------------------------------------------------------------------------
// Handle battle input
void handleBattleInput(u16 currentPadState, u16 previousPadState) {
    if (currentBattle.state != BATTLE_STATE_ACTIVE || !currentBattle.isPlayerTurn) {
        return;
    }

    // Handle battle actions based on button presses
    if ((currentPadState & KEY_A) && !(previousPadState & KEY_A)) {
        // Attack
        setBattleAction(BATTLE_ACTION_ATTACK);
        processBattleTurn();
    }
    else if ((currentPadState & KEY_B) && !(previousPadState & KEY_B)) {
        // Defend
        setBattleAction(BATTLE_ACTION_DEFEND);
        processBattleTurn();
    }
    else if ((currentPadState & KEY_X) && !(previousPadState & KEY_X)) {
        // Time Stop (check if player has enough energy)
        if (playerCharacter.timeEnergy >= TIME_STOP_ENERGY_COST) {
            setBattleAction(BATTLE_ACTION_TIME_STOP);
            processBattleTurn();
        }
    }
    else if ((currentPadState & KEY_Y) && !(previousPadState & KEY_Y)) {
        // Slow Motion (check if player has enough energy)
        if (playerCharacter.timeEnergy >= SLOW_MOTION_ENERGY_COST) {
            setBattleAction(BATTLE_ACTION_SLOW_MOTION);
            processBattleTurn();
        }
    }
    else if ((currentPadState & KEY_L) && !(previousPadState & KEY_L)) {
        // Rewind (check if player has enough energy)
        if (playerCharacter.timeEnergy >= REWIND_ENERGY_COST) {
            setBattleAction(BATTLE_ACTION_REWIND);
            processBattleTurn();
        }
    }
    else if ((currentPadState & KEY_R) && !(previousPadState & KEY_R)) {
        // Escape
        setBattleAction(BATTLE_ACTION_ESCAPE);
        processBattleTurn();
    }
}

//---------------------------------------------------------------------------------
// Battle UI and rendering functions (placeholders for now)
void drawBattleScreen(void) {
    // Clear screen by drawing spaces over the entire console area
    // Console is typically 32x28 characters
    int x, y;
    for (y = 0; y < 28; y++) {
        for (x = 0; x < 32; x++) {
            consoleDrawText(x, y, " ");
        }
    }

    // Draw battle scene header
    consoleDrawText(10, 2, "=== BATTLE ===");

    // Draw enemy name
    consoleDrawText(10, 4, currentBattle.enemy.name);

    // Draw battle area indicators
    consoleDrawText(2, 8, "PLAYER");
    consoleDrawText(20, 8, "ENEMY");

    // Draw battle UI
    drawBattleUI();

    // Draw enemy sprite
    drawEnemySprite();
}

void drawBattleUI(void) {
    char buffer[32];
    int i;

    // Clear the bottom area for UI
    for (i = 18; i < 28; i++) {
        consoleDrawText(0, i, "                                        ");
    }

    // Draw player health bar
    consoleDrawText(1, 19, "PLAYER HP:");
    sprintf(buffer, "%d/%d", playerCharacter.health, playerCharacter.maxHealth);
    consoleDrawText(12, 19, buffer);

    // Draw simple health bar (10 characters wide)
    int healthPercent = (playerCharacter.health * 10) / playerCharacter.maxHealth;
    consoleDrawText(1, 20, "[");
    for (i = 0; i < 10; i++) {
        if (i < healthPercent) {
            consoleDrawText(2 + i, 20, "=");
        } else {
            consoleDrawText(2 + i, 20, " ");
        }
    }
    consoleDrawText(12, 20, "]");

    // Draw enemy health bar
    consoleDrawText(15, 19, "ENEMY HP:");
    sprintf(buffer, "%d/%d", currentBattle.enemy.health, currentBattle.enemy.maxHealth);
    consoleDrawText(26, 19, buffer);

    // Draw enemy health bar
    int enemyHealthPercent = (currentBattle.enemy.health * 10) / currentBattle.enemy.maxHealth;
    consoleDrawText(15, 20, "[");
    for (i = 0; i < 10; i++) {
        if (i < enemyHealthPercent) {
            consoleDrawText(16 + i, 20, "=");
        } else {
            consoleDrawText(16 + i, 20, " ");
        }
    }
    consoleDrawText(26, 20, "]");

    // Draw time energy
    consoleDrawText(1, 22, "TIME ENERGY:");
    sprintf(buffer, "%d/%d", playerCharacter.timeEnergy, playerCharacter.maxTimeEnergy);
    consoleDrawText(14, 22, buffer);

    // Draw battle menu
    consoleDrawText(1, 24, "A:ATTACK  B:DEFEND");
    consoleDrawText(1, 25, "X:TIME STOP  Y:SLOW");
    consoleDrawText(1, 26, "L:REWIND  R:ESCAPE");

    // Show current turn
    sprintf(buffer, "TURN: %d", currentBattle.turnCount);
    consoleDrawText(20, 24, buffer);

    // Show active time effects
    if (currentBattle.timeStopActive) {
        consoleDrawText(20, 25, "TIME STOP!");
    } else if (currentBattle.slowMotionActive) {
        consoleDrawText(20, 25, "SLOW MOTION!");
    } else {
        consoleDrawText(20, 25, "           ");
    }
}

void drawEnemySprite(void) {
    // Only draw enemy if battle is active
    if (currentBattle.state == BATTLE_STATE_IDLE) {
        return;
    }

    // Use sprite ID 1 for enemy (player uses ID 0)
    u8 enemySpriteId = 1;

    // Position enemy on the right side of screen
    s16 enemyX = 200;  // Right side of screen
    s16 enemyY = 80;   // Middle height

    // Clear the OAM entry first
    oamClear(enemySpriteId, 1);

    // Set enemy sprite based on enemy type
    // For now, use a simple sprite pattern - we can enhance this later
    // Use tiles from the loaded sprite sheet
    u8 tileIndex = 0;
    u8 palette = 0;

    // Different tile patterns for different enemy types
    switch (currentBattle.enemy.type) {
        case ENEMY_TYPE_SLIME:
            tileIndex = 16;  // Use different tiles for slime
            break;
        case ENEMY_TYPE_GOBLIN:
            tileIndex = 32;  // Different tiles for goblin
            break;
        case ENEMY_TYPE_ORC:
            tileIndex = 48;  // Different tiles for orc
            break;
        default:
            tileIndex = 16;  // Default to slime
            break;
    }

    // Set the sprite in OAM
    oamSet(enemySpriteId, enemyX, enemyY, 3, 0, 0, tileIndex, palette);

    // Update OAM
    oamUpdate();
}

void updateBattleAnimations(void) {
    // Handle battle animations and effects
    static u8 animationFrame = 0;
    animationFrame++;

    // Simple damage flash effect for enemy when hit
    if (currentBattle.enemy.health > 0 && animationFrame % 8 < 4) {
        // Could add flash effect here by changing palette temporarily
        // For now, just update the enemy sprite
        drawEnemySprite();
    }

    // Handle battle state transitions
    switch (currentBattle.state) {
        case BATTLE_STATE_ENCOUNTER:
            // Encounter animation - enemy appears
            if (animationFrame > 30) {  // ~0.5 seconds at 60fps
                currentBattle.state = BATTLE_STATE_ACTIVE;
                currentBattle.isPlayerTurn = 1;  // Player goes first
            }
            break;

        case BATTLE_STATE_VICTORY:
            // Victory animation
            consoleDrawText(10, 10, "VICTORY!");
            if (animationFrame > 120) {  // ~2 seconds
                endBattle();
            }
            break;

        case BATTLE_STATE_DEFEAT:
            // Defeat animation
            consoleDrawText(10, 10, "DEFEAT!");
            if (animationFrame > 120) {  // ~2 seconds
                endBattle();
            }
            break;

        case BATTLE_STATE_ESCAPE:
            // Escape animation
            consoleDrawText(10, 10, "ESCAPED!");
            if (animationFrame > 120) {  // ~2 seconds
                endBattle();
            }
            break;

        default:
            break;
    }
}