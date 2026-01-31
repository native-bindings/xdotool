# CLAUDE.md - xdotool

This document provides guidance for AI assistants working with this codebase.

## Project Overview

**xdotool** is a Node.js native addon that provides C++ bindings to libxdo for X11 automation. It enables programmatic control of mouse, keyboard, windows, and screenshot capture on Linux X11 systems.

**Key Technologies:**
- C++20 with NAN (Native Abstractions for Node.js)
- TypeScript wrapper for async/Promise-based API
- cmake-js for native module compilation
- libxdo (xdotool library) for X11 interaction

## Codebase Structure

```
xdotool/
├── src/                      # C++ native module source
│   ├── main.cpp              # Module entry point, exports XdoTool, Screenshooter, Keyboard
│   ├── XdoTool.cpp/h         # Main xdotool bindings class
│   ├── XService.cpp/h        # Base class for X11 services (ObjectWrap + ResourceManager)
│   ├── XTask.cpp/h           # Base class for async tasks
│   ├── XdoToolTask.cpp/h     # xdo-specific task base class
│   ├── XdoToolTaskWorker.cpp/h # NAN AsyncWorker for task execution
│   ├── Screenshooter.cpp/h   # Screenshot capture functionality
│   ├── Keyboard.cpp/h        # Keyboard input monitoring
│   ├── ResourceManager.cpp/h # Memory management for native resources
│   ├── TypeConverter.cpp/h   # V8 <-> C++ type conversion utilities
│   ├── ClassCreator.h        # Helper for creating NAN classes
│   ├── xdo_c.h               # libxdo C header wrapper
│   └── tasks/                # Individual async task implementations
│       ├── ActivateWindow.cpp/h
│       ├── EnterText.cpp/h
│       ├── GetFocusedWindow.cpp/h
│       ├── GetImage.cpp/h
│       ├── GetMouseLocation.cpp/h
│       ├── GetViewportDimensions.cpp/h
│       ├── GetWindowPID.cpp/h
│       ├── GetWindowProperty.cpp/h
│       ├── GetWindowSize.cpp/h
│       ├── MoveMouse.cpp/h
│       ├── QueryKeymap.cpp/h
│       ├── SearchWindows.cpp/h
│       ├── SendKeysequence.cpp/h
│       ├── Sync.cpp/h
│       └── WindowHasProperty.cpp/h
├── typescript/               # TypeScript async wrappers
│   ├── index.ts              # Public exports
│   ├── xdo.ts                # XdoToolAsync class and type definitions
│   ├── AsyncWrapper.ts       # Base class for Promise wrappers
│   ├── Keyboard.ts           # KeyboardAsync wrapper
│   ├── Screenshooter.ts      # ScreenshooterAsync wrapper
│   └── tsconfig.json
├── types/app/                # Custom type declarations
│   └── index.d.ts            # bindings module declaration
├── test/                     # Test files (AVA framework)
│   ├── test.ts               # Main test suite
│   └── tsconfig.json
├── CMakeLists.txt            # CMake build configuration
├── Makefile                  # Development convenience targets
├── package.json              # npm package configuration
├── tsconfig.base.json        # Base TypeScript configuration
└── .nvmrc                    # Node.js version (v20.19.6)
```

## Architecture

### Three-Layer Design

1. **C++ Native Layer** (`src/`): Direct libxdo bindings using NAN
   - `XService`: Base class wrapping Display connection
   - `XdoTool`: Main class with all xdotool operations
   - `XScreenshooter`: Screenshot capture
   - `XKeyboard`: Keyboard state monitoring

2. **Async Task System** (`src/tasks/`):
   - Each operation is a separate task class inheriting from `XTask` or `XdoToolTask`
   - Tasks execute in `XdoToolTaskWorker` (NAN AsyncWorker) to avoid blocking Node.js event loop
   - Tasks implement `Execute()` for work and `GetResult()` for V8 value creation

3. **TypeScript Wrapper** (`typescript/`):
   - `AsyncWrapper`: Base class providing `resolveOrReject` helper
   - Wraps callback-based native API into Promise-based async API

### Class Hierarchy

```
Nan::ObjectWrap + ResourceManager
        └── XService (Display connection)
              ├── XdoTool (main automation)
              ├── XScreenshooter (screenshots)
              └── XKeyboard (keyboard monitoring)

XTask (async task base)
    └── XdoToolTask (xdo-specific tasks)
          └── Individual task classes
```

