<template>
    <div>
        <label for="name">{{ title }}</label>
        <a-time-picker v-model="time" @openChange="handleOpen" :open="open" format="HH:mm">
            <a-button slot="addon" slot-scope="panel" size="small" type="primary" @click="handleClose">
                Save
            </a-button>
        </a-time-picker>
    </div>
</template>

<script>
export default {
    name: 'time-picker',
    props: ['name', 'title'],
    data: () => {
        time: null,
        open: false
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
