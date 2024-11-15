#ifndef RTC_H
#define RTC_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#define RTC_MASK            5
#define RTC_IRQ             8
#define RTC_REG_IN      0x70
#define RTC_REG_OUT     0x71
#define RTC_REG_UPDATING   10
#define RTC_REG_BIN   11
#define SECONDS    0
#define MINUTES    2
#define HOURS      4   
#define DAY        7
#define MONTH      8
#define YEAR       9
#define BINARY              BIT(2)
#define UPDATING            BIT(7)

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} rtc_info;

extern rtc_info date_time; 

void rtc_setup();
int rtc_subscribe_int();
int rtc_unsubscribe_int();
int rtc_read(uint8_t command, uint8_t *output);
int updating_rtc();
int rtc_binary();
int bcd();
int update_time_date();
uint8_t to_binary(uint8_t bcd_number);

#endif // RTC_H
