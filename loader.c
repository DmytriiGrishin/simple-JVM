#include "loader.h"

uint32_t to_32little_endian(uint32_t dword)
{

    return (((dword >>  0) & 0xff) << 24)
         | (((dword >>  8) & 0xff) << 16)
         | (((dword >> 16) & 0xff) <<  8)
         | (((dword >> 24) & 0xff) <<  0);
}
uint16_t to_16little_endian(uint16_t word)
{

    return (((word >>  0) & 0xff) << 8)
         | (((word >>  8) & 0xff) << 0);
}

uint64_t GetConstantPoolSize(FILE* file,struct class_header* class, int i);
void parse_attributes(struct attribute_info* here, uint16_t count, FILE* from);

enum v_loader_status load_file(char const* name, struct class_header* class, FILE* err){
		int i;
		FILE* class_file = fopen(name, "rb");
		
		if(class_file==NULL){
			fprintf(err, "LOADER_IO_ERROR");
			return LOADER_IO_ERROR;
		}
		fread(&(class->magic), 1, sizeof(uint32_t), class_file);
		class->magic = to_32little_endian(class->magic);
		if(class->magic!=0xCAFEBABE){
			fprintf(err, "LOADER_FORMAT_ERROR, %X", class->magic);
			fclose(class_file);
			return LOADER_FORMAT_ERROR;
		}
		fread(&(class->minor_version), 1, sizeof(uint32_t), class_file);
		class->minor_version = to_16little_endian(class->minor_version);		
		class->major_version = to_16little_endian(class->major_version);

		fread(&(class->constant_pool_count), 1, sizeof(uint16_t), class_file);
		class->constant_pool_count = to_16little_endian(class->constant_pool_count);


		class->constant_pool = malloc(sizeof(struct cp_info*)*(class->constant_pool_count-1));
		for(i=1;i<class->constant_pool_count;i++){
			class->constant_pool[i] =malloc(sizeof(struct cp_info));
			fread(&(class->constant_pool[i]->tag), 1, sizeof(uint8_t), class_file);
			int size = GetConstantPoolSize(class_file, class, i);
			if(size==0){
				return LOADER_FORMAT_ERROR;
			}
			if(class->constant_pool[i]->tag!=CONSTANT_Utf8){
				class->constant_pool[i]->info = malloc(size*sizeof(uint8_t));
				for(int j=0; j<size; j++){
					fread(&(class->constant_pool[i]->info[j]),1, sizeof(uint8_t), class_file);
				}
			} else {
				for(int j=0; j<size; j++){
					fread(&(class->constant_pool[i]->info[j]),1, sizeof(uint8_t), class_file);
				}
				class->constant_pool[i]->info[size] = 0x00;
			}

			if(class->constant_pool[i]->tag == CONSTANT_Long || class->constant_pool[i]->tag == CONSTANT_Double){
				class->constant_pool[i+1]=0x00;
				i++;
			}
		}
		fread(&(class->access_flags),1,sizeof(uint16_t),class_file);
		class->access_flags = to_16little_endian(class->access_flags);

		fread(&(class->this_class),1,sizeof(uint16_t),class_file);
		class->this_class=to_16little_endian(class->this_class);
		
		fread(&(class->super_class),1,sizeof(uint16_t),class_file);
		class->super_class = to_16little_endian(class->super_class);
		fread(&(class->interfaces_count),1,sizeof(uint16_t),class_file);
		class->interfaces_count=to_16little_endian(class->interfaces_count);

