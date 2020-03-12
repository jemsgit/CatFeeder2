<template>
    <div class="content">
        <a-button @click="onDisconnect()" class="content__button">
            Disconnect
        </a-button>
        <form novalidate class="md-layout" @submit.prevent="savePortion">
        <div >
            <label for="first-name">First Name</label>
            <a-slider :defaultValue="30" name="first-name" id="first-name"/>
        </div>
        </form>
    </div>
</template>

<script>
export default {
    name: 'activity',
    props: ['isvisible'],
    methods: {
        onDisconnect(e) {
            this.$emit('ondisconnect', e);
        },
        savePortion(e) {
            console.log(e)
        }
    }
}
</script>
