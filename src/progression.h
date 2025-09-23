/*---------------------------------------------------------------------------------


    Chronic Echo - Progression System Header
    -- Character leveling, stats, equipment, and experience management


---------------------------------------------------------------------------------*/
#ifndef PROGRESSION_H
#define PROGRESSION_H

#include <snes.h>

//---------------------------------------------------------------------------------
// Constants
#define MAX_LEVEL 50
#define BASE_EXP_PER_LEVEL 100
#define EXP_MULTIPLIER 1.2f
#define MAX_EQUIPMENT_SLOTS 6
#define MAX_INVENTORY_SIZE 16

// Equipment slot types
typedef enum {
    EQUIP_SLOT_WEAPON = 0,
    EQUIP_SLOT_ARMOR,
    EQUIP_SLOT_ACCESSORY1,
    EQUIP_SLOT_ACCESSORY2,
    EQUIP_SLOT_TIME_DEVICE,
    EQUIP_SLOT_SPECIAL
} EquipmentSlot;

//---------------------------------------------------------------------------------
// Character stats structure
typedef struct {
    u16 level;          // Current level (1-50)
    u32 experience;     // Current experience points
    u32 expToNext;      // Experience needed for next level

    // Base stats
    u16 maxHp;          // Maximum health points
    u16 currentHp;      // Current health points
    u16 maxMp;          // Maximum magic points
    u16 currentMp;      // Current magic points
    u16 attack;         // Physical attack power
    u16 defense;        // Physical defense
    u16 magic;          // Magic attack power
    u16 magicDef;       // Magic defense
    u16 speed;          // Action speed
    u16 luck;           // Critical hit chance and item find

    // Time manipulation stats
    u16 timePower;      // Base time manipulation energy
    u16 timeRegen;      // Time energy regeneration rate
    u16 timeEfficiency; // Energy cost reduction percentage
} CharacterStats;

//---------------------------------------------------------------------------------
// Equipment item structure
typedef struct {
    u8 id;              // Unique equipment ID
    char name[16];      // Equipment name
    u8 type;            // Equipment type/slot
    u8 rarity;          // Rarity level (1-5)
    u8 levelReq;        // Required level to equip

    // Stat bonuses
    s16 hpBonus;
    s16 mpBonus;
    s16 attackBonus;
    s16 defenseBonus;
    s16 magicBonus;
    s16 magicDefBonus;
    s16 speedBonus;
    s16 luckBonus;

    // Time manipulation bonuses
    s16 timePowerBonus;
    s16 timeRegenBonus;
    s16 timeEfficiencyBonus;

    u8 equipped;        // Currently equipped flag
} Equipment;

//---------------------------------------------------------------------------------
// Inventory structure
typedef struct {
    u8 itemCount;                           // Number of items in inventory
    u8 equipmentIds[MAX_INVENTORY_SIZE];    // Equipment IDs in inventory
} Inventory;

//---------------------------------------------------------------------------------
// Progression state structure
typedef struct {
    CharacterStats stats;                   // Current character stats
    Equipment equippedItems[MAX_EQUIPMENT_SLOTS]; // Currently equipped items
    Inventory inventory;                    // Player inventory
    u32 totalExperience;                    // Total experience earned
    u16 levelsGained;                       // Total levels gained
    u8 statPointsAvailable;                 // Unspent stat points
} ProgressionState;

//---------------------------------------------------------------------------------
// Experience calculation functions
u32 calculateExpForLevel(u16 level);
u32 getExpToNextLevel(u16 currentLevel, u32 currentExp);

//---------------------------------------------------------------------------------
// Leveling functions
u8 canLevelUp(CharacterStats* stats);
void performLevelUp(CharacterStats* stats);
void distributeStatPoint(CharacterStats* stats, u8 statType);

//---------------------------------------------------------------------------------
// Equipment functions
u8 canEquipItem(CharacterStats* stats, Equipment* item);
void equipItem(ProgressionState* state, u8 inventoryIndex, EquipmentSlot slot);
void unequipItem(ProgressionState* state, EquipmentSlot slot);
void calculateEquipmentBonuses(CharacterStats* baseStats, Equipment equippedItems[MAX_EQUIPMENT_SLOTS]);

//---------------------------------------------------------------------------------
// Inventory management
u8 addItemToInventory(Inventory* inventory, u8 equipmentId);
u8 removeItemFromInventory(Inventory* inventory, u8 inventoryIndex);
Equipment* getInventoryItem(Inventory* inventory, u8 index);

//---------------------------------------------------------------------------------
// Progression state management
void initProgressionSystem(void);
void loadProgressionState(ProgressionState* state);
void saveProgressionState(ProgressionState* state);
void resetProgressionState(ProgressionState* state);

//---------------------------------------------------------------------------------
// Battle rewards
void grantExperience(ProgressionState* state, u32 expAmount);
void grantEquipment(ProgressionState* state, u8 equipmentId);

//---------------------------------------------------------------------------------
// UI and display functions
void renderLevelDisplay(u8 x, u8 y);
void renderStatScreen(u8 x, u8 y);
void renderEquipmentMenu(u8 x, u8 y);

//---------------------------------------------------------------------------------
// Debug functions
void debugProgressionInfo(void);
void debugStatInfo(CharacterStats* stats);
void debugEquipmentInfo(Equipment* item);

//---------------------------------------------------------------------------------
// Global progression state
extern ProgressionState playerProgression;

#endif // PROGRESSION_H