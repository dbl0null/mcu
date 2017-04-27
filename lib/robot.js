'use strict';

class Robot {

    constructor(options) {
        this.robot_id = options.robot_id;
        this.image = options.image;
    }

    join(conference) {

        conference.ensurePipeline();
    }
}

module.exports = Robot;
