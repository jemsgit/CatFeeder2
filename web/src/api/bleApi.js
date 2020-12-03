import bluetoothService from '../bluetoothService';
import { timeParser, alarmsParser } from '../parsers';
import { wait } from '../utils';
import config from '../config';

function checkErrorResponse(resp) {
    if (resp === 'f') {
        throw Error('fail');
    }
}

async function getTime(needWake = true) {
    if (needWake) {
        await wake();
    }
    await bluetoothService.sendData(config.api.getTime);
    let resp = await bluetoothService.getResponse();
    checkErrorResponse(resp);
    return timeParser(resp);
}

async function getAlarms(needWake = true) {
    if (needWake) {
        await wake();
    }
    await bluetoothService.sendData(config.api.getAlarms);
    let resp = await bluetoothService.getResponse();
    checkErrorResponse(resp);
    return alarmsParser(resp);
}

async function getPortionSize(needWake = true) {
    if (needWake) {
        await wake();
    }
    await bluetoothService.sendData(config.api.getPortion);
    let resp = await bluetoothService.getResponse();
    checkErrorResponse(resp);
    return parseInt(resp, 10);
}

async function setTime(time) {
    await wake();
    await bluetoothService.sendData(config.api.setTime.replace('{value}', time));
    let resp = await bluetoothService.getResponse();
    checkErrorResponse(resp);
    return resp;
}

async function addAlarm(alarm) {
    await wake();
    await bluetoothService.sendData(config.api.addAlarm.replace('{value}', alarm));
    let resp = await bluetoothService.getResponse();
    checkErrorResponse(resp);
    resp = await getAlarms();
    return resp;
}

async function deleteAlarm(alarmIndex) {
    await wake();
    await bluetoothService.sendData(config.api.deleteAlarm.replace('{value}', alarmIndex));
    let resp = await bluetoothService.getResponse();
    checkErrorResponse(resp);
    resp = await getAlarms();
    return resp;
}

async function setPortionSize(size) {
    await wake();
    await bluetoothService.sendData(config.api.setPortion.replace('{value}', size));
    let resp = await bluetoothService.getResponse();
    checkErrorResponse(resp);
    return resp;
}

async function feed() {
    await wake();
    await bluetoothService.sendData(config.api.feed);
    let resp = await bluetoothService.getResponse();
    checkErrorResponse(resp);
    return resp;
}

async function loadComplexData() {
    let time = null;
    let alarms = [];
    let portionSize;
    await wake();
    try {
        time = await getTime(false);
    } catch (e) {
        console.log(e)
    }
    try {
        alarms = await getAlarms(false);
    } catch (e) {
        console.log(e)
    }
    try {
        portionSize = await getPortionSize(false);
    } catch (e) {
        console.log(e)
    }

    return {
        time,
        alarms,
        portionSize
    };
}

async function wake() {
    await bluetoothService.sendData('wake');
    await wait(1300);
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
    loadComplexData
}