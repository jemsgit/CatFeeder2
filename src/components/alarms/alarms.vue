<template>
    <div :class="b()">
        <span :class="b('title')">Alarms settings</span>
        <ul id="timers-list" :class="b('list')">
            <li v-for="(alarm) in alarms" :key="alarm.index">
                <time-picker
                    name="index"
                    v-bind:value="alarm.value"
                    :isactive="alarm.isActive"
                    deleteable="true"
                    @ondelete="deleteAlarm(alarm.index)"
                    :class="b('alarm-item')"
                ></time-picker>
            </li>
        </ul>
        <span v-if="!alarms || !alarms.length" :class="b('no-alarms')"> No alarms!</span>
        <div v-if="alarms && alarms.length < 5" :class="b('new-alarm')">
            <a-button
                v-if="!newAlarmVisible"
                type="primary"
                shape="circle"
                icon="plus"
                size="large"
                @click="toggleNewAlarm"
                :class="b('new-alarm-button')"
            >
            </a-button>
            <div :class="b('add-new')" v-if="newAlarmVisible">
                <label :class="b('new-alarm-label')">Add new alarm</label>
                <section :class="b('new-alarm-content')">
                    <a-time-picker
                        v-model="newAlarm"
                        format="HH:mm"
                        size="large"
                        popupClassName="mobile-timepicker"
                    >
                    </a-time-picker>
                    <a-button size="large" type="default" @click="addNewAlarm" icon="save" :disabled="!isTimeValid">
                        Save
                    </a-button>
                    <a-button size="large" type="danger" @click="toggleNewAlarm" icon='close-circle'>
                        Cancel
                    </a-button>
                </section>
            </div>
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
            newAlarm: moment(),
            newAlarmVisible: false
        }
    },
    methods: {
        deleteAlarm(index) {
            console.log(index)
            this.$emit('ondelete', index)
        },
        addNewAlarm() {
            this.newAlarmVisible = false;
            this.$emit('addalarm', this.newAlarm.format('HH:mm'));
            this.newAlarm = moment();
        },
        toggleNewAlarm() {
            this.newAlarmVisible = !this.newAlarmVisible;
        }
    },
    block: 'alarms',
    components: {TimePicker},
    computed: {
        isTimeValid: function() {
            return moment(this.newAlarm).isValid()
        }
    }
}

</script>

<style scoped>
    .alarms {
        &__title {
            font-size: 16px;
            font-weight: 500;
            color: white;
            margin-left: 20px;
        }

        &__list {
            padding: 0;
            list-style: none;
        }

        &__alarm-item {
            border-bottom: 1px white solid;
        }

        &__new-alarm {
            text-align: center;
        }

        &__new-alarm-button {
            width: 50px;
            height: 50px;
            border: 1px white solid;
            margin-top: 10px;
        }

        &__new-alarm-label {
            display: block;
            margin-bottom: 20px;
            color: white;
            font-size: 16px;
            font-weight: 500;
        }

        &__no-alarms {
            text-align: center;
            display: block;
            font-size: 20px;
            color: #fffa48;
        }
    }

        
</style>