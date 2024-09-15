/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/5/22.
//

#pragma once

#include "common/rc.h"
#include "sql/stmt/stmt.h"
#include "storage/field/field.h"
#include "sql/stmt/filter_stmt.h"

#include<memory>

class Table;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt
{
public:
  UpdateStmt() = default;
  UpdateStmt(Table *table, Value *values, int value_amount,FieldMeta field,
    FilterStmt* filter_stmt);
public:
  static RC create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt);
  
  StmtType type() const override
  {
    return StmtType::UPDATE;
  }
  FieldMeta* update_fields()
  {
    return &field_;
  }
  FilterStmt * filter_stmt()
  {
    return filter_stmt_.get();
  }

public:
  Table *table() const { return table_; }
  Value *values() const { return values_; }
  int    value_amount() const { return value_amount_; }

private:
  Table *table_        = nullptr;
  Value *values_       = nullptr;
  int    value_amount_ = 0;

  FieldMeta field_;
  std::unique_ptr<FilterStmt> filter_stmt_;
};
