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

#include "sql/stmt/update_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

#include<unordered_map>

UpdateStmt::UpdateStmt(Table *table, const Value *values, int value_amount,FieldMeta field,
  FilterStmt* filter_stmt)
    : table_(table), values_(values), value_amount_(value_amount),field_(field),filter_stmt_(filter_stmt)
{}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
  // TODO
  std::string table_name = update.relation_name;
  if(db == nullptr || table_name.empty()) {
    LOG_WARN("invalid argument. db is null or table name is empty");
    return RC::INVALID_ARGUMENT;
  }

  Table *table = db->find_table(table_name.c_str());
  if (nullptr == table) {
    LOG_WARN("not exist table table_name=%s", table_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  FieldMeta update_field;
  bool has_match_field = false;
  const TableMeta &table_meta = table->table_meta();
    for (int i = table_meta.sys_field_num(); i < table_meta.field_num(); i++) {
      const FieldMeta *field_meta = table_meta.field(i);
      std::string cur_field_name = field_meta->name();
      auto cur_field_type = field_meta->type();
      
      if(cur_field_name == update.attribute_name && cur_field_type == update.value.attr_type()) {
        if(cur_field_type == AttrType::CHARS)
        {
          auto cur_field_len = field_meta->len();
          if(update.value.length() > cur_field_len)
            continue;
        }  
          has_match_field = true;
          update_field = *field_meta;
          break;
      }
    }

  if(!has_match_field) {
    LOG_WARN("update field type mismatch.");
    return RC::INVALID_ARGUMENT;
  }


  std::unordered_map<std::string,Table*> table_ref_map; //where条件可能涉及到多表查询 
  //这里存储的其实是别名到table的映射，当然此题目前不需要考虑
  table_ref_map[table_name] = table;
  FilterStmt *filter_stmt = nullptr;
  RC rc = FilterStmt::create(
      db, table, &table_ref_map, update.conditions.data(), static_cast<int>(update.conditions.size()), filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create filter statement. rc=%d:%s", rc, strrc(rc));
    return rc;
  }

  stmt = new UpdateStmt(table,&(const_cast<UpdateSqlNode&>(update).value), 1,update_field,filter_stmt);
  return RC::SUCCESS;
}


