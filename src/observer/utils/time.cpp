#include"time.h"
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include "common/log/log.h"

/*
 | Year (13 bits) | Month (4 bits) | Day (5 bits) |
 | 31          19 | 18          15 | 14          0|
*/
bool check_date(int year, int month, int day) {
    static int months[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool is_leap = (year % 400 == 0 || (year % 100 && year % 4 == 0));

    if (year > 0 && (month > 0) && (month <= 12) && (day > 0) && (day <= ((month == 2 && is_leap) ? 1 : 0) + months[month]))
        return true;

    return false;
}

int string_to_data(const std::string &str,int & date) {
    int y,m,d;
    sscanf(str.c_str(), "%d-%d-%d", &y, &m, &d);//not check return value eq 3, lex guarantee
    bool b = check_date(y,m,d);
    if(!b) return -1;
    date = y*10000+m*100+d;
    return 0;
}

std::string date_to_string(uint32_t date) {
    int year = date / 10000;  // 提取年份
    int month = (date % 10000) / 100;  // 提取月份
    int day = date % 100;  // 提取天数

    // 使用字符串流格式化输出
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << year << "-"
       << std::setfill('0') << std::setw(2) << month << "-"
       << std::setfill('0') << std::setw(2) << day;

    return ss.str();
}





