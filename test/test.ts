import { Suite } from 'sarg';
import {
    XdoToolAsync,
    XdoToolBindings,
    ScreenshooterAsync,
    KeyboardAsync
} from '../typescript';

const suite = new Suite();
const {test} = suite;

async function exec() {
    const xdoBindings = new XdoToolBindings();
    const xdo = new XdoToolAsync(xdoBindings);
    const windows = await xdo.searchWindows({
        winclassname: 'Code'
    });
    for(const w of windows) {
        if(!(await xdo.windowHasProperty(w, '_NET_WM_DESKTOP'))) {
            continue;
        }
        await xdo.activateWindow(w);
        // console.log(require('fs').writeFileSync(__dirname + '/test.raw', buffer))
        break;
    }

    const ss1 = new ScreenshooterAsync(xdoBindings);
    const ss2 = new ScreenshooterAsync(xdoBindings);

    const keyboard = new KeyboardAsync(xdoBindings);
    let lastArrayBuffer: ArrayBuffer | undefined;

    while(true) {
        const buffers = await Promise.all([ss1.getImage(), ss2.getImage()]);
        console.log(buffers);

        const ab = await keyboard.queryKeymap();
        console.log(ab, ab === lastArrayBuffer);
        lastArrayBuffer = ab;
    }
}

test('test', async () => {
    await exec();
    let i = 10000;
    while(--i) {
        console.log('.');
        await new Promise((resolve) => setTimeout(resolve, 1000));
    }
});

export default suite;