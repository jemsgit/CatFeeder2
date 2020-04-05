export function stringToBytes(string) {
    var array = new Uint8Array(string.length);
    for (var i = 0, l = string.length; i < l; i++) {
        array[i] = string.charCodeAt(i);
    }
    return array.buffer;
}

// ASCII only
export function bytesToString(buffer) {
    return String.fromCharCode.apply(null, new Uint8Array(buffer));
}

export function promisify(action) {
    return new Promise((resolve, reject) => {
        action((data) => {
            resolve(data)
        }, (err) => {
            reject(err)
        })
    })
}

export function wait(ms) {
    return new Promise(resolve => setTimeout(resolve, ms))
}