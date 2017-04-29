var gstream = require('./build/Release/gstream');

var stream = gstream({
    conferenceId: 'conference1',
    defaultView: 'robot'
});
console.log(stream);

stream.addRobot({
    robot_id: 'roboti1',
    image: 'image1'
}, (err, robot) => {
    console.log(err, robot);
});
