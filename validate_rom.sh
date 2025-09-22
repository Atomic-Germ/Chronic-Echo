#!/bin/bash

# Chronic Echo SNES ROM Validation Script
# Verifies that the built ROM meets basic requirements

ROM_FILE="$1"
if [ -z "$ROM_FILE" ]; then
    echo "Usage: $0 <rom_file>"
    echo "Example: $0 build/ChronicEchos.sfc"
    exit 1
fi

if [ ! -f "$ROM_FILE" ]; then
    echo "Error: ROM file '$ROM_FILE' not found"
    exit 1
fi

echo "Validating $ROM_FILE..."

# Check file size (should be around 262KB for LoROM)
SIZE=$(stat -f%z "$ROM_FILE")
SIZE_KB=$((SIZE / 1024))

if [ $SIZE_KB -lt 200 ] || [ $SIZE_KB -gt 300 ]; then
    echo "Warning: ROM size ($SIZE_KB KB) seems unusual. Expected ~262KB"
else
    echo "✓ ROM size: $SIZE_KB KB (within expected range)"
fi

# Check if it's a valid SNES ROM by looking for header
# SNES ROMs should start with specific bytes
HEADER_BYTES=$(dd if="$ROM_FILE" bs=1 count=4 2>/dev/null | od -t x1 -An | tr -d ' ')
if [ "$HEADER_BYTES" = "78 9c 00 00" ]; then
    echo "✓ Valid SNES ROM header detected"
else
    echo "Warning: ROM header doesn't match expected SNES format"
fi

# Check for non-zero content (not all zeros)
NON_ZERO=$(dd if="$ROM_FILE" bs=1 count=1024 2>/dev/null | tr -d '\0' | wc -c)
if [ $NON_ZERO -gt 0 ]; then
    echo "✓ ROM contains non-zero data"
else
    echo "Error: ROM appears to be all zeros"
    exit 1
fi

# Check for expected strings in ROM (basic content validation)
STRINGS_FOUND=$(strings "$ROM_FILE" | grep -c "CHRONIC" || true)
if [ $STRINGS_FOUND -gt 0 ]; then
    echo "✓ Found expected game strings in ROM"
else
    echo "Warning: Expected game strings not found in ROM"
fi

echo "Validation complete!"
echo "ROM appears to be valid and ready for testing."