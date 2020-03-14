<template>
    <div :class="b()">
        <label for="portion" :class="b('title')">Portion size</label>
        <span :class="b('value')"> {{ value }} </span>
        <a-slider
            v-model="value"
            :defaultValue="30"
            @change="save"
            name="portion"
            id="portion"
        />
    </div>
</template>

<script>
let timeoutId;
export default {
    name: 'portion',
    props: ['value'],
    block: 'portion',
    methods: {
        save(e) {
            if(timeoutId) {
                clearTimeout(timeoutId);
            }
            timeoutId = setTimeout(() => {
                this.$emit('save', this.value)
            }, 1500);
        }
    }
}
</script>

<style scoped>
    .portion {
        color: white;
        text-align: center;

        &__portion-title {
            font-size: 16px;
            font-weight: 500;
            display: block;
        }

        &__portion-value {
            margin: 0 10px;
            display: inline-block;
        }
    }

        
</style>