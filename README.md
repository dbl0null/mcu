[![Build Status](https://secure.travis-ci.org/ghafran/mcu.png)](http://travis-ci.org/ghafran/mcu)

# MCU
A multipoint control unit (MCU) to bridge video conferencing. Oh yea, in nodejs.

## Installing
```
npm install mcu
```

## Example

```
// reference mcu module
var mcu = require('mcu');

// create a conference instance
var conference = new mcu.Conference({
    conference_id: 'conference1',
    defaultView: 'composite'
});

// join robot
var robot = new mcu.Robot({
    robot_id: 'robot1',
    image: 'http://placehold.it/1280x720' // url or local file path
});

robot.join(conference).then(() => {
    console.log('robot successfully joined.');
});

// join webrtc participant
var participant = new mcu.WebRTCParticipant({
    participant_id: 'participant1',
    offer: '', // get offer from RTCPeerConnection
    iceCandidates: [] // get initial ice candidates from RTCPeerConnection
});

participant.join(conference).then((answer) => {
    // send answer to remote RTCPeerConnection
    // answer will include iceCandidates for mcu
    console.log('webtrc participant joined sucessfully.', answer);
});

```

# Goals
Single RTCPeerConnection with mcu for group calls
Robot participants for automated interactions

# Development

```
sudo npm install -g gulp node-gyp
node-gyp configure build
```
