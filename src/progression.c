/*---------------------------------------------------------------------------------


    Chronic Echo - Progression System Implementation
    -- Character leveling, stats, equipment, and experience management


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <string.h>
#include <stdio.h>
#include "progression.h"

//---------------------------------------------------------------------------------
// Global progression state
ProgressionState playerProgression;

//---------------------------------------------------------------------------------
// Sample equipment database (would normally be loaded from external files)
#define MAX_EQUIPMENT_ITEMS 8
static Equipment equipmentDatabase[MAX_EQUIPMENT_ITEMS];
static u8 equipmentCount = 0;

//---------------------------------------------------------------------------------
// Initialize sample equipment data
void initSampleEquipment(void) {
    // Equipment 0: Wooden Sword
    equipmentDatabase[0].id = 0;
    strcpy(equipmentDatabase[0].name, "Wooden Sword");
    equipmentDatabase[0].type = EQUIP_SLOT_WEAPON;
    equipmentDatabase[0].rarity = 1;
    equipmentDatabase[0].levelReq = 1;
    equipmentDatabase[0].hpBonus = 0;
    equipmentDatabase[0].mpBonus = 0;
    equipmentDatabase[0].attackBonus = 5;
    equipmentDatabase[0].defenseBonus = 0;
    equipmentDatabase[0].magicBonus = 0;
    equipmentDatabase[0].magicDefBonus = 0;
    equipmentDatabase[0].speedBonus = 0;
    equipmentDatabase[0].luckBonus = 0;
    equipmentDatabase[0].timePowerBonus = 0;
    equipmentDatabase[0].timeRegenBonus = 0;
    equipmentDatabase[0].timeEfficiencyBonus = 0;
    equipmentDatabase[0].equipped = 0;

    // Equipment 1: Leather Armor
    equipmentDatabase[1].id = 1;
    strcpy(equipmentDatabase[1].name, "Leather Armor");
    equipmentDatabase[1].type = EQUIP_SLOT_ARMOR;
    equipmentDatabase[1].rarity = 1;
    equipmentDatabase[1].levelReq = 1;
    equipmentDatabase[1].hpBonus = 10;
    equipmentDatabase[1].mpBonus = 0;
    equipmentDatabase[1].attackBonus = 0;
    equipmentDatabase[1].defenseBonus = 3;
    equipmentDatabase[1].magicBonus = 0;
    equipmentDatabase[1].magicDefBonus = 1;
    equipmentDatabase[1].speedBonus = 0;
    equipmentDatabase[1].luckBonus = 0;
    equipmentDatabase[1].timePowerBonus = 0;
    equipmentDatabase[1].timeRegenBonus = 0;
    equipmentDatabase[1].timeEfficiencyBonus = 0;
    equipmentDatabase[1].equipped = 0;

    // Equipment 2: Time Amulet
    equipmentDatabase[2].id = 2;
    strcpy(equipmentDatabase[2].name, "Time Amulet");
    equipmentDatabase[2].type = EQUIP_SLOT_ACCESSORY1;
    equipmentDatabase[2].rarity = 2;
    equipmentDatabase[2].levelReq = 5;
    equipmentDatabase[2].hpBonus = 0;
    equipmentDatabase[2].mpBonus = 5;
    equipmentDatabase[2].attackBonus = 0;
    equipmentDatabase[2].defenseBonus = 0;
    equipmentDatabase[2].magicBonus = 2;
    equipmentDatabase[2].magicDefBonus = 2;
    equipmentDatabase[2].speedBonus = 1;
    equipmentDatabase[2].luckBonus = 1;
    equipmentDatabase[2].timePowerBonus = 10;
    equipmentDatabase[2].timeRegenBonus = 2;
    equipmentDatabase[2].timeEfficiencyBonus = 5;
    equipmentDatabase[2].equipped = 0;

    equipmentCount = 3;
}

//---------------------------------------------------------------------------------
// Experience calculation functions
u32 calculateExpForLevel(u16 level) {
    if (level <= 1) return 0;
    return (u32)(BASE_EXP_PER_LEVEL * (level - 1) * EXP_MULTIPLIER);
}

u32 getExpToNextLevel(u16 currentLevel, u32 currentExp) {
    u32 expForNextLevel = calculateExpForLevel(currentLevel + 1);
    if (currentExp >= expForNextLevel) return 0;
    return expForNextLevel - currentExp;
}

//---------------------------------------------------------------------------------
// Leveling functions
u8 canLevelUp(CharacterStats* stats) {
    return (stats->experience >= calculateExpForLevel(stats->level + 1));
}

void performLevelUp(CharacterStats* stats) {
    if (!canLevelUp(stats)) return;

    stats->level++;
    stats->levelsGained++;

    // Base stat increases (can be customized per class/job)
    stats->maxHp += 5;
    stats->maxMp += 3;
    stats->attack += 2;
    stats->defense += 1;
    stats->magic += 1;
    stats->magicDef += 1;
    stats->speed += 1;
    stats->luck += 1;

    // Time manipulation stat increases
    stats->timePower += 5;
    stats->timeRegen += 1;

    // Restore HP/MP on level up
    stats->currentHp = stats->maxHp;
    stats->currentMp = stats->maxMp;

    // Award stat point for player allocation
    stats->statPointsAvailable++;
}

void distributeStatPoint(CharacterStats* stats, u8 statType) {
    if (stats->statPointsAvailable <= 0) return;

    switch (statType) {
        case 0: stats->maxHp += 2; break;
        case 1: stats->maxMp += 2; break;
        case 2: stats->attack += 1; break;
        case 3: stats->defense += 1; break;
        case 4: stats->magic += 1; break;
        case 5: stats->magicDef += 1; break;
        case 6: stats->speed += 1; break;
        case 7: stats->luck += 1; break;
        case 8: stats->timePower += 3; break;
        case 9: stats->timeRegen += 1; break;
    }

    stats->statPointsAvailable--;
}

//---------------------------------------------------------------------------------
// Equipment functions
u8 canEquipItem(CharacterStats* stats, Equipment* item) {
    return (stats->level >= item->levelReq);
}

void equipItem(ProgressionState* state, u8 inventoryIndex, EquipmentSlot slot) {
    if (inventoryIndex >= state->inventory.itemCount) return;
    if (slot >= MAX_EQUIPMENT_SLOTS) return;

    u8 equipmentId = state->inventory.equipmentIds[inventoryIndex];
    Equipment* item = &equipmentDatabase[equipmentId];

    if (!canEquipItem(&state->stats, item)) return;
    if (item->type != slot) return;

    // Unequip current item in this slot
    if (state->equippedItems[slot].id != 255) { // 255 = no equipment
        state->equippedItems[slot].equipped = 0;
    }

    // Equip new item
    memcpy(&state->equippedItems[slot], item, sizeof(Equipment));
    state->equippedItems[slot].equipped = 1;

    // Remove from inventory
    removeItemFromInventory(&state->inventory, inventoryIndex);

    // Recalculate stats
    calculateEquipmentBonuses(&state->stats, state->equippedItems);
}

void unequipItem(ProgressionState* state, EquipmentSlot slot) {
    if (slot >= MAX_EQUIPMENT_SLOTS) return;
    if (state->equippedItems[slot].id == 255) return;

    // Add to inventory (if space available)
    if (addItemToInventory(&state->inventory, state->equippedItems[slot].id)) {
        state->equippedItems[slot].equipped = 0;
        state->equippedItems[slot].id = 255; // Mark as empty

        // Recalculate stats
        calculateEquipmentBonuses(&state->stats, state->equippedItems);
    }
}

void calculateEquipmentBonuses(CharacterStats* baseStats, Equipment equippedItems[MAX_EQUIPMENT_SLOTS]) {
    // Reset to base stats first (this function assumes baseStats contains base values)
    // Note: In practice, you'd want to store base stats separately

    u8 i;
    for (i = 0; i < MAX_EQUIPMENT_SLOTS; i++) {
        if (equippedItems[i].equipped) {
            Equipment* item = &equippedItems[i];
            baseStats->maxHp += item->hpBonus;
            baseStats->maxMp += item->mpBonus;
            baseStats->attack += item->attackBonus;
            baseStats->defense += item->defenseBonus;
            baseStats->magic += item->magicBonus;
            baseStats->magicDef += item->magicDefBonus;
            baseStats->speed += item->speedBonus;
            baseStats->luck += item->luckBonus;
            baseStats->timePower += item->timePowerBonus;
            baseStats->timeRegen += item->timeRegenBonus;
            baseStats->timeEfficiency += item->timeEfficiencyBonus;
        }
    }
}

//---------------------------------------------------------------------------------
// Inventory management
u8 addItemToInventory(Inventory* inventory, u8 equipmentId) {
    if (inventory->itemCount >= MAX_INVENTORY_SIZE) return 0;

    inventory->equipmentIds[inventory->itemCount] = equipmentId;
    inventory->itemCount++;
    return 1;
}

u8 removeItemFromInventory(Inventory* inventory, u8 inventoryIndex) {
    if (inventoryIndex >= inventory->itemCount) return 0;

    // Shift items down
    u8 i;
    for (i = inventoryIndex; i < inventory->itemCount - 1; i++) {
        inventory->equipmentIds[i] = inventory->equipmentIds[i + 1];
    }

    inventory->itemCount--;
    return 1;
}

Equipment* getInventoryItem(Inventory* inventory, u8 index) {
    if (index >= inventory->itemCount) return NULL;
    return &equipmentDatabase[inventory->equipmentIds[index]];
}

//---------------------------------------------------------------------------------
// Progression state management
void initProgressionSystem(void) {
    // Initialize sample equipment
    initSampleEquipment();

    // Reset progression state
    resetProgressionState(&playerProgression);
}

void loadProgressionState(ProgressionState* state) {
    // TODO: Implement save/load from SRAM
    // For now, just reset to defaults
    resetProgressionState(state);
}

void saveProgressionState(ProgressionState* state) {
    // TODO: Implement save to SRAM
    // This would write the progression state to SNES SRAM
}

void resetProgressionState(ProgressionState* state) {
    // Initialize character stats
    state->stats.level = 1;
    state->stats.experience = 0;
    state->stats.expToNext = calculateExpForLevel(2);

    // Base stats at level 1
    state->stats.maxHp = 50;
    state->stats.currentHp = 50;
    state->stats.maxMp = 20;
    state->stats.currentMp = 20;
    state->stats.attack = 10;
    state->stats.defense = 8;
    state->stats.magic = 5;
    state->stats.magicDef = 6;
    state->stats.speed = 7;
    state->stats.luck = 5;

    // Time manipulation stats
    state->stats.timePower = 50;
    state->stats.timeRegen = 2;
    state->stats.timeEfficiency = 0;

    // Initialize equipment slots as empty (255 = no equipment)
    u8 i;
    for (i = 0; i < MAX_EQUIPMENT_SLOTS; i++) {
        state->equippedItems[i].id = 255;
        state->equippedItems[i].equipped = 0;
    }

    // Clear inventory
    state->inventory.itemCount = 0;

    // Add starting equipment
    addItemToInventory(&state->inventory, 0); // Wooden Sword
    addItemToInventory(&state->inventory, 1); // Leather Armor

    // Progression tracking
    state->totalExperience = 0;
    state->levelsGained = 0;
    state->statPointsAvailable = 0;
}

//---------------------------------------------------------------------------------
// Battle rewards
void grantExperience(ProgressionState* state, u32 expAmount) {
    state->stats.experience += expAmount;
    state->totalExperience += expAmount;

    // Check for level ups
    while (canLevelUp(&state->stats)) {
        performLevelUp(&state->stats);
    }

    // Update exp to next level
    state->stats.expToNext = getExpToNextLevel(state->stats.level, state->stats.experience);
}

void grantEquipment(ProgressionState* state, u8 equipmentId) {
    if (equipmentId >= equipmentCount) return;
    addItemToInventory(&state->inventory, equipmentId);
}

//---------------------------------------------------------------------------------
// UI and display functions
void renderLevelDisplay(u8 x, u8 y) {
    char buffer[32];
    sprintf(buffer, "Lv.%d EXP:%lu/%lu", playerProgression.stats.level,
            playerProgression.stats.experience, playerProgression.stats.experience + playerProgression.stats.expToNext);
    consoleDrawText(x, y, buffer);
}

void renderStatScreen(u8 x, u8 y) {
    char buffer[32];

    // Basic stats
    sprintf(buffer, "HP: %d/%d", playerProgression.stats.currentHp, playerProgression.stats.maxHp);
    consoleDrawText(x, y, buffer);
    sprintf(buffer, "MP: %d/%d", playerProgression.stats.currentMp, playerProgression.stats.maxMp);
    consoleDrawText(x, y+1, buffer);
    sprintf(buffer, "ATK:%d DEF:%d", playerProgression.stats.attack, playerProgression.stats.defense);
    consoleDrawText(x, y+2, buffer);
    sprintf(buffer, "MAG:%d MDF:%d", playerProgression.stats.magic, playerProgression.stats.magicDef);
    consoleDrawText(x, y+3, buffer);
    sprintf(buffer, "SPD:%d LCK:%d", playerProgression.stats.speed, playerProgression.stats.luck);
    consoleDrawText(x, y+4, buffer);

    // Time stats
    sprintf(buffer, "TIME:%d REG:%d", playerProgression.stats.timePower, playerProgression.stats.timeRegen);
    consoleDrawText(x, y+5, buffer);
}

void renderEquipmentMenu(u8 x, u8 y) {
    char buffer[32];
    u8 i;

    consoleDrawText(x, y, "EQUIPMENT:");
    for (i = 0; i < MAX_EQUIPMENT_SLOTS; i++) {
        const char* slotNames[] = {"Weapon:", "Armor:", "Acc1:", "Acc2:", "TimeDev:", "Special:"};
        consoleDrawText(x, y + 1 + i, slotNames[i]);

        if (playerProgression.equippedItems[i].equipped) {
            consoleDrawText(x + 8, y + 1 + i, playerProgression.equippedItems[i].name);
        } else {
            consoleDrawText(x + 8, y + 1 + i, "[Empty]");
        }
    }
}

//---------------------------------------------------------------------------------
// Debug functions
void debugProgressionInfo(void) {
    consoleDrawText(0, 0, "=== PROGRESSION DEBUG ===");
    renderLevelDisplay(0, 1);
    renderStatScreen(0, 3);
    renderEquipmentMenu(0, 10);
}

void debugStatInfo(CharacterStats* stats) {
    char buffer[32];
    sprintf(buffer, "Level: %d", stats->level);
    consoleDrawText(0, 0, buffer);
    sprintf(buffer, "EXP: %lu/%lu", stats->experience, stats->experience + stats->expToNext);
    consoleDrawText(0, 1, buffer);
    sprintf(buffer, "HP: %d/%d", stats->currentHp, stats->maxHp);
    consoleDrawText(0, 2, buffer);
}

void debugEquipmentInfo(Equipment* item) {
    char buffer[32];
    sprintf(buffer, "Item: %s", item->name);
    consoleDrawText(0, 0, buffer);
    sprintf(buffer, "ATK+%d DEF+%d", item->attackBonus, item->defenseBonus);
    consoleDrawText(0, 1, buffer);
    sprintf(buffer, "Lv.%d Req", item->levelReq);
    consoleDrawText(0, 2, buffer);
}