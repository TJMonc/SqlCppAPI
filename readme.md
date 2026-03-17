Notes:

Structural Changes
    - Add a QueryCondition struct that simply stores a private sql string and a vector of values for prepared queries (Could add more)

Add/change the following operator overloads:
    - Record
        - T& Record::operator()(T& field) : Meant to be used as way to access record[fieldname] without manually casting to prefered type. Could also be T& operator[](T& field).

        - QueryCondition {x}Value::operator[ComparisonOperators]([any primitive type] other) : Allows for better filtering
        - add more Value assignment operators to be able to store the value directly
        - all primitive types need overloads for implicit casting