import fs from 'fs';
import remote from 'remote';
const dialog = remote.require('dialog');


export function openJsonFile() {
    const options = {
        title: "Load Profile",
        properties: ["openFile"],
    };
    return new Promise((resolve, reject) => {
        dialog.showOpenDialog(options, (fileNames) => {
            if (fileNames === undefined) {
                return reject("No files selected");
            }
            const fileName = fileNames[0];
            fs.readFile(fileName, 'utf8', (err, data) => {
                console.log(data);
                if (err) {
                    return reject(err);
                } 
                return resolve(JSON.parse(data));
            });
        });
    });
}

export function saveJsonFile(data) {
    const options = {
        title: "Save Profile",
    };
    return new Promise((resolve, reject) => {
        dialog.showSaveDialog(options, (fileName) => {
            if (fileName === undefined) {
                return reject("No files selected");
            } 
            fs.writeFile(fileName, JSON.stringify(data), (err) => {
                if (err) {
                    return reject(err);
                } 
                return resolve();
            });
        });
    });
}
