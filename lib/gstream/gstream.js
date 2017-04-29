var gstream = require('./build/Release/gstream');

var stream = gstream(10);
console.log(stream.plusOne());

// stream.addRobot({
//     robot_id: 'roboti1',
//     image: 'image1'
// }, (err, robot) => {
//     console.log(err, robot);
// });
