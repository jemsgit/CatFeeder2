import moment from 'moment';
import { EMPTY_RESULT } from './bluetoothService';
const activeSign = '**';

export function timeParser(time) {
    if (!time) {
        return null;
    }
    return moment(time, 'DD-MM-YYYY HH:mm:ss').format('HH:mm')
}

export function alarmsParser(alarms) {
    if (!alarms || alarms == EMPTY_RESULT) {
        return [];
    }
    let list = alarms.split('\r\n');
    return list.map(item => {
        let isActive = item.includes(activeSign);
        if (isActive) {
            item = item.replace(activeSign, '')
        }
        let [index, value] = item.split(/\s*-\s*/);
        index = parseInt(index, 10);
        return {
            index,
            value,
            isActive
        }
    })
}