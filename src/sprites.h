/*---------------------------------------------------------------------------------


    Chronic Echos - Sprite Management System Header
    -- Player character and object sprite handling


---------------------------------------------------------------------------------*/
#ifndef SPRITES_H
#define SPRITES_H

#include <snes.h>

//---------------------------------------------------------------------------------
// Sprite Constants
#define PLAYER_SPRITE_ID 0
#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16
#define PLAYER_SPEED 2
#define PROJECTILE_SPEED 4
#define PROJECTILE_WIDTH 8
#define PROJECTILE_HEIGHT 8
#define MAX_PROJECTILES 8

//---------------------------------------------------------------------------------
// Player Character Structure
typedef struct {
    s16 x;              // X position (world coordinates)
    s16 y;              // Y position (world coordinates)
    s16 vx;             // X velocity
    s16 vy;             // Y velocity
    u8 facing;          // 0=right, 1=left, 2=up, 3=down
    u8 animationFrame;  // Current animation frame
    u8 spriteId;        // OAM sprite ID
    u8 active;          // Is this sprite active?
} Player;

//---------------------------------------------------------------------------------
// Projectile Structure
typedef struct {
    s16 x;              // X position
    s16 y;              // Y position
    s16 vx;             // X velocity
    s16 vy;             // Y velocity
    u8 active;          // Is this projectile active?
    u8 spriteId;        // OAM sprite ID
} Projectile;

//---------------------------------------------------------------------------------
// External declarations
extern Player player;
extern Projectile projectiles[MAX_PROJECTILES];

// Sprite graphics data
extern char sprites_new, sprites_new_end;
extern char sprites_new_pal, sprites_new_pal_end;

//---------------------------------------------------------------------------------
// Function declarations
void initSprites(void);
void initPlayer(void);
void updatePlayer(void);
void drawPlayer(void);
void movePlayer(s16 dx, s16 dy);
void debugPlayerInfo(void);
void initProjectiles(void);
void createProjectile(s16 x, s16 y, s16 vx, s16 vy);
void updateProjectiles(void);
void drawProjectiles(void);

#endif // SPRITES_H