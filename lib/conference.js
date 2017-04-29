'use strict';

var promise = require('bluebird'),
    gstream = require('../build/Release/gstream');

class Conference {

    constructor(options) {
        this.conferenceId = options.conferenceId;
        this.defaultView = options.defaultView;
    }

    ensureStream() {
        var self = this;
        if(!self.stream) {
            self.stream = gstream.createStream(self);
        }
        return promise.resolve();
    }

    joinRobot(robot) {
        var self = this;
        return this.ensureStream().then(() => {
            return new promise((resolve, reject) => {
                self.stream.addRobot(robot, (err, stream) => {
                    if(err) {
                        reject(err);
                    } else {
                        self.stream = stream;
                        resolve();
                    }
                });
            });
        });
    }
}

module.exports = Conference;
