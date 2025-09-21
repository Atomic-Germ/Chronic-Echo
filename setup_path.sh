#!/bin/bash
# Setup script for PVSnesLib development environment
# This script adds PVSnesLib tools to your PATH

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PVSNESLIB_BIN="$PROJECT_DIR/pvsneslib/devkitsnes/bin"
PVSNESLIB_TOOLS="$PROJECT_DIR/pvsneslib/devkitsnes/tools"

# Check if dependencies are installed
if [ ! -d "$PROJECT_DIR/pvsneslib" ]; then
    echo "Error: PVSnesLib not found. Run 'make deps' first."
    exit 1
fi

# Add to PATH for current session
export PATH="$PVSNESLIB_BIN:$PVSNESLIB_TOOLS:$PATH"

echo "PVSnesLib tools added to PATH for this session."
echo "To make this permanent, add the following to your ~/.zshrc or ~/.bash_profile:"
echo "export PATH=\"$PVSNESLIB_BIN:\$PVSNESLIB_TOOLS:\$PATH\""

# Test that tools are accessible
echo "Testing tool accessibility..."
if command -v wla-65816 >/dev/null 2>&1; then
    echo "✓ wla-65816 found: $(which wla-65816)"
else
    echo "✗ wla-65816 not found in PATH"
fi

if command -v wlalink >/dev/null 2>&1; then
    echo "✓ wlalink found: $(which wlalink)"
else
    echo "✗ wlalink not found in PATH"
fi

if command -v gfx4snes >/dev/null 2>&1; then
    echo "✓ gfx4snes found: $(which gfx4snes)"
else
    echo "✗ gfx4snes not found in PATH"
fi