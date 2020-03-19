import Vue from 'vue';
import { Button, Slider, Spin, TimePicker, Icon } from 'ant-design-vue';
import vueBemCn from 'vue-bem-cn';
import bluetoothService from './bluetoothService';
import Activity from './components/activity/activity.vue';
import DeviceList from './components/device-list/device-list.vue';
import 'ant-design-vue/dist/antd.css';
import './styles.css';

Vue.use(vueBemCn, {});
Vue.use(Button);
Vue.use(Slider);
Vue.use(Spin);
Vue.use(TimePicker);
Vue.use(Icon);

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
//createVueApp()
let intevalId = null;

function createVueApp() {
    let vueApp = new Vue({
        el: '#app',
        components: { DeviceList, Activity },
        data: function() {
            return {
                devices: [],
                showList: true,
                showContent: false,
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
                console.log(12312)
                this.showList = true;
                this.showContent = false;
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