<template>
    <div v-if="isvisible" class="content">
        <button @click="onDisconnect()" class="content__button">
            Disconnect
        </button>
    </div>
</template>

<script>
export default {
    name: 'activity',
    props: ['isvisible'],
    methods: {
        onDisconnect(e) {
            this.$emit('ondisconnect', e);
        }
    }
}
</script>
