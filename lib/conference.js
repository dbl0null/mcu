'use strict';

var promise = require('bluebird'),
    gstream = require('gstream');

class Conference {

    constructor(options) {
        this.conference_id = options.conference_id;
        this.defaultView = options.defaultView;
    }

    ensureStream() {
        var self = this;
        return new promise((resolve, reject) => {
            if(!self.stream) {
                self.stream = gstream.createStream(self, (err, stream) => {
                    if(err) {
                        reject(err);
                    } else {
                        self.stream = stream;
                        resolve();
                    }
                });
            }
            return resolve();
        });
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
