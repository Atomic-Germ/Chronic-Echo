/*---------------------------------------------------------------------------------


    Chronic Echo - Battle System Header
    -- Battle mechanics with time manipulation integration


---------------------------------------------------------------------------------*/
#ifndef BATTLE_H
#define BATTLE_H

#include <snes.h>
#include "player.h"

//---------------------------------------------------------------------------------
// Battle States
typedef enum {
    BATTLE_STATE_IDLE = 0,        // No battle active
    BATTLE_STATE_ENCOUNTER,       // Encounter animation/transition
    BATTLE_STATE_ACTIVE,          // Active battle in progress
    BATTLE_STATE_VICTORY,         // Player won the battle
    BATTLE_STATE_DEFEAT,          // Player lost the battle
    BATTLE_STATE_ESCAPE           // Player escaped the battle
} BattleState;

//---------------------------------------------------------------------------------
// Enemy Types
typedef enum {
    ENEMY_TYPE_SLIME = 0,
    ENEMY_TYPE_GOBLIN,
    ENEMY_TYPE_ORC,
    ENEMY_TYPE_SKELETON,
    ENEMY_TYPE_DRAGON,
    ENEMY_TYPE_BOSS
} EnemyType;

//---------------------------------------------------------------------------------
// Enemy Structure
typedef struct {
    EnemyType type;
    char name[16];
    u16 health;
    u16 maxHealth;
    u16 attack;
    u16 defense;
    u16 experience;    // XP awarded for defeating
    u8 level;
    u8 spriteId;       // Sprite ID for rendering
} Enemy;

//---------------------------------------------------------------------------------
// Battle Structure
typedef struct {
    BattleState state;
    Enemy enemy;
    u16 turnCount;
    u8 playerAction;   // Current player action selection
    u8 battleResult;   // Result of battle (victory/defeat/escape)
    u8 isPlayerTurn;   // 1 if player's turn, 0 if enemy's turn
    u8 timeStopActive; // 1 if time stop is active
    u8 slowMotionActive; // 1 if slow motion is active
} Battle;

//---------------------------------------------------------------------------------
// Battle Actions
#define BATTLE_ACTION_ATTACK 0
#define BATTLE_ACTION_DEFEND 1
#define BATTLE_ACTION_TIME_STOP 2
#define BATTLE_ACTION_SLOW_MOTION 3
#define BATTLE_ACTION_REWIND 4
#define BATTLE_ACTION_ESCAPE 5

//---------------------------------------------------------------------------------
// Battle Constants
#define MAX_BATTLE_TURNS 100
#define TIME_STOP_DURATION 3    // Turns time stop lasts
#define SLOW_MOTION_DURATION 2  // Turns slow motion lasts
#define ESCAPE_CHANCE_BASE 30   // Base escape chance percentage
#define TIME_STOP_ENERGY_COST 15
#define SLOW_MOTION_ENERGY_COST 10
#define REWIND_ENERGY_COST 20

//---------------------------------------------------------------------------------
// Global battle instance
extern Battle currentBattle;

//---------------------------------------------------------------------------------
// Function declarations
void initBattleSystem(void);
void startBattle(EnemyType enemyType);
void updateBattle(void);
void endBattle(void);
u8 isBattleActive(void);
void setBattleAction(u8 action);
void processBattleTurn(void);
void handleBattleInput(u16 currentPadState, u16 previousPadState);

// Battle mechanics
u8 calculateDamage(u16 attackerAttack, u16 defenderDefense);
u8 attemptEscape(void);
void applyTimeStop(void);
void applySlowMotion(void);
void rewindBattleTurn(void);

// UI functions
void drawBattleScreen(void);
void drawBattleUI(void);
void drawEnemySprite(void);
void updateBattleAnimations(void);

// Enemy definitions
void initEnemyDefinitions(void);
Enemy createEnemy(EnemyType type);

#endif // BATTLE_H