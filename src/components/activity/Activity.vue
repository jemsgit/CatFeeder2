<template>
    <div class="content">
        <span style='color: red'>12321</span>
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
