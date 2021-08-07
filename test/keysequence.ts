import { Suite } from 'sarg';
import {
    XdoToolAsync,
    XdoToolBindings
} from '../typescript';

const suite = new Suite();
const {test} = suite;

test('test', async () => {
    const bindings = new XdoToolBindings();
    const xdo = new XdoToolAsync(bindings);
    const ids = await xdo.searchWindows({
        winname: 'Visual Studio Code'
    });
    for(const id of ids) {
        if(!await xdo.windowHasProperty(id,'_NET_WM_DESKTOP')){
            continue;
        }
        await xdo.activateWindow(id);
        await xdo.sendKeysequence(id,'Up',0);
        await xdo.sendKeysequence(id,'Down',0);
        await xdo.sendKeysequence(id,'Up',0);
        break;
    }
});

export default suite;