		class->interfaces = malloc(class->interfaces_count*sizeof(uint16_t));
		for(i = 0; i<class->interfaces_count;i++){
			fread(class->interfaces+i, 1, sizeof(uint16_t), class_file);
			class->interfaces[i]=to_16little_endian(class->interfaces[i]);
		}
		fread(&(class->fields_count), 1, sizeof(uint16_t), class_file);
		class->fields_count = to_16little_endian(class->fields_count);
		if(class->fields_count>0){
			class->fields = malloc(sizeof(struct field_info)*class->fields_count);
			for(i=0;i<class->fields_count;i++){
				fread(&(class->fields[i].access_flags), 1, sizeof(uint16_t), class_file);
				class->fields[i].access_flags = to_16little_endian(class->fields[i].access_flags);
				fread(&(class->fields[i].name_index), 1, sizeof(uint16_t), class_file);
				class->fields[i].name_index = to_16little_endian(class->fields[i].name_index);
				fread(&(class->fields[i].descriptor_index), 1, sizeof(uint16_t), class_file);
				class->fields[i].descriptor_index = to_16little_endian(class->fields[i].descriptor_index);
				fread(&(class->fields[i].attributes_count), 1, sizeof(uint16_t), class_file);
				class->fields[i].attributes_count = to_16little_endian(class->fields[i].attributes_count);
				class->fields[i].attributes = malloc(sizeof(struct attribute_info)*class->fields[i].attributes_count);
				parse_attributes(class->fields[i].attributes, class->fields[i].attributes_count, class_file);
			}
		}
		fread(&(class->methods_count), 1 , sizeof(uint16_t), class_file);
		class->methods_count = to_16little_endian(class->methods_count);
		class->methods = malloc(sizeof(struct field_info)*class->methods_count);
		for(i=0;i<class->methods_count;i++){
			fread(&(class->methods[i].access_flags), 1, sizeof(uint16_t), class_file);
			class->methods[i].access_flags = to_16little_endian(class->methods[i].access_flags);
			fread(&(class->methods[i].name_index), 1, sizeof(uint16_t), class_file);
			class->methods[i].name_index = to_16little_endian(class->methods[i].name_index);
			fread(&(class->methods[i].descriptor_index), 1, sizeof(uint16_t), class_file);
			class->methods[i].descriptor_index = to_16little_endian(class->methods[i].descriptor_index);
			fread(&(class->methods[i].attributes_count), 1, sizeof(uint16_t), class_file);
			class->methods[i].attributes_count = to_16little_endian(class->methods[i].attributes_count);
			class->methods[i].attributes = malloc(sizeof(struct attribute_info)*class->methods[i].attributes_count);
			parse_attributes(class->methods[i].attributes, class->methods[i].attributes_count, class_file);
		}
		fread(&(class->attributes_count), 1, sizeof(uint16_t), class_file);
		class->attributes_count = to_16little_endian(class->attributes_count);
		class->attributes = malloc(sizeof(struct attribute_info)*class->attributes_count);
		parse_attributes(class->attributes, class->attributes_count, class_file);
		return LOADER_OK;
}

uint64_t GetConstantPoolSize(FILE* file, struct class_header* class, int i){
	uint8_t tag = class->constant_pool[i]->tag;
	uint8_t frst, scnd;
	switch(tag){
	case CONSTANT_Class:
		return 2;
	case CONSTANT_Fieldref:
		return 4;
	case CONSTANT_Methodref:
		return 4;
	case CONSTANT_InterfaceMethodref:
		return 4;
	case CONSTANT_String:
		return 2;
	case CONSTANT_Integer:
		return 4;
	case CONSTANT_Float:
		return 4;
	case CONSTANT_Long:
		return 8;
	case CONSTANT_Double:
		return 8;
	case CONSTANT_NameAndType:
		return 4;
	case CONSTANT_Utf8:
		
		fread(&frst, 1, sizeof(uint8_t), file);
		fread(&scnd, 1, sizeof(uint8_t), file);
		class->constant_pool[i]->info = malloc(2*sizeof(uint8_t));
		class->constant_pool[i]->info[0] = frst;
		class->constant_pool[i]->info[1] = scnd;
		uint64_t res = (uint16_t)(frst)<<8 | scnd;
		class->constant_pool[i]->info = realloc(class->constant_pool[i]->info,(res+1)*sizeof(uint8_t));
		return res;
	default:
		break;
	}
	return 0;
}

void parse_attributes(struct attribute_info* here, uint16_t count, FILE* from){
	uint32_t size;
	for (int j = 0; j < count; ++j)
	{
	fread(&(here->attribute_name_index), 1, sizeof(uint16_t), from);
	here->attribute_name_index = to_16little_endian(here->attribute_name_index);
	fread(&(here->attribute_length),1,sizeof(uint32_t),from);
	uint32_t attr_len =  here->attribute_length = to_32little_endian(here->attribute_length);
	here->info = malloc(sizeof(uint8_t)*attr_len);
	for(int i=0;i<attr_len;i++){
		fread(&(here->info[i]),1,sizeof(uint8_t),from);
	}
	here++;
}
}