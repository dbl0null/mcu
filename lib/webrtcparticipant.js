'use strict';

class WebRTCParticipant {

    constructor(options) {
        this.participant_id = options.participant_id;
        this.offer = options.offer;
        this.iceCandidates = options.iceCandidates;
    }
}

module.exports = WebRTCParticipant;
