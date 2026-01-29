import AsyncWrapper from "./AsyncWrapper";
import { Keyboard, XdoTool, XKeyboard } from "./xdo";

export default class KeyboardAsync extends AsyncWrapper {
    private readonly keyboard: XKeyboard;
    public constructor(xdo: XdoTool) {
        super();
        this.keyboard = new Keyboard(xdo);
    }

    public keycodeToKeysym(keycode: number) {
        return this.keyboard.keycodeToKeysym(keycode);
    }

    public keysymToString(keysym: number) {
        return this.keyboard.keysymToString(keysym);
    }

    public queryKeymap(): Promise<Buffer> {
        return new Promise<Buffer>((resolve, reject) => (
            this.keyboard.queryKeymap(
                this.resolveOrReject(resolve, reject)
            )
        ));
    }
}
