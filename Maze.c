//Defines for MAZE robot
#define UPWARDS upwards=true;
#define LEFT turned_left=true;
#define RIGHT turned_left=false;
#define HORIZONTAL upwards=false;

//Robot turning functions
void forward(void)
{
    motor_forward(50,500);
}

void drive_straight(void)
 {
    motor_forward(80,10);
 }

void turn_left_slight (void)
{
    motor_turn(15,35,10);
}

void turn_left (void)
{
    motor_turn(10,40,10);
}

void turn_right_slight (void)
{
    motor_turn(35,15,10);
}

void turn_right (void)
{
    motor_turn(40,10,110);
}

void tank_turn_left (void)
{
    motor_backward(50,180);
    SetMotors(1,0,50,50,525);
}

void turn_right_max (void)
{
    motor_turn(50,0,10);
}

void turn_left_max (void)
{
    motor_turn(0,50,10);
}

void tank_turn_right (void)
{
    motor_backward(50,180);
    SetMotors(0,1,50,50,525);
}    

void backwards(void)
{
    motor_forward(0,10);
    motor_backward(50,200);
}

void turn_180 (void)
{
    SetMotors(1,0,60,60,900);
}

//Setting all needed robot fuctions on
void getting_started(void)                                                      
{
    print_mqtt("Group7/initialization", "\nRebooted\n");
    Ultra_Start();
    motor_start();          
    motor_forward(0,0);
    IR_Start();
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 12000, 12000, 9000, 9000);
    print_mqtt("Group7/initialization", "Component initialization complete.\nWaiting for user input.");
}

