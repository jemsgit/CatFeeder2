import bluetoothService from '../bluetoothService';

async function getTime() {
    await bluetoothService.sendData('getTime');
    let answer = await bluetoothService.getAnswer();
    return answer;
}

async function getAlarms() {
    await bluetoothService.sendData('getAlarms');
    let answer = await bluetoothService.getAnswer();
    return answer;
}

async function getPortionSize() {
    await bluetoothService.sendData('getPortionSize');
    let answer = await bluetoothService.getAnswer();
    return answer;
}

async function setTime(time) {
    await bluetoothService.sendData(`getTime ${time}`);
    let answer = await bluetoothService.getAnswer();
    return answer;
}

async function addAlarm(alarm) {
    await bluetoothService.sendData(`addAlarm ${alarm}`);
    let answer = await bluetoothService.getAnswer();
    return answer;
}

async function deleteAlarm(alarmIndex) {
    await bluetoothService.sendData(`deleteAlarm ${alarmIndex}`);
    let answer = await bluetoothService.getAnswer();
    return answer;
}

async function setPortionSize(size) {
    await bluetoothService.sendData(`setPortionSize ${size}`);
    let answer = await bluetoothService.getAnswer();
    return answer;
}

async function feed() {
    await bluetoothService.sendData(`feed`);
    let answer = await bluetoothService.getAnswer();
    return answer;
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
    loadComplexData
}