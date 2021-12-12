//Time between lines: Week 5, Assignment 3

void zmain(void)
{
    printf("\nBoot\n");
    send_mqtt("Group7/debug", "Boot");
    motor_start();
    motor_forward(0,0);
	float viiva, toisella;
	bool aloitus = false;
    bool viivalla = false;
    struct sensors_ dig;
    IR_Start();
	IR_flush();
    reflectance_start();
    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000); // set center sensor threshold to 11000 and others to 9000
    while(SW1_Read() == 1){
        vTaskDelay(10);    
    }
    motor_forward(80,10);
    while(true)
    {
        reflectance_digital(&dig); 
        if(dig.L3 == 1 && dig.R3 == 1 && !aloitus){
            motor_forward(0,0);
            IR_wait();
			viiva = xTaskGetTickCount();
            motor_forward(80,10);
			aloitus = true;
            viivalla = true;
        }
		else if(dig.L3 == 1 && dig.R3 == 1 && !viivalla){
			toisella = xTaskGetTickCount() - viiva;
			print_mqtt("Group7/lap", "%.2f sekunttia kului viivojen valissa!", toisella/1000);
            motor_forward(0,0);
            IR_wait();
			viiva = xTaskGetTickCount();
            motor_forward(80,10);
            viivalla = true;
		}
        else if(viivalla)
        {
            motor_forward(80,10);
            if(dig.L3 == 0 && dig.R3 == 0)
            {
                viivalla = false;
            }
        }
        vTaskDelay(10);
    }
}   