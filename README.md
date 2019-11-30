# xdotool

C++ bindings for Node.js to handle X11 using the full power of libxdo.

## Installation

```
yarn add xdotool
```

## Examples

### Key logging

A few lines of code and you're able to spy what key strokes are being pressed on the keyboard:

```ts
const bindings = new XdoToolBindings();
const kbd = new KeyboardAsync(bindings);
while(true) { // Keeps printing until you kill the process
    const buffer = Buffer.from(await kbd.queryKeymap(), 0, 32);
    for(let j = 0; j < 32; j++) {
        for(let h = 0; h < 8; h++) {
            if(buffer[j] & (1 << h)) {
                const keycode = j * 8 + h;
                const keysym = kbd.keycodeToKeysym(keycode);
                console.log(kbd.keysymToString(keysym));
            }
        }
    }
}
```

### Activate window

In the example below we look for windows with the name of `Visual Studio Code`. If any valid match is found, we focus it.

```ts
import { XdoToolAsync, XdoToolBindings } from 'xdotool';

// Activate Visual Studio Code window
function activateVisualStudioCodeWindow() {
    const xdo = new XdoToolAsync(new XdoToolBindings());
    const windows = await xdo.searchWindows({
        winclassname: 'Visual Studio Code'
    });
    for(const w of windows) {
        if(!(await xdo.windowHasProperty(w, '_NET_WM_DESKTOP'))) {
            continue;
        }
        await xdo.activateWindow(w);
        break;
    }
}
```

### Screenshot

Our X11-based screenshooter is only capable of returning raw 8-bit RGB image format data at the moment but it is very fast and memory efficient. Getting a screenshot is super simple:

```ts
import { ScreenshooterAsync, XdoToolBindings } from 'xdotool';
import { promises as fs } from 'fs';

async function screenshot() {
    const xdo = new XdoToolBindings();
    const screenshooter = new ScreenshooterAsync(xdo);
    const arrayBuffer = await screenshooter.getImage();
    fs.writeFile(`${__dirname}/screenshot_${new Date()}.rgb`, Buffer.from(arrayBuffer));
}
```

For obvious reasons, the `ArrayBuffer` returned by `screenshooter.getImage` is reused on every call. So, unless you create a new instance of screenshooter, if you try to keep the reference, it'll be changed if you're calling this method elsewhere at the same time.

## Requirements

- Boost
- CMake
- libx11
- xdotool