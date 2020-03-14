<template>
    <div class="time-picker">
        <label v-if="title" for="name" class="time-picker__label">{{ title }}</label>
        <span v-if="!editing" class="time-picker__current-value">
            {{ value }}
        </span>

        <a-button
            v-if="editable && !editing"
            size="large"
            type="primary"
            @click="editTime"
            icon="edit"
        >
            Edit
        </a-button>
        <a-button
            v-if="deleteable && !editing"
            size="large"
            type='danger'
            @click="deleteTime"
            icon="delete"
        >
            Delete
        </a-button>

        <div v-if="editing">
            <a-time-picker v-model="time" format="HH:mm" size="large" class="time-picker__value">
            </a-time-picker>
            <a-button size="large" type="primary" @click="save()" icon="save">
                Save
            </a-button>
            <a-button size="large" type="danger" @click="cancel()" icon='close-circle'>
                Cancel
            </a-button>
        </div>
    </div>
</template>

<script>
import moment from 'moment';
export default {
    name: 'time-picker',
    props: ['name', 'title', 'value', 'editable', 'deleteable'],
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
    }
}
</script>

<style scoped>
    .time-picker {
        text-align: center;

        &__label {
            font-size: 16px;
            font-weight: 500;
            color: white;
            display: block;
            margin-bottom: 10px;
            text-align: center;
        }

        &__current-value {
            font-size: 22px;
            font-weight: 500;
            color: white;
        }

    }
</style>