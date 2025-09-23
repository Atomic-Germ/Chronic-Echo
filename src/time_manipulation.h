/*---------------------------------------------------------------------------------


    Chronic Echo - Time Manipulation System Header
    -- Position history tracking and time rewind mechanics


---------------------------------------------------------------------------------*/
#ifndef TIME_MANIPULATION_H
#define TIME_MANIPULATION_H

#include <snes.h>

//---------------------------------------------------------------------------------
// Constants
#define POSITION_HISTORY_SIZE 300  // Store 5 seconds of position history at 60fps
#define REWIND_ENERGY_COST 5       // Time energy cost per rewind frame
#define MAX_REWIND_DISTANCE 180    // Maximum frames that can be rewound at once

//---------------------------------------------------------------------------------
// Position History Entry Structure
typedef struct {
    s16 x;              // Player X position
    s16 y;              // Player Y position
    u16 frameNumber;    // Frame when this position was recorded
} PositionHistoryEntry;

//---------------------------------------------------------------------------------
// Position History Buffer Structure
typedef struct {
    PositionHistoryEntry entries[POSITION_HISTORY_SIZE];
    u16 head;           // Index of newest entry
    u16 tail;           // Index of oldest entry
    u16 count;          // Number of entries in buffer
    u16 currentFrame;   // Current frame counter
    u8 isRewinding;     // Flag indicating if currently rewinding
} PositionHistoryBuffer;

//---------------------------------------------------------------------------------
// Global position history buffer
extern PositionHistoryBuffer positionHistory;

//---------------------------------------------------------------------------------
// Function declarations

// Buffer management
void initPositionHistory(void);
void recordCurrentPosition(s16 x, s16 y);
u8 canRewind(void);
u8 canRewindDistance(u16 frames);

// Rewind mechanics
u8 rewindToFrame(u16 targetFrame);
u8 rewindByFrames(u16 frameCount);
void stopRewind(void);

// History queries
PositionHistoryEntry* getPositionAtFrame(u16 frameNumber);
u16 getCurrentRewindFrame(void);
u16 getOldestFrame(void);
u16 getNewestFrame(void);

// Utility functions
u16 calculateFrameDistance(u16 fromFrame, u16 toFrame);
u16 getRewindEnergyCost(u16 frameCount);

#endif // TIME_MANIPULATION_H