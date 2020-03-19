import bluetoothService from '../bluetoothService';
import { wait } from '../utils';

async function getTime() {
    let answer;
    try {
        await wake();
        await bluetoothService.sendData('getTime');
        answer = await bluetoothService.getAnswer();
    } catch (e) {
        console.log(e)
    }
    return answer;
}

async function getAlarms() {
    let answer;
    try {
        await wake();
        await bluetoothService.sendData('getAlarms');
        answer = await bluetoothService.getAnswer();
    } catch (e) {
        console.log(e)
    }
    return answer;
}

async function getPortionSize() {
    let answer;
    try {
        await wake();
        await bluetoothService.sendData('getPortionSize');
        answer = await bluetoothService.getAnswer();
    } catch (e) {
        console.log(e)
    }
    return answer;
}

async function setTime(time) {
    let answer;
    try {
        await wake();
        await bluetoothService.sendData(`setTime-${time}`);
        answer = await bluetoothService.getAnswer();
    } catch (e) {
        console.log(e)
    }
    return answer;
}

async function addAlarm(alarm) {
    let answer;
    try {
        await wake();
        await bluetoothService.sendData(`addAlarm-${alarm}`);
        answer = await bluetoothService.getAnswer();
    } catch (e) {
        console.log(e)
    }
    return answer;
}

async function deleteAlarm(alarmIndex) {
    let answer;
    try {
        await wake();
        await bluetoothService.sendData(`deleteAlarm ${alarmIndex}`);
        answer = await bluetoothService.getAnswer();
    } catch (e) {
        console.log(e)
    }
    return answer;
}

async function setPortionSize(size) {
    await bluetoothService.sendData(`setPortionSize ${size}`);
    let answer = await bluetoothService.getAnswer();
    return answer;
}

async function feed() {
    await bluetoothService.sendData(`wake\r\n`);
    await bluetoothService.sendData(`feed`);
    let answer = await bluetoothService.getAnswer();
    return answer;
}

async function loadComplexData() {
    let time = await getTime();
    let alarms = await getAlarms();
    console.log(time);
    console.log(alarms)
        //let portionSize = await getPortionSize();
    return {
        time,
        alarms,
        portionSize: 30
    }
}

async function wake() {
    await bluetoothService.sendData('wake');
    await wait(1000);
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