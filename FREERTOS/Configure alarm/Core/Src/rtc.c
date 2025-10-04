#include "main.h"
#include "stdio.h"
void rtc_configure_alarm_a(RTC_AlarmTypeDef *alarm)
{
    alarm->Alarm = RTC_ALARM_A;
    alarm->AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    alarm->AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;   // ignore date, trigger daily
    alarm->AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
    alarm->AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    alarm->AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    alarm->AlarmTime.SubSeconds = 0;
    //alarm->AlarmSubSecondMask   = RTC_ALARMSUBSECONDMASK_ALL;  // ignore subseconds
    alarm->AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
    if (HAL_RTC_SetAlarm_IT(&hrtc, alarm, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler(); // check for errors
    }
}


void rtc_configure_alarm_b(RTC_AlarmTypeDef *alarm)
{
	alarm->Alarm = RTC_ALARM_B;
	alarm->AlarmMask = RTC_ALARMMASK_NONE ;
	alarm->AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	alarm->AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
	alarm->AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	alarm->AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	alarm->AlarmTime.SubSeconds = 0;
	alarm->AlarmSubSecondMask   = RTC_ALARMSUBSECONDMASK_NONE;


	HAL_RTC_SetAlarm_IT(&hrtc, alarm, RTC_FORMAT_BIN);
}

static inline uint8_t bcd2dec(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}
void show_alarm_settings_of_a(void)
{
    static char showalarm[40];
    static char *time = showalarm;

    RTC_AlarmTypeDef rtc_alarm;

    // Read raw register values (RTC ALARM A registers)
    rtc_alarm.AlarmTime.Hours   = bcd2dec((RTC->ALRMAR >> 16) & 0x3F);
    rtc_alarm.AlarmTime.Minutes = bcd2dec((RTC->ALRMAR >> 8)  & 0x7F);
    rtc_alarm.AlarmTime.Seconds = bcd2dec((RTC->ALRMAR)       & 0x7F);
    rtc_alarm.AlarmDateWeekDay  = bcd2dec((RTC->ALRMAR >> 24) & 0x3F);

    rtc_alarm.AlarmTime.TimeFormat = (RTC->ALRMAR & RTC_ALRMAR_PM) ? RTC_HOURFORMAT12_PM : RTC_HOURFORMAT12_AM;

    char *format = (rtc_alarm.AlarmTime.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

    sprintf(showalarm,
            "\nAlarm Setting of A:\t%02d:%02d:%02d:%02d [%s]",
            rtc_alarm.AlarmDateWeekDay,
            rtc_alarm.AlarmTime.Hours,
            rtc_alarm.AlarmTime.Minutes,
            rtc_alarm.AlarmTime.Seconds,
            format);

    xQueueSend(q_print, &time, portMAX_DELAY);
}

void show_alarm_settings_of_b(void)
{
    static char showalarm[40];
    static char *time = showalarm;

    RTC_AlarmTypeDef rtc_alarm;

    // Read raw register values (RTC ALARM B registers)
    rtc_alarm.AlarmTime.Hours   = bcd2dec((RTC->ALRMBR >> 16) & 0x3F);
    rtc_alarm.AlarmTime.Minutes = bcd2dec((RTC->ALRMBR >> 8)  & 0x7F);
    rtc_alarm.AlarmTime.Seconds = bcd2dec((RTC->ALRMBR)       & 0x7F);
    rtc_alarm.AlarmDateWeekDay  = bcd2dec((RTC->ALRMBR >> 24) & 0x3F);

    rtc_alarm.AlarmTime.TimeFormat = (RTC->ALRMBR & RTC_ALRMBR_PM) ? RTC_HOURFORMAT12_PM : RTC_HOURFORMAT12_AM;

    char *format = (rtc_alarm.AlarmTime.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

    sprintf(showalarm,
            "\nAlarm Setting of B:\t%02d:%02d:%02d:%02d [%s]",
            rtc_alarm.AlarmDateWeekDay,
            rtc_alarm.AlarmTime.Hours,
            rtc_alarm.AlarmTime.Minutes,
            rtc_alarm.AlarmTime.Seconds,
            format);

    xQueueSend(q_print, &time, portMAX_DELAY);
}
