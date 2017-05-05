var express = require('express');
var app = express();
var bodyParser = require('body-parser');
var mcu = require('../../lib/mcu');

var conferences = {};

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
    extended: true
}));

app.get('/', function (req, res) {
    res.sendFile(__dirname + '/index.html');
});

app.post('/join', function (req, res) {

    var conference = conferences[req.body.conference_id];
    if(!conference) {
        conference = new mcu.Conference({
            conferenceId: req.body.conference_id,
            defaultView: 'composite'
        });
    }

    var participant = new mcu.WebRTCParticipant({
        participant_id: req.body.participant_id,
        offer: req.body.offer,
        iceCandidates: req.body.iceCandidates
    });

    // console.log('join participant', participant);

    participant.join(conference).then((answer) => {
        console.log('join with answer.', answer);
        res.json({
            answer: answer
        });
    });

});

app.listen(3000);
