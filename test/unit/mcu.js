'use strict';

var promise = require('bluebird'),
    expect = require('chai').expect,
    chance = require('chance').Chance(),
    mcu = require('../../lib/mcu');

describe('mcu', () => {

    describe('initialize', () => {

        it('Conference', () => {

            var conference = new mcu.Conference({
                conference_id: chance.word(),
                defaultView: chance.word()
            });
            expect(conference).to.exist;
            return promise.resolve();
        });

        it('Robot', () => {

            var robot = new mcu.Robot({
                robot_id: chance.word(),
                image: chance.word()
            });
            expect(robot).to.exist;
            return promise.resolve();
        });

        it('WebRTCParticipant', () => {

            var participant = new mcu.WebRTCParticipant({
                participant_id: chance.word(),
                offer: chance.word(),
                iceCandidates: []
            });
            expect(participant).to.exist;
            return promise.resolve();
        });
    });
});
