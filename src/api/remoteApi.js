import axios from 'axios';
import url from 'url';
import config from '../config';

const fetcher = {
    get: (url, data, headers) => {
        return axios.get(resolveUrl(url), { headers })
    },
    post: (url, data, headers) => {
        return axios.post(resolveUrl(url), data, { headers } )
    }
}

function resolveUrl(path) {
    return url.resolve(config.host, path);
}

async function getTime() {
    let time = await fetcher.get('/getTime');
    return time;
}

async function getAlarms() {
    let alarms = await fetcher.get('/getAlarms');
    return alarms;
}

async function getPortionSize() {
    let portion = await fetcher.get('/getPortionSize');
    return portion;
}

async function setTime(time) {
    let res = await fetcher.post('/setTime', {time});
    return res;
}

async function addAlarm(alarm) {
    let res = await fetcher.post('/addAlarm', {alarm});
    return res;
}

async function deleteAlarm(alarmIndex) {
    let res = await fetcher.post('/deleteAlarm', {index: alarmIndex});
    return res;
}

async function setPortionSize(size) {
    let res = await fetcher.post('/setPortion', {size});
    return res;
}

async function feed() {
    let alarms = await fetcher.get('/feed');
    return alarms;
}

async function sayMew(text) {
    let alarms = await fetcher.post('/sayMew', { text });
    return alarms;
}

async function loadComplexData() {
    let time = await getTime();
    let alarms = await getAlarms();
    let portionSize = await getPortionSize();
    return {
        time,
        alarms,
        portionSize
    }
}

export default {
    getTime,
    getAlarms,
    getPortionSize,
    setTime,
    addAlarm,
    deleteAlarm,
    setPortionSize,
    feed,
    loadComplexData,
    sayMew
}