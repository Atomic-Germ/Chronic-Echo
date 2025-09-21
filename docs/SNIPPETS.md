# SNES Development Snippets

This file documents reusable code patterns and techniques for SNES development in this project.

## Fade In/Out Effects

To create smooth fade in or fade out effects for screen transitions:

### Fade Out (Dimming)
```c
// Start with brightness = 15 (full brightness)
// In your game loop:
if (fadeFrameCount % 4 == 0 && brightness > 0) {
    brightness--;
    setBrightness(brightness);
}
fadeFrameCount++;
if (brightness <= 0) {
    // Transition to next screen
}
```

### Fade In (Brightening)
```c
// Start with brightness = 0 (screen off)
// In your game loop:
if (fadeFrameCount % 4 == 0 && brightness < 15) {
    brightness++;
    setBrightness(brightness);
}
fadeFrameCount++;
if (brightness >= 15) {
    // Fade in complete, start normal operation
}
```

### Usage Notes
- Use a frame counter to control the fade speed (every 4 frames = ~15 frames total for full fade)
- Call `setScreenOn()` when starting the fade to ensure the screen is active
- Clear screen content during transitions using `clearScreenForTransition()`
- Reset `fadeFrameCount = 0` when starting a new fade
- Set initial `brightness` appropriately (0 for fade in, 15 for fade out)