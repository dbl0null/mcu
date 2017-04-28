var gstream = require('./build/Release/gstream');

gstream.createStream({
    conferenceId: 'test',
    defaultView: 'test'
}, (err, stream) => {
    console.log(err, stream);
    if(!err) {
        stream.addRobot({
            robot_id: 'roboti1',
            image: 'image1'
        }, (err, robot) => {
            console.log(err, robot);
        });
    }
});
