#!/bin/bash
# Basic ROM validation script for CI/testing
# This can be expanded for more comprehensive testing

ROM_FILE="$1"
if [ -z "$ROM_FILE" ]; then
    echo "Usage: $0 <rom_file>"
    exit 1
fi

echo "🔍 Validating ROM: $ROM_FILE"

# Check if file exists
if [ ! -f "$ROM_FILE" ]; then
    echo "❌ ROM file not found: $ROM_FILE"
    exit 1
fi

# Check file size (SNES ROMs are typically 256KB, 512KB, 1MB, etc.)
SIZE=$(stat -c%s "$ROM_FILE" 2>/dev/null || stat -f%z "$ROM_FILE" 2>/dev/null)
if [ -z "$SIZE" ]; then
    echo "❌ Could not determine file size"
    exit 1
fi

echo "📊 ROM size: $SIZE bytes"

# Basic size validation (should be at least 32KB, max 4MB for SNES)
if [ $SIZE -lt 32768 ]; then
    echo "❌ ROM too small (minimum 32KB expected)"
    exit 1
fi

if [ $SIZE -gt 4194304 ]; then
    echo "❌ ROM too large (maximum 4MB expected)"
    exit 1
fi

# Check for non-zero content (basic corruption check)
# For SNES ROMs, check a few different regions since they often have zeros at the start
TOTAL_BYTES=$(dd if="$ROM_FILE" bs=1 count=65536 2>/dev/null | wc -c)
ZERO_BYTES=$(dd if="$ROM_FILE" bs=1 count=65536 2>/dev/null | tr -d '\0' | wc -c)
NON_ZERO_RATIO=$(( (TOTAL_BYTES - ZERO_BYTES) * 100 / TOTAL_BYTES ))

if [ $NON_ZERO_RATIO -lt 5 ]; then
    echo "❌ ROM appears to be mostly empty or corrupted (only ${NON_ZERO_RATIO}% non-zero)"
    exit 1
fi

echo "📊 ROM content density: ${NON_ZERO_RATIO}% non-zero bytes"

# Try to detect SNES ROM header (basic format check)
HEADER_OFFSET=65472  # SNES header location
if [ $SIZE -gt 65536 ]; then
    HEADER_INFO=$(dd if="$ROM_FILE" bs=1 skip=$HEADER_OFFSET count=32 2>/dev/null | strings 2>/dev/null || echo "")
    if [ -n "$HEADER_INFO" ]; then
        echo "📝 ROM header detected: $HEADER_INFO"
    fi
fi

echo "✅ ROM validation passed!"
exit 0