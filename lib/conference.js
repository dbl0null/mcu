'use strict';

var promise = require('bluebird'),
    gstream = require('../build/Release/gstream'),
    transform = require('sdp-transform');

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
                        resolve();
                    }
                });
            });
        });
    }

    joinWebRTCParticipant(participant) {
        var self = this;
        var offer transform.parse(participant.offer);
        // generate answer
        var answer = '';
        
        return this.ensureStream().then(() => {
            return new promise((resolve, reject) => {
                self.stream.addWebRTCParticipant(participant, (err) => {
                    console.log(err);
                    if(err) {
                        console.log(err);
                        reject(err);
                    } else {
                        resolve(answer);
                    }
                });
            });
        });
    }
}

module.exports = Conference;
