<template>
    <time-picker
        name="currentTime"
        title="Current time"
        :value="value"
        :editable="true"
        :syncing="true"
        format="YYYY:MM:DD:H:mm"
        @onsave="save"
    ></time-picker>
</template>

<script>
import TimePicker from '../time-picker/time-picker';
import moment from 'moment';

export default {
    name: 'current-time',
    props: ['value'],
    methods: {
        save(e) {
            this.$emit('save', e)
        }
    },
    components: {TimePicker}
}
</script>

<style scoped>
    .overlay {
            position: absolute;
            top: 0;
            bottom: 0;
            left: 0;
            right: 0;
            background: rgba(208, 222, 255, 0.8);
            z-index: 20;
    }
</style>