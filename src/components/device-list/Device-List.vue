<template>
    <div v-if="isvisible" class="device-list">
        <span class="device-list__title">Список устройств
            <a class="refresh-link" v-on:click.prevent="onRefresh">REFRESH</a>
        </span>
        <div v-if="!devices.length">use remote?</div>
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
import bluetoothService from '../../bluetoothService';
export default {
    props: ['isvisible'],
    name: 'device-list',
    data: () => {
        return {
            devices: []
        }
    },
    mounted: () => {

    },
    methods: {
        getDevices() {
            this.devices = [];
            bluetoothService.getDevices((device) => { this.devices.push(device) });
        },
        onRefresh(e) {
            this.getDevices();
        },
        async onSelectDevice(device) {
            let connected = await bluetoothService.connectToDevice(device.id);
            if (connected) {
                this.$emit('onselectdevice', device);
            }
        }
    }
}
</script>

<style scoped>
    @import './device-list.css';
</style>