<template>
    <div class="time-picker">
        <label for="name" class="time-picker__label">{{ title }}</label>
        <a-time-picker v-model="time" @openChange="handleOpen" :open="open" format="HH:mm" class="time-picker__value">
            <a-button slot="addon" slot-scope="panel" size="small" type="primary" @click="handleClose">
                Save
            </a-button>
        </a-time-picker>
    </div>
</template>

<script>
export default {
    name: 'time-picker',
    props: ['name', 'title', 'value'],
    data: () => {
        return {
            time: '12:30',
            open: false
        }
    },
    methods: {
        savePortion(e) {
            console.log(e)
        },
        handleOpen(e) {
            this.currentTimeOpen = true;
        },
        handleClose(e) {
            this.currentTimeOpen = false;
            this.$emit('onsave', e)
        },
    }
}
</script>

<style scoped>
    .time-picker__label {
        font-size: 16px;
        color: red;
        display: block;
        text-align: center;
    }

    .time-picker__value {
        display: block;
        margin: 0 auto;
    }
</style>