/*---------------------------------------------------------------------------------


    Chronic Echo - Dialogue System Header
    -- NPC conversations, text display, and player choices


---------------------------------------------------------------------------------*/
#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <snes.h>

//---------------------------------------------------------------------------------
// Constants
#define MAX_DIALOGUE_TEXT 256
#define MAX_CHOICES 4
#define MAX_DIALOGUE_LINES 4
#define DIALOGUE_BOX_HEIGHT 6
#define DIALOGUE_BOX_WIDTH 28
#define DIALOGUE_BOX_X 2
#define DIALOGUE_BOX_Y 18

//---------------------------------------------------------------------------------
// Dialogue states
typedef enum {
    DIALOGUE_STATE_INACTIVE = 0,    // No dialogue active
    DIALOGUE_STATE_DISPLAYING,      // Text is being displayed
    DIALOGUE_STATE_WAITING,         // Waiting for player input
    DIALOGUE_STATE_CHOOSING,        // Player making a choice
    DIALOGUE_STATE_COMPLETE         // Dialogue finished
} DialogueState;

//---------------------------------------------------------------------------------
// Dialogue choice structure
typedef struct {
    char text[64];          // Choice text
    u8 nextNodeId;          // Next dialogue node ID
    u8 enabled;             // Whether this choice is available
} DialogueChoice;

//---------------------------------------------------------------------------------
// Dialogue node structure
typedef struct {
    u8 id;                          // Unique node identifier
    char speaker[16];               // Speaker name (empty for narration)
    char text[MAX_DIALOGUE_TEXT];   // Dialogue text
    u8 choiceCount;                 // Number of choices (0 = auto-advance)
    DialogueChoice choices[MAX_CHOICES]; // Available choices
    u8 nextNodeId;                  // Next node if no choices (0 = end)
    u8 hasEvent;                    // Whether this node triggers an event
    u8 eventId;                     // Event ID to trigger
} DialogueNode;

//---------------------------------------------------------------------------------
// Active dialogue structure
typedef struct {
    DialogueState state;
    u8 currentNodeId;
    u8 textCharIndex;               // Current character being displayed
    u8 textDisplaySpeed;            // Characters per frame (1-5)
    u8 selectedChoice;              // Currently selected choice (0-based)
    u8 choiceCount;                 // Total number of choices
    u8 isSkippable;                 // Whether player can skip text
    u8 speakerSpriteId;             // Sprite ID for speaker portrait
} ActiveDialogue;

//---------------------------------------------------------------------------------
// Global dialogue instance
extern ActiveDialogue currentDialogue;

//---------------------------------------------------------------------------------
// Function declarations

// Initialize dialogue system
void initDialogueSystem(void);

// Start a dialogue with specified NPC
void startDialogue(u8 npcId);

// Update dialogue state (call every frame)
void updateDialogue(void);

// Render dialogue box and text
void renderDialogue(void);

// Check if dialogue is currently active
u8 isDialogueActive(void);

// End current dialogue
void endDialogue(void);

// Set dialogue display speed (1-5, higher = faster)
void setDialogueSpeed(u8 speed);

// Skip current text display
void skipDialogueText(void);

// Navigate dialogue choices
void selectNextChoice(void);
void selectPreviousChoice(void);
void confirmChoice(void);

// Get current dialogue text (for testing)
const char* getCurrentDialogueText(void);

// Get current speaker name
const char* getCurrentSpeaker(void);

// Get number of available choices
u8 getChoiceCount(void);

// Get choice text by index
const char* getChoiceText(u8 choiceIndex);

// Get currently selected choice
u8 getSelectedChoice(void);

#endif // DIALOGUE_H