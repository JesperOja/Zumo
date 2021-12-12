//Robot tank turning functions
void tank_turn_left (void)
{
    int r = rand();							//Getting huge random number
    r= r%300;								//changeing random number to be 1-300
    motor_backward(60,180);
    SetMotors(1,0,60,100,(600-r));			//Setting turn to be random from ~90 decrees - ~160 decrees
}
    
void tank_turn_right (void)
{
    int r = rand();
    r= r%300;
    motor_backward(60,180);
    SetMotors(0,1,100,60,(600-r));
}
void getting_started(void)												//Setting all needed fuctions on
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

void zmain(void)
{
    getting_started();
    struct sensors_ dig;
    print_mqtt("Group7/initialization", "Component initialization complete.\nWaiting for user input.");
    while (SW1_Read()==1)
    {
         vTaskDelay(10);
    }
    bool line_encountered = false;
    while(!line_encountered)
    {
        motor_forward(50,100);
        reflectance_digital(&dig);
        if (dig.L3+dig.L2+dig.L1+dig.R1+dig.R2+dig.R3 > 3)      //If a line is encountered, more than 3 sensors will
        {                                                       //give a digital output of 1
            line_encountered = true;
            motor_forward(0,0);
        }
    }
    print_mqtt("Group7/ready", "zumo");
    IR_wait();

    int start = xTaskGetTickCount();
    print_mqtt("Group7/start", "%d",start);
    motor_forward(50,500);
    while(true)
    {
        int d = Ultra_GetDistance();
        reflectance_digital(&dig);
		
        if(SW1_Read() == 0)
		{   
            int stop = xTaskGetTickCount();
            int run = stop-start;
			print_mqtt("Group7/stop", "%d", stop);
            print_mqtt("Group7/time", "%d", run);
			motor_forward(0,0);
            IR_wait();
            break;
		}
        else if(d<4)
        {
            int time = xTaskGetTickCount();
            print_mqtt("Group7/obstacle", "%d", time);
            int r = rand();
            if((r%2) == 0){									//Will turn left or right at random when near obstacle
                tank_turn_left(); 						
            }
            else{
                tank_turn_right();
            }
        }
        else if ((dig.R2 || dig.R3) == 1)					//If either of the rightmost sensor are at the outer lines, turning left
        {
            tank_turn_left();
        }
		else if ((dig.L3 || dig.L2) == 1)
		{
            tank_turn_right();
        }
        else
            drive_straight();
    }
    while(true)
    {
        vTaskDelay(100000);
    }
}