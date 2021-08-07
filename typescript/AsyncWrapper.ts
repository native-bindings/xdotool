
export default abstract class AsyncWrapper {
    protected resolveOrReject<T>(resolve: (result: T) => void, reject: (error: Error) => void) {
        return (err: Error | undefined, result: T) => {
            if(typeof err !== 'undefined') {
                reject(err);
            } else {
                resolve(result);
            }
        };
    }
}
