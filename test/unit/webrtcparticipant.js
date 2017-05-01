'use strict';

var fs = require('fs'),
    expect = require('chai').expect,
    chance = require('chance').Chance(),
    mcu = require('../../lib/mcu');

describe('WebRTCParticipant', () => {

    describe('basic', () => {

        it('join', () => {
            var conference = new mcu.Conference({
                conference_id: chance.word(),
                defaultView: chance.word()
            });
            expect(conference).to.exist;

            var offer = fs.readFileSync(__dirname + '/assets/webrtc_offer', 'utf8');
            var iceCandidatesAll = fs.readFileSync(__dirname + '/assets/webrtc_ice_candidates', 'utf8');
            var iceCandidates = iceCandidatesAll.split('\n');
            var participant = new mcu.WebRTCParticipant({
                participant_id: chance.word(),
                offer: offer,
                iceCandidates: iceCandidates
            });
            expect(participant).to.exist;
            return participant.join(conference).then((answer) => {
                console.log('participant successfully joined.', answer);
            });
        });
    });
});
