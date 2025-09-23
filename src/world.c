/*---------------------------------------------------------------------------------


    Chronic Echo - World Map System Implementation
    -- World map navigation, areas, and NPC management


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <string.h>
#include "world.h"
#include "player.h"
#include "sprites.h"
#include "dialogue.h"

//---------------------------------------------------------------------------------
// Global world instance
World gameWorld;

//---------------------------------------------------------------------------------
// Sample overworld tilemap (32x24)
const u8 overworldTilemap[WORLD_WIDTH * WORLD_HEIGHT] = {
    // Row 0-7: Grass field with path
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

    // Row 8-15: Path leading to village
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

    // Row 16-23: Village area
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

//---------------------------------------------------------------------------------
void initWorld(void) {
    // Initialize world structure
    gameWorld.currentArea = 0;
    gameWorld.areaCount = 0;
    gameWorld.cameraX = 0;
    gameWorld.cameraY = 0;

    // Create overworld area
    Area* overworld = &gameWorld.areas[0];
    overworld->type = AREA_OVERWORLD;
    strcpy(overworld->name, "Overworld");
    memcpy(overworld->tilemap, overworldTilemap, sizeof(overworldTilemap));
    overworld->npcCount = 0;
    overworld->bgTileSet = 0; // Will be set when graphics are loaded
    overworld->bgMapSet = 0;

    gameWorld.areaCount = 1;

    // Add some sample NPCs
    addNPCToArea(0, NPC_TYPE_VILLAGER, 200, 150, "Villager", "Welcome to our village! Be careful of the monsters.");
    addNPCToArea(0, NPC_TYPE_MERCHANT, 250, 180, "Merchant", "I sell potions and equipment. What do you need?");
}

//---------------------------------------------------------------------------------
void loadArea(u8 areaIndex) {
    if (areaIndex >= gameWorld.areaCount) {
        return;
    }

    gameWorld.currentArea = areaIndex;
    Area* area = &gameWorld.areas[areaIndex];

    // Load area graphics (placeholder - will be implemented with actual graphics)
    // bgInitTileSet(area->bgTileSet, ...);
    // bgInitMapSet(area->bgMapSet, ...);

    // Reset camera position
    gameWorld.cameraX = 0;
    gameWorld.cameraY = 0;
}

//---------------------------------------------------------------------------------
void updateWorldNavigation(void) {
    // Handle player movement input
    u16 pad0 = padsCurrent(0);

    s16 newX = playerCharacter.x;
    s16 newY = playerCharacter.y;

    // Movement speed (pixels per frame)
    const s16 MOVE_SPEED = 2;

    if (pad0 & KEY_LEFT) {
        newX -= MOVE_SPEED;
    }
    if (pad0 & KEY_RIGHT) {
        newX += MOVE_SPEED;
    }
    if (pad0 & KEY_UP) {
        newY -= MOVE_SPEED;
    }
    if (pad0 & KEY_DOWN) {
        newY += MOVE_SPEED;
    }

    // Check collision before moving
    if (!checkTileCollision(newX, newY)) {
        playerCharacter.x = newX;
        playerCharacter.y = newY;
    }

    // Update camera to follow player (simple follow camera)
    gameWorld.cameraX = playerCharacter.x - 128; // Center on screen (256/2)
    gameWorld.cameraY = playerCharacter.y - 112; // Center on screen (224/2)

    // Clamp camera bounds
    if (gameWorld.cameraX < 0) gameWorld.cameraX = 0;
    if (gameWorld.cameraY < 0) gameWorld.cameraY = 0;
    if (gameWorld.cameraX > (WORLD_WIDTH * TILE_SIZE) - 256) gameWorld.cameraX = (WORLD_WIDTH * TILE_SIZE) - 256;
    if (gameWorld.cameraY > (WORLD_HEIGHT * TILE_SIZE) - 224) gameWorld.cameraY = (WORLD_HEIGHT * TILE_SIZE) - 224;

    // Check for NPC interactions
    if (pad0 & KEY_A) {
        NPC* npc = getNPCAtPosition(playerCharacter.x, playerCharacter.y);
        if (npc) {
            interactWithNPC(npc);
        }
    }
}

//---------------------------------------------------------------------------------
void renderWorld(void) {
    Area* area = &gameWorld.areas[gameWorld.currentArea];

    // Render tilemap (placeholder - will be implemented with actual BG rendering)
    // For now, just clear screen and show area name
    consoleDrawText(1, 1, area->name);

    // TODO: Render NPCs and world elements
    /*
    // Render NPCs
    for (int i = 0; i < area->npcCount; i++) {
        NPC* npc = &area->npcs[i];
        if (npc->active) {
            // Convert world coordinates to screen coordinates
            int screenX, screenY;
            screenX = npc->x - gameWorld.cameraX;
            screenY = npc->y - gameWorld.cameraY;

            // Only render if on screen
            if (screenX >= -16 && screenX < 256 && screenY >= -16 && screenY < 224) {
                // Update sprite position (placeholder)
                // oamSet(npc->spriteId, screenX, screenY, 3, 0, 0, npc->spriteId, 0);
            }
        }
    }

    // Update OAM
    oamUpdate();
    */
}

