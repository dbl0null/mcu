'use strict';

class WebRTCParticipant {

    constructor(options) {
        this.participant_id = options.participant_id;
        this.offer = options.offer;
        this.iceCandidates = options.iceCandidates;
    }

    join(conference) {

        this.conference = conference;
        return conference.joinWebRTCParticipant(this).then((answer)=>{
            return answer;
        });
    }
}

module.exports = WebRTCParticipant;
