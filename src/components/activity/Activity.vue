<template>
    <div :class="b()">
        <overlay :isvisible="loading"></overlay>
        <a-button @click="onDisconnect()" block>
            Disconnect
        </a-button>
        
        <form novalidate>
            <current-time
                @save="saveCurrentTime"
                :value="currentTime"
            >
            </current-time>

            <alarms
                :alarms="alarms"
                @ondelete="deleteAlarm"
                @addalarm="addNewAlarm"
            >
            </alarms>

            <portion
                :value="portionSize"
                @save="savePortion"    
            >
            </portion>

        </form>

        <a-button
            type="danger"
            icon="download" 
            size="large"
            @click="feed()"
            :class="b('feed-button')"
        >
            Feed!
        </a-button>

    </div>
</template>

<script>
import CurrentTime from '../current-time/current-time';
import Alarms from '../alarms/alarms';
import Portion from '../portion/portion';
import Overlay from '../overlay/overlay';
import bluetoothService from '../../bluetoothService';
import api from '../../api/api';
import moment from 'moment';

let timeoutId;

export default {
    name: 'activity',
    props: ['isvisible'],
    block: 'content',
    data: () => {
        return {
            portionSize: 30,
            currentTime: '12:30',
            alarms: ['12:30'],
            loading: false
        }
    },
    methods: {
        async onDisconnect(e) {
            let result = await bluetoothService.disconnect();
            if(result) {
                this.$emit('ondisconnect', e);
            }
        },
        async savePortion(e) {
            //await api.bluetooth.setPortionSize(e);
        },
        async saveCurrentTime(e) {
            console.log(e)
            this.loading = true;
            setTimeout(() => {
                this.loading = false;
            }, 1500)
            //await api.bluetooth.setTime(e);
        },
        async addNewAlarm(e) {
            this.loading = true;
            setTimeout(() => {
                this.loading = false;
                this.alarms.push(e);
            }, 1500)
            
        },
        async deleteAlarm(index) {
            console.log(index)
            this.loading = true;
            setTimeout(() => {
                this.loading = false;
                this.alarms.splice(index, 1);
            }, 1500)
            
            //await api.bluetooth.deleteAlarm(index);
        },
        async saveAlarms(e) {
            console.log(e)
            //await api.bluetooth.setAlarm(e);
        },
        loadData() {
            //api.bluetooth.loadComplexData();
        },
        async feed() {
            console.log('feed')
            //await api.bluetooth.feed(e);
        }
    },
    components: {CurrentTime, Alarms, Portion, Overlay}
}
</script>

<style scoped>
    .content {
        &__feed-button {
            width: 300px;
            display: block;
            margin: 20px auto;
        }
    }
</style>
