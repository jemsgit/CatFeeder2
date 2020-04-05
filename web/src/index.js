import Vue from 'vue';
import { Button, Slider, Spin, TimePicker, Icon, message } from 'ant-design-vue';
import vueBemCn from 'vue-bem-cn';
import bluetoothService from './bluetoothService';
import Activity from './components/activity/activity.vue';
import DeviceList from './components/device-list/device-list.vue';
import 'ant-design-vue/dist/antd.css';
import './styles.css';

message.config({
    top: '100px',
    duration: 2,
    maxCount: 3,
});

Vue.use(vueBemCn, {});
Vue.use(Button);
Vue.use(Slider);
Vue.use(Spin);
Vue.use(TimePicker);
Vue.use(Icon);
Vue.prototype.$message = message;

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

function createVueApp() {
    let vueApp = new Vue({
        el: '#app',
        components: { DeviceList, Activity },
        data: function() {
            return {
                devices: [],
                showList: true,
                showContent: false,
                showRemoteContent: false,
                debug: '',
                customCommand: '',
                connecting: false
            }
        },
        methods: {
            onSelectDevice: function(device) {
                this.showContent = true;
                this.showList = false;
            },
            disconnect: async function() {
                let result = await bluetoothService.disconnect();
                if (result) {
                    this.showList = true;
                    this.showContent = false;
                }
            },
            onShowRemote: function() {
                this.showRemoteContent = true;
                this.showList = false;
            },
            onBackBT: function() {
                this.showList = true;
                this.showRemoteContent = false;
            },
            updateLog: function(data) {
                this.debug += '\r\n' + data;
                //var debugElement = document.querySelector(".debug p");
                //debugElement.scrollTop = debugElement.scrollHeight;
            },
            sendCustomCommand: async function() {
                //await bluetoothService.sendData(this.customCommand);
                this.customCommand = ''
            }
        }
    })
}