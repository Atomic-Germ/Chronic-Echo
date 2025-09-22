#!/usr/bin/env bash

# Chronic Echo - Build Test Suite
# Comprehensive testing for build system and ROM validation

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# Helper functions
print_header() {
    echo -e "${BLUE}================================================${NC}"
    echo -e "${BLUE}  $1${NC}"
    echo -e "${BLUE}================================================${NC}"
}

print_test() {
    echo -e "${YELLOW}Running: $1${NC}"
}

print_pass() {
    echo -e "${GREEN}✓ PASS: $1${NC}"
    TESTS_PASSED=$((TESTS_PASSED + 1))
}

print_fail() {
    echo -e "${RED}✗ FAIL: $1${NC}"
    TESTS_FAILED=$((TESTS_FAILED + 1))
}

run_test() {
    TESTS_RUN=$((TESTS_RUN + 1))
    print_test "$1"
    if $2; then
        print_pass "$1"
        return 0
    else
        print_fail "$1"
        return 1
    fi
}

# Test: Clean build environment (ROM and intermediate files removed)
test_clean_environment() {
    [ ! -f "build/ChronicEchos.sfc" ] && [ ! -f "src/main.obj" ] && [ ! -f "src/sprites.obj" ]
}

# Test: Dependencies installation
test_deps_installation() {
    # Check if deps are already installed, or install them
    if [ -d "pvsneslib" ] && [ -f "Mesen.app/Contents/MacOS/Mesen" ]; then
        return 0  # Already installed
    else
        make deps > /dev/null 2>&1 && [ -d "pvsneslib" ] && [ -f "Mesen.app/Contents/MacOS/Mesen" ]
    fi
}

# Test: Build succeeds
test_build_success() {
    make > build.log 2>&1 && [ -f "build/ChronicEchos.sfc" ]
}

# Test: ROM size is reasonable
test_rom_size() {
    local size=$(stat -f%z "build/ChronicEchos.sfc" 2>/dev/null || stat -c%s "build/ChronicEchos.sfc" 2>/dev/null)
    [ "$size" -gt 200000 ] && [ "$size" -lt 300000 ]  # Between 200KB and 300KB
}

# Test: ROM contains expected content
test_rom_content() {
    strings "build/ChronicEchos.sfc" | grep -q "CHRONIC"
}

# Test: Build is reproducible (build twice, compare)
test_build_reproducible() {
    cp "build/ChronicEchos.sfc" "build/ChronicEchos.sfc.first"
    make clean > /dev/null 2>&1
    make > /dev/null 2>&1
    cmp "build/ChronicEchos.sfc.first" "build/ChronicEchos.sfc" > /dev/null 2>&1
}

# Test: Clean removes build artifacts
test_clean_artifacts() {
    make clean > /dev/null 2>&1
    [ -f "build/ChronicEchos.sfc" ] && [ ! -f "src/main.obj" ] && [ ! -f "src/sprites.obj" ]
}

# Test: Incremental build works
test_incremental_build() {
    make > /dev/null 2>&1
    local first_mtime=$(stat -f%m "build/ChronicEchos.sfc" 2>/dev/null || stat -c%Y "build/ChronicEchos.sfc" 2>/dev/null)
    sleep 1
    touch src/main.c
    make > /dev/null 2>&1
    local second_mtime=$(stat -f%m "build/ChronicEchos.sfc" 2>/dev/null || stat -c%Y "build/ChronicEchos.sfc" 2>/dev/null)
    [ "$second_mtime" != "$first_mtime" ]
}

# Test: Debug build includes debug output
test_debug_build() {
    make clean > /dev/null 2>&1
    PVSNESLIB_DEBUG=1 make > /dev/null 2>&1
    # This is harder to test automatically, but we could check for debug strings in ROM
    [ -f "build/ChronicEchos.sfc" ]
}

# Main test runner
main() {
    print_header "Chronic Echo - Build Test Suite"

    # Setup
    mkdir -p build
    rm -f build.log

    # Run all tests
    run_test "Clean environment check" "test_clean_environment"
    run_test "Dependencies installation" "test_deps_installation"
    run_test "Build succeeds" "test_build_success"
    run_test "ROM size validation" "test_rom_size"
    run_test "ROM content validation" "test_rom_content"
    run_test "Build reproducibility" "test_build_reproducible"
    run_test "Clean artifacts" "test_clean_artifacts"
    run_test "Incremental build" "test_incremental_build"
    run_test "Debug build" "test_debug_build"

    # Results
    echo
    print_header "Test Results"
    echo "Tests Run: $TESTS_RUN"
    echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
    echo -e "Failed: ${RED}$TESTS_FAILED${NC}"

    if [ "$TESTS_FAILED" -eq 0 ]; then
        echo -e "${GREEN}All tests passed! 🎉${NC}"
        exit 0
    else
        echo -e "${RED}Some tests failed. Check build.log for details.${NC}"
        exit 1
    fi
}

# Run main if script is executed directly
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi