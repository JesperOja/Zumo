Assignment 2.

//ultrasonic sensor//
void zmain(void)
{
    Ultra_Start();                          // Ultra Sonic Start function
    motor_start();              // enable motor controller
    motor_forward(0,0);         // set speed to zero to stop motors
	while(SW1_Read() == 1){
        vTaskDelay(10);    
    }
    motor_forward(80,10);
    while(true) {
        int d = Ultra_GetDistance();
        // Print the detected distance (centimeters)
        printf("distance = %d\r\n", d);
        vTaskDelay(200);
        
        if(d <= 10)
        {
            motor_backward(50,300);
            motor_turn(0,205,280);     // turn
            motor_forward(80,10);
        }
        
    }
}   