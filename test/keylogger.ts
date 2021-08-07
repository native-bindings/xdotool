import assert from 'assert';
import { Suite } from 'sarg';
import { KeyboardAsync, XdoToolBindings } from '../typescript';

const suite = new Suite();
const {test} = suite;

test('it should log keys user is typing', async () => {
    const bindings = new XdoToolBindings();
    const kbd = new KeyboardAsync(bindings);
    let buffer: Buffer;
    let lastBuffer: Buffer | null = null;
    const startedAt = new Date();
    while((Date.now() - startedAt.getTime()) <= 10000) {
        buffer = Buffer.from(await kbd.queryKeymap(),0,32);
        if(lastBuffer !== null) {
            assert.strict.ok(buffer.buffer === lastBuffer.buffer)
        }
        lastBuffer = buffer;
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
    console.log('Finished!');
});

export default suite;