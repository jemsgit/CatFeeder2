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
                let service = device.characteristics.find(function(item) {
                    return item.properties.includes('Read') &&
                        item.properties.includes('WriteWithoutResponse');
                });
                if (service) {
                    this.service = service.service;
                    this.characteristic = service.characteristic
                }
                this.listen();
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
            this.characteristic = null;
            result = true;
        } catch (e) {
            result = false;
        }
        return result;
    }

    async sendData(data) {
        if (!this.deviceId || !this.service) {
            throw Error;
        }
        return await promisify(
            ble.write.bind(ble, this.deviceId, this.service, this.characteristic, stringToBytes(data)));
    }

    listen() {
        let message = '';
        ble.startNotification(this.deviceId, this.service, this.characteristic, (data) => {
            message += bytesToString(data);
            if (message.includes('\r\n\r\n')) {
                this.incomingMessage = message.replace(/\r\n\r\n/g, '');
                message = '';
            }
        }, () => { console.log('error') });
    }

    async getAnswer() {
        this.incomingMessage = null;
        return new Promise((res, rej) => {
            let intId = setInterval(() => {
                if (this.incomingMessage) {
                    let data = this.incomingMessage;
                    this.incomingMessage = null;
                    console.log(data)
                    clearInterval(intId);
                    res(data);
                } else if (this.error) {
                    rej(this.error);
                    this.incomingMessage = null;
                    this.error = null;
                    clearInterval(intId);
                }
            }, 200)
        }, )
    }
}

let service = new BluetoothService();

export default service;