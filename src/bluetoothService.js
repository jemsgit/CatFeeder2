import { stringToBytes, bytesToString, promisify } from './utils';

class BluetoothService {
    constructor() {
        this.deviceId = null;
        this.service = null;
        this.incomingMessage = null;
        this.error = null;
        this.maxTimeToSearch = 30;
        this.servicesToSearch = [];
    }

    getDevices(updateDeviceList) {
        ble.scan(this.servicesToSearch, this.maxTimeToSearch, (data) => {
            updateDeviceList(data)
        })
    }

    async connectToDevice(uuid) {
        let result = false;
        ble.stopScan(() => {});
        try {
            let device = await promisify(ble.connect.bind(ble, uuid));
            if (device) {
                this.deviceId = uuid;
                this.service = device.characteristics.find(function(item) {
                    return item.properties.includes('Read') &&
                        item.properties.includes('WriteWithoutResponse');
                });
                result = true;
            }
        } catch (e) {
            result = false;
        }
        return result;
    }

    async disconnect() {
        let result = false;
        try {
            await promisify(ble.disconnect.bind(ble, this.deviceId));
            this.deviceId = null;
            this.service = null;
            result = true;
        } catch (e) {
            result = false;
        }
        return result;
    }

    async sendData(data) {
        if(!this.deviceId || !this.service) {
            throw Error;
        }
        return await this.promisify(
            ble.write.bind(ble, this.deviceId, this.service, stringToBytes(data)));
    }

    listen() {
        ble.startNotification(this.deviceId, this.service, this.characteristic, (data) => {
            message += bytesToString(data);
            if (message.includes('\r\n\r\n')) {
                this.incomingMessage = message.replace(/\r\n\r\n/g, '');
                success(this.incomingMessage);
                this.incomingMessage = null;
            }
        }, () => { console.log('error') });
    }

    async getAnswer() {
        return new Promise((res, rej) => {
            let intId = setInterval(() => {
                if (this.incomingMessage) {
                    res(this.incomingMessage);
                    this.incomingMessage = null;
                    clearInterval(intId);
                } else if (this.error) {
                    rej(this.error);
                    this.error = null;
                    clearInterval(intId);
                }
            }, 200)
        }, )
    }
}

let service = new BluetoothService();

export default service;