#include "robot.h"

void setup_robot(struct Robot *robot){
    int mazeOpt = 0;
    if(mazeOpt == 0) {
       robot->x = OVERALL_WINDOW_WIDTH/2-50;
        robot->y = OVERALL_WINDOW_HEIGHT-50;
        robot->true_x = OVERALL_WINDOW_WIDTH/2-50;
        robot->true_y = OVERALL_WINDOW_HEIGHT-50;
        robot->width = ROBOT_WIDTH;
        robot->height = ROBOT_HEIGHT;
        robot->direction = 0;
        robot->angle = 0;
    } else if(mazeOpt == 1) {
        robot->x = OVERALL_WINDOW_WIDTH/2-50;
        robot->y = OVERALL_WINDOW_HEIGHT-50;
        robot->true_x = OVERALL_WINDOW_WIDTH/2-190;
        robot->true_y = OVERALL_WINDOW_HEIGHT-50;
        robot->width = ROBOT_WIDTH;
        robot->height = ROBOT_HEIGHT;
        robot->direction = 0;
        robot->angle = 0;
    } else if(mazeOpt == 2) {
        robot->x = 50;
        robot->y = OVERALL_WINDOW_HEIGHT-50;
        robot->true_x = 50;
        robot->true_y = OVERALL_WINDOW_HEIGHT-50;
        robot->width = ROBOT_WIDTH;
        robot->height = ROBOT_HEIGHT;
        robot->direction = 0;
        robot->angle = 0;
        robot->currentSpeed = 0;
        robot->crashed = 0;
        robot->auto_mode = 0;
    } else if(mazeOpt == 3) {
        robot->x = 150;
        robot->y = 240;
        robot->true_x = 150;
        robot->true_y = 240;
        robot->width = ROBOT_WIDTH;
        robot->height = ROBOT_HEIGHT;
        robot->direction = 0;
        robot->angle = 180;
        robot->currentSpeed = 0;
        robot->crashed = 0;
        robot->auto_mode = 0;
    }

    robot->currentSpeed = 0;
    robot->crashed = 0;
    robot->auto_mode = 0;

    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
}
int robot_off_screen(struct Robot * robot){
    if(robot->x < 0 || robot-> y < 0){
        return 0;
    }
    if(robot->x > OVERALL_WINDOW_WIDTH || robot->y > OVERALL_WINDOW_HEIGHT){
        return 0;
    }
    return 1;
}

int checkRobotHitWall(struct Robot * robot, struct Wall * wall) {

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotHitWalls(struct Robot * robot, struct Wall_collection * head) {
   struct Wall_collection *ptr = head;
   int hit = 0;

   while(ptr != NULL) {
      hit = (hit || checkRobotHitWall(robot, &ptr->wall));
      ptr = ptr->next;
   }
   return hit;

}

int checkRobotReachedEnd(struct Robot * robot, int x, int y, int width, int height){

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 x,width,y,height);

    return overlap;
}

void robotCrash(struct Robot * robot) {
    robot->currentSpeed = 0;
    if (!robot->crashed)
        printf("Ouchies!!!!!\n\nPress space to start again\n");
    robot->crashed = 1;
}

void robotSuccess(struct Robot * robot, int msec) {
    robot->currentSpeed = 0;
    if (!robot->crashed){
        printf("Success!!!!!\n\n");
        printf("Time taken %d seconds %d milliseconds \n", msec/1000, msec%1000);
        printf("Press space to start again\n");
    }
    robot->crashed = 1;
}

int checkRobotSensor(int x, int y, int sensorSensitivityLength, struct Wall * wall)  {
    //viewing_region of sensor is a square of 2 pixels * chosen length of sensitivity
    int overlap = checkOverlap(x,2,y,sensorSensitivityLength,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotSensorFrontRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorLeftAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5);

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2 + 12.5)*cos((robot->angle - 90)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle - 90)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2 + 12.5)*sin((robot->angle - 90)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle - 90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorFrontLeftDiagonalAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5);

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2 + 10)*cos((robot->angle - 45)*(PI)/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle- 45)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2 + 12)*sin((robot->angle - 45)*(PI)/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle- 45)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorFrontRightDiagonalAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+(ROBOT_WIDTH/2 - 10)*cos((robot->angle + 45)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle+45)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2 - 12)*sin((robot->angle + 45)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle+45)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

