#include "main.h"

void process_cmd(cmd_variable* command);
int get_number(uint8_t arr[],int l);
int check_alarm_settings(RTC_AlarmTypeDef alarm);
char* invalid_msg = "INVALID INPUT";

void menu_function(void* pv)
{
    uint32_t cmd_address;
    cmd_variable *cmd;
    const char* menu_msg = "\n==========================\n"
                            "         MENU             \n"
                            "==========================\n"
                            "Alarm Settings       --->2\n"
                            "EXIT                 --->3\n"
                             "Enter your choice here : ";

    for(int i=0;i<200;i++);
    while(1)
    {
        // Print main menu
        xQueueSend(q_print, &menu_msg, portMAX_DELAY);

        // Wait for command from cmd_handler_function
        xTaskNotifyWait(0, 0, &cmd_address, portMAX_DELAY);
        cmd = (cmd_variable*)(uintptr_t)cmd_address;

        if(cmd->len == 1)
        {
            int option = cmd->cmd[0] - '0';
            if(option == 2)
            {
                curr_state = sAlarmMenu;
                xTaskNotify(xHandlealarm_task, 0, eNoAction);
            }
            else
            {
                xQueueSend(q_print, &invalid_msg, portMAX_DELAY);
                continue;
            }
        }
        else
        {
            xQueueSend(q_print, &invalid_msg, portMAX_DELAY);
            continue;
        }

        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
    }
}

void process_cmd(cmd_variable* command)
{
    // Extract data from q_enqueue into command struct
    int i = 0;
    uint8_t ch;
    command->len = 0;

    while(1)
    {
        xQueueReceive(q_enqueue, &ch, 0);
        if(ch == '.') break; // End of command marker
        command->cmd[i++] = ch;
        command->len++;
    }

    switch(curr_state)
    {
        case sMainMenu:
            // Send menu command to menu task
            xTaskNotify(xHandlemenu, (uint32_t)(uintptr_t)command, eSetValueWithOverwrite);
            break;

        case sAlarmMenu:
        case set_alarm_a:
        case set_alarm_b:
            xTaskNotify(xHandlealarm_task, (uint32_t)(uintptr_t)command, eSetValueWithOverwrite);
            break;
    }
}

void print_task(void* pv)
{
    uint32_t* msg;
    while(1)
    {
        xQueueReceive(q_print, &msg, portMAX_DELAY);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen((char*)msg), HAL_MAX_DELAY);
    }
}

