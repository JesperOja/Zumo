//Time between button press, week 6 assignment 1

void zmain(void)
{
    printf("\nBoot\n");
    send_mqtt("Group 7/debug", "Boot");

     while(SW1_Read() == 1){
        vTaskDelay(10);    
    }
    float eka = xTaskGetTickCount();
    vTaskDelay(500);
    while(true)
    {
        if(SW1_Read() == 0)
        {
            float toka = xTaskGetTickCount() - eka;
            eka = xTaskGetTickCount();
            print_mqtt("Group 7/button", "%.2f sekunttia kului edellisesta napin painalluksesta!", toka/1000);
            vTaskDelay(200);
            while(SW1_Read() == 0)
            {
                vTaskDelay(20);
            }
        }
    }
 }   