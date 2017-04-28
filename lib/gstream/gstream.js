var gstream = require('bindings')('gstream');

gstream.createStream({
    conferenceId: 'test',
    defaultView: 'test'
}, function (err, stream) {
    console.log(stream);
});