void cmd_handler_function(void* pv)
{
    BaseType_t xResult;
    static cmd_variable command;
    while(1)
    {
        xResult = xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
        if(xResult == pdTRUE)
        {
            process_cmd(&command);
        }
    }
}
int check_alarm_settings(RTC_AlarmTypeDef alarm)
{
	int flag =1;
	if(alarm.AlarmDateWeekDay >31)
		flag =0;
	if(alarm.AlarmTime.Hours >12)
		flag =0;
	if(alarm.AlarmTime.Minutes >59)
		flag =0;
	if(alarm.AlarmTime.Seconds >59)
		flag =0;
	return flag;
}
void alarm_task(void* pv)
{
	const char* alarm_menu = "=====================\n"
						     "|    ALARM MENU      |\n"
							 "=====================\n"
							 "Set alarm A -------> 0\n"
							 "Set alarm B -------> 1\n"
							 "Show alarm A ------> 2\n"
							 "Show alarm B ------> 3\n"
			                 "Enter your choice here : ";
	const char* day = "Tell date(1-31)\n";
	const char* hour= "Tell Hour(1-12)\n";
	const char* minute= "Tell Minute(0-59)\n";
	const char* second= "Tell second(0-59)\n";
	const char* success= "Successful\n";

	#define DD_alarm_Config 0
	#define HH_alarm_Config 1
	#define MM_alarm_Config 2
	#define SS_alarm_Config 3

	RTC_AlarmTypeDef  halarm;

	uint32_t address_store;
	cmd_variable *cand;
	while(1)
	{
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		xQueueSend(q_print,&alarm_menu,portMAX_DELAY);

		while(curr_state != sMainMenu)
		{
			xTaskNotifyWait(0,0,&address_store,portMAX_DELAY);
			cand = (cmd_variable*)(uintptr_t)address_store;
			switch (curr_state)
			{
				case sAlarmMenu:
				{
					if(cand->len == 1)
					{
						int option = cand->cmd[0] - '0';
						if(option ==0)
						{
							curr_state =set_alarm_a;
							xQueueSend(q_print,&day,portMAX_DELAY);
							break;
						}
						else if(option == 1)
						{
							curr_state =set_alarm_b;
							xQueueSend(q_print,&day,portMAX_DELAY);
							break;
						}
						else if(option == 2)
						{
							show_alarm_settings_of_a();
							curr_state = sMainMenu;
							break;
						}
						else if(option ==3)
						{
							show_alarm_settings_of_b();
							curr_state = sMainMenu;
							break;
						}
						else
						{
							curr_state = sMainMenu;
							xQueueSend(q_print,&invalid_msg,portMAX_DELAY);
							break;
						}
					}
					else
					{
						curr_state = sMainMenu;
						xQueueSend(q_print,&invalid_msg,portMAX_DELAY);
						break;
					}
				}break;
				case set_alarm_a:
				{
					switch(alarm_state)
					{
					case DD_alarm_Config:
					{
					halarm.AlarmDateWeekDay =get_number(cand->cmd,cand->len);
						alarm_state = HH_alarm_Config;
						xQueueSend(q_print,&hour,portMAX_DELAY);
						break;
					}
					case HH_alarm_Config:
					{
						halarm.AlarmTime.Hours =get_number(cand->cmd,cand->len);
						alarm_state = MM_alarm_Config;
						xQueueSend(q_print,&minute,portMAX_DELAY);
						break;
					}
					case MM_alarm_Config:
					{
						halarm.AlarmTime.Minutes =get_number(cand->cmd,cand->len);
						alarm_state = SS_alarm_Config;
						xQueueSend(q_print,&second,portMAX_DELAY);
						break;
					}
					case SS_alarm_Config:
					{
						halarm.AlarmTime.Seconds =get_number(cand->cmd,cand->len);
						curr_state = sMainMenu;
						if(check_alarm_settings(halarm))
						{
							rtc_configure_alarm_a(&halarm);
							show_alarm_settings_of_a();
							xQueueSend(q_print,&success,portMAX_DELAY);

						}
						else
						{
							xQueueSend(q_print,&invalid_msg,portMAX_DELAY);
						}
						alarm_state = 0;
						break;
					}
					}break;
				}

				case set_alarm_b:
				{
					switch(alarm_state)
					{
					case DD_alarm_Config:
					{
						halarm.AlarmDateWeekDay =get_number(cand->cmd,cand->len);
						alarm_state = HH_alarm_Config;
						xQueueSend(q_print,&hour,portMAX_DELAY);
						break;
					}
					case HH_alarm_Config:
					{
						halarm.AlarmTime.Hours =get_number(cand->cmd,cand->len);
						alarm_state = MM_alarm_Config;
						xQueueSend(q_print,&minute,portMAX_DELAY);
						break;
					}
					case MM_alarm_Config:
					{
						halarm.AlarmTime.Minutes =get_number(cand->cmd,cand->len);
						alarm_state = SS_alarm_Config;
						xQueueSend(q_print,&second,portMAX_DELAY);
						break;
					}
					case SS_alarm_Config:
					{
						halarm.AlarmTime.Seconds =get_number(cand->cmd,cand->len);
						curr_state = sMainMenu;
						if(check_alarm_settings(halarm))
						{
							rtc_configure_alarm_b(&halarm);
							show_alarm_settings_of_b();
							xQueueSend(q_print,&success,portMAX_DELAY);
						}
						else
						{
							xQueueSend(q_print,&invalid_msg,portMAX_DELAY);
						}
						alarm_state = 0;
						break;
					}
					}
				}
			}
		}
		xTaskNotify(xHandlemenu,0,eNoAction);
	}
}

int get_number(uint8_t arr[],int l)
{
	int y;
	if(l==2)
		y =10*(arr[0] - '0') + arr[1] - '0';
	else
		y=arr[0]-'0';
	return y;
}
