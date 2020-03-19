<template>
    <div :class="b()">
        <overlay :isvisible="loading"></overlay>
        <a-button @click="onDisconnect()" block size="large" type="danger">
            Disconnect
        </a-button>
        
        <form novalidate>
            <current-time
                @save="saveCurrentTime"
                :value="currentTime"
                :class="b('time-settings')" 
            >
            </current-time>

            <alarms
                :alarms="alarms"
                @ondelete="deleteAlarm"
                @addalarm="addNewAlarm"
                :class="b('alarms-settings')" 
            >
            </alarms>

            <portion
                :value="portionSize"
                @save="savePortion"
                :class="b('portion-settings')"   
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
        <span class="feed-success">Yammy!</span>

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
        async savePortion(value) {
            this.portionSize = value
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
            let el = document.querySelector('.feed-success')
            el.classList.add('visible');
            setTimeout(()=> {
                el.classList.remove('visible')
            }, 200)
            //await api.bluetooth.feed(e);
        }
    },
    components: {CurrentTime, Alarms, Portion, Overlay}
}
</script>

<style scoped>
    .content {
        &__time-settings {
            margin-top: 20px;
        }

        &__portion-settings,
        &__alarms-settings{
            margin-top: 40px;
        }

        &__feed-button {
            width: 200px;
            display: block;
            margin: 50px auto 10px;
        }

        .feed-success {
            display: block;
            opacity: 0;
            color: #fdd923;
            font-size: 20px;
            font-weight: 500;
            text-align: center;
            transition: opacity 2s;
        }

        .feed-success.visible {
            opacity: 1;
            color: green;
            transition: opacity 0.2s;
        }
    }
</style>
