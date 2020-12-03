import axios from 'axios';
import url from 'url';
import config from '../config';
import { timeParser, alarmsParser } from '../parsers';

const fetcher = {
    get: (url, data, headers) => {
        return axios.get(resolveUrl(url), { headers })
    },
    post: (url, data, headers) => {
        return axios.post(resolveUrl(url), data, { headers })
    }
}

function resolveUrl(path) {
    return url.resolve(config.host, path);
}

async function getTime() {
    let { method, url } = config.remoteApi.getTime
    let { data } = await fetcher[method](url);
    return timeParser(data);
}

async function getAlarms() {
    let { method, url } = config.remoteApi.getAlarms
    let { data } = await fetcher[method](url);
    return alarmsParser(data);
}

async function getPortionSize() {
    let { method, url } = config.remoteApi.getPortion
    let { data } = await fetcher[method](url);
    return data;
}

async function setTime(value) {
    let { method, url } = config.remoteApi.setTime
    let res = await fetcher[method](url, { value });
    return res;
}

async function addAlarm(value) {
    let { method, url } = config.remoteApi.addAlarm
    let res = await fetcher[method](url, { value });
    res = await getAlarms();
    return res;
}

async function deleteAlarm(alarmIndex) {
    let { method, url } = config.remoteApi.deleteAlarm
    let res = await fetcher[method](url.replace('{value}', alarmIndex));
    res = await getAlarms();
    return res;
}

async function setPortionSize(value) {
    let { method, url } = config.remoteApi.setPortion
    let res = await fetcher[method](url, { value });
    return res;
}

async function feed() {
    let { method, url } = config.remoteApi.feed
    await fetcher[method](url);
    return true;
}

async function sayMew(text) {
    let { method, url } = config.remoteApi.sayMew
    await fetcher[method](url, { massage: text }, { 'Content-Type': 'application/x-www-form-urlencoded' });
    return true;
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