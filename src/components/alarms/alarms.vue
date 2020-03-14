<template>
    <div :class="b()">
        <span :class="b('title')">Alarms settings</span>
        <ul id="timers-list" :class="b('list')">
            <li v-for="(alarm, index) in alarms" :key="index">
                <time-picker
                    name="index"
                    v-bind:value="alarm"
                    deleteable="true"
                    @ondelete="deleteAlarm(index)"
                ></time-picker>
            </li>
        </ul>
        <span v-if="!alarms || !alarms.length"> No alarms!</span>
        <div v-if="alarms && alarms.length < 5" :class="b('new-alarm')">
            <label :class="b('new-alarm-label')">Add new alarm</label>
            <a-time-picker v-model="newAlarm" format="HH:mm" size="large" class="time-picker__value">
            </a-time-picker>
            <a-button size="large" type="primary" @click="addNewAlarm" icon="save">
                Save
            </a-button>
        </div>
    </div>
</template>

<script>
import TimePicker from '../time-picker/time-picker';
import moment from 'moment';

export default {
    name: 'alarms',
    props: ['alarms'],
    data: () => {
        return { 
            newAlarm: moment()
        }
    },
    methods: {
        deleteAlarm(index) {
            console.log(index)
            this.$emit('ondelete', index)
        },
        addNewAlarm() {
            this.$emit('addalarm', this.newAlarm.format('HH:mm'));
            this.newAlarm = moment();
        }
    },
    block: 'alarms',
    components: {TimePicker}
}

</script>

<style scoped>
    .alarms {
        text-align: center;
        margin: 30px 0;

        &__title {
            font-size: 16px;
            font-weight: 500;
            color: white;
        }

        &__list {
            margin: 20px auto;
            list-style: none;
        }

        &__alarm-item {
            display: inline-block;
            margin-right: 20px;
        }

        &__new-alarm-label {
            display: block;
            color: white;
            font-size: 16px;
            font-weight: 500;
        }
    }

        
</style>