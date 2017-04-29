'use strict';

var expect = require('chai').expect,
    chance = require('chance').Chance(),
    mcu = require('../../lib/mcu');

describe('robot', () => {

    describe('basic', () => {

        it('join', () => {
            var conference = new mcu.Conference({
                conference_id: chance.word(),
                defaultView: chance.word()
            });
            expect(conference).to.exist;
            var robot = new mcu.Robot({
                robot_id: 'robot1',
                image: 'http://placehold.it/1280x720' // url or local file path
            });
            expect(robot).to.exist;
            return robot.join(conference).then(() => {
                console.log('robot successfully joined.');
            });
        });
    });
});
