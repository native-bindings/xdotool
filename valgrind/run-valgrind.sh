#!/bin/bash
#
# Run Valgrind memory check on xdotool native bindings
#
# Usage:
#   ./valgrind/run-valgrind.sh [options]
#
# Options:
#   --full          Run full leak check (default)
#   --quick         Run quick check with fewer iterations
#   --verbose       Show detailed test progress
#   --gen-supp      Generate suppressions for new false positives
#   --track-origins Track origin of uninitialized values
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Default options
LEAK_CHECK="full"
ITERATIONS=50
VERBOSE=0
GEN_SUPP=0
TRACK_ORIGINS=0
EXTRA_ARGS=""

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --full)
            LEAK_CHECK="full"
            ITERATIONS=50
            shift
            ;;
        --quick)
            LEAK_CHECK="summary"
            ITERATIONS=10
            shift
            ;;
        --verbose)
            VERBOSE=1
            shift
            ;;
        --gen-supp)
            GEN_SUPP=1
            shift
            ;;
        --track-origins)
            TRACK_ORIGINS=1
            shift
            ;;
        *)
            EXTRA_ARGS="$EXTRA_ARGS $1"
            shift
            ;;
    esac
done

# Check for valgrind
if ! command -v valgrind &> /dev/null; then
    echo "Error: valgrind is not installed"
    echo "Install with: sudo apt-get install valgrind"
    exit 1
fi

# Check for node
if ! command -v node &> /dev/null; then
    echo "Error: node is not installed"
    exit 1
fi

# Check if built
if [[ ! -f "$PROJECT_DIR/out/index.js" ]]; then
    echo "Error: Project not built. Run 'npm run install' first."
    exit 1
fi

# Build valgrind command
VALGRIND_OPTS=(
    "--leak-check=$LEAK_CHECK"
    "--show-leak-kinds=definite,possible"
    "--errors-for-leak-kinds=definite"
    "--error-exitcode=1"
    "--suppressions=$SCRIPT_DIR/node.supp"
)

if [[ $GEN_SUPP -eq 1 ]]; then
    VALGRIND_OPTS+=("--gen-suppressions=all")
fi

if [[ $TRACK_ORIGINS -eq 1 ]]; then
    VALGRIND_OPTS+=("--track-origins=yes")
fi

# Environment variables for test script
export MEMCHECK_ITERATIONS=$ITERATIONS
export MEMCHECK_VERBOSE=$VERBOSE

# Output file for results
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
LOG_FILE="$PROJECT_DIR/valgrind/results_$TIMESTAMP.log"

echo "=== xdotool Valgrind Memory Check ==="
echo "Leak check level: $LEAK_CHECK"
echo "Iterations: $ITERATIONS"
echo "Log file: $LOG_FILE"
echo ""

# Run valgrind
valgrind "${VALGRIND_OPTS[@]}" \
    node --expose-gc "$SCRIPT_DIR/memcheck.js" $EXTRA_ARGS \
    2>&1 | tee "$LOG_FILE"

EXIT_CODE=${PIPESTATUS[0]}

echo ""
echo "Results saved to: $LOG_FILE"

if [[ $EXIT_CODE -eq 0 ]]; then
    echo "✓ No definite memory leaks detected"
else
    echo "✗ Memory leaks detected (exit code: $EXIT_CODE)"
fi

exit $EXIT_CODE
