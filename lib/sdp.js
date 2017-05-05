'use strict';

var _ = require('lodash'),
    transform = require('sdp-transform');

class SDP {

    static buildWebRTCConfig(offer) {
        var sdpOffer = transform.parse(offer);
        var config = [];
        _.each(sdpOffer.media, function (media) {
            console.log('media offer', media);
        });
        return config;
    }

    static buildWebRTCAnswer(offer) {
        var answer = '';
        return answer;
    }
}

module.exports = SDP;
