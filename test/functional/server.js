var express = require('express');
var app = express();
var mcu = require('../../lib/mcu');

app.get('/', function (req, res) {
    res.sendFile(__dirname + '/index.html');
});

app.listen(3000);
