<template>
    <div :class="b()">
        <div :class="b('overlay')"><a-spin /></div>
        <a-button @click="onDisconnect()" block>
            Disconnect
        </a-button>
        
        <form novalidate>
            <time-picker
                name="currentTime"
                title="Текущее время кормушки"
                v-bind:value="currentTime"
                @onsave="saveCurrentTime"
            ></time-picker>

            <div :class="b('alarms')">
                <span :class="b('alarms-title')">Alarms settings</span>
                <ul id="timers-list">
                    <li v-for="alarm in alarms" :key="alarm">
                        {{ alarm }}
                        <a-button
                            :class="b('delete-alarm')"
                        >
                            Delete
                        </a-button>
                    </li>
                </ul>
                <a-button
                    v-if="alarms.length < 5"
                    :class="b('add-button')"
                >
                    Add
                </a-button>
            </div>

            <div :class="b('portion')">
                <label for="portion">Portion size</label>
                <a-slider
                    v-model="portionSize"
                    :defaultValue="30"
                    @change="savePortion"
                    name="portion"
                    id="portion"/>
            </div>
        </form>

        <a-button @click="feed()" :class="b('feed')">
            Feed!
        </a-button>
    </div>
</template>

<script>
import TimePicker from '../time-picker/time-picker';
import bluetoothService from '../../bluetoothService';
import api from '../../api/api';
export default {
    name: 'activity',
    props: ['isvisible'],
    block: 'content',
    data: () => {
        return {
            portionSize: 30,
            currentTime: null,
            alarms: ['12:30']
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
            console.log(e)
            await api.bluetooth.setPortionSize(e);
        },
        async aveCurrentTime(e) {
            console.log(e)
            await api.bluetooth.setTime(e);
        },
        async saveAlarms(e) {
            console.log(e)
            await api.bluetooth.setAlarm(e);
        },
        loadData() {
            api.bluetooth.loadComplexData();
        },
        async feed() {
            console.log('feed')
            await api.bluetooth.feed(e);
        }
    },
    components: {TimePicker}
}
</script>

<style lang="postcss" scoped>
    .content {
        &__overlay {
            position: absolute;
            top: 0;
            bottom: 0;
            left: 0;
            right: 0;
            background: rgba(208, 222, 255, 0.8);
            z-index: 20;

            .ant-spin {
                position: absolute;
                top: 30%;
                left: 50%;
            }
        }

        &__alarms {
            color: red;
        }

        &__alarms-title {
            font-seze: 16px;
            color: red;
        }

        &__portion-title {
            font-seze: 16px;
            color: red;
        }
    }
</style>
