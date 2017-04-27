// reference mcu module
var mcu = require('mcu');

// create a conference instance
var conference = new mcu.conference({
    name: 'test conference',
    code: 'test',
    pin: '1234',
    defaultView: 'composite'
});

// create robot
var robot = new mcu.robot({
    name: 'robot',
    image: 'http://placehold.it/1280x720' // url or local file path
});

// join participant
var participant = new mcu.participant({
    name: 'robot',
    sdpOffer: '' // get offer from RTCPeerConnection
    iceCandidates: [] // get initial ice candidates from RTCPeerConnection
});

participant.processOffer(offer).then((answer) => {
    // send answer to remote RTCPeerConnection
    // answer will include iceCandidates for mcu
});

// configure conference
conference.joinRobot(robot).then(() => {
    console.log('robot joined sucessfully.');
    return conference.joinParticipant(participant).then(() => {
        console.log('participant joined sucessfully.');
    });
});
