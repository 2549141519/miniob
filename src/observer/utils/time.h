#pragma once

#include<string>
#include <cstdint>

bool check_date(int year, int month, int day);
//检查 年 月 日 是否是一个合法的日期

int string_to_data(const std::string &str,int & date);

std::string date_to_string(uint32_t date);
