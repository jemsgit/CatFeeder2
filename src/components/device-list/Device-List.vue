<template>
    <div v-if="isVisible" class="device-list">
        <span class="device-list__title">Список устройств
            <a class="refresh-link" v-on:click.prevent="onRefresh">REFRESH</a>
        </span>
        <ul>
            <li v-for="device in devices" v-on:click="onSelectDevice(device)" class="device-list__item">
                {{ device.name }}
            </li>
        </ul>
    </div>
</template>

<script>
export default {
    props: ['devices', 'onSelectDevice', 'onRefresh', 'isVisible'],
    name: 'DeviceList'
}
</script>

<style scoped>
 @import './device-list.css';
</style>