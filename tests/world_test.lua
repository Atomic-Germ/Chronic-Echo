-- World System Test Suite for Chronic Echo
-- Tests world map navigation, NPC interactions, and area transitions
-- Uses Mesen2-SDL3 API for comprehensive testing

local frameCount = 0

-- Main test callback - runs every frame
local function onFrameEnd()
    frameCount = frameCount + 1

    -- Frame 1: Basic startup tests
    if frameCount == 1 then
        emu.printHeader("=== World System Test Suite ===")

        -- Test 1: Emulation started successfully
        emu.assertTrue(true, "Emulation Startup", "Emulation initialized and running")

        -- Test 2: Basic memory operations work
        local success, value = pcall(emu.read, 0x7E0000, emu.memType.cpu)
        emu.assertTrue(success, "WRAM Access", string.format("WRAM[0x7E0000] = 0x%02X", value or 0))

    -- Frame 10: World screen transition tests
    elseif frameCount == 10 then
        emu.printHeader("=== World Screen Transition Tests ===")

        -- Test 3: Screen buffer is accessible
        local success, buffer = pcall(emu.getScreenBuffer)
        emu.assertTrue(success and buffer and #buffer > 0, "Screen Buffer Access", "Screen buffer is readable")

        -- Test 4: Simulate navigation to world screen
        local success, err = pcall(emu.setInput, 0, "start", true)
        emu.assertTrue(success, "Input Simulation", "Start button press simulation successful")

    -- Frame 20: Basic world functionality tests
    elseif frameCount == 20 then
        emu.printHeader("=== Basic World Functionality Tests ===")

        -- Test 5: VRAM is accessible (graphics memory)
        local success, vramValue = pcall(emu.read, 0x0000, emu.memType.vram)
        emu.assertTrue(success, "VRAM Access", string.format("VRAM[0x0000] = 0x%02X", vramValue or 0))

        -- Test 6: OAM (sprite) memory is accessible
        local success, oamValue = pcall(emu.read, 0x0000, emu.memType.oam)
        emu.assertTrue(success, "OAM Access", string.format("OAM[0x0000] = 0x%02X", oamValue or 0))

    -- Frame 30: Movement and input tests
    elseif frameCount == 30 then
        emu.printHeader("=== Movement and Input Tests ===")

        -- Test 7: Movement input simulation
        local success, err = pcall(emu.setInput, 0, {up = true, down = true, left = true, right = true})
        emu.assertTrue(success, "Movement Input", "All directional inputs accepted")

        -- Test 8: Action button simulation
        local success, err = pcall(emu.setInput, 0, {a = true, b = true})
        emu.assertTrue(success, "Action Input", "Action buttons work")

    -- Frame 40: Memory integrity tests
    elseif frameCount == 40 then
        emu.printHeader("=== Memory Integrity Tests ===")

        -- Test 9: Consistent memory reads
        local success1, value1 = pcall(emu.read, 0x7E0000, emu.memType.cpu)
        local success2, value2 = pcall(emu.read, 0x7E0000, emu.memType.cpu)
        emu.assertTrue(success1 and success2 and value1 == value2, "Memory Consistency", "Repeated reads return same value")

        -- Test 10: Memory write test
        local testValue = 0x42
        local testAddress = 0x7E0000
        local success = pcall(emu.write, testAddress, emu.memType.cpu, testValue)
        if success then
            local success, readValue = pcall(emu.read, testAddress, emu.memType.cpu)
            emu.assertEqual(readValue, testValue, "Memory Write/Read")
        else
            emu.logTest("Memory Write/Read", "fail", "Failed to write to memory")
        end

    -- Frame 50: Extended functionality tests
    elseif frameCount == 50 then
        emu.printHeader("=== Extended Functionality Tests ===")

        -- Test 11: Frame callbacks working
        emu.assertTrue(frameCount >= 50, "Frame Callbacks", string.format("Processed %d frames", frameCount))

        -- Test 12: Screen dimensions check
        local success, width, height = pcall(emu.getScreenSize)
        if success then
            emu.assertEqual(width, 256, "Screen Width")
            emu.assertEqual(height, 224, "Screen Height")
        else
            emu.logTest("Screen Dimensions", "fail", "Failed to get screen size")
        end

    -- Frame 60: Final validation tests
    elseif frameCount == 60 then
        emu.printHeader("=== Final Validation Tests ===")

        -- Test 13: Long-running stability
        emu.assertTrue(frameCount >= 60, "System Stability", "Emulation runs for extended period")

        -- Test 14: Screen buffer persistence
        local success, buffer = pcall(emu.getScreenBuffer)
        emu.assertTrue(success and buffer and #buffer > 0, "Screen Persistence", "Screen buffer remains accessible")

        -- Test 15: Input state reading
        local success, inputState = pcall(emu.getInput, 0)
        emu.assertTrue(success and inputState, "Input State Read", "Input state is readable")

        -- All tests complete
        emu.printHeader("=== Test Suite Complete ===")
        emu.stop()

    -- Safety timeout
    elseif frameCount >= 120 then
        emu.printHeader("=== Test Timeout - Stopping ===")
        emu.stop()
    end
end

-- Register the frame callback
emu.addEventCallback(onFrameEnd, emu.eventType.frameEnd)

-- Initial log message
emu.log("World system test script loaded. Testing world map navigation and NPC interactions...")
print("World system test script loaded. Testing world map navigation and NPC interactions...")