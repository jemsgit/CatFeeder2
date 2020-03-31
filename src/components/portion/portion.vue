<template>
    <div :class="b()">
        <span :class="b('title')">Portion size: {{ portion }}</span>
        <a-slider
            v-model="portion"
            :marks="marks"
            :min="40"
            :max="130"
            :defaultValue="50"
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
            portion: 50,
            marks: {
                40: {
                    style: {
                        color: 'white',
                        'font-weight': 600
                    },
                    label: 'Tiny',
                },
                130: {
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
    watch: { 
      	value: function(newVal, oldVal) {
          this.portion = newVal;
        }
    },
    methods: {
        save(e) {
            if(timeoutId) {
                clearTimeout(timeoutId);
            }
            timeoutId = setTimeout(() => {
                this.$emit('save', this.portion)
            }, 500);
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