<template>
    <div class="content">
        <div class="overlay"><a-spin /></div>
        <a-button @click="onBack()">
            Back to BT
        </a-button>
        
        <form novalidate>
            <time-picker name="currentTime" title="Текущее время кормушки" @onsave="saveCurrentTime"></time-picker>
            <div>
                Настройки таймеров кормления
                <ul id="timers-list">
                    <li v-for="alarm in alarms" :key="alarm">
                        {{ alarm }}<span>Delete</span>
                    </li>
                </ul>
                <span v-if="alarms.length < 5"> Add </span>
            </div>
            <div>
                <label for="portion">Размер порции</label>
                <a-slider
                    v-model="portionSize"
                    :defaultValue="30"
                    @change="savePortion"
                    name="portion"
                    id="portion"/>
            </div>
        </form>

        <a-button @click="feed()">
            Feed!
        </a-button>
        <a-input placeholder="say Mew" v-bind="message"/>
        <a-button @click="sayMew()">
            Mew!
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
    data: () => {
        return {
            portionSize: 30,
            currentTime: null,
            alarms: ['12:30'],
            message: ''
        }
    },
    methods: {
        onBack(e) {
            console.log(e)
        },
        savePortion(e) {
            console.log(e)
            await api.remote.setPortionSize(e);
        },
        saveCurrentTime(e) {
            console.log(e)
            await api.remote.setPortionSize(e);
        },
        saveAlarms(e) {
            console.log(e)
            await api.remote.setPortionSize(e);
        },
        loadData() {
            api.remote.loadComplexData();
        },
        feed() {
            console.log('feed')
            await api.remote.feed(e);
        },
        sayMew() {
            console.log('feed')
            await api.remote.sayMew(this.message);
            this.message = ''
        }
    },
    components: {TimePicker}
}
</script>
