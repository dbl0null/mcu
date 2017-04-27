'use strict';

class Robot {

    constructor(options) {
        this.robot_id = options.robot_id;
        this.image = options.image;
    }

    join(conference) {

        this.conference = conference;
        return conference.joinRobot(this).then(()=>{
            return;
        });
    }
}

module.exports = Robot;
