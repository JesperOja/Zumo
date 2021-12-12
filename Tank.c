Assignment 3

//Tank Turn//
void tank_turn(uint8 speed, bool left, uint32 delay){
    
    if(left){
        SetMotors(1,0, speed, speed, delay);   
    }else{
        SetMotors(0,1, speed, speed, delay);
    }
}


//Tank//
void zmain(void)
{
    int number1, lower=300 ,high= 1100;
    srand(time(NULL));
    IR_Start();
    Ultra_Start();                          // Ultra Sonic Start function
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors
    IR_wait();   
    
    motor_forward(80,10);
    while(true) {
        int d = Ultra_GetDistance();
        // Print the detected distance (centimeters)
        printf("distance = %d\r\n", d);
        
        
        if(d <= 10)
        {
            motor_forward(0,100);
            number1 = (rand() % (high-lower+1))+lower;
            int r = rand();
            r = r%2;
            motor_backward(50,700);
            tank_turn(80, r, number1);
            motor_forward(80,10);
            
        }
        vTaskDelay(200);
    }
}   