//reflectance : Week 4, Assignment 1

void zmain(void)
{
    motor_start();
    motor_forward(0,0);
	int counter = 0;
    struct sensors_ dig;
    IR_Start();
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    IR_wait();
    motor_forward(140,10);
    while(true)
    {
		
        reflectance_digital(&dig); 
       
        if(dig.L3 == 1 && dig.R3 == 1 && counter == 0){
            motor_forward(0,0);
            IR_wait();
			counter++;
            motor_forward(140,10);
        }
        else if(dig.L3 == 1 && dig.R3 == 1 && counter == 80){
			motor_forward(0,0);
		}
		else if(dig.L3 == 1 && dig.R3 == 1){
			counter++;
		}
		
        vTaskDelay(10);
    }
}   