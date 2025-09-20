#!/usr/bin/env bash

# Chronic Echos Build Script
# This script sets up the environment and builds the SNES ROM

set -e  # Exit on any error

echo "Setting up pvsneslib environment..."
export PVSNESLIB_HOME="$(pwd)/pvsneslib"
export PATH="$PATH:$PVSNESLIB_HOME/devkitsnes/tools:$PVSNESLIB_HOME/devkitsnes/bin"

echo "Building Chronic Echos ROM..."
make clean
make

echo "Build complete! ROM: ChronicEchos.sfc"
echo "To test: open -a Snes9x ChronicEchos.sfc"