<template>
    <div v-if="isvisible" class='overlay'><a-spin /></div>
</template>

<script>
export default {
    name: 'overlay',
    props: ['isvisible']
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

        .ant-spin {
            position: absolute;
            top: 30%;
            left: 50%;
        }
    }
</style>