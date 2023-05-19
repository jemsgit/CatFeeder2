# Cat Feeder

This project was made to schedule feeding of my cat. You can set up scheduler or feed cat via bluetooth module HM-10 or via internet with ESP module and your server (or router port forwarding).

Folder hardware contains arduino sketch for bluetooth powersave configuration and arduino + esp sketches for bluetooth + wi-fi configuration. Also it contains WORKING libraries with some bugfixing which I've found after ALL INTERNET FORUMS INVESTIGATION!) Put it to you arduino home folder.

In web folder there is an cordova application for smatphone.

## Usage
Install deps

`npm i`

For webpack build (with live reload, stay it in separeted cmd tab)

`cordova build -- --webpackConfig webpack.config.js --livereload`

For cordova build

`cordova run android --device`

Check adb devices (need adb to be installed) to be able to install and debug application

`./adb devices`
