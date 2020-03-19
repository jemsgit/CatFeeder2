<template>
    <div :class="b()">
        <span :class="b('title')">Portion size: {{ portion }}</span>
        <a-slider
            v-model="portion"
            :marks="marks"
            :min="15"
            :max="50"
            :defaultValue="30"
            @change="save"
            name="portion"
            id="portion"
            :class="b('slider')"
        />
    </div>
</template>

<script>
let timeoutId;
export default {
    name: 'portion',
    props: ['value'],
    data() {
        return {
            portion: 0,
            marks: {
                15: {
                    style: {
                        color: 'white',
                        'font-weight': 600
                    },
                    label: 'Tiny',
                },
                50: {
                    style: {
                        color: 'white',
                        'font-weight': 600
                    },
                    label: 'Fatty',
                }
            }
        }
    },
    block: 'portion',
    mounted: function () {
        this.portion = this.value;
    },
    methods: {
        save(e) {
            if(timeoutId) {
                clearTimeout(timeoutId);
            }
            timeoutId = setTimeout(() => {
                this.$emit('save', this.portion)
            }, 1500);
        }
    }
}
</script>

<style scoped>
    .portion {
        color: white;

        &__title {
            font-size: 18px;
            font-weight: 500;
            padding: 0 20px;
            display: block;
        }

        &__slider { 
            margin: 10px 20px;
        }

        .ant-slider-handle {
            width: 25px;
            height: 25px;
            margin-top: -10px;
        }
    }  
</style>

<style>
    .portion {
        .ant-slider-handle {
            width: 25px;
            height: 25px;
            margin-top: -10px;
        }
    }  
</style>