//Main program
void zmain(void)
{
    getting_started();
    struct sensors_ dig;
	
    while (SW1_Read()==1)
    {
         vTaskDelay(10);
    }
    bool line_encountered = false;
    while(!line_encountered)
    {
        motor_forward(80,100);
        reflectance_digital(&dig);
        if (dig.L3+dig.L2+dig.L1+dig.R1+dig.R2+dig.R3 > 3)                      //If a line is encountered, more than 3 sensors will
        {                                                                       //give a digital output of 1
            line_encountered = true;
            motor_forward(0,0);
        }
    }
    print_mqtt("Group7/ready", "maze");
    IR_wait();
    
    int start = xTaskGetTickCount();
    print_mqtt("Group7/start", "%d",start);
    motor_forward(80,400);
    bool line_passed = false;                                                   //Making a boolean for verifying that a line is passed
    bool upwards = true;                                                        //Boolean to check if robot is going upwards
    bool turned_left = false;                                                   //Boolean to check if robot turned left before, if not turn left next
    int x = 0,y = -1;
    while(true)
    {
        reflectance_digital(&dig);
        
        while(!line_encountered)
        {
            vTaskDelay(10);
            int d = Ultra_GetDistance();
            reflectance_digital(&dig);
           
            if(d<6)                                                             //If there is obstacle right after the turn, turn back to keep going straight
            {
                if(!upwards && !turned_left)                                    //If robot is going horizontal and detecting obstacle ahead, turning 180 decrees
                {
                    turn_180();
                    LEFT;
                    HORIZONTAL;
                }
                else if(!upwards && turned_left)                                //If robot is going horizontal and detecting obstacle ahead, turning 180 decrees
                {
                    turn_180();
                    RIGHT;
                    HORIZONTAL;
                }
                else if(turned_left){                                                       
                    tank_turn_right();
                    RIGHT;
                    HORIZONTAL;
                }
                else
                {
                    tank_turn_left();
                    LEFT;
                    HORIZONTAL;   
                }
            }
            else if(y == 13)                                                    //if y coordinate is 13, drive a little bit forward before stopping
            {
                motor_forward(80,600);
                
                int stop = xTaskGetTickCount();
                int run = stop-start;
			    print_mqtt("Group7/stop", "%d", stop);
                print_mqtt("Group7/time", "%d", run);
			    motor_forward(0,0);
                IR_wait();
                
            }
		    else if(dig.L3 == 0 && dig.L2 <= 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 <= 1 && dig.R3 == 0)
	    	{
	    		drive_straight();   
		    }else if (dig.L3+dig.L2+dig.L1+dig.R1+dig.R2+dig.R3 > 3)
            {
                line_encountered = true;
                
                if(upwards)                                                     //If robot is moving upwards while countering line, increase y coordinate by 1,
                    y++;
                else if(!turned_left && !upwards)                               //else change x coordinate by increasing it by one if last turn was right, 
                    x++;
                else if(turned_left && !upwards)                                //or decrease it by 1 if last turn was left
                    x--;
                
                print_mqtt("Group7/position", "%d %d", x,y);
            }else if (dig.L3 == 0 && dig.L2 <= 1 && dig.L1 <= 1 && dig.R1 == 1 && dig.R2 <= 1 && dig.R3 <= 1)
	    	{
	    		turn_right_slight();
	    	}else if (dig.L3 <= 1 && dig.L2 <= 1 && dig.L1 == 1 && dig.R1 <= 1 && dig.R2 <= 1 && dig.R3 == 0)
		    {
		    	turn_left_slight();
	    	}else if (dig.L3 == 0 && dig.L2 == 0 && dig.L1 == 0 && dig.R1 <= 1 && dig.R2 == 1 && dig.R3 <= 1)
	    	{
	    		turn_right();
	    	}else if (dig.L3 <= 1 && dig.L2 == 1 && dig.L1 <= 1 && dig.R1 == 0 && dig.R2 == 0 && dig.R3 == 0)
	    	{
	    		turn_left();
	    	}else if (dig.L3 == 0 && dig.L2 == 0 && dig.L1 == 0 && dig.R1 == 0 && dig.R2 <= 1 && dig.R3 == 1)
		    {
			    turn_right_max();
		    }else if (dig.L3 == 1 && dig.L2 <= 1 && dig.L1 == 0 && dig.R1 == 0 && dig.R2 == 0 && dig.R3 == 0)
	    	{
		    	turn_left_max();
		    }
            else                                                                //If none of the sensors return 1(not on the line) stop the robot
	    	{
			    motor_forward(0,0);
                IR_wait();
		    }
        }
        while (line_encountered)
        {
            while(!line_passed){
                reflectance_digital(&dig);
                motor_forward(50,310);
                int d = Ultra_GetDistance();
                if (dig.L3+dig.L2+dig.L1+dig.R1+dig.R2+dig.R3 <= 3)             //When 3 or less sensors give an output of 1 
                {
                    if(y>= 11)                                                  //When y position is 11 or more, turning left if last turn was right, and vice versa,
                    {                                                           //except when x coordinate is 0, turning to go upwards and keep going straight
                        if(x>0 && !upwards && !turned_left)
                        {
                            tank_turn_left();
                            LEFT;
                            UPWARDS;
                        }
                        else if(x<0 && !upwards && turned_left)
                        {
                            tank_turn_right();
                            RIGHT;
                            UPWARDS;
                        }
                        else if(x>0 && !upwards && turned_left)
                        {
                            tank_turn_right();
                            RIGHT;
                            UPWARDS;
                        }
                        else if(x<0 && !upwards && !turned_left)
                        {
                            tank_turn_left();
                            LEFT;
                            UPWARDS;
                        }
                        else if(x==0 && !upwards && !turned_left)
                        {
                            tank_turn_left();
                            LEFT;
                            UPWARDS;
                        }
                        else if(x==0 && !upwards && turned_left)
                        {
                            tank_turn_right();
                            RIGHT;
                            UPWARDS;
                        }
                        else if(x>0 && upwards)
                        {
                            tank_turn_left();
                            HORIZONTAL;
                            LEFT;
                        }
                        else if(x<0 && upwards)
                        {
                            tank_turn_right();
                            HORIZONTAL;
                             RIGHT;
                        }
                        else if(!upwards && y>= 11 && x==0)
                        {
                            if(turned_left)
                            {
                                tank_turn_right();
                                RIGHT;
                                UPWARDS;
                            }
                            else
                            {
                                tank_turn_left();
                                LEFT;
                                UPWARDS;
                            }
                        }
                        else
                            drive_straight();
                    } 
                    else if(d<=10)                                              //When distance to obstacle is 6 or less
                    {
                        if(x<0 && !upwards && turned_left)                      //If robot is going horizontaly and finds obstacle, turning upwards                                          
                        {
                            tank_turn_right();
                            RIGHT;
                            UPWARDS;
                        }
                        else if(x>0 && !upwards && !turned_left)                //If robot is going horizontaly and finds obstacle, turning upwards                                        
                        {
                            tank_turn_left();
                            LEFT;
                            UPWARDS;
                        }
                        else if(x<0)                                            //If x coordinate is less than 0, and obstacle is found, turning right
                        {
                            tank_turn_right();
                            RIGHT;
                            HORIZONTAL;
                        }
                        else if(x>0)                                            //If x coordinate is less than 0, and obstacle is found, turning left
                        {
                            tank_turn_left();
                            LEFT;
                            HORIZONTAL;
                        }
                        else                                                    //If x coordinate is 0, will take random turn to left or right
                        {
                            int r = rand();
                            r= r%2;
                            if(r==0)
                            {
                                tank_turn_left();
                                LEFT;
                                HORIZONTAL;       
                            }
                            else if(r==1)
                            {
                                tank_turn_right();
                                RIGHT;
                                HORIZONTAL;
                            }
                        }
                    }
                    else if(d>20 && !upwards)                                   //When distance to obstacle is more than 20 and robot is traveling horizontaly
                    {
                        if(turned_left)
                        {
                            tank_turn_right();
                            RIGHT;
                            UPWARDS;
                        }
                        else
                        {
                            tank_turn_left();
                            LEFT;
                            UPWARDS;
                        }
                    }

                    line_passed = true;
                    line_encountered = false;                                   //Resetting the line encounter bool
                }
            }
        }
        if(line_passed)
        {
            line_passed = false;     
        }
    }
 }