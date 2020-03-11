<template>
    <div v-if="isvisible" class="device-list">
        <span class="device-list__title">Список устройств
            <a class="refresh-link" v-on:click.prevent="onRefresh">REFRESH</a>
        </span>
        <ul>
            <li
                v-for="device in devices"
                v-on:click="onSelectDevice(device)"
                v-bind:key="device.id"
                class="device-list__item"
            >
                {{ device.name }}
            </li>
        </ul>
    </div>
</template>

<script>
export default {
    props: ['devices', 'isvisible'],
    name: 'device-list',
    methods: {
        onRefresh(e) {
            this.$emit('onrefresh', e);
        },
        onSelectDevice(device) {
            this.$emit('onselectdevice', device);
        }
    }
}
</script>

<style scoped>
    @import './device-list.css';
</style>