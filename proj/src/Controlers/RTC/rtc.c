#include "rtc.h"

int rtc_hookid = RTC_MASK;
rtc_info date_time;
uint8_t binary_mode;

void rtc_setup() {
    binary_mode = rtc_binary();
    update_time_date();
}

int rtc_subscribe_int() {
    return sys_irqsetpolicy(RTC_IRQ , IRQ_REENABLE, &rtc_hookid);
}

int rtc_unsubscribe_int() {
    return sys_irqrmpolicy(&rtc_hookid);
}

int rtc_read(uint8_t command, uint8_t *output) {
    if (sys_outb(RTC_REG_IN, command) != 0) return 1;
	if (util_sys_inb(RTC_REG_OUT, output) != 0) return 1;
    return 0;
}


int updating_rtc() {
    uint8_t result;
    if (rtc_read(RTC_REG_UPDATING, &result)) return 1;
	return result & UPDATING;
}

int rtc_binary() {
    uint8_t result;
    if (rtc_read(RTC_REG_BIN, &result)) return 1;
	return result & BINARY;
}

int bcd() {
    return !rtc_binary();
}

uint8_t to_binary(uint8_t bcd_number) {
    return ((bcd_number >> 4) * 10) + (bcd_number & 0xF);
}

int update_time_date() {
    
    if (updating_rtc() != 0) return 1;
    uint8_t output;

    if (rtc_read(SECONDS, &output) != 0) return 1;
    date_time.seconds = binary_mode ? output : to_binary(output);
    if (rtc_read(MINUTES, &output) != 0) return 1;
    date_time.minutes = binary_mode ? output : to_binary(output);
    if (rtc_read(HOURS, &output) != 0) return 1;
    date_time.hours = binary_mode ? output : to_binary(output);
    if (rtc_read(DAY, &output) != 0) return 1;
    date_time.day = binary_mode ? output : to_binary(output);
    if (rtc_read(MONTH, &output) != 0) return 1;
    date_time.month = binary_mode ? output : to_binary(output);
    if (rtc_read(YEAR, &output) != 0) return 1;
    date_time.year = binary_mode ? output : to_binary(output);

    return 0;
}
