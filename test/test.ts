import { Suite } from 'sarg';
import XdoToolAsync, { XdoToolBindings } from '../typescript';

const suite = new Suite();
const {test} = suite;

test('test', async () => {
    const xdo = new XdoToolAsync(new XdoToolBindings());
    const windows = await xdo.searchWindows({
        winclassname: 'Insiders'
    });
    for(const w of windows) {
        if(!(await xdo.windowHasProperty(w, '_NET_WM_DESKTOP'))) {
            continue;
        }
        console.log(w,await xdo.activateWindow(w));
    }
    // while(true) {
    //     const [location, viewportDimensions] = await Promise.all([
    //         xdo.getMouseLocation(),
    //         xdo.getViewportDimensions(0)
    //     ]);
    //     console.log(location, viewportDimensions);
    // }
});

export default suite;