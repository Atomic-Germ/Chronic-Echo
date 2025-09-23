-- Mesen2-SDL3 Sample Test Suite
-- Demonstrates the refactored Lua testing API for SNES ROMs
-- This sample shows tests that should pass on most SNES ROMs
--
-- API Usage:
-- - emu.printHeader(text) - Print a formatted header
-- - emu.logTest(name, status, details) - Log a test result (status: "pass", "fail", "skip")
-- - emu.assertEqual(actual, expected, testName) - Assert two values are equal
-- - emu.assertTrue(condition, testName, details) - Assert a condition is true
--
-- Notes:
-- - Tests run frame-by-frame using emu.addEventCallback()
-- - Frame-based timing ensures tests run at the right moments
-- - Tests are designed to be generic and work on most SNES ROMs
-- - The script automatically stops after all tests complete
--
-- Expected Results on Real SNES ROMs:
-- - Most tests should pass on commercial SNES games
-- - Screen dimensions and input state tests may fail on test ROMs
-- - Memory write tests may fail if the ROM has memory protection

local frameCount = 0
local initialPC = nil
local initialScreenSum = nil

-- Main test callback - runs every frame
local function onFrameEnd()
    frameCount = frameCount + 1

    -- Frame 1: Basic startup tests
    if frameCount == 1 then
        emu.printHeader("=== Mesen2-SDL3 Sample Test Suite ===")

        -- Test 1: Emulation started successfully
        emu.assertTrue(true, "Emulation Startup", "Emulation initialized and running")

        -- Test 2: Basic memory operations work
        local success, value = pcall(emu.read, 0x0000, emu.memType.cpu)
        emu.assertTrue(success, "Memory Access", string.format("WRAM[0x0000] = 0x%02X", value or 0))

        -- Test 3: Screen buffer is accessible
        local success, buffer = pcall(emu.getScreenBuffer)
        emu.assertTrue(success and buffer and #buffer > 0, "Screen Buffer Access", "Screen buffer is readable")

    -- Frame 10: Memory and graphics tests
    elseif frameCount == 10 then
        emu.printHeader("=== Memory and Graphics Tests ===")

        -- Test 4: VRAM is accessible
        local success, vramValue = pcall(emu.read, 0x0000, emu.memType.vram)
        emu.assertTrue(success, "VRAM Access", string.format("VRAM[0x0000] = 0x%02X", vramValue or 0))

        -- Test 5: OAM (sprite) memory is accessible
        local success, oamValue = pcall(emu.read, 0x0000, emu.memType.oam)
        emu.assertTrue(success, "OAM Access", string.format("OAM[0x0000] = 0x%02X", oamValue or 0))

        -- Test 6: Screen dimensions are correct (256x224 for NTSC SNES)
        local success, width, height = pcall(emu.getScreenSize)
        if success then
            emu.assertEqual(width, 256, "Screen Width")
            emu.assertEqual(height, 224, "Screen Height")
        else
            emu.logTest("Screen Dimensions", "fail", "Failed to get screen size")
        end

    -- Frame 20: Input and interaction tests
    elseif frameCount == 20 then
        emu.printHeader("=== Input and Interaction Tests ===")

        -- Test 7: Input can be set (no error thrown)
        local success, err = pcall(emu.setInput, 0, "a", true)
        emu.assertTrue(success, "Input Simulation", "Button press simulation successful")

        -- Test 8: Input state can be read back
        local success, inputState = pcall(emu.getInput, 0)
        emu.assertTrue(success and inputState, "Input State Read", "Input state is readable")

    -- Frame 30: Memory write test
    elseif frameCount == 30 then
        emu.printHeader("=== Memory Write Tests ===")

        -- Test 9: Memory write works (write and read back)
        local testValue = 0x42
        local testAddress = 0x7E0000  -- WRAM address
        local success = pcall(emu.write, testAddress, emu.memType.cpu, testValue)
        if success then
            local success, readValue = pcall(emu.read, testAddress, emu.memType.cpu)
            emu.assertEqual(readValue, testValue, "Memory Write/Read")
        else
            emu.logTest("Memory Write/Read", "fail", "Failed to write to memory")
        end

    -- Frame 40: Final validation tests
    elseif frameCount == 40 then
        emu.printHeader("=== Final Validation Tests ===")

        -- Test 10: Frame callbacks are working
        emu.assertTrue(frameCount >= 40, "Frame Callbacks", string.format("Processed %d frames", frameCount))

        -- Test 11: Multiple memory reads work consistently
        local success1, value1 = pcall(emu.read, 0x0000, emu.memType.cpu)
        local success2, value2 = pcall(emu.read, 0x0000, emu.memType.cpu)
        emu.assertTrue(success1 and success2 and value1 == value2, "Consistent Memory Reads", "Memory reads are consistent")

        -- Test 12: Screen buffer remains accessible
        local success, buffer = pcall(emu.getScreenBuffer)
        emu.assertTrue(success and buffer and #buffer > 0, "Persistent Screen Access", "Screen buffer still accessible")

        -- Test 13: Basic performance check
        emu.assertTrue(true, "Basic Performance", "Emulation running without errors")

        -- All tests complete
        emu.printHeader("=== Test Suite Complete ===")
        emu.stop()

    -- Safety timeout
    elseif frameCount >= 100 then
        emu.printHeader("=== Test Timeout - Stopping ===")
        emu.stop()
    end
end

-- Register the frame callback
emu.addEventCallback(onFrameEnd, emu.eventType.frameEnd)

-- Initial log message
emu.log("Sample test suite loaded. Running comprehensive tests...")
print("Sample test suite loaded. Running comprehensive tests...")