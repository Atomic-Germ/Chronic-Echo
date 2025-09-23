/*---------------------------------------------------------------------------------


    Chronic Echo - Player Character System Header
    -- Player stats, inventory, and position management


---------------------------------------------------------------------------------*/
#ifndef PLAYER_H
#define PLAYER_H

#include <snes.h>

//---------------------------------------------------------------------------------
// Constants
#define MAX_INVENTORY_SLOTS 16
#define MAX_LEVEL 99
#define BASE_HEALTH 100
#define BASE_TIME_ENERGY 50

//---------------------------------------------------------------------------------
// Item types (simple enum for now)
typedef enum {
    ITEM_NONE = 0,
    ITEM_POTION,
    ITEM_TIME_CRYSTAL,
    ITEM_KEY,
    ITEM_WEAPON,
    ITEM_ARMOR
} ItemType;

//---------------------------------------------------------------------------------
// Item structure
typedef struct {
    ItemType type;
    u8 quantity;
    char name[16];  // Simple name storage
} Item;

//---------------------------------------------------------------------------------
// Player Character Structure (RPG System)
typedef struct {
    // Basic stats
    u16 health;
    u16 maxHealth;
    u16 timeEnergy;     // Time manipulation energy
    u16 maxTimeEnergy;
    u16 experience;
    u16 expToNext;      // Experience needed for next level
    u8 level;

    // Position (for time manipulation tracking)
    s16 x;
    s16 y;

    // Inventory system
    Item inventory[MAX_INVENTORY_SLOTS];
    u8 inventoryCount;  // Number of occupied slots

    // Status flags
    u8 active;          // Is player active?
} PlayerCharacter;

//---------------------------------------------------------------------------------
// Global player instance
extern PlayerCharacter playerCharacter;

//---------------------------------------------------------------------------------
// Function declarations
void initPlayerCharacter(void);
void updatePlayerCharacterStats(void);
u8 addItemToCharacterInventory(ItemType type, u8 quantity);
u8 removeItemFromCharacterInventory(u8 slot, u8 quantity);
u8 getCharacterInventorySlot(u8 index);
void setPlayerCharacterPosition(s16 x, s16 y);
void getPlayerCharacterPosition(s16* x, s16* y);
u8 levelUpPlayerCharacter(void);
void healPlayerCharacter(u16 amount);
void restorePlayerCharacterTimeEnergy(u16 amount);
void healPlayer(u16 amount);
void restoreTimeEnergy(u16 amount);

#endif // PLAYER_H