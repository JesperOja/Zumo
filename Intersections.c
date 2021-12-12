//Intersections: Week 4, Assignment 3

void zmain(void)
{
    motor_start();
    motor_forward(0,0);
    struct sensors_ dig;
    int counter = 0;
    IR_Start();
    reflectance_start();
    reflectance_set_threshold(13000, 13000, 13000, 13000, 13000, 13000);
	printf("Waiting for button presh");
    while(SW1_Read() == 1){
        vTaskDelay(10);    
    }
    
    motor_forward(50,10);
    while(true)
    {  
        reflectance_digital(&dig); 
		 	
        if(dig.L3 == 1 && dig.R3 == 1 && counter == 0){
            motor_forward(0,0);
            counter++;
            IR_wait();
            motor_forward(50,500);
        }else if(dig.R3 == 0 && dig.L3 == 0 && dig.L1 == 1 && dig.R1 == 1 && dig.L2 == 0 && dig.R2 == 0){
            motor_forward(80,10);
        }else if(dig.R3 == 1 && dig.L3 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.L2 == 1 && dig.R2 == 1){
            if(counter == 1){
            counter++;
            motor_turn(10,80,250);
            }else if (counter==4){
                motor_forward(0,0);
            }
            else
            {
                counter++;
                motor_turn(80,10,250);
            }
        }
        vTaskDelay(50);
    }
}   