//---------------------------------------------------------------------------------
u8 checkTileCollision(s16 x, s16 y) {
    // Convert pixel coordinates to tile coordinates
    u8 tileX = x / TILE_SIZE;
    u8 tileY = y / TILE_SIZE;

    // Bounds checking
    if (tileX >= WORLD_WIDTH || tileY >= WORLD_HEIGHT) {
        return 1; // Collision (out of bounds)
    }

    Area* area = &gameWorld.areas[gameWorld.currentArea];
    TileType tile = (TileType)area->tilemap[tileY * WORLD_WIDTH + tileX];

    // Check for collision tiles
    switch (tile) {
        case TILE_MOUNTAIN:
        case TILE_WATER:
        case TILE_BUILDING:
            return 1; // Collision
        default:
            return 0; // No collision
    }
}

//---------------------------------------------------------------------------------
void addNPCToArea(u8 areaIndex, NPCType type, s16 x, s16 y, const char* name, const char* dialogue) {
    if (areaIndex >= gameWorld.areaCount) {
        return;
    }

    Area* area = &gameWorld.areas[areaIndex];
    if (area->npcCount >= MAX_NPCS) {
        return;
    }

    NPC* npc = &area->npcs[area->npcCount];
    npc->active = 1;
    npc->type = type;
    npc->x = x;
    npc->y = y;
    npc->spriteId = area->npcCount; // Simple sprite ID assignment
    npc->hasQuest = 0;

    strcpy(npc->name, name);
    strcpy(npc->dialogue, dialogue);

    area->npcCount++;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
NPC* getNPCAtPosition(s16 playerX, s16 playerY) {
    Area* area = &gameWorld.areas[gameWorld.currentArea];
    int i;

    for (i = 0; i < area->npcCount; i++) {
        NPC* npc = &area->npcs[i];
        if (npc->active) {
            int dx = npc->x - playerX;
            int dy = npc->y - playerY;
            if ((dx * dx + dy * dy) < 1024) {
                return npc;
            }
        }
    }

    return NULL;
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
void interactWithNPC(NPC* npc) {
    // Start dialogue with this NPC
    // For now, use a simple NPC ID based on array index
    // In a full implementation, this would map to specific dialogue trees
    u8 npcId = 0; // Default to first NPC dialogue

    // Find the NPC index in the current area
    Area* area = &gameWorld.areas[gameWorld.currentArea];
    int i;
    for (i = 0; i < area->npcCount; i++) {
        if (&area->npcs[i] == npc) {
            npcId = i;
            break;
        }
    }

    startDialogue(npcId);
}

//---------------------------------------------------------------------------------
void transitionToArea(u8 newAreaIndex, s16 spawnX, s16 spawnY) {
    if (newAreaIndex >= gameWorld.areaCount) {
        return;
    }

    // Load new area
    loadArea(newAreaIndex);

    // Set player position
    playerCharacter.x = spawnX;
    playerCharacter.y = spawnY;
}