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
            self.stream = gstream(self);
        }
        return promise.resolve();
    }

    joinRobot(robot) {
        var self = this;
        return this.ensureStream().then(() => {
            return new promise((resolve, reject) => {
                self.stream.addRobot(robot, (err) => {
                    console.log(err);
                    if(err) {
                        console.log(err);
                        reject(err);
                    } else {
                        console.log('asfd');
                        resolve();
                    }
                });
            });
        });
    }
}

module.exports = Conference;
