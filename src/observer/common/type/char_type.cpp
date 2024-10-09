/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/lang/comparator.h"
#include "common/log/log.h"
#include "common/type/char_type.h"
#include "common/value.h"

int CharType::compare(const Value &left, const Value &right) const
{
  if(left.attr_type() == AttrType::CHARS && right.attr_type() == AttrType::CHARS)
    return common::compare_string(
        (void *)left.value_.pointer_value_, left.length_, (void *)right.value_.pointer_value_, right.length_);
  else if(left.attr_type() == AttrType::CHARS) {
    Value result(0);
    left.cast_to(left,AttrType::INTS,result);
    return result.get_int() - right.get_int();
  }
    Value result(0);
    left.cast_to(right,AttrType::INTS,result);
    return left.get_int() - result.get_int();
  }

RC CharType::set_value_from_str(Value &val, const string &data) const
{
  val.set_string(data.c_str());
  return RC::SUCCESS;
}

RC CharType::cast_to(const Value &val, AttrType type, Value &result) const
{
  switch (type) {
    case AttrType::INTS: {
      auto data = val.get_string();
      try {
        // 尝试将字符串转为整数
        int converted_value = std::stoi(data);
        result.set_int(converted_value); // 假设 Value 有一个 set_int 的方法
      } catch (const std::invalid_argument& e) {
        // 如果无法转换，则将 result 设为 0
        result.set_int(0);
      } catch (const std::out_of_range& e) {
        // 捕获转换过程中的范围错误，可以选择将 result 设为某个默认值，如 0
        result.set_int(0);
      }
      break;
    }
    default: return RC::UNIMPLEMENTED;
  }
  return RC::SUCCESS;
}

int CharType::cast_cost(AttrType type)
{
  if (type == AttrType::CHARS) {
    return 0;
  }
  return INT32_MAX;
}

RC CharType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << val.value_.pointer_value_;
  result = ss.str();
  return RC::SUCCESS;
}