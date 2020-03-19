<template>
    <div :class="b()">
        <label v-if="title" for="name" :class="b('label')">{{ title }}</label>
        <div v-if="!editing" :class="b('content')">
            <span :class="b('current-value')">
                {{ value }}
            </span>
            <section :class="b('buttons')">
                <a-button
                    v-if="editable && !editing"
                    size="large"
                    type="default"
                    @click="editTime"
                    icon="edit"
                >
                </a-button>
                <a-button
                    v-if="deleteable && !editing"
                    size="large"
                    type='danger'
                    @click="deleteTime"
                    icon="close-circle"
                >
                </a-button>
            </section>
        </div>

        <div v-if="editing" :class="b('content')">
            <a-time-picker
                v-model="time"
                format="HH:mm"
                size="large"
                :class="b('value')"
                popupClassName="mobile-timepicker">
            </a-time-picker>
            <section :class="b('buttons')">
                <a-button size="large" type="default" @click="save()" icon="save" :disabled="!isTimeValid">
                </a-button>
                <a-button size="large" type="danger" @click="cancel()" icon='close-circle'>
                </a-button>
            </section>
        </div>
    </div>
</template>

<script>
import moment from 'moment';
export default {
    name: 'time-picker',
    props: ['name', 'title', 'value', 'editable', 'deleteable'],
    block: 'time-picker',
    data: () => {
        return {
            time: null,
            editing: false
        }
    },
    methods: {
        save(e) {
            this.$emit('onsave', this.time.format('HH:mm'));
            this.editing = false;
        },
        cancel(e) {
            this.editing = false;
        },
        editTime(e) {
            this.time = moment();
            this.editing = true;
        },
        deleteTime() {
            this.$emit('ondelete');
        }
    },
    computed: {
        isTimeValid: function() {
            console.log(this.time)
            return moment(this.time).isValid()
        }
    }
}
</script>

<style scoped>
    .time-picker {
        padding: 10px 20px;

        &__label {
            font-size: 16px;
            font-weight: 500;
            color: white;
            display: block;
            margin-bottom: 10px;
        }

        &__content {
            display: flex;
            flex-direction: row;
            justify-content: space-between;
            align-items: center;
        }

        &__current-value {
            font-size: 28px;
            font-weight: 500;
            color: white;
        }

        &__buttons {
            button {
                width: 50px;
                margin-left: 10px;
            }
        }

    }

    .mobile-timepicker {
        .ant-time-picker-panel-combobox {
            zoom: 1.5;
        }

        .ant-time-picker-panel-inner {
            font-size: 18px;
        }
    }
</style>