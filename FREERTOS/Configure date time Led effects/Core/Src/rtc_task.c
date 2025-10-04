#include "main.h"

void rtc_configure_time(RTC_TimeTypeDef *time)
{

	time->TimeFormat = RTC_HOURFORMAT12_AM;
	time->DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
	time->StoreOperation = RTC_STOREOPERATION_RESET;

	HAL_RTC_SetTime(&hrtc,time,RTC_FORMAT_BIN);
}

void rtc_configure_date(RTC_DateTypeDef *date)
{
	HAL_RTC_SetDate(&hrtc,date,RTC_FORMAT_BIN);
}

void show_time_date(void)
{
	static char showtime[40];//created two character arrays.
	static char showdate[40];

	RTC_DateTypeDef rtc_date;//created rtc_date,rtc_time variable
	RTC_TimeTypeDef rtc_time;

	static char *time = showtime;//time is a pointer to pointer (which is showtime)
	static char *date = showdate;

	memset(&rtc_date,0,sizeof(rtc_date));//*rtc_date is a pointer where 0 are present with rtc_date as size which is 4 ,so in short *rtc_date = 0,0,0,0
	memset(&rtc_time,0,sizeof(rtc_time));//*rtc_time is 0,0,0,0,0,0,0,0

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	char *format;
	format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

	/* Display time Format : hh:mm:ss [AM/PM] */
	sprintf((char*)showtime,"%s:\t%02d:%02d:%02d [%s]","\nCurrent Time&Date",rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds,format);
	xQueueSend(q_print,&time,portMAX_DELAY);

	/* Display date Format : date-month-year */
	sprintf((char*)showdate,"\t%02d-%02d-%2d\n",rtc_date.Date, rtc_date.Month, 2000 + rtc_date.Year);
	xQueueSend(q_print,&date,portMAX_DELAY);
}
