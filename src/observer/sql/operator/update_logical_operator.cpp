#include "sql/operator/update_logical_operator.h"
UpdateLogicalOperator::UpdateLogicalOperator(Table *table, std::vector<Value>& values,FieldMeta& field)
    : table_(table), values_(values),field_(field)
{
}