#!/usr/bin/env node
/**
 * Memory leak test script for Valgrind
 *
 * This script exercises the native bindings repeatedly to detect memory leaks.
 * Run with: valgrind --leak-check=full --suppressions=valgrind/node.supp node --expose-gc valgrind/memcheck.js
 */

const path = require('path');

// Load the native bindings
let bindings;
try {
    bindings = require(path.join(__dirname, '..', 'out', 'index.js'));
} catch (e) {
    console.error('Failed to load bindings. Make sure the project is built.');
    console.error('Run: npm run install');
    process.exit(1);
}

const { XdoToolBindings, XdoToolAsync, KeyboardAsync, ScreenshooterAsync } = bindings;

const ITERATIONS = parseInt(process.env.MEMCHECK_ITERATIONS, 10) || 50;
const VERBOSE = process.env.MEMCHECK_VERBOSE === '1';

function log(msg) {
    if (VERBOSE) {
        console.log(`[memcheck] ${msg}`);
    }
}

function forceGC() {
    if (global.gc) {
        global.gc();
        log('Forced garbage collection');
    }
}

async function testXdoToolLifecycle() {
    log('Testing XdoTool lifecycle...');
    for (let i = 0; i < ITERATIONS; i++) {
        const xdoBindings = new XdoToolBindings();
        const xdo = new XdoToolAsync(xdoBindings);

        // Exercise various methods
        await xdo.searchWindows({ winname: '' });
        await xdo.getMouseLocation();

        if (i % 10 === 0) {
            forceGC();
            log(`  XdoTool iteration ${i}/${ITERATIONS}`);
        }
    }
    forceGC();
    log('XdoTool lifecycle test complete');
}

async function testSearchWindows() {
    log('Testing searchWindows with various parameters...');
    const xdoBindings = new XdoToolBindings();
    const xdo = new XdoToolAsync(xdoBindings);

    const searchParams = [
        { winname: '' },
        { winclass: '' },
        { winclassname: '' },
        { title: 'test' },
        { pid: 1 },
        { onlyVisible: true },
        { maxDepth: 5 },
        { limit: 10 },
        { winname: '', winclass: '', onlyVisible: true },
    ];

    for (let i = 0; i < ITERATIONS; i++) {
        for (const params of searchParams) {
            try {
                await xdo.searchWindows(params);
            } catch (e) {
                // Some searches may fail, that's OK for memory testing
            }
        }

        if (i % 10 === 0) {
            forceGC();
            log(`  searchWindows iteration ${i}/${ITERATIONS}`);
        }
    }
    forceGC();
    log('searchWindows test complete');
}

async function testKeyboard() {
    log('Testing Keyboard...');
    const xdoBindings = new XdoToolBindings();

    for (let i = 0; i < ITERATIONS; i++) {
        const keyboard = new KeyboardAsync(xdoBindings);

        try {
            const keymap = await keyboard.queryKeymap();
            // Verify we got a valid buffer
            if (keymap.byteLength !== 32) {
                console.error('Invalid keymap buffer size');
            }
        } catch (e) {
            // May fail if no X display, continue anyway
        }

        if (i % 10 === 0) {
            forceGC();
            log(`  Keyboard iteration ${i}/${ITERATIONS}`);
        }
    }
    forceGC();
    log('Keyboard test complete');
}

async function testScreenshooter() {
    log('Testing Screenshooter...');
    const xdoBindings = new XdoToolBindings();

    for (let i = 0; i < ITERATIONS; i++) {
        try {
            const screenshooter = new ScreenshooterAsync(xdoBindings);
            const buffer = await screenshooter.getImage();

            // Access buffer to ensure it's valid
            if (buffer.byteLength === 0) {
                log('  Warning: Empty screenshot buffer');
            }
        } catch (e) {
            // May fail if no X display
        }

        if (i % 10 === 0) {
            forceGC();
            log(`  Screenshooter iteration ${i}/${ITERATIONS}`);
        }
    }
    forceGC();
    log('Screenshooter test complete');
}

async function testWindowOperations() {
    log('Testing window operations...');
    const xdoBindings = new XdoToolBindings();
    const xdo = new XdoToolAsync(xdoBindings);

    for (let i = 0; i < ITERATIONS; i++) {
        try {
            const windows = await xdo.searchWindows({ winname: '' });

            for (const window of windows.slice(0, 3)) {
                try {
                    await xdo.getWindowPID(window);
                    await xdo.getWindowSize(window);
                    await xdo.windowHasProperty(window, 'WM_NAME');
                } catch (e) {
                    // Window may have been destroyed
                }
            }
        } catch (e) {
            // Continue on error
        }

        if (i % 10 === 0) {
            forceGC();
            log(`  Window operations iteration ${i}/${ITERATIONS}`);
        }
    }
    forceGC();
    log('Window operations test complete');
}

async function testRapidAllocation() {
    log('Testing rapid allocation/deallocation...');

    for (let i = 0; i < ITERATIONS * 2; i++) {
        // Create and immediately discard
        const xdoBindings = new XdoToolBindings();
        new XdoToolAsync(xdoBindings);
        new KeyboardAsync(xdoBindings);

        if (i % 20 === 0) {
            forceGC();
            log(`  Rapid allocation iteration ${i}/${ITERATIONS * 2}`);
        }
    }
    forceGC();
    log('Rapid allocation test complete');
}

async function main() {
    console.log('=== xdotool Valgrind Memory Check ===');
    console.log(`Iterations per test: ${ITERATIONS}`);
    console.log(`Verbose: ${VERBOSE}`);
    console.log(`GC available: ${typeof global.gc === 'function'}`);
    console.log('');

    if (typeof global.gc !== 'function') {
        console.warn('WARNING: Run with --expose-gc for accurate leak detection');
        console.warn('Example: node --expose-gc valgrind/memcheck.js');
        console.warn('');
    }

    const startTime = Date.now();

    try {
        await testXdoToolLifecycle();
        await testSearchWindows();
        await testKeyboard();
        await testScreenshooter();
        await testWindowOperations();
        await testRapidAllocation();
    } catch (e) {
        console.error('Test failed with error:', e);
        process.exit(1);
    }

    // Final GC before exit
    forceGC();
    forceGC(); // Run twice to catch weak refs

    const elapsed = ((Date.now() - startTime) / 1000).toFixed(2);
    console.log('');
    console.log(`=== All tests completed in ${elapsed}s ===`);
    console.log('Check Valgrind output for memory leak summary.');
}

main()
    .then(() => {
        // Small delay to allow cleanup
        setTimeout(() => process.exit(0), 100);
    })
    .catch((e) => {
        console.error('Fatal error:', e);
        process.exit(1);
    });