void robotUpdate(struct SDL_Renderer * renderer, struct Robot * robot){
    double xDir, yDir;

    int robotCentreX, robotCentreY, xTR, yTR, xTL, yTL, xBR, yBR, xBL, yBL;
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);


    //Other Display options:
    // The actual square which the robot is tested against (not so nice visually with turns, but easier
    // to test overlap
    SDL_Rect rect = {robot->x, robot->y, robot->height, robot->width};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);

    /*
    //Center Line of Robot. Line shows the direction robot is facing
    xDir = -30 * sin(-robot->angle*PI/180);
    yDir = -30 * cos(-robot->angle*PI/180);
    xDirInt = robot->x+ROBOT_WIDTH/2+ (int) xDir;
    yDirInt = robot->y+ROBOT_HEIGHT/2+ (int) yDir;
    SDL_RenderDrawLine(renderer,robot->x+ROBOT_WIDTH/2, robot->y+ROBOT_HEIGHT/2, xDirInt, yDirInt);
    */

    //Rotating Square
    //Vector rotation to work out corners x2 = x1cos(angle)-y1sin(angle), y2 = x1sin(angle)+y1cos(angle)
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTR = (int) xDir;
    yTR = (int) yDir;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBR = (int) xDir;
    yBR = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBL = (int) xDir;
    yBL = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTL = (int) xDir;
    yTL = (int) yDir;

    SDL_RenderDrawLine(renderer,xTR, yTR, xBR, yBR);
    SDL_RenderDrawLine(renderer,xBR, yBR, xBL, yBL);
    SDL_RenderDrawLine(renderer,xBL, yBL, xTL, yTL);
    SDL_RenderDrawLine(renderer,xTL, yTL, xTR, yTR);

    //Front Sensor
    int sensor_sensitivity =  floor(SENSOR_VISION/5);
    int i;
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(ROBOT_WIDTH/2 - 10)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2 - 10)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    // Front Left Diag Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2 + 10)*cos((robot->angle - 45)*(PI)/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle- 45)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2 + 12)*sin((robot->angle - 45)*(PI)/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle- 45)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    // Front Right Diag Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(ROBOT_WIDTH/2 - 10)*cos((robot->angle + 45)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle+45)*PI/180));
        yDir = round(robotCentreY+(ROBOT_WIDTH/2 - 12)*sin((robot->angle + 45)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle+45)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

    //Front Left Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+(-ROBOT_WIDTH/2 + 12.5)*cos((robot->angle)*PI/180 - 90)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*sin((robot->angle - 90)*PI/180));
        yDir = round(robotCentreY+(-ROBOT_WIDTH/2 + 12.5)*sin((robot->angle)*PI/180 - 90)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*i)*cos((robot->angle - 90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

//    // Back Sensor
//    for (i = 0; i < 5; i++)
//    {
//        xDir = round(robotCentreX+(ROBOT_WIDTH/2 - 10)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*-i + 75)*sin((robot->angle)*PI/180));
//        yDir = round(robotCentreY+(ROBOT_WIDTH/2 - 10)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*-i + 75)*cos((robot->angle)*PI/180));
//        xTL = (int) xDir;
//        yTL = (int) yDir;
//
//        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
//        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
//        SDL_RenderDrawRect(renderer, &rect);
//        SDL_RenderFillRect(renderer, &rect);
//    }
//
//    // Back Right Diag Sensor
//    for (i = 0; i < 5; i++)
//    {
//        xDir = round(robotCentreX+(-ROBOT_WIDTH/2 + 10)*cos((robot->angle - 45)*(PI)/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*-i + 80)*sin((robot->angle- 45)*PI/180));
//        yDir = round(robotCentreY+(-ROBOT_WIDTH/2 + 12)*sin((robot->angle - 45)*(PI)/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*-i + 80)*cos((robot->angle- 45)*PI/180));
//        xTL = (int) xDir;
//        yTL = (int) yDir;
//
//        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
//        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
//        SDL_RenderDrawRect(renderer, &rect);
//        SDL_RenderFillRect(renderer, &rect);
//    }
//
//    // back Left Diag Sensor
//    for (i = 0; i < 5; i++)
//    {
//        xDir = round(robotCentreX+(ROBOT_WIDTH/2 - 10)*cos((robot->angle + 45)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*-i + 80)*sin((robot->angle+45)*PI/180));
//        yDir = round(robotCentreY+(ROBOT_WIDTH/2 - 12)*sin((robot->angle + 45)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensor_sensitivity*-i + 80)*cos((robot->angle+45)*PI/180));
//        xTL = (int) xDir;
//        yTL = (int) yDir;
//
//        SDL_Rect rect = {xTL, yTL, 2, sensor_sensitivity};
//        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
//        SDL_RenderDrawRect(renderer, &rect);
//        SDL_RenderFillRect(renderer, &rect);
//    }
}



void robotMotorMove(struct Robot * robot) {
    double x_offset, y_offset;
    switch(robot->direction){
        case UP :
            robot->currentSpeed += DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed > MAX_ROBOT_SPEED)
                robot->currentSpeed = MAX_ROBOT_SPEED;
            break;
        case DOWN :
            robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed < -MAX_ROBOT_SPEED)
                robot->currentSpeed = -MAX_ROBOT_SPEED;
            break;
        case LEFT :
            robot->angle = (robot->angle+360-DEFAULT_ANGLE_CHANGE)%360;
            break;
        case RIGHT :
            robot->angle = (robot->angle+DEFAULT_ANGLE_CHANGE)%360;
            break;
    }
    robot->direction = 0;
    x_offset = (-robot->currentSpeed * sin(-robot->angle*PI/180));
    y_offset = (-robot->currentSpeed * cos(-robot->angle*PI/180));

    robot->true_x += x_offset;
    robot->true_y += y_offset;

    x_offset = round(robot->true_x);
    y_offset = round(robot->true_y);

    robot->x = (int) x_offset;
    robot->y = (int) y_offset;
}

void robotAutoMotorMove(struct Robot * robot, int front_right_sensor, int front_right__diagonal_sensor, int front_left_diagonal_sensor, int left_sensor) {
    // Might need to tone down speeds and turns for it to be "road legal" ok these dont work anymore but idk
    // 17,3,8
    // 13,4,8
    // NEED TO INCREMENT SPEEDS FOR IT TO BE LEGAL

    int startCounter = 0;


    if(robot->currentSpeed <= 4 && startCounter < 4) {
        robot->currentSpeed++;
        startCounter++;
    }


    if(front_right_sensor >= 1) {
        robot->direction = DOWN;
        printf("slow down\n");
    }






    // Turn right and slow down
    if(front_right_sensor > 0 || front_left_diagonal_sensor > 2 && left_sensor > 1) {

        //robot->angle = (robot->angle+DEFAULT_ANGLE_CHANGE)%360;
        robot->direction = RIGHT;
        //robot->currentSpeed = 3;
        if(robot->currentSpeed > 0) {
            //robot->direction = DOWN;
        } else {
            robot->currentSpeed += DEFAULT_SPEED_CHANGE;
        }
        printf("Turn right & Slow down\n");

    }



    // Turn right and slow down
    else if(front_right_sensor > 0 || front_left_diagonal_sensor > 2 && left_sensor >= 0) {

        //robot->angle = (robot->angle+DEFAULT_ANGLE_CHANGE)%360;
        robot->direction = RIGHT;
        //robot->currentSpeed = 3;
        if(robot->currentSpeed > 0) {
            //robot->direction = DOWN;
        } else {
            robot->currentSpeed += DEFAULT_SPEED_CHANGE;
        }
        printf("Turn right & Slow down\n");

    }

    else if((left_sensor == 1 || left_sensor == 1) && front_left_diagonal_sensor > 0 && front_right_sensor == 0 && front_right__diagonal_sensor == 0) {
        robot->direction = UP;
        printf("up");
    }

    else if(front_left_diagonal_sensor > 1 && front_right__diagonal_sensor > 1 && front_right_sensor == 0) {
        robot->direction = UP;
        printf("Go straight\n");
    }



    // Turn right if getting too close on left
    else if(front_left_diagonal_sensor == 1) {
        if(robot->currentSpeed < 6) {
            //robot->currentSpeed += DEFAULT_SPEED_CHANGE;
        }
        robot->direction = RIGHT;
        printf("right with speed increase\n");
    }

    else if(front_right__diagonal_sensor > 0) {
        robot->direction = LEFT;
        //robot->direction = DOWN;
        printf("left");
    }

    else if(front_right_sensor <= 1 && front_right__diagonal_sensor <= 1 && front_left_diagonal_sensor <= 1 && left_sensor <= 2) {
        robot->direction = LEFT;
        printf("nothing ahead left");
    }



    if(front_right_sensor > 1 && front_right__diagonal_sensor > 1 && front_left_diagonal_sensor < 2 && left_sensor <= 2) {
        robot->direction = LEFT;

    }

    // Zoom if on a straigh
    if((front_left_diagonal_sensor == 2 && front_right_sensor == 0 && front_right__diagonal_sensor == 0) || (front_left_diagonal_sensor == 1 && front_right_sensor == 0 && front_right__diagonal_sensor == 0)) {
        //robot->currentSpeed = 13;
        if(robot->currentSpeed < 8) {
            robot->direction = UP;
        } else {
            robot->direction = DOWN;
        }
        printf("go faster\n");
    }

        // Turn left and slow down but not as much
    if ((front_right_sensor == 0 && front_left_diagonal_sensor == 0) && left_sensor < 1) {
        if (robot->currentSpeed < 2) {
            //robot->currentSpeed += DEFAULT_SPEED_CHANGE;
        }
        //robot->angle = (robot->angle-DEFAULT_ANGLE_CHANGE)%360;
//        if(robot->currentSpeed < 10) {
//            robot->currentSpeed += DEFAULT_SPEED_CHANGE;
//        } else {
//            robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
//        }
        //robot->currentSpeed = 10;
        robot->direction = LEFT;
        printf("turn left\n");
    }

    if(left_sensor > 3) {
        robot->direction = RIGHT;
        printf("wall very close left\n");
    }

    //    if(front_left_diagonal_sensor == 1) {
//        robot->angle = 0;
//    }

//    else if ((robot->currentSpeed>0) && ((front_left_sensor == 1) || (front_right_sensor == 1)) ) {
//        robot->direction = DOWN;
//    }
//    else if ((robot->currentSpeed==0) && ((front_left_sensor == 1) || (front_right_sensor == 1)) ) {
//        robot->direction = LEFT;
//    }
//    else if ((robot->currentSpeed==0) && ((front_left_sensor == 1) || (front_right_sensor == 0)) ) {
//        robot->direction = RIGHT;
//    }
//    else if ((robot->currentSpeed==0) && ((front_left_sensor == 0) || (front_right_sensor == 1)) ) {
//        robot->direction = RIGHT;
//    }
}
