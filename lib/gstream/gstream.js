var gstream = require('./build/Release/gstream');

// var stream = gstream.createStream({
//     conferenceId: 'test',
//     defaultView: 'test'
// });
// console.log(stream);

var stream = gstream.createStream(2);
console.log(stream);


//
// gstream.createStream({
//     conferenceId: 'test',
//     defaultView: 'test'
// }, (err, stream) => {
//     if(err) {
//         console.log(err);
//     } else {
//         stream.addRobot({
//             robot_id: 'roboti1',
//             image: 'image1'
//         }, (err, robot) => {
//             console.log(err, robot);
//         });
//     }
// });
