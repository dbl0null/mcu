'use strict';

var _ = require('lodash'),
    transform = require('sdp-transform');

class SDP {

    static buildWebRTCConfig(offer) {
        var sdpOffer = transform.parse(offer);
        var config = [];
        _.each(sdpOffer.media, function(media){
          config
        });
        return config;
    }

    static buildWebRTCAnswer(offer) {
        var answer = '';
        return answer;
    }


}

module.exports = SDP;
