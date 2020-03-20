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
    block: 'content',
    data: () => {
        return {
            portionSize: 50,
            currentTime: '',
            alarms: [],
            loading: false
        }
    },
    mounted: async function() {
        await this.loadData();
    },
    methods: {
        async onDisconnect(e) {
            let result = await bluetoothService.disconnect();
            if(result) {
                this.$emit('ondisconnect', e);
            } else {
                this.showErrorMessage('You are here forever');
            }
        },
        async savePortion(value) {
            this.loading = true;
            try {
                await api.bluetooth.setPortionSize(value);
                this.portionSize = value;
            } catch(e) {
                console.log(e);
                this.showErrorMessage('Error data loading');
            } finally {
                this.loading = false;
            }
            
        },
        async saveCurrentTime(time) {
            this.loading = true;
            try {
                await api.bluetooth.setTime(time);
                this.currentTime = moment(time, "YYYY:MM:DD:HH:mm").format("HH:mm");
            } catch(er) {
                console.log(er);
                this.showErrorMessage('Error data loading');
            } finally {
                this.loading = false;
            }
        },
        async addNewAlarm(alarm) {
            console.log(alarm)
            this.loading = true;
            try {
                let newAlarmList = await api.bluetooth.addAlarm(alarm);
                this.alarms = newAlarmList;
            } catch(er) {
                console.log(er);
                this.showErrorMessage('Error data loading');
            } finally {
                this.loading = false;
            }
            
        },
        async deleteAlarm(index) {
            this.loading = true;
            try {
                let newAlarmList = await api.bluetooth.deleteAlarm(index);
                this.alarms = newAlarmList;
            } catch(e) {
                console.log(e);
                this.showErrorMessage('Error data loading');
            } finally {
                this.loading = false;
            }
        },
        async loadData() {
            this.loading = true;
            let data = {};
            try {
                data = await api.bluetooth.loadComplexData();
                if(data) {
                    if(!data.time || !data.alarms.length || !data.portionSize) {
                        this.showErrorMessage('Some data wasnt loaded');
                    }
                    this.currentTime = data.time;
                    this.alarms = data.alarms;
                    this.portionSize = data.portionSize;
                }
            } catch(e) {
                console.log('cant get data');
                this.showErrorMessage('Error data loading');
            } finally {
                this.loading = false;
            }
        },
        async feed() {
            this.loading = true;
            try {
                let result = await api.bluetooth.feed();
                this.showSuccessFeed();
                this.showSuccessMessage('Yammy!');
            } catch(e) {
                console.log(e);
                this.showErrorMessage('Error data loading');
            } finally {
                this.loading = false;
            }
        },
        showSuccessFeed() {
            let el = document.querySelector('.feed-success')
            el.classList.add('visible');
            setTimeout(()=> {
                el.classList.remove('visible')
            }, 200)
        },
        showErrorMessage(data) {
            this.$message.error(data);
        },
        showSuccessMessage(data) {
            this.$message.success(data);
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
