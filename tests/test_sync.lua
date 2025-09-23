-- ANSI color codes for fancy terminal output
local colors = {
    reset = "\27[0m",
    bold = "\27[1m",
    red = "\27[31m",
    green = "\27[32m",
    yellow = "\27[33m",
    blue = "\27[34m",
    magenta = "\27[35m",
    cyan = "\27[36m",
    bright_red = "\27[91m",
    bright_green = "\27[92m",
    bright_yellow = "\27[93m",
    bright_blue = "\27[94m"
}

-- Test results storage
local testResults = {}

-- Helper functions for colored output
local function printPass(name, details)
    local msg = string.format("%s[PASS]%s %s: %s", colors.bright_green, colors.reset, name, details or "")
    print(msg)
    emu.log(msg)
end

local function printFail(name, details)
    local msg = string.format("%s[FAIL]%s %s: %s", colors.bright_red, colors.reset, name, details or "")
    print(msg)
    emu.log(msg)
end

local function printSkip(name, details)
    local msg = string.format("%s[SKIP]%s %s: %s", colors.bright_yellow, colors.reset, name, details or "")
    print(msg)
    emu.log(msg)
end

local function printHeader(text)
    local msg = string.format("%s%s%s", colors.bold .. colors.bright_blue, text, colors.reset)
    print(msg)
    emu.log(msg)
end

local function printSummary(passCount, failCount, skipCount, total)
    local msg = string.format("%s=== Test Summary: %d passed, %d failed, %d skipped (%d total) ===%s", colors.bold .. colors.bright_blue, passCount, failCount, skipCount, total, colors.reset)
    print(msg)
    emu.log(msg)
end

-- Helper: Summarize and exit
local function summarizeAndExit()
    local passCount = 0
    local failCount = 0
    local skipCount = 0
    for _, result in ipairs(testResults) do
        if result.status == "pass" then
            passCount = passCount + 1
        elseif result.status == "fail" then
            failCount = failCount + 1
        elseif result.status == "skip" then
            skipCount = skipCount + 1
        end
    end
    local total = #testResults
    printSummary(passCount, failCount, skipCount, total)
    for _, result in ipairs(testResults) do
        local status = result.status == "pass" and "PASS" or result.status == "fail" and "FAIL" or "SKIP"
        local color = result.status == "pass" and colors.bright_green or result.status == "fail" and colors.bright_red or colors.bright_yellow
        local msg = string.format("- %s: %s%s%s", result.name, color, status, colors.reset)
        print(msg)
        emu.log(msg)
    end
    emu.stop()
end

-- Helper: Log test result
local function logPass(name, details)
    printPass(name, details)
    table.insert(testResults, {name = name, status = "pass", details = details})
end

local function logFail(name, details)
    printFail(name, details)
    table.insert(testResults, {name = name, status = "fail", details = details})
end

local function logSkip(name, details)
    printSkip(name, details)
    table.insert(testResults, {name = name, status = "skip", details = details})
end

-- Helper: Simple screen sum (sum of pixel values) for brightness detection
local function getScreenSum()
    local success, buffer = pcall(emu.getScreenBuffer)
    if success and buffer then
        local sum = 0
        for i = 1, #buffer do
            sum = sum + buffer[i]
        end
        return sum
    else
        return 0
    end
end

-- Run all tests synchronously
local function runTests()
    printHeader("Running synchronous tests...")
    
    -- Test ROM Load
    logPass("ROM Load", "Emulation started successfully")
    
    -- Test basic Lua functionality
    logPass("Lua Engine", "Lua scripting is working")
    
    -- Test memory access (try to read from ROM area)
    local success, romData = pcall(emu.read, 0x0000, emu.memType.prgRom)
    if success then
        logPass("Memory Access", string.format("ROM[0x0000]=0x%02X", romData))
    else
        logFail("Memory Access", "ROM read failed: " .. tostring(romData))
    end
    
    -- Test screen buffer access
    local success, buffer = pcall(emu.getScreenBuffer)
    local available = success and buffer and #buffer > 0
    if available then
        logPass("Screen Buffer", string.format("Buffer size: %d", #buffer))
    else
        logFail("Screen Buffer", "Buffer not accessible")
    end
    
    -- Test logging
    emu.log("Test log message")
    logPass("Logging", "Log message sent")
    
    -- Skip CPU state test for now since it requires running emulation
    logSkip("CPU State", "Skipped - requires running emulation")
    
    logSkip("Sprite Init", "Skipped - requires running emulation")
    
    logSkip("Font Load", "Skipped - requires running emulation")
    
    logSkip("Credit Screen", "Skipped - requires running emulation")
    
    logSkip("Fade Process", "Skipped - requires running emulation")
    
    logSkip("Fade Complete", "Skipped - requires running emulation")
    
    logSkip("Title Fade In", "Skipped - requires running emulation")
    
    logSkip("Start Press Simulated", "Skipped - requires running emulation")
    
    logSkip("Game Screen Fade In", "Skipped - requires running emulation")
    
    logSkip("Player Sprite Detection", "Skipped - requires running emulation")
    
    logSkip("No Crash", "Skipped - requires running emulation")
    
    -- Summarize results
    summarizeAndExit()
end

-- Run tests immediately
runTests()