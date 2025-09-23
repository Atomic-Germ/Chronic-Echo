/*---------------------------------------------------------------------------------


    Chronic Echo - Time Manipulation System Implementation
    -- Position history tracking and time rewind mechanics


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <string.h>  // For memset

#include "time_manipulation.h"
#include "player.h"

//---------------------------------------------------------------------------------
// Global position history buffer
PositionHistoryBuffer positionHistory = {0};

//---------------------------------------------------------------------------------
// Initialize the position history buffer
void initPositionHistory(void)
{
    memset(&positionHistory, 0, sizeof(PositionHistoryBuffer));
    positionHistory.head = 0;
    positionHistory.tail = 0;
    positionHistory.count = 0;
    positionHistory.currentFrame = 0;
    positionHistory.isRewinding = 0;
}

//---------------------------------------------------------------------------------
// Record the current player position in the history buffer
void recordCurrentPosition(s16 x, s16 y)
{
    if (positionHistory.isRewinding) {
        return;  // Don't record while rewinding
    }

    // Create new entry
    PositionHistoryEntry entry;
    entry.x = x;
    entry.y = y;
    entry.frameNumber = positionHistory.currentFrame;

    // Add to circular buffer
    positionHistory.entries[positionHistory.head] = entry;

    // Update buffer indices
    positionHistory.head = (positionHistory.head + 1) % POSITION_HISTORY_SIZE;

    if (positionHistory.count < POSITION_HISTORY_SIZE) {
        positionHistory.count++;
    } else {
        // Buffer is full, advance tail to maintain circular behavior
        positionHistory.tail = (positionHistory.tail + 1) % POSITION_HISTORY_SIZE;
    }

    positionHistory.currentFrame++;
}

//---------------------------------------------------------------------------------
// Check if rewind is currently possible
u8 canRewind(void)
{
    return (positionHistory.count > 0 && !positionHistory.isRewinding);
}

//---------------------------------------------------------------------------------
// Check if we can rewind a specific distance
u8 canRewindDistance(u16 frames)
{
    if (!canRewind() || frames > MAX_REWIND_DISTANCE) {
        return 0;
    }

    // Check if we have enough history
    return (positionHistory.count >= frames);
}

//---------------------------------------------------------------------------------
// Rewind to a specific frame number
u8 rewindToFrame(u16 targetFrame)
{
    if (!canRewind()) {
        return 0;
    }

    // Find the entry closest to the target frame
    PositionHistoryEntry* entry = getPositionAtFrame(targetFrame);
    if (!entry) {
        return 0;  // Frame not found in history
    }

    // Check time energy cost
    u16 frameDistance = calculateFrameDistance(positionHistory.currentFrame, targetFrame);
    u16 energyCost = getRewindEnergyCost(frameDistance);

    if (playerCharacter.timeEnergy < energyCost) {
        return 0;  // Not enough time energy
    }

    // Perform rewind
    playerCharacter.x = entry->x;
    playerCharacter.y = entry->y;
    playerCharacter.timeEnergy -= energyCost;

    positionHistory.isRewinding = 1;

    return 1;  // Success
}

//---------------------------------------------------------------------------------
// Rewind by a specific number of frames
u8 rewindByFrames(u16 frameCount)
{
    if (!canRewindDistance(frameCount)) {
        return 0;
    }

    u16 targetFrame = positionHistory.currentFrame - frameCount;
    return rewindToFrame(targetFrame);
}

//---------------------------------------------------------------------------------
// Stop rewinding and resume normal recording
void stopRewind(void)
{
    positionHistory.isRewinding = 0;
}

//---------------------------------------------------------------------------------
// Get position entry for a specific frame number
PositionHistoryEntry* getPositionAtFrame(u16 frameNumber)
{
    if (positionHistory.count == 0) {
        return 0;  // No history available
    }

    // Search backwards from head for the frame
    u16 index = positionHistory.head;
    u16 checked = 0;

    while (checked < positionHistory.count) {
        index = (index == 0) ? POSITION_HISTORY_SIZE - 1 : index - 1;

        if (positionHistory.entries[index].frameNumber == frameNumber) {
            return &positionHistory.entries[index];
        }

        checked++;
    }

    return 0;  // Frame not found
}

//---------------------------------------------------------------------------------
// Get the current frame number for rewinding
u16 getCurrentRewindFrame(void)
{
    return positionHistory.currentFrame;
}

//---------------------------------------------------------------------------------
// Get the oldest frame number in history
u16 getOldestFrame(void)
{
    if (positionHistory.count == 0) {
        return 0;
    }

    return positionHistory.entries[positionHistory.tail].frameNumber;
}

//---------------------------------------------------------------------------------
// Get the newest frame number in history
u16 getNewestFrame(void)
{
    if (positionHistory.count == 0) {
        return 0;
    }

    u16 newestIndex = (positionHistory.head == 0) ? POSITION_HISTORY_SIZE - 1 : positionHistory.head - 1;
    return positionHistory.entries[newestIndex].frameNumber;
}

//---------------------------------------------------------------------------------
// Calculate frame distance between two frames
u16 calculateFrameDistance(u16 fromFrame, u16 toFrame)
{
    if (fromFrame >= toFrame) {
        return fromFrame - toFrame;
    }
    return 0;  // Invalid - can't rewind to future
}

//---------------------------------------------------------------------------------
// Calculate time energy cost for rewinding a certain distance
u16 getRewindEnergyCost(u16 frameCount)
{
    return frameCount * REWIND_ENERGY_COST;
}

//---------------------------------------------------------------------------------
// Handle time manipulation input from controller
void handleTimeManipulationInput(u16 currentPadState, u16 previousPadState)
{
    // Check for rewind button press (L button)
    if ((currentPadState & REWIND_BUTTON) && !(previousPadState & REWIND_BUTTON)) {
        // L button was just pressed - attempt to rewind by 1 frame
        if (canRewindDistance(1)) {
            rewindByFrames(1);
        }
        // Note: No else clause - failed rewinds are silent for now
        // Could add audio/visual feedback here in future phases
    }

    // Check for fast forward button (R button) - reserved for future feature
    if ((currentPadState & FAST_FORWARD_BUTTON) && !(previousPadState & FAST_FORWARD_BUTTON)) {
        // R button pressed - could implement fast forward in future
        // For now, do nothing
    }

    // Check for rewind button hold (continuous rewind)
    if (currentPadState & REWIND_BUTTON) {
        // L button is being held - could implement continuous rewind
        // For now, single press only to prevent accidental overuse
    }
}