#include "robot.h"

void setup_robot(struct Robot *robot){

//        // default
//       robot->x = OVERALL_WINDOW_WIDTH/2-50;
//        robot->y = OVERALL_WINDOW_HEIGHT-50;
//        robot->true_x = OVERALL_WINDOW_WIDTH/2-50;
//        robot->true_y = OVERALL_WINDOW_HEIGHT-50;
//        robot->width = ROBOT_WIDTH;
//        robot->height = ROBOT_HEIGHT;
//        robot->direction = 0;
//        robot->angle = 0;
//        robot->startupCounter = 0;
//        robot->stillCounter = 0;

     //maze 1
    robot->x = 270;
    robot->y = 460;
    robot->true_x = 270;
    robot->true_y = 460;
    robot->width = ROBOT_WIDTH;
    robot->height = ROBOT_HEIGHT;
    robot->direction = 0;
    robot->angle = 0;
    robot->currentSpeed = 0;
    robot->crashed = 0;
    robot->auto_mode = 0;
        robot->startupCounter = 0;
        robot->stillCounter = 0;


//        //Maze 5
//
//    robot->x = 170;
//    robot->y = 460;
//    robot->true_x = 170;
//    robot->true_y = 460;
//    robot->width = ROBOT_WIDTH;
//    robot->height = ROBOT_HEIGHT;
//    robot->direction = 0;
//    robot->angle = 0;
//    robot->currentSpeed = 0;
//    robot->crashed = 0;
//    robot->auto_mode = 0;



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

    if(robot->currentSpeed == 0) {
        robot->stillCounter++;
    }

    if(robot->startupCounter < 5 ) {

        // Robot Startup. When the robot begins, it will do a short period of going forward, with a left turn every 3 ticks. It does this for 4 ticks. (ie it does 3 UPs and 1 LEFT.)
        if((robot->startupCounter % 2) == 1) {
            printf("LEFT");
            robot->direction = LEFT;
            robot->startupCounter++;
        } else {
            printf("UP");
            robot->direction = UP;
            robot->startupCounter++;
        }
        } else {
            // If the robot has been still for more than 2 ticks, get unstuck. This uses the robot's alternator to switch between 2 UP ticks and 1 RIGHT tick.
            if(robot->stillCounter > 1) {
                printf("too still\n");
                        if(robot->alternator == 1 || robot->alternator == 2) {
                                robot->direction = UP;
                                if(robot->alternator == 2) {
                                    robot->alternator = 0;
                                    printf("alt down2\n");
                                } else {
                                    robot->alternator = 2;
                                    printf("alt down1\n");
                                }
                            } else {
                                robot->direction = RIGHT;
                                robot->alternator = 1;
                                printf("alt right\n");
                            }

                        robot->stillCounter = 0;

            } // If both left sensors are close, but the robot is a slight distance away, slow down.
            else if(left_sensor == 3 && front_left_diagonal_sensor == 4 && front_right_sensor == 0 && front_right__diagonal_sensor == 0) {
                printf("down\n");
                if(robot->currentSpeed != 0) {
                    robot->direction = DOWN;
                }

            } // If the robot is right up against the wall, about to hit, slow down and even reverse a bit (as if it turns right it will hit the wall).
            else if(front_left_diagonal_sensor == 4 && left_sensor == 4 && front_right_sensor == 2 && front_right__diagonal_sensor == 0) {
                if(robot->currentSpeed != -1) {
                    robot->direction = DOWN;
                }
            } // Robot must slow down instead of turning right to not hit wall
            else if( front_left_diagonal_sensor == 4 && front_right_sensor == 4 && front_right__diagonal_sensor == 2 && left_sensor == 4) {
                printf("right\n");
                if(robot->currentSpeed != 0) {
                    robot->direction = DOWN;
                }
            }  // Robot must slow down instead of turning right to not hit wall
            else if(front_left_diagonal_sensor == 3 && front_right_sensor == 3 && front_right__diagonal_sensor == 3 && left_sensor == 4) {
                if(robot->currentSpeed != 0) {
                    robot->direction = DOWN;
                }
            } // Robot must slow down instead of turning right to not hit wall
            else if(front_right__diagonal_sensor == 2 && left_sensor == 1 && front_left_diagonal_sensor == 0 && front_right_sensor == 0) {
                printf("down\n");
                if(robot->currentSpeed != 0) {
                    robot->direction = DOWN;
                }
            } // Specific set of conditions, where the robot should turn right instead of slowing down to allow it to turn in time & not hit the wall.
            else if(front_left_diagonal_sensor == 2 && front_right_sensor == 3 && front_right__diagonal_sensor == 2 && left_sensor == 1) {
                printf("right\n");
                robot->direction = RIGHT;
            } // Specific set of conditions, where the robot should turn right instead of slowing down to allow it to turn in time & not hit the wall.
            else if(front_left_diagonal_sensor == 4 && left_sensor == 3 && front_right_sensor == 0 && front_right__diagonal_sensor == 0) {
                printf("right\n");
                robot->direction = RIGHT;
            } // If the left diag sensor detects something, but isnt too close, and the left sensor is low/off, slow down
            else if((front_left_diagonal_sensor == 1 || front_left_diagonal_sensor == 2) && (left_sensor == 0 || left_sensor == 1)) {
                printf("down\n");
                if(robot->currentSpeed != 0) {
                    robot->direction = DOWN;
                }
            } // If sensors detect something on the front/right, but nothing on the left, turn left to avoid.
            else if(front_right_sensor == 1 && front_right__diagonal_sensor == 1 && left_sensor == 0 && front_left_diagonal_sensor == 0) {
                printf("left\n");
                robot->direction = LEFT;
            } // If the robot is a certain distance away from the wall, and nothing is on the right, go forward
            else if(left_sensor == 4 && front_left_diagonal_sensor == 3 && (front_right_sensor == 0)  && (front_right__diagonal_sensor == 0)) {
                printf("up\n");
                if(robot->currentSpeed < 6) {
                   robot->direction = UP;
                }
            } // Slow down when robot approaches wall when already very close.
            else if(left_sensor == 4 && front_left_diagonal_sensor == 3 && (front_right_sensor == 1 || front_right_sensor == 2 || front_right_sensor == 3)  && (front_right__diagonal_sensor == 0 || front_right__diagonal_sensor == 1 || front_right__diagonal_sensor == 2)) {
                if(robot->currentSpeed != 0) {
                        printf("slow down\n");
                   robot->direction = DOWN;
                }
            } // If the front right diagonal sensor is greater than 2 (close), and the left sensors arent too close, turn left
            else if(front_right__diagonal_sensor > 2 && front_left_diagonal_sensor < 2 && front_right_sensor < 5 && left_sensor < 2) {
                printf("down\n");
                robot->direction = LEFT;
            } // If the robot is too close on the left, go right
            else if(left_sensor > 3 || front_left_diagonal_sensor > 3) {
                robot->direction = RIGHT;
                printf("right\n");
            } // If nothing is detected, turn left
            else if(left_sensor == 0 && front_left_diagonal_sensor == 0 && front_right_sensor == 0 && front_right__diagonal_sensor == 0) {
                robot->direction = LEFT;
                printf("left\n");
            } // If the front detects nothing, but the left sensors are both 3 (most likely a corner), turn right
            else if(front_left_diagonal_sensor == 3 && front_right_sensor == 0 && left_sensor == 3) {
                printf("right\n");
                robot->direction = RIGHT;
            } // Often stuck position. If left sensor is close, front left diag is slightly far, and front sensor is close but not too close, go right.
            else if(left_sensor == 3 && front_left_diagonal_sensor == 1 && (front_right_sensor == 0 || front_right_sensor == 1 || front_right_sensor == 2)) {
                printf("stuck right\n");
                    robot->direction = RIGHT;
            } // Enter the series of conditions for when the front sensor detects anything
            else if(front_right_sensor >= 1) {
                    printf("enter front >= 1");
                    // If both diagonal sensors detect something, but the left detects nothing, go right (it is approaching head on.)
                    if((front_left_diagonal_sensor >= 1 ||front_right__diagonal_sensor >= 1) && left_sensor == 0) {
                        robot->direction = RIGHT;
                    } // If all sensors detect something, and left sensors detect something close, slow down, and turn right when still.
                    else if(left_sensor > 2 && front_left_diagonal_sensor > 1 && front_right_sensor > 0 && front_right__diagonal_sensor > 0) {
                            if(robot->currentSpeed != 0) {
                                robot->direction = DOWN;
                            } else {
                                robot->direction = RIGHT;
                            }

                    } // If left sensors are greater than certain value, or left sensor and front sensor are specific values, alternate between slowing down & turning right.
                    else if((left_sensor >= 2 && front_left_diagonal_sensor >= 2) || (front_left_diagonal_sensor == 3 && front_right_sensor == 2)) {

//                        robot->direction = RIGHT;
                        printf("stuck go right\n");
                        if(robot->alternator == 1 || robot->alternator == 2) {
                            if(robot->currentSpeed != 0) {
                                    robot->direction = DOWN;
                                if(robot->alternator == 2) {
                                    robot->alternator = 0;
                                    printf("alt down2\n");
                                } else {
                                    robot->alternator = 2;
                                    printf("alt down1\n");
                                }
                            } else {
                                robot->alternator = 0;
                            }
                        } else {
                                robot->direction = RIGHT;
                                robot->alternator = 1;
                                printf("alt right\n");
                        }
                    } // to be honest, I'm not sure if this runs or not
                     else if(robot->alternator == 1 || robot->alternator == 2) {
                            if(robot->currentSpeed != 0) {
                                robot->direction = DOWN;
                                if(robot->alternator == 2) {
                                    robot->alternator = 0;
                                    printf("alt down2\n");
                                } else {
                                    robot->alternator = 2;
                                    printf("alt down1\n");
                                }
                            }

                        robot->alternator = 0;

                    } else {
                        if(robot->currentSpeed != 0) {
                            robot->direction = DOWN;
                            robot->alternator = 1;
                            printf("alt right\n");
                        }

                    }
            } // If the left sensor is a specific distance, and nothing else is detected, go right.
            else if(left_sensor == 2 && front_left_diagonal_sensor == 0 && front_right_sensor == 0 && front_right__diagonal_sensor == 0) {
                printf("edge case right");
                robot->direction = RIGHT;
            } // If nothing else is detected, go left
            else if(front_left_diagonal_sensor == 0 && front_right_sensor == 0 && front_right__diagonal_sensor == 0) {
                printf("nothing else left");
                robot->direction = LEFT;
            } // If bot is certain distance from the wall & straight, speed up to ceratin speed
            else if((front_left_diagonal_sensor == 2 && left_sensor == 3) ||(front_left_diagonal_sensor == 3 && left_sensor == 4)) {
                // Can do maze 0 faster on 6, cant do maze 1 on 6
                if(robot->currentSpeed < 5) {
                    robot->direction = UP;
                }

            } // If right diag sensor detects something, but nothing else, go left.
            else if(front_right__diagonal_sensor > 0 && front_left_diagonal_sensor == 0 && left_sensor == 0 && front_right_sensor == 0) {
                printf("slight on right, go left\n");
                robot->direction = LEFT;
            }
            printf("\n");

        }



}
