/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2021/5/14.
//

#ifndef __OBSERVER_SQL_EXECUTOR_VALUE_H_
#define __OBSERVER_SQL_EXECUTOR_VALUE_H_
#include "common/log/log.h"
#include <string.h>
#include <time.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

class TupleValue {
public:
  TupleValue() = default;
  virtual ~TupleValue() = default;

  virtual void to_string(std::ostream &os) const = 0;
  virtual int compare(const TupleValue &other) const = 0;
private:
};

class IntValue : public TupleValue {
public:
  explicit IntValue(int value) : value_(value) {
  }

  void to_string(std::ostream &os) const override {
    os << value_;
  }
  int get() const{
    return value_;
  }
  int compare(const TupleValue &other) const override {
    const IntValue & int_other = (const IntValue &)other;
    return value_ - int_other.value_;
  }

private:
  int value_;
};

class FloatValue : public TupleValue {
public:
  explicit FloatValue(float value) : value_(value) {
  }

  void to_string(std::ostream &os) const override {
    os << value_;
  }
  float get() const {
    return value_;
  }
  int compare(const TupleValue &other) const override {
    const FloatValue & float_other = (const FloatValue &)other;
    float result = value_ - float_other.value_;
    if (result > 0) { // 浮点数没有考虑精度问题
      return 1;
    }
    if (result < 0) {
      return -1;
    }
    return 0;
  }
private:
  float value_;
};

class StringValue : public TupleValue {
public:
  StringValue(const char *value, int len) : value_(value, len){
  }
  explicit StringValue(const char *value) : value_(value) {
  }

  void to_string(std::ostream &os) const override {
    os << value_;
  }
  std::string get() const{
    return value_;
  }
  int compare(const TupleValue &other) const override {
    const StringValue &string_other = (const StringValue &)other;
    return strcmp(value_.c_str(), string_other.value_.c_str());
  }
private:
  std::string value_;
};
class DateValue : public TupleValue {
public:
  DateValue(const char *value, int len) : value_(value, len){
  }
  explicit DateValue(const char *value) : value_(value) {
  }
  void to_string(std::ostream &os) const override {
    os << value_;
  }
  int to_time() const{
    //将字符串转成时间
    size_t index = value_.find("-",0); 
    size_t index_2 = value_.find("-",index+1); 
    std::string year = value_.substr(0,4); 
    std::string month = value_.substr(5,index_2-index-1); 
    std::string day = value_.substr(index_2+1,index_2+3);
    int date = stoi(year+month+day);
    LOG_INFO("it is time %d",date);
    return date;
  }
  int compare(const TupleValue &other) const override {
    const DateValue &date_other = (const DateValue &)other;
    if(date_other.to_time()>to_time())  return -1; 
    else if (date_other.to_time()<to_time()) return 1; 
    return 0;
  }
  std::string get() const {
    return value_;
  }
private:
  std::string value_;
};

#endif //__OBSERVER_SQL_EXECUTOR_VALUE_H_
