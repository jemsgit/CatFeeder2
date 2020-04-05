<template>
    <div class="device-list">
        <overlay :isvisible="connecting"></overlay>
        <span class="device-list__title">Devices
            <a-icon class="device-list__update" type="reload" v-on:click.prevent="onRefresh"/>
        </span>
        <div v-if="!devices.length" @click="showRemote">Use remote?</div>
        <ul>
            <li
                v-for="device in devices"
                v-on:click="onSelectDevice(device)"
                v-bind:key="device.id"
                class="device-list__item"
            >
                {{ device.name || device.id }}
            </li>
        </ul>
    </div>
</template>

<script>
import Overlay from '../overlay/overlay';
import bluetoothService from '../../bluetoothService';
export default {
    name: 'device-list',
    data: () => {
        return {
            devices: [],
            connecting: false
        }
    },
    mounted: function() {
        this.getDevices();
    },
    methods: {
        getDevices() {
            this.devices = [];
            bluetoothService.getDevices((device) => { this.devices.push(device) });
        },
        onRefresh(e) {
            this.getDevices();
        },
        showRemote(e) {
            this.$emit('onshowremote');
        },
        async onSelectDevice(device) {
            this.connecting = true;
            let connected = await bluetoothService.connectToDevice(device.id);
            if (connected) {
                this.$emit('onselectdevice', device);
            }
            this.connecting = false;
        }
    },
    components: {Overlay}
}
</script>

<style scoped>
    @import './device-list.css';
</style>