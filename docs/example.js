// reference mcu module
var mcu = require('mcu');

// create a conference instance
var conference = new mcu.Conference({
    name: 'test conference',
    code: 'test',
    pin: '1234',
    defaultView: 'composite'
});

// join robot
var robot = new mcu.Robot({
    name: 'robot',
    image: 'http://placehold.it/1280x720' // url or local file path
});

robot.join(conference).then(() => {
    console.log('robot successfully joined.');
});

// join webrtc participant
var participant = new mcu.WebRTCParticipant({
    name: 'robot',
    offer: '', // get offer from RTCPeerConnection
    iceCandidates: [] // get initial ice candidates from RTCPeerConnection
});

participant.join(conference).then((answer) => {
    // send answer to remote RTCPeerConnection
    // answer will include iceCandidates for mcu
    console.log('webtrc participant joined sucessfully.');
});
