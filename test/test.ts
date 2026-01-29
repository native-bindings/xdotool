import timers from "node:timers";
import {
    XdoToolAsync,
    XdoToolBindings,
    ScreenshooterAsync,
    KeyboardAsync,
} from "../typescript";
import test from "ava";

test("it should query keyboard keymap", async (t) => {
    const xdoBindings = new XdoToolBindings();
    const keyboard = new KeyboardAsync(xdoBindings);

    const ab = await keyboard.queryKeymap();
    t.assert(ab.byteLength === 32);
});

test("it should list windows", async (t) => {
    const xdoBindings = new XdoToolBindings();
    const xdo = new XdoToolAsync(xdoBindings);
    const windows = await xdo.searchWindows({
        winname: "",
    });
    t.assert(Array.isArray(windows));
});

test("it should return the same array buffer for getImage", async (t) => {
    const xdoBindings = new XdoToolBindings();
    const ss1 = new ScreenshooterAsync(xdoBindings);
    const firstBuffer = (await ss1.getImage()).buffer;

    t.assert(firstBuffer === (await ss1.getImage()).buffer);
});

test("it should automatically take screenshot of the root window if no window argument is provided", async (t) => {
    const xdoBindings = new XdoToolBindings();
    const ss1 = new ScreenshooterAsync(xdoBindings);

    for (let i = 0; i < 5; i++) {
        const buffer = await ss1.getImage();
        t.deepEqual(buffer[0], 0xff);
        t.deepEqual(buffer[1], 0x00);
        t.deepEqual(buffer[2], 0x00);
        await timers.promises.setTimeout(500);
    }
});

test("it should take screenshot of a specific window", async (t) => {
    const xdoBindings = new XdoToolBindings();
    const xdo = new XdoToolAsync(xdoBindings);
    for (const window of await xdo.searchWindows({
        winclass: "",
    })) {
        const ss1 = new ScreenshooterAsync(xdoBindings, window);

        for (let i = 0; i < 5; i++) {
            const buffer = await ss1.getImage();
            t.deepEqual(buffer[0], 0xff);
            t.deepEqual(buffer[1], 0x00);
            t.deepEqual(buffer[2], 0x00);
            await timers.promises.setTimeout(500);
        }
    }
});
