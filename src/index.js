import Vue from 'vue';
import { Button, Slider } from 'ant-design-vue';
import bluetoothService from './bluetoothService';
import Activity from './components/activity/activity.vue';
import DeviceList from './components/device-list/device-list.vue';
import 'ant-design-vue/dist/antd.css';
import './styles.css';

Vue.use(Button);
Vue.use(Slider);

var cordovaApp = {
    // Application Constructor
    initialize: function() {
        document.addEventListener('deviceready', this.onDeviceReady.bind(this), false);
    },

    // deviceready Event Handler
    //
    // Bind any cordova events here. Common events are:
    // 'pause', 'resume', etc.
    onDeviceReady: function() {
        createVueApp();
    },
};

cordovaApp.initialize();

let intevalId = null;

function createVueApp() {
    let vueApp = new Vue({
        el: '#app',
        components: { DeviceList, Activity },
        data: function() {
            return {
                devices: [],
                showList: false,
                showContent: true,
                temp: null,
                debug: '',
                customCommand: ''
            }
        },
        methods: {
            getDeviceList: function() {
                this.devices = [];
                bluetoothService.getDevices((device) => { this.devices.push(device) });
            },
            selectDevice: async function(device) {
                try {
                    this.updateLog(device)
                    let connected = await bluetoothService.connectToDevice(device.id);
                    if (connected) {
                        this.updateLog('connected')
                        this.deviceId = device.id;
                        this.showList = false;
                        this.showContent = true;
                        this.updateLog('elm inited');
                    } else {
                        this.updateLog('connect failed');
                    }
                } catch (e) {
                    this.updateLog('connect failed');
                    console.log(e);
                }
            },
            stopMonitoring: function() {
                clearInterval(intevalId);
            },
            disconnect: async function() {
                console.log(12312)
                try {
                    this.stopMonitoring();
                    await bluetoothService.disconnect();
                    this.updateLog('disconnected');
                    this.showList = true;
                    this.showContent = false;
                } catch (e) {
                    this.updateLog('err disconnect');
                    console.log(e)
                }
            },
            onRefresh: function() {
                this.updateLog('refresh');
                if (this.showList) {
                    this.getDeviceList();
                }
            },
            setBTDebug: function() {
                bluetoothService.setDebug(this.updateLog.bind(this))
            },
            updateLog: function(data) {
                this.debug += '\r\n' + data;
                //var debugElement = document.querySelector(".debug p");
                //debugElement.scrollTop = debugElement.scrollHeight;
            },
            sendCustomCommand: async function() {
                await bluetoothService.sendData(this.customCommand);
                this.customCommand = ''
            }
        },
        computed: {
            temperature: function() {
                if (this.temp) {
                    this.updateLog(this.temp);
                    let temp = parseInt(this.temp, 16);
                    return temp - 40 + ' °C';
                }
                return '';
            }
        }
    })
    vueApp.getDeviceList();
}