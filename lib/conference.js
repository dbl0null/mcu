'use strict';

var promise = require('bluebird'),
    mcu = require('../build/Release/mcu'),
    sdp = require('./sdp');

class Conference {

    constructor(options) {
        this.conferenceId = options.conferenceId;
        this.defaultView = options.defaultView;
    }

    ensureStream() {
        var self = this;
        if(!self.conference) {
            self.conference = mcu(self);
        }
        return promise.resolve();
    }

    joinRobot(robot) {
        var self = this;
        return this.ensureStream().then(() => {
            return new promise((resolve, reject) => {
                self.conference.addRobot(robot, (err) => {
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
        var offer = participant.offer.type === 'offer' ? participant.offer.sdp : participant.offer;
        var config = sdp.buildWebRTCConfig(offer);
        var answer = sdp.buildWebRTCAnswer(config);
        return this.ensureStream().then(() => {
            return new promise((resolve, reject) => {
                self.conference.addWebRTCParticipant(config, (err) => {
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