## Build Commands

```bash
# Install dependencies and build native module
npm install

# The install script runs:
# cmake-js -B Release --prefer-clang --out build/cmake-js compile

# Build TypeScript only
npx tsc -b typescript

# Configure debug build (for IDE/debugging)
make configure

# Create compile_commands.json symlink
make create_compile_commands_link
```

## Development Workflow

### Adding a New Task/Operation

1. Create task class in `src/tasks/`:
   ```cpp
   // src/tasks/MyNewTask.h
   #include "../XdoToolTask.h"

   class MyNewTask : public XdoToolTask {
   public:
       explicit MyNewTask(xdo_t* xdo, /* params */);
       void Execute() override;
       v8::Local<v8::Value> GetResult() override;
   private:
       // result storage
   };
   ```

2. Add to `CMakeLists.txt`:
   ```cmake
   set(NODE_XDOTOOL_SOURCE_FILES ...
       src/tasks/MyNewTask.cpp
       ...)
   ```

3. Add NAN_METHOD in `XdoTool.cpp`:
   - Include header
   - Add method implementation
   - Register in `Init()` methods map

4. Add TypeScript types and async wrapper in `typescript/xdo.ts`

### Working with X11

- All X11 operations must be thread-safe (module calls `XInitThreads()` at startup)
- Use `TypeConverter` for V8 <-> C++ conversions
- Window IDs are passed as strings in JavaScript, converted via `TypeConverter::GetWindow()`

## Testing

```bash
# Run tests (requires X11 display)
npx ava test/test.ts

# Tests use tsx for TypeScript execution
# Configured in package.json "ava" section
```

Tests require a running X11 server with a red background for screenshot tests.

## Code Style and Conventions

### C++
- C++20 standard
- 4-space indentation
- NAN macros for V8 integration (`NAN_METHOD`, `NAN_MODULE_INIT`)
- Error handling via `Nan::ThrowError()`
- Memory managed through `ResourceManager::AddResource()`

### TypeScript
- Strict mode enabled
- 4-space indentation
- ES2019 target, CommonJS modules
- All async operations return Promises
- Callback pattern: `(err: Error | undefined, value: T) => void`

### File Naming
- C++: PascalCase for classes (e.g., `XdoTool.cpp`, `SearchWindows.cpp`)
- TypeScript: PascalCase for files matching class names

## Key Types

```typescript
// Window identifier (opaque, passed as string)
type XID = unknown;

// Search options for finding windows
interface IXdoSearch {
    title?: string;
    winclass?: string;
    winclassname?: string;
    winname?: string;
    pid?: number;
    maxDepth?: number;
    onlyVisible?: boolean;
    screen?: number;
    desktop?: number;
    limit?: number;
}

// Mouse location result
interface IMouseLocation {
    screen_num: number;
    x: number;
    y: number;
}
```

## System Requirements

- Linux with X11 (Xorg)
- libxdo (xdotool library)
- libx11
- Boost
- CMake 3.10+
- Node.js v20.x (see .nvmrc)
- Clang (preferred) or GCC

## Common Patterns

### Using the API
```typescript
import { XdoToolAsync, XdoToolBindings, ScreenshooterAsync, KeyboardAsync } from 'xdotool';

// Create bindings instance (connects to X11)
const bindings = new XdoToolBindings();

// Create async wrappers
const xdo = new XdoToolAsync(bindings);
const screenshooter = new ScreenshooterAsync(bindings);
const keyboard = new KeyboardAsync(bindings);

// Use async/await
const windows = await xdo.searchWindows({ winname: 'Firefox' });
const location = await xdo.getMouseLocation();
const screenshot = await screenshooter.getImage(); // Returns Buffer
```

### Error Handling in Native Code
```cpp
NAN_METHOD(XdoTool::SomeMethod) {
    XdoTool* tool;
    if(!TypeConverter::Unwrap(info.This(), &tool)) {
        Nan::ThrowError("Method called in invalid context");
        return;
    }
    // ... validation and task creation
}
```

## Important Notes

- Screenshot data is raw 8-bit RGB format
- `Screenshooter.getImage()` reuses the same ArrayBuffer for efficiency
- Window IDs must be validated with `windowHasProperty()` before certain operations
- The `sync()` method flushes X11 commands and waits for completion
