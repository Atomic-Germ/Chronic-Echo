/*---------------------------------------------------------------------------------


    Chronic Echo - Player Character System Implementation
    -- Player stats, inventory, and position management


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <string.h>  // For memset, strcpy

#include "player.h"

//---------------------------------------------------------------------------------
// Global player character instance
PlayerCharacter playerCharacter = {0};

//---------------------------------------------------------------------------------
// Item name lookup table
static const char* itemNames[] = {
    "None",
    "Potion",
    "Time Crystal",
    "Key",
    "Weapon",
    "Armor"
};

/*---------------------------------------------------------------------------------


    Chronic Echo - Player Character System Implementation
    -- Player stats, inventory, and position management


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <string.h>  // For memset, strcpy

#include "player.h"
#include "progression.h"  // Include progression system

//---------------------------------------------------------------------------------
// Global player character instance
PlayerCharacter playerCharacter = {0};

//---------------------------------------------------------------------------------
// Item name lookup table
static const char* itemNames[] = {
    "None",
    "Potion",
    "Time Crystal",
    "Key",
    "Weapon",
    "Armor"
};

//---------------------------------------------------------------------------------
void initPlayerCharacter(void)
{
    // Initialize position
    playerCharacter.x = 120;  // Center of screen
    playerCharacter.y = 104;  // Center of screen

    // Initialize core stats
    playerCharacter.health = BASE_HEALTH;
    playerCharacter.maxHealth = BASE_HEALTH;
    playerCharacter.timeEnergy = BASE_TIME_ENERGY;
    playerCharacter.maxTimeEnergy = BASE_TIME_ENERGY;

    // Initialize RPG progression
    playerCharacter.level = 1;
    playerCharacter.experience = 0;
    playerCharacter.expToNext = 100;  // Simple leveling curve

    // Initialize combat stats
    playerCharacter.attack = 10;   // Base attack
    playerCharacter.defense = 5;   // Base defense

    // Clear inventory
    memset(playerCharacter.inventory, 0, sizeof(playerCharacter.inventory));
    playerCharacter.inventoryCount = 0;

    // Set active
    playerCharacter.active = 1;
}

//---------------------------------------------------------------------------------
void updatePlayerCharacterStats(void)
{
    // Ensure stats don't exceed maximums
    if (playerCharacter.health > playerCharacter.maxHealth) {
        playerCharacter.health = playerCharacter.maxHealth;
    }
    if (playerCharacter.timeEnergy > playerCharacter.maxTimeEnergy) {
        playerCharacter.timeEnergy = playerCharacter.maxTimeEnergy;
    }

    // Update experience and leveling
    while (playerCharacter.experience >= playerCharacter.expToNext && playerCharacter.level < MAX_LEVEL) {
        levelUpPlayerCharacter();
    }
}

//---------------------------------------------------------------------------------
u8 addItemToCharacterInventory(ItemType type, u8 quantity)
{
    u8 canAdd;
    u8 i;
    
    // First, try to stack with existing items of same type
    for (i = 0; i < MAX_INVENTORY_SLOTS; i++) {
        if (playerCharacter.inventory[i].type == type && playerCharacter.inventory[i].quantity < 99) {
            canAdd = 99 - playerCharacter.inventory[i].quantity;
            if (quantity <= canAdd) {
                playerCharacter.inventory[i].quantity += quantity;
                return 1;  // Success
            } else {
                playerCharacter.inventory[i].quantity = 99;
                quantity -= canAdd;
            }
        }
    }

    // If we still have quantity to add, find empty slot
    if (quantity > 0 && playerCharacter.inventoryCount < MAX_INVENTORY_SLOTS) {
        for (i = 0; i < MAX_INVENTORY_SLOTS; i++) {
            if (playerCharacter.inventory[i].type == ITEM_NONE) {
                playerCharacter.inventory[i].type = type;
                playerCharacter.inventory[i].quantity = (quantity > 99) ? 99 : quantity;
                strcpy(playerCharacter.inventory[i].name, itemNames[type]);
                playerCharacter.inventoryCount++;
                return 1;  // Success
            }
        }
    }

    return 0;  // Failed - no space or invalid operation
}

//---------------------------------------------------------------------------------
u8 removeItemFromCharacterInventory(u8 slot, u8 quantity)
{
    if (slot >= MAX_INVENTORY_SLOTS || playerCharacter.inventory[slot].type == ITEM_NONE) {
        return 0;  // Invalid slot or empty
    }

    if (playerCharacter.inventory[slot].quantity >= quantity) {
        playerCharacter.inventory[slot].quantity -= quantity;
        if (playerCharacter.inventory[slot].quantity == 0) {
            playerCharacter.inventory[slot].type = ITEM_NONE;
            playerCharacter.inventoryCount--;
        }
        return 1;  // Success
    }

    return 0;  // Not enough quantity
}

//---------------------------------------------------------------------------------
u8 getCharacterInventorySlot(u8 index)
{
    if (index >= MAX_INVENTORY_SLOTS) {
        return ITEM_NONE;
    }
    return playerCharacter.inventory[index].type;
}

//---------------------------------------------------------------------------------
void setPlayerCharacterPosition(s16 x, s16 y)
{
    playerCharacter.x = x;
    playerCharacter.y = y;
}

//---------------------------------------------------------------------------------
void getPlayerCharacterPosition(s16* x, s16* y)
{
    if (x) *x = playerCharacter.x;
    if (y) *y = playerCharacter.y;
}

//---------------------------------------------------------------------------------
u8 levelUpPlayerCharacter(void)
{
    if (playerCharacter.level >= MAX_LEVEL) {
        return 0;  // Already max level
    }

    playerCharacter.level++;
    playerCharacter.experience -= playerCharacter.expToNext;

    // Calculate new exp requirement (simple curve)
    playerCharacter.expToNext = playerCharacter.level * 100;

    // Increase stats on level up
    playerCharacter.maxHealth += 10;
    playerCharacter.maxTimeEnergy += 5;

    // Heal to full on level up
    playerCharacter.health = playerCharacter.maxHealth;
    playerCharacter.timeEnergy = playerCharacter.maxTimeEnergy;

    return 1;  // Success
}

//---------------------------------------------------------------------------------
void healPlayerCharacter(u16 amount)
{
    playerCharacter.health += amount;
    if (playerCharacter.health > playerCharacter.maxHealth) {
        playerCharacter.health = playerCharacter.maxHealth;
    }
}

//---------------------------------------------------------------------------------
void restorePlayerCharacterTimeEnergy(u16 amount)
{
    playerCharacter.timeEnergy += amount;
    if (playerCharacter.timeEnergy > playerCharacter.maxTimeEnergy) {
        playerCharacter.timeEnergy = playerCharacter.maxTimeEnergy;
    }
}