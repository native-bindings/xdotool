declare module "bindings" {
    export default function<T>(opts: string | {
        try: string[][];
        bindings: string;
    }): T;
}
