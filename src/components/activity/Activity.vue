    <template>
        <div v-if="isVisible" class="content">
            <button v-on:click="onDisconnect()" class="content__button">
                Disconnect
            </button>
        </div>
    </template>

<script>
export default {
    props: ['onDisconnect', 'isVisible']
}
</script>
