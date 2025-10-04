#include "main.h"

void process_cmd(cmd_variable* command);
int get_number(uint8_t arr[],int l);
int check_time_configurations(RTC_TimeTypeDef rtc_time);
int check_date_configurations(RTC_DateTypeDef rtc_date);
char* invalid_msg = "INVALID INPUT";

void menu_function(void* pv)
{
    uint32_t cmd_address;
    cmd_variable *cmd;
    const char* menu_msg = "\n==========================\n"
                            "         MENU             \n"
                            "==========================\n"
                            "LED EFFECT           --->0\n"
                            "Date and Time        --->1\n"
                            "EXIT                 --->3\n"
                             "Enter your choice here : ";
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
            if(option == 0)
            {
                curr_state = sLedEffect;
                xTaskNotify(xHandleled_task, 0, eNoAction);
            }
            else if(option == 1)
            {
                curr_state = sRtcMenu;
                xTaskNotify(xHandlertc_task, 0, eNoAction);
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

        case sLedEffect:
            // Send LED command to LED task (second phase notification)
            xTaskNotify(xHandleled_task, (uint32_t)(uintptr_t)command, eSetValueWithOverwrite);
            break;

        case sRtcMenu:
        case sRtcTimeConfig:
        case sRtcDateConfig:
            xTaskNotify(xHandlertc_task, (uint32_t)(uintptr_t)command, eSetValueWithOverwrite);
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

void led_task(void* pv)
{
    uint32_t cmd_address;
    cmd_variable *cmd;
    char* msg_led = "\n=====================\n"
                    "         LED EFFECT  \n"
                    "=====================\n"
                    "(none,e1,e2,e3,e4)\n"
                    "Enter your choice here : ";

    while(1)
    {
        // First phase: wait for menu to tell us to start LED mode
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

        // Show LED menu
        xQueueSend(q_print, &msg_led, portMAX_DELAY);

        // Second phase: wait for cmd_handler_function to send actual LED command
        xTaskNotifyWait(0, 0, &cmd_address, portMAX_DELAY);
        cmd = (cmd_variable*)(uintptr_t)cmd_address;

        if(cmd->len <= 4)
        {
            if(strcmp((char*)cmd->cmd, "e1") == 0)
                led_effect(1);
            else if(strcmp((char*)cmd->cmd, "e2") == 0)
                led_effect(2);
            else if(strcmp((char*)cmd->cmd, "e3") == 0)
                led_effect(3);
            else if(strcmp((char*)cmd->cmd, "e4") == 0)
                led_effect(4);
            else if(strcmp((char*)cmd->cmd, "none") == 0)
                led_effect_stop();
            else
                xQueueSend(q_print, &invalid_msg, portMAX_DELAY);
        }
        else
        {
            xQueueSend(q_print, &invalid_msg, portMAX_DELAY);
        }

        // Return control to main menu
        curr_state = sMainMenu;
        xTaskNotify(xHandlemenu, 0, eNoAction);
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

void rtc_task(void* pv)
{
	const char* msg_rtc = "\n========================\n"
							"|         RTC MENU     |\n"
							"========================\n";
	const char* rtc1    =
							"Configure Time            ----> 0\n"
							"Configure Date            ----> 1\n"
							"Enable reporting          ----> 2\n"
							"Exit                      ----> 3\n"
							"Enter your choice here : ";
	const char *msg_rtc_hh = "Enter hour(1-12):\n";
	const char *msg_rtc_mm = "Enter minutes(0-59):\n";
	const char *msg_rtc_ss = "Enter seconds(0-59):\n";

	const char *msg_rtc_dd  = "Enter date(1-31):\n";
	const char *msg_rtc_mo  ="Enter month(1-12):\n";
	const char *msg_rtc_dow  = "Enter day(1-7 sun:1):\n";
	const char *msg_rtc_yr  = "Enter year(0-99):\n";

	const char *msg_conf = "Configuration successful\n";
	const char *msg_rtc_report = "Enable time&date reporting(y/n)?: ";

	uint32_t cmd_addr;
	cmd_variable *cmd;

	RTC_DateTypeDef rtc_date;
	RTC_TimeTypeDef rtc_time;

    #define HH_Config 0
	#define MM_Config 1
	#define SS_Config 2
	#define DD_Config 3
	#define MO_Config 4
	#define DOW_Config 5
	#define YR_Config 6


    while(1)
    {
    	xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
        xQueueSend(q_print, &msg_rtc, portMAX_DELAY);
        show_time_date();
        xQueueSend(q_print, &rtc1, portMAX_DELAY);
        while(curr_state != sMainMenu)
        {
            xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);//
            cmd = (cmd_variable*)(uintptr_t)cmd_addr;
            switch(curr_state)
        	{
            case sRtcMenu: { //ekk baar RTC_menu se bahar nikelga tho phir apne aap se curr_state change ho jayga
					if(cmd->len == 1)
					{
						int option = cmd->cmd[0] - '0';
						if(option == 0)
						{
							curr_state = sRtcTimeConfig;
							xQueueSend(q_print, &msg_rtc_hh, portMAX_DELAY);
							break;
						}
						else if(option == 1)
						{
							curr_state = sRtcDateConfig;
							xQueueSend(q_print, &msg_rtc_dd, portMAX_DELAY);
							break;
						}
						else if(option == 3)
						{
							curr_state = sMainMenu;
							break;
						}
						else
						{
							xQueueSend(q_print, &invalid_msg, portMAX_DELAY);
							curr_state = sMainMenu;
						}
					}
					else
					{
						xQueueSend(q_print, &invalid_msg, portMAX_DELAY);
						curr_state = sMainMenu;
						break;
					}
					break;
        	}
            case sRtcTimeConfig:
            {
            	switch(rtc_state)
            	{
            	case HH_Config:
            	{
            		rtc_time.Hours =get_number(cmd->cmd,cmd->len);
            		rtc_state = MM_Config;
            		xQueueSend(q_print, &msg_rtc_mm, portMAX_DELAY);
            		break;
            	}
            	case MM_Config:
            	{
            		rtc_time.Minutes =get_number(cmd->cmd,cmd->len);
            		rtc_state = SS_Config;
            		xQueueSend(q_print, &msg_rtc_ss, portMAX_DELAY);
            		break;
            	}
            	case SS_Config:
            	{
            		rtc_time.Seconds =get_number(cmd->cmd,cmd->len);
            		curr_state = sMainMenu;
            		if(check_time_configurations(rtc_time))
            		{
            			rtc_configure_time(&rtc_time);
                		xQueueSend(q_print, &msg_conf, portMAX_DELAY);
                		show_time_date();
            		}
            		else
            		{
						xQueueSend(q_print, &invalid_msg, portMAX_DELAY);
            		}
            		rtc_state = 0;
            		break;
            	}
            	}
            	break;
            }
            case sRtcDateConfig:
            {
            	switch(rtc_state_date)
            	{
            	case DD_Config:
            	{
            		rtc_date.Date =get_number(cmd->cmd,cmd->len);
            		rtc_state_date = MO_Config;
            		xQueueSend(q_print, &msg_rtc_mo, portMAX_DELAY);
            		break;
            	}
            	case MO_Config:
            	{
            		rtc_date.Month =get_number(cmd->cmd,cmd->len);
            		rtc_state_date = DOW_Config;
            		xQueueSend(q_print, &msg_rtc_dow, portMAX_DELAY);
            		break;
            	}
            	case DOW_Config:
            	{
            		rtc_date.WeekDay =get_number(cmd->cmd,cmd->len);
            		rtc_state_date = YR_Config;
            		xQueueSend(q_print, &msg_rtc_yr, portMAX_DELAY);
            		break;
            	}
            	case YR_Config:
            	{
            		rtc_date.Year =get_number(cmd->cmd,cmd->len);
              		curr_state = sMainMenu;
                		if(check_date_configurations(rtc_date))
                		{
                			rtc_configure_date(&rtc_date);
                    		xQueueSend(q_print, &msg_conf, portMAX_DELAY);
                    		show_time_date();
                		}
                		else
                		{
    						xQueueSend(q_print, &invalid_msg, portMAX_DELAY);
                		}
                		rtc_state_date = 0;
                		break;
            	}
            	}
            	break;
            }
				}
        }
        xTaskNotify(xHandlemenu, 0, eNoAction);
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
int check_time_configurations(RTC_TimeTypeDef rtc_time)
{
	int flag = 1;
	if(rtc_time.Hours > 12)
		flag=0;
	if(rtc_time.Minutes > 59)
		flag=0;
	if(rtc_time.Seconds >59)
		flag=0;
	return flag;
}
int check_date_configurations(RTC_DateTypeDef rtc_date)
{
	int flag =1;
	if(rtc_date.Date >31)
		flag= 0;
	if(rtc_date.Month >12)
		flag= 0;
	if(rtc_date.WeekDay >7)
		flag= 0;
	if(rtc_date.Year>99)
		flag= 0;
	return flag;
}
