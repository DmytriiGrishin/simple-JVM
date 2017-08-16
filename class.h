#pragma once
#include <stdint.h>
#define CONSTANT_Class  7  
#define CONSTANT_Fieldref  9  
#define CONSTANT_Methodref  10  
#define CONSTANT_InterfaceMethodref  11  
#define CONSTANT_String  8  
#define CONSTANT_Integer  3  
#define CONSTANT_Float  4  
#define CONSTANT_Long  5  
#define CONSTANT_Double  6  
#define CONSTANT_NameAndType  12  
#define CONSTANT_Utf8  1
struct attribute_info{
    uint16_t attribute_name_index;
    uint32_t attribute_length;
    uint8_t* info;//[attribute_length];
};

struct cp_info{
    uint8_t tag;
    uint8_t* info;
};

struct field_info{
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    struct attribute_info* attributes; //[attributes_count];
};

struct code_attribute{
 uint16_t attribute_name_index;
 uint32_t attribute_length;
 uint16_t max_stack;
 uint16_t max_locals;
 uint32_t code_length;
 uint8_t *code;
 uint16_t exception_table_length;
 struct ex_table_t{   uint16_t start_pc;
           uint16_t end_pc;
           uint16_t handler_pc;
           uint16_t catch_type;
    } *exception_table;
    uint16_t attributes_count;
    struct attribute_info* attributes; //[attributes_count];
};


struct method_info{
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    struct attribute_info* attributes; //[attributes_count];
};


struct  class_header{
    uint32_t magic;
    uint16_t minor_version;
    uint16_t major_version;
    uint16_t constant_pool_count;
    struct cp_info **constant_pool; //[constant_pool_count-1];
    uint16_t access_flags;
    uint16_t this_class;
    uint16_t super_class;
    uint16_t interfaces_count;
    uint16_t* interfaces; //[interfaces_count];
    uint16_t fields_count;
    struct field_info *fields; //[fields_count];
    uint16_t methods_count;
    struct method_info* methods; //[methods_count];
    uint16_t attributes_count;
    struct attribute_info* attributes; //[attributes_count];
};

