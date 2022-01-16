import AsyncWrapper from "./AsyncWrapper";
import {
    Screenshooter,
    XdoCallback,
    XdoTool,
    XID
} from "./xdo";

export interface XScreenshooter {
    getImage: (callback: XdoCallback<ArrayBuffer>) => void;
}

export default class ScreenshooterAsync extends AsyncWrapper {
    readonly #ss: XScreenshooter;
    public constructor(xdo: XdoTool, window?: XID) {
        super();
        this.#ss = new Screenshooter(xdo, window);
    }
    public getImage(): Promise<ArrayBuffer> {
        return new Promise<ArrayBuffer>((resolve, reject) => (
            this.#ss.getImage(
                this.resolveOrReject(resolve, reject)
            )
        ));
    }
}
