/*---------------------------------------------------------------------------------


    Chronic Echo - Dialogue System Implementation
    -- NPC conversations, text display, and player choices


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <string.h>
#include "dialogue.h"
#include "world.h"  // For NPC data access

//---------------------------------------------------------------------------------
// Global dialogue instance
ActiveDialogue currentDialogue;

//---------------------------------------------------------------------------------
// Sample dialogue data (would normally be loaded from external files)
// This is a simple conversation tree for demonstration

#define MAX_DIALOGUE_NODES 16
static DialogueNode dialogueNodes[MAX_DIALOGUE_NODES];
static u8 dialogueNodeCount = 0;

//---------------------------------------------------------------------------------
// Initialize sample dialogue data
void initSampleDialogue(void) {
    // Node 0: Villager greeting
    dialogueNodes[0].id = 0;
    strcpy(dialogueNodes[0].speaker, "Villager");
    strcpy(dialogueNodes[0].text, "Hello, traveler! Welcome to our village. What brings you here?");
    dialogueNodes[0].choiceCount = 2;
    strcpy(dialogueNodes[0].choices[0].text, "I'm exploring the world.");
    dialogueNodes[0].choices[0].nextNodeId = 1;
    dialogueNodes[0].choices[0].enabled = 1;
    strcpy(dialogueNodes[0].choices[1].text, "I'm looking for adventure.");
    dialogueNodes[0].choices[1].nextNodeId = 2;
    dialogueNodes[0].choices[1].enabled = 1;
    dialogueNodes[0].nextNodeId = 0;
    dialogueNodes[0].hasEvent = 0;

    // Node 1: Response to exploration
    dialogueNodes[1].id = 1;
    strcpy(dialogueNodes[1].speaker, "Villager");
    strcpy(dialogueNodes[1].text, "Ah, exploration! There's much to see in these lands. Be careful of the time anomalies.");
    dialogueNodes[1].choiceCount = 1;
    strcpy(dialogueNodes[1].choices[0].text, "Time anomalies?");
    dialogueNodes[1].choices[0].nextNodeId = 3;
    dialogueNodes[1].choices[0].enabled = 1;
    dialogueNodes[1].nextNodeId = 0;
    dialogueNodes[1].hasEvent = 0;

    // Node 2: Response to adventure
    dialogueNodes[2].id = 2;
    strcpy(dialogueNodes[2].speaker, "Villager");
    strcpy(dialogueNodes[2].text, "Adventure, eh? Well, you've come to the right place! Just watch out for those temporal disturbances.");
    dialogueNodes[2].choiceCount = 1;
    strcpy(dialogueNodes[2].choices[0].text, "Temporal disturbances?");
    dialogueNodes[2].choices[0].nextNodeId = 3;
    dialogueNodes[2].choices[0].enabled = 1;
    dialogueNodes[2].nextNodeId = 0;
    dialogueNodes[2].hasEvent = 0;

    // Node 3: Explanation of time anomalies
    dialogueNodes[3].id = 3;
    strcpy(dialogueNodes[3].speaker, "Villager");
    strcpy(dialogueNodes[3].text, "Yes, strange things happen with time around here. Some say a great power sleeps beneath the mountains. Farewell, traveler!");
    dialogueNodes[3].choiceCount = 0;
    dialogueNodes[3].nextNodeId = 0; // End conversation
    dialogueNodes[3].hasEvent = 0;

    dialogueNodeCount = 4;
}

//---------------------------------------------------------------------------------
// Initialize dialogue system
void initDialogueSystem(void) {
    currentDialogue.state = DIALOGUE_STATE_INACTIVE;
    currentDialogue.currentNodeId = 0;
    currentDialogue.textCharIndex = 0;
    currentDialogue.textDisplaySpeed = 2; // Default speed
    currentDialogue.selectedChoice = 0;
    currentDialogue.choiceCount = 0;
    currentDialogue.isSkippable = 1;
    currentDialogue.speakerSpriteId = 0;

    // Initialize sample dialogue data
    initSampleDialogue();
}

//---------------------------------------------------------------------------------
// Start a dialogue with specified NPC
void startDialogue(u8 npcId) {
    if (currentDialogue.state != DIALOGUE_STATE_INACTIVE) {
        return; // Dialogue already active
    }

    // Find the NPC in the current area
    NPC* npc = NULL;
    u8 i;
    for (i = 0; i < gameWorld.areas[gameWorld.currentArea].npcCount; i++) {
        if (gameWorld.areas[gameWorld.currentArea].npcs[i].active) {
            // Simple NPC ID matching (could be improved)
            if (i == npcId) {
                npc = &gameWorld.areas[gameWorld.currentArea].npcs[i];
                break;
            }
        }
    }

    if (npc == NULL) {
        return; // NPC not found
    }

    // Start dialogue with first node
    currentDialogue.state = DIALOGUE_STATE_DISPLAYING;
    currentDialogue.currentNodeId = 0; // Start with node 0
    currentDialogue.textCharIndex = 0;
    currentDialogue.selectedChoice = 0;
    currentDialogue.speakerSpriteId = npc->spriteId;

    // Copy choice data from current node
    DialogueNode* node = &dialogueNodes[currentDialogue.currentNodeId];
    currentDialogue.choiceCount = node->choiceCount;
}

//---------------------------------------------------------------------------------
// Update dialogue state (call every frame)
void updateDialogue(void) {
    if (currentDialogue.state == DIALOGUE_STATE_INACTIVE) {
        return;
    }

    DialogueNode* currentNode = &dialogueNodes[currentDialogue.currentNodeId];

    switch (currentDialogue.state) {
        case DIALOGUE_STATE_DISPLAYING:
            // Animate text display
            if (currentDialogue.textCharIndex < strlen(currentNode->text)) {
                currentDialogue.textCharIndex += currentDialogue.textDisplaySpeed;
                if (currentDialogue.textCharIndex > strlen(currentNode->text)) {
                    currentDialogue.textCharIndex = strlen(currentNode->text);
                }
            } else {
                // Text display complete
                if (currentDialogue.choiceCount > 0) {
                    currentDialogue.state = DIALOGUE_STATE_CHOOSING;
                } else {
                    currentDialogue.state = DIALOGUE_STATE_WAITING;
                }
            }
            break;

        case DIALOGUE_STATE_WAITING:
            // Wait for player to advance (A button)
            if (padsCurrent(0) & KEY_A) {
                if (currentNode->nextNodeId == 0) {
                    // End of conversation
                    endDialogue();
                } else {
                    // Advance to next node
                    currentDialogue.currentNodeId = currentNode->nextNodeId;
                    currentDialogue.textCharIndex = 0;
                    currentDialogue.state = DIALOGUE_STATE_DISPLAYING;
                    currentDialogue.selectedChoice = 0;

                    // Update choice count for new node
                    DialogueNode* nextNode = &dialogueNodes[currentDialogue.currentNodeId];
                    currentDialogue.choiceCount = nextNode->choiceCount;
                }
            }
            break;

        case DIALOGUE_STATE_CHOOSING:
            // Handle choice navigation
            if (padsCurrent(0) & KEY_UP) {
                selectPreviousChoice();
            } else if (padsCurrent(0) & KEY_DOWN) {
                selectNextChoice();
            } else if (padsCurrent(0) & KEY_A) {
                confirmChoice();
            }
            break;

        case DIALOGUE_STATE_COMPLETE:
            // Dialogue finished
            endDialogue();
            break;
    }
}

//---------------------------------------------------------------------------------
// Render dialogue box and text
void renderDialogue(void) {
    if (currentDialogue.state == DIALOGUE_STATE_INACTIVE) {
        return;
    }

    DialogueNode* currentNode = &dialogueNodes[currentDialogue.currentNodeId];

    // Draw dialogue box background
    u8 y, x;
    for (y = DIALOGUE_BOX_Y; y < DIALOGUE_BOX_Y + DIALOGUE_BOX_HEIGHT; y++) {
        for (x = DIALOGUE_BOX_X; x < DIALOGUE_BOX_X + DIALOGUE_BOX_WIDTH; x++) {
            // Draw border or background
            if (y == DIALOGUE_BOX_Y || y == DIALOGUE_BOX_Y + DIALOGUE_BOX_HEIGHT - 1 ||
                x == DIALOGUE_BOX_X || x == DIALOGUE_BOX_X + DIALOGUE_BOX_WIDTH - 1) {
                consoleDrawText(x, y, "#"); // Border character
            } else {
                consoleDrawText(x, y, " "); // Background
            }
        }
    }

    // Draw speaker name if present
    if (strlen(currentNode->speaker) > 0) {
        consoleDrawText(DIALOGUE_BOX_X + 1, DIALOGUE_BOX_Y + 1, currentNode->speaker);
        consoleDrawText(DIALOGUE_BOX_X + 1 + strlen(currentNode->speaker), DIALOGUE_BOX_Y + 1, ":");
    }

    // Draw dialogue text (with typewriter effect)
    u8 textY = DIALOGUE_BOX_Y + 2;
    u8 textX = DIALOGUE_BOX_X + 1;
    u8 charsDisplayed = 0;
    u8 maxCharsPerLine = DIALOGUE_BOX_WIDTH - 2;

    while (charsDisplayed < currentDialogue.textCharIndex && charsDisplayed < strlen(currentNode->text)) {
        char ch = currentNode->text[charsDisplayed];
        if (ch == '\n' || (charsDisplayed > 0 && (charsDisplayed % maxCharsPerLine) == 0)) {
            textY++;
            textX = DIALOGUE_BOX_X + 1;
            if (ch != '\n') {
                charsDisplayed--; // Don't consume the character
            }
        } else {
            consoleDrawText(textX++, textY, &ch);
        }
        charsDisplayed++;
    }

    // Draw choices if in choosing state
    if (currentDialogue.state == DIALOGUE_STATE_CHOOSING) {
        u8 choiceY = DIALOGUE_BOX_Y + 4;
        u8 i;
        for (i = 0; i < currentDialogue.choiceCount; i++) {
            char choiceIndicator[3] = "  ";
            if (i == currentDialogue.selectedChoice) {
                choiceIndicator[0] = '>';
            }
            consoleDrawText(DIALOGUE_BOX_X + 1, choiceY + i, choiceIndicator);
            consoleDrawText(DIALOGUE_BOX_X + 3, choiceY + i, currentNode->choices[i].text);
        }
    }

    // Draw continue prompt if waiting
    if (currentDialogue.state == DIALOGUE_STATE_WAITING) {
        consoleDrawText(DIALOGUE_BOX_X + DIALOGUE_BOX_WIDTH - 3, DIALOGUE_BOX_Y + DIALOGUE_BOX_HEIGHT - 2, ">>");
    }
}

//---------------------------------------------------------------------------------
// Check if dialogue is currently active
u8 isDialogueActive(void) {
    return currentDialogue.state != DIALOGUE_STATE_INACTIVE;
}

//---------------------------------------------------------------------------------
// End current dialogue
void endDialogue(void) {
    currentDialogue.state = DIALOGUE_STATE_INACTIVE;
    currentDialogue.currentNodeId = 0;
    currentDialogue.textCharIndex = 0;
    currentDialogue.selectedChoice = 0;
    currentDialogue.choiceCount = 0;
}

//---------------------------------------------------------------------------------
// Set dialogue display speed (1-5, higher = faster)
void setDialogueSpeed(u8 speed) {
    if (speed >= 1 && speed <= 5) {
        currentDialogue.textDisplaySpeed = speed;
    }
}

//---------------------------------------------------------------------------------
// Skip current text display
void skipDialogueText(void) {
    if (currentDialogue.state == DIALOGUE_STATE_DISPLAYING) {
        DialogueNode* currentNode = &dialogueNodes[currentDialogue.currentNodeId];
        currentDialogue.textCharIndex = strlen(currentNode->text);

        if (currentDialogue.choiceCount > 0) {
            currentDialogue.state = DIALOGUE_STATE_CHOOSING;
        } else {
            currentDialogue.state = DIALOGUE_STATE_WAITING;
        }
    }
}

//---------------------------------------------------------------------------------
// Navigate dialogue choices
void selectNextChoice(void) {
    if (currentDialogue.state == DIALOGUE_STATE_CHOOSING && currentDialogue.choiceCount > 1) {
        currentDialogue.selectedChoice++;
        if (currentDialogue.selectedChoice >= currentDialogue.choiceCount) {
            currentDialogue.selectedChoice = 0;
        }
    }
}

void selectPreviousChoice(void) {
    if (currentDialogue.state == DIALOGUE_STATE_CHOOSING && currentDialogue.choiceCount > 1) {
        if (currentDialogue.selectedChoice == 0) {
            currentDialogue.selectedChoice = currentDialogue.choiceCount - 1;
        } else {
            currentDialogue.selectedChoice--;
        }
    }
}

void confirmChoice(void) {
    if (currentDialogue.state == DIALOGUE_STATE_CHOOSING) {
        DialogueNode* currentNode = &dialogueNodes[currentDialogue.currentNodeId];
        u8 selectedChoice = currentDialogue.selectedChoice;

        if (selectedChoice < currentDialogue.choiceCount && currentNode->choices[selectedChoice].enabled) {
            u8 nextNodeId = currentNode->choices[selectedChoice].nextNodeId;

            if (nextNodeId == 0) {
                // End conversation
                endDialogue();
            } else {
                // Advance to next node
                currentDialogue.currentNodeId = nextNodeId;
                currentDialogue.textCharIndex = 0;
                currentDialogue.state = DIALOGUE_STATE_DISPLAYING;
                currentDialogue.selectedChoice = 0;

                // Update choice count for new node
                DialogueNode* nextNode = &dialogueNodes[nextNodeId];
                currentDialogue.choiceCount = nextNode->choiceCount;
            }
        }
    }
}

//---------------------------------------------------------------------------------
// Get current dialogue text (for testing)
const char* getCurrentDialogueText(void) {
    if (currentDialogue.state == DIALOGUE_STATE_INACTIVE) {
        return "";
    }
    return dialogueNodes[currentDialogue.currentNodeId].text;
}

//---------------------------------------------------------------------------------
// Get current speaker name
const char* getCurrentSpeaker(void) {
    if (currentDialogue.state == DIALOGUE_STATE_INACTIVE) {
        return "";
    }
    return dialogueNodes[currentDialogue.currentNodeId].speaker;
}

//---------------------------------------------------------------------------------
// Get number of available choices
u8 getChoiceCount(void) {
    return currentDialogue.choiceCount;
}

//---------------------------------------------------------------------------------
// Get choice text by index
const char* getChoiceText(u8 choiceIndex) {
    if (choiceIndex >= currentDialogue.choiceCount) {
        return "";
    }
    return dialogueNodes[currentDialogue.currentNodeId].choices[choiceIndex].text;
}

//---------------------------------------------------------------------------------
// Get currently selected choice
u8 getSelectedChoice(void) {
    return currentDialogue.selectedChoice;
}