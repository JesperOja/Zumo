Assignment 1.

//motor
void zmain(void)
{
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors

    printf("\nStart\n");
    while(SW1_Read() == 1){
        vTaskDelay(10);    
    }
    motor_forward(100,3200);     // moving forward
    motor_turn(205,0,255);     // turn
     motor_forward(100,2570);
     motor_turn(205,0,255);
    motor_forward(100,2570);
    motor_turn(210,0,255);
    motor_turn(210,170,1300);
    motor_forward(100,600);
   
     
    motor_forward(0,0);         // stop motors
    

    motor_stop();               // disable motor controller
    
    while(true)
    {
        vTaskDelay(100);
    }
}