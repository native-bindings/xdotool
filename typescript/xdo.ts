import bindings from "bindings";
import AsyncWrapper from "./AsyncWrapper";
import { XScreenshooter } from "./Screenshooter";

export type XWindow = string;

export interface IXdoViewportDimensions {
    width: number;
    height: number;
}

export enum IXdoSearchRequire {
    SearchAny,
    SearchAll
}

export interface IXdoSearch {
    /**
     * @deprecated
     */
    title?: string;
    winclass?: string;
    winclassname?: string;
    winname?: string;
    pid?: number;
    /**
     * depth of search. 1 means only toplevel windows
     */
    maxDepth?: number;
    /**
     * boolean; set true to search only visible windows
     */
    onlyVisible?: boolean;
    /**
     * what screen to search, if any. If none given, search all screens
     */
    screen?: number;
    /**
     * What desktop to search, if any. If none given, search all screens.
     */
    desktop?: number;
    /**
     * How many results to return? If 0, return all.
     */
    limit?: number;
}

export interface IMouseLocation {
    screen_num: number;
    x: number;
    y: number;
}

export interface IMoveMouseOptions {
    x: number;
    y: number;
    screen_num?: number;
}

export type XdoCallback<T> = (
    err: Error | undefined,
    value: T
) => void;

export interface XdoTool {
    getMouseLocation: (
        callback: XdoCallback<IMouseLocation>
    ) => void;
    sendKeysequence: (
        window: XWindow,
        sequence: string,
        delay: number,
        callback: XdoCallback<void>
    ) => void;
    moveMouse: (
        opts: IMoveMouseOptions,
        callback: XdoCallback<void>
    ) => void;
    enterText: (
        win: string,
        text: string,
        delay: number,
        callback: XdoCallback<void>
    ) => void;
    searchWindows: (
        search: IXdoSearch,
        callback: XdoCallback<string[]>
    ) => void;
    getViewportDimensions: (
        screen: number,
        callback: XdoCallback<IXdoViewportDimensions>
    ) => void;
    getFocusedWindow: (callback: XdoCallback<XWindow>) => void;
    /**
     * Get the PID owning a window. Not all applications support this.
     * It looks at the _NET_WM_PID property of the window.
     *
     * @return the process id or 0 if no pid found.
     */
    getWindowPID: (
        window: XWindow,
        callback: XdoCallback<number>
    ) => void;
    sync: (callback: XdoCallback<void>) => void;
    activateWindow: (
        window: XWindow,
        callback: XdoCallback<void>
    ) => void;
    windowHasProperty: (
        window: XWindow,
        property: string,
        callback: XdoCallback<boolean>
    ) => void;
}

export interface XKeyboard {
    queryKeymap: (
        callback: XdoCallback<ArrayBuffer>
    ) => void;
    keycodeToKeysym: (keycode: number) => number;
    keysymToString: (keysym: number) => string;
}

export interface IXdoTool {
    XdoTool: {
        new(): XdoTool;
    };
    Screenshooter: {
        new(xdo: XdoTool, window?: XWindow): XScreenshooter;
    };
    Keyboard: {
        new(xdo: XdoTool): XKeyboard;
    };
}

export default class XdoToolAsync extends AsyncWrapper {
    public constructor(private readonly xdo: XdoTool) {
        super();
    }
    public windowHasProperty(window: XWindow, property: string): Promise<boolean> {
        return new Promise<boolean>((resolve, reject) => (
            this.xdo.windowHasProperty(
                window,
                property,
                this.resolveOrReject(resolve, reject)
            )
        ));
    }
    public getFocusedWindow(): Promise<XWindow> {
        return new Promise<XWindow>((resolve, reject) => (
            this.xdo.getFocusedWindow(
                this.resolveOrReject(resolve, reject)
            )
        ));
    }
    public sync(): Promise<void> {
        return new Promise<void>((resolve, reject) => (
            this.xdo.sync(
                this.resolveOrReject(resolve, reject)
            )
        ));
    }

    public enterText(window: XWindow, text: string, delay: number): Promise<void> {
        return new Promise<void>((resolve, reject) => (
            this.xdo.enterText(
                window,
                text,
                delay,
                this.resolveOrReject(resolve, reject)
            )
        ));
    }

    public sendKeysequence(window: XWindow, sequence: string, delay: number): Promise<void> {
        return new Promise<void>((resolve, reject) => (
            this.xdo.sendKeysequence(
                window,
                sequence,
                delay,
                this.resolveOrReject(resolve, reject)
            )
        ));
    }

    public activateWindow(window: XWindow): Promise<void> {
        return new Promise<void>((resolve, reject) => (
            this.xdo.activateWindow(
                window,
                this.resolveOrReject(resolve, reject)
            )
        ));
    }

    public moveMouse(opts: IMoveMouseOptions): Promise<void> {
        return new Promise<void>((resolve, reject) => (
            this.xdo.moveMouse(opts, this.resolveOrReject(resolve, reject))
        ));
    }

    public getWindowPID(window: string): Promise<number> {
        return new Promise<number>((resolve, reject) => (
            this.xdo.getWindowPID(window, this.resolveOrReject(resolve, reject))
        ));
    }

    public getViewportDimensions(screen: number): Promise<IXdoViewportDimensions> {
        return new Promise<IXdoViewportDimensions>((resolve, reject) => (
            this.xdo.getViewportDimensions(
                screen,
                this.resolveOrReject<IXdoViewportDimensions>(resolve, reject)
            )
        ));
    }

    public getMouseLocation(): Promise<IMouseLocation> {
        return new Promise<IMouseLocation>((resolve, reject) => (
            this.xdo.getMouseLocation(this.resolveOrReject<IMouseLocation>(resolve, reject))
        ));
    }

    public searchWindows(search: IXdoSearch): Promise<string[]> {
        return new Promise<string[]>((resolve, reject) => (
            this.xdo.searchWindows(
                search,
                this.resolveOrReject<string[]>(resolve, reject)
            )
        ));
    }
}

const {
    XdoTool,
    Screenshooter,
    Keyboard
} = bindings<IXdoTool>({
    try: [
        ['module_root', 'build','cmake-js', 'Debug', 'bindings'],
        ['module_root', 'build','cmake-js', 'Release', 'bindings']
    ],
    bindings: 'node_xdotool'
});

export {
    XdoTool as XdoToolBindings,
    Screenshooter,
    Keyboard
};
