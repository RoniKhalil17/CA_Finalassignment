#!/bin/bash

passed=0
failed=0
skipped=0

for test in tests/task*/*.bin; do
    [ -f "$test" ] || continue

    reference="${test%.bin}.res"

    if [ ! -f "$reference" ]; then
        echo "SKIP: $test"
        ((skipped+=1))
        continue
    fi

    # Run your simulator
    if ! ./simulator "$test" output.bin > /dev/null; then
        echo "FAIL: $test (simulator error)"
        ((failed+=1))
        continue
    fi

    # Compare register output with reference
    if cmp -s output.bin "$reference"; then
        echo "PASS: $test"
        ((passed+=1))
    else
        echo "FAIL: $test (register mismatch)"
        ((failed+=1))
    fi
done

echo ""
echo "Passed: $passed"
echo "Failed: $failed"
echo "Skipped: $skipped"

[ "$failed" -eq 0 ] && [ "$passed" -gt 0 ]