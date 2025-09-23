/*---------------------------------------------------------------------------------


    Chronic Echo - World Map System Header
    -- World map navigation, areas, and NPC management


---------------------------------------------------------------------------------*/
#ifndef WORLD_H
#define WORLD_H

#include <snes.h>

//---------------------------------------------------------------------------------
// Constants
#define WORLD_WIDTH 32
#define WORLD_HEIGHT 24
#define TILE_SIZE 16
#define MAX_NPCS 16
#define MAX_AREAS 8

//---------------------------------------------------------------------------------
// Tile types for world map
typedef enum {
    TILE_GRASS = 0,
    TILE_PATH,
    TILE_FOREST,
    TILE_MOUNTAIN,
    TILE_WATER,
    TILE_BUILDING,
    TILE_DOOR,
    TILE_NPC
} TileType;

//---------------------------------------------------------------------------------
// Area types
typedef enum {
    AREA_OVERWORLD = 0,
    AREA_FOREST,
    AREA_MOUNTAIN,
    AREA_VILLAGE,
    AREA_DUNGEON,
    AREA_CAVE
} AreaType;

//---------------------------------------------------------------------------------
// NPC types
typedef enum {
    NPC_TYPE_VILLAGER = 0,
    NPC_TYPE_MERCHANT,
    NPC_TYPE_GUIDE,
    NPC_TYPE_GUARD,
    NPC_TYPE_QUEST_GIVER
} NPCType;

//---------------------------------------------------------------------------------
// NPC structure
typedef struct {
    u8 active;
    NPCType type;
    s16 x, y;           // World position
    u8 spriteId;        // Sprite index
    char name[16];
    char dialogue[128]; // Simple dialogue text
    u8 hasQuest;        // Whether this NPC has a quest
} NPC;

//---------------------------------------------------------------------------------
// Area structure
typedef struct {
    AreaType type;
    char name[16];
    u8 tilemap[WORLD_WIDTH * WORLD_HEIGHT]; // Tile data
    NPC npcs[MAX_NPCS];    // NPCs in this area
    u8 npcCount;
    u8 bgTileSet;          // Background tileset index
    u8 bgMapSet;           // Background map index
} Area;

//---------------------------------------------------------------------------------
// World structure
typedef struct {
    Area areas[MAX_AREAS];
    u8 currentArea;
    u8 areaCount;
    s16 cameraX;  // Camera position for scrolling
    s16 cameraY;
} World;

//---------------------------------------------------------------------------------
// Global world instance
extern World gameWorld;

//---------------------------------------------------------------------------------
// Function declarations
void initWorld(void);
void loadArea(u8 areaIndex);
void updateWorldNavigation(void);
void renderWorld(void);
u8 checkTileCollision(s16 x, s16 y);
void addNPCToArea(u8 areaIndex, NPCType type, s16 x, s16 y, const char* name, const char* dialogue);
NPC* getNPCAtPosition(s16 playerX, s16 playerY);
void interactWithNPC(NPC* npc);
void transitionToArea(u8 newAreaIndex, s16 spawnX, s16 spawnY);

#endif // WORLD_H