export default {
    host: 'http://104.248.43.82:8085',
    //host: 'http://localhost:8085',
    api: {
        getTime: 'T',
        setTime: 'sT-{value}',
        addAlarm: 'adA-{value}',
        deleteAlarm: 'dA-{value}',
        getAlarms: 'as',
        feed: 'f',
        setActiveAlarm: 'sAA-{value}',
        getPortion: 'P',
        setPortion: 'sP-{value}'
    },
    remoteApi: {
        getTime: {
            url: '/time',
            method: 'get'
        },
        setTime: {
            url: '/time',
            method: 'post'
        },
        addAlarm: {
            url: '/addAlarms',
            method: 'post'
        },
        deleteAlarm: {
            url: '/deleteAlarm/{value}',
            method: 'post'
        },
        getAlarms: {
            url: '/alarms',
            method: 'get'
        },
        feed: {
            url: '/feed',
            method: 'get'
        },
        getPortion: {
            url: '/portion',
            method: 'get'
        },
        setPortion: {
            url: '/portion',
            method: 'post'
        },
        sayMew: {
            url: '/',
            method: 'post'
        }
    }
}