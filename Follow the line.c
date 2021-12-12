//Follow the line : Week 4, Assignment 2
void zmain(void)
{
    motor_start();
    motor_forward(0,0);
    struct sensors_ dig;
    IR_Start();
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    IR_wait();
    motor_forward(80,10);
    while(true)
    {
        reflectance_digital(&dig); 
       
        if(dig.L3 == 1 && dig.R3 == 1){
            motor_forward(0,0);
            IR_wait();
            motor_forward(80,200);
        }else if(dig.L1 == 1 && dig.R1 == 1){
            motor_forward(80,10);
        }
        else if(dig.R1 == 1 && dig.R2 == 1){
            motor_turn(60,20,10);
        }
        else if(dig.R2 == 1 && dig.R3 == 1){
            motor_turn(60,5,10);
        }
        else if(dig.L1 == 1 && dig.L2 == 1){
            motor_turn(20,60,10);
        }
        else if(dig.L2 == 1 && dig.L3 == 1){
            motor_turn(5,60,10);
        }
        vTaskDelay(10);
    }
}   