import { stringToBytes, bytesToString, promisify } from './utils';

class BluetoothService {
    constructor() {
        this.deviceId = null;
        this.service = null;
        this.incomingMessage = null;
        this.error = null;
        this.regex = /([0-9A-Z]{2}\s[0-9A-Z]{2}\s[0-9A-Z]{2})/g;
        this.maxTimeToSearch = 10;
        this.servicesToSearch = []
    }

    async getDevices() {
        return await promisify(ble.scan.bind(ble, this.servicesToSearc, this.maxTimeToSearch))
    }

    async connectToDevice(uuid) {
        let result = false;
        ble.stopScan(() => {});
        try{
            let device = await promisify(ble.connect.bind(ble, uuid));
            if(device) {
                this.deviceId = uuid;
                this.service = device.characteristics.find(function(item){
                    return item.properties.indexOf('Read') > -1 && item.properties.indexOf('WriteWithoutResponse') > -1
                });
            }
        } catch(e) {
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
        } catch(e) {
            result = false;
        }
        return result;
    }

    async sendData(data) {
        return await this.promisify(ble.write.bind(ble, this.deviceId, this.service, stringToBytes(data)));
    }

    listen() {
        ble.startNotification(this.deviceId, this.service, this.characteristic, (data) => {
            message += bytesToString(data);
            if(message.indexOf('\r\n\r\n') != -1){
                this.incomingMessage = message.replace(/\r\n\r\n/g, '');
                success(this.incomingMessage);
                this.incomingMessage = null;
            }
        }, () => {console.log('error')});
    }

    async getAnswer() {
        return new Promise((res, rej) => {
            let intId = setInterval(() => {
                if(this.incomingMessage) {
                    res(this.incomingMessage);
                    this.debug('<<' + this.incomingMessage);
                    this.incomingMessage = null;
                    clearInterval(intId);
                } else if(this.error) {
                    rej(this.error);
                    this.debug('<<' + this.error);
                    this.error = null;
                    clearInterval(intId);
                }
            }, 200)
        }, )
    }
}

let service = new BluetoothService();

export default service;