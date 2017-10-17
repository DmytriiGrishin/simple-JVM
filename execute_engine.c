#include "execute_engine.h"
#include "class.h"
#include "class_heap.h"
#include "loader.h"


uint8_t vm_push(struct v_stack_frame* frame, int64_t value){
	frame->stack++;	
	*(frame->stack)= value;
	return 0;
}
int64_t vm_pop(struct v_stack_frame* frame){
	return *(frame->stack--);
}

uint64_t parse_args(uint8_t* type){
	type++;
	uint64_t ret=0;
	while(*type!=')'){
		ret++;
		type++;
	}
	return ret;
}


uint16_t find_method(struct class_header* this, char* name){
	uint16_t m_count = this->methods_count;
	uint16_t i;
	for(i=0; i<m_count; i++){
		uint16_t name_index = this->methods[i].name_index;
		if(!strcmp(name, (char*)this->constant_pool[name_index]->info)){
			return i;
		}
	}
	return 0;
} 
uint16_t find_code_attribute(struct class_header* this, uint16_t method_id){
	uint16_t attr_count = this->methods[method_id].attributes_count;
	uint16_t i;
	for(i=0; i<attr_count; i++){
		uint16_t attr_name_id = this->methods[method_id].attributes[i].attribute_name_index;
		if(!strcmp("Code", (char*)this->constant_pool[attr_name_id]->info)){
			return i;
		} 
	}
	return 0;
}
int vm_add_frame(struct v_machine* vm, struct v_stack_frame* frame){
	frame->prev_frame = vm->stack;
	vm->stack = frame;
	vm->stack_size++;
	return 0;
}
struct v_stack_frame* vm_init_frame(struct class_header* class, char* method_name){
	struct v_stack_frame* ret = malloc(sizeof(struct v_stack_frame));
	ret->current_class = class;
	uint16_t main_index = find_method(class, method_name);
	uint16_t code_index = find_code_attribute(class, main_index);
	ret->stack = malloc((class->methods[main_index].attributes[code_index].info[0]<<8 | class->methods[main_index].attributes[code_index].info[1])*sizeof(uint64_t));
	ret->array = malloc(sizeof(uint64_t)* (class->methods[main_index].attributes[code_index].info[2]<<8 | class->methods[main_index].attributes[code_index].info[3]));
	ret->code_length = class->methods[main_index].attributes[code_index].info[4]<<24 | class->methods[main_index].attributes[code_index].info[5] << 16 | class->methods[main_index].attributes[code_index].info[6] << 8 | class->methods[main_index].attributes[code_index].info[7] ;
	ret->code = class->methods[main_index].attributes[code_index].info + 8;
	ret->pc=0;
	return ret;
}
struct v_machine* vm_init(struct class_heap_t* heap, struct class_header* class, struct v_machine* vm ){
	vm->class_heap = heap;
	vm->stack_size = 0;
	vm->stack = 0;
	vm_add_frame(vm, vm_init_frame(class, "main"));
	return vm;
}
uint8_t ldc(struct v_machine* vm,uint8_t value){
	/* todo: switch for constatn type*/
	int const_size=0;
	switch(vm->stack->current_class->constant_pool[value]->tag){
		case CONSTANT_Class:
		const_size = 2;
		break;
	case CONSTANT_Fieldref:
		const_size = 4;
		break;
	case CONSTANT_Methodref:
		const_size = 4;
		break;
	case CONSTANT_InterfaceMethodref:
		const_size = 4;
		break;
	case CONSTANT_String:
		const_size = 2;
		break;
	case CONSTANT_Integer:
		const_size = 4;
		break;
	case CONSTANT_Float:
		const_size = 4;
		break;
	case CONSTANT_Long:
		const_size = 8;
		break;
	case CONSTANT_Double:
		const_size = 8;
		break;
	case CONSTANT_NameAndType:
		const_size = 4;
		break;
	case CONSTANT_Utf8:
		const_size = strlen((char*)(vm->stack->current_class->constant_pool[value]->info));
		break;
	}
	uint64_t res=0;
	for(int i=0; i<const_size;i++){	
			res= res * 256 + vm->stack->current_class->constant_pool[value]->info[i];
	}
	vm_push(vm->stack, res);
	return 0;
}
uint8_t istore_0(struct v_machine* vm){
    vm->stack->array[0]=vm_pop(vm->stack);
    return 0;
}
uint8_t istore_1(struct v_machine* vm){
 vm->stack->array[1]=vm_pop(vm->stack);
    return 0;
}
uint8_t istore_2(struct v_machine* vm){
 vm->stack->array[2]=vm_pop(vm->stack);
    return 0;
}
uint8_t istore_3(struct v_machine* vm){
 vm->stack->array[3]=vm_pop(vm->stack);
    return 0;
}

uint8_t iload_0(struct v_machine* vm){
	vm_push(vm->stack, vm->stack->array[0]);
	return 0;
}
uint8_t iload_1(struct v_machine* vm){
	vm_push(vm->stack, vm->stack->array[1]);
	return 0;
}
uint8_t iload_2(struct v_machine* vm){
	vm_push(vm->stack, vm->stack->array[2]);
	return 0;
}
uint8_t iload_3(struct v_machine* vm){
	vm_push(vm->stack, vm->stack->array[3]);
	return 0;
}

uint8_t iconst_m1(struct v_machine* vm){
	vm_push(vm->stack, -1);
	return 0;
}
uint8_t iconst_0(struct v_machine* vm){
	vm_push(vm->stack, 0);
	return 0;
}
uint8_t iconst_1(struct v_machine* vm){
	vm_push(vm->stack, 1);
	return 0;
}
uint8_t iconst_2(struct v_machine* vm){
	vm_push(vm->stack, 2);
	return 0;
}
uint8_t iconst_3(struct v_machine* vm){
	vm_push(vm->stack, 3);
	return 0;
}
uint8_t iconst_4(struct v_machine* vm){
	vm_push(vm->stack, 4);
	return 0;
}
uint8_t iconst_5(struct v_machine* vm){
	vm_push(vm->stack, 5);
	return 0;
}

uint8_t bipush(struct v_machine* vm, int8_t value ){
	int64_t to_push = value;
	vm_push(vm->stack, to_push);
	return 0;
}

uint8_t if_icmpeq(struct v_machine* vm){
	int16_t offset;
	vm->stack->pc++;
	offset = *(vm->stack->code+vm->stack->pc);
	offset = offset << 8;
	vm->stack->pc++;
	offset+=*(vm->stack->code+vm->stack->pc);
	int64_t first= vm_pop(vm->stack);
	int64_t second= vm_pop(vm->stack);
	if(first==second){
		vm->stack->pc+=offset-3;
	}
	return 0;
}
uint8_t if_icmpne(struct v_machine* vm){
	int16_t offset;
	vm->stack->pc++;
	offset = *(vm->stack->code+vm->stack->pc);
	offset = offset << 8;
	vm->stack->pc++;
	offset+=*(vm->stack->code+vm->stack->pc);
	int64_t first= vm_pop(vm->stack);
	int64_t second= vm_pop(vm->stack);
	if(first!=second){
		vm->stack->pc+=offset-3;
	}
	return 0;
}
uint8_t if_icmplt(struct v_machine* vm){
	int16_t offset;
	vm->stack->pc++;
	offset = *(vm->stack->code+vm->stack->pc);
	offset = offset << 8;
	vm->stack->pc++;
	offset+=*(vm->stack->code+vm->stack->pc);
	int64_t first= vm_pop(vm->stack);
	int64_t second= vm_pop(vm->stack);
	if(first>second){
		vm->stack->pc+=offset-3;
	}
	return 0;
}
uint8_t if_icmpge(struct v_machine* vm){
	int16_t offset;
	vm->stack->pc++;
	offset = *(vm->stack->code+vm->stack->pc);
	offset = offset << 8;
	vm->stack->pc++;
	offset+=*(vm->stack->code+vm->stack->pc);
	int64_t first= vm_pop(vm->stack);
	int64_t second= vm_pop(vm->stack);
	if(first<=second){
		vm->stack->pc+=offset-3;
	}
	return 0;
}
uint8_t if_icmpgt(struct v_machine* vm){
	int16_t offset;
	vm->stack->pc++;
	offset = *(vm->stack->code+vm->stack->pc);
	offset = offset << 8;
	vm->stack->pc++;
	offset+=*(vm->stack->code+vm->stack->pc);
	int64_t first= vm_pop(vm->stack);
	int64_t second= vm_pop(vm->stack);
	if(first<second){
		vm->stack->pc+=offset-3;
	}
	return 0;
}
uint8_t if_icmple(struct v_machine* vm){
	int16_t offset;
	vm->stack->pc++;
	offset = *(vm->stack->code+vm->stack->pc);
	offset = offset << 8;
	vm->stack->pc++;
	offset+=*(vm->stack->code+vm->stack->pc);
	int64_t first= vm_pop(vm->stack);
	int64_t second= vm_pop(vm->stack);
	if(first>=second){
		vm->stack->pc+=offset-3;
	}
	return 0;
}


uint8_t vm_goto(struct v_machine* vm){
	int16_t offset;
	vm->stack->pc++;
	offset = (int16_t)*(vm->stack->code+vm->stack->pc);
	offset = offset << 8;
	vm->stack->pc++;
	offset+=*(vm->stack->code+vm->stack->pc);
	vm->stack->pc+=offset-3;
	return 0;
}

uint8_t iadd(struct v_machine* vm){
	vm_push(vm->stack, vm_pop(vm->stack)+vm_pop(vm->stack));
	return 0;
}

uint8_t iand(struct v_machine* vm){
	vm_push(vm->stack, vm_pop(vm->stack)&vm_pop(vm->stack));
	return 0;
}

uint8_t vm_return(struct v_machine* vm){
	if(vm->stack->prev_frame != 0){
		vm->stack = vm->stack->prev_frame;
	} else {
		vm->stack->pc = vm->stack->code_length;
	}
	return 0;
}

uint8_t iinc(struct v_machine* vm){
	uint8_t index;
	int64_t adding;
	vm->stack->pc++;
	index = *(vm->stack->code+vm->stack->pc);
	vm->stack->pc++;
	adding=*(vm->stack->code+vm->stack->pc);
	vm->stack->array[index]+=adding;
	return 0;
}
uint8_t invokestatic(struct v_machine* vm){
	struct class_header* class;
	uint16_t id, class_name_id, class_id, method_name_type_id, method_name_id, method_type_id;
	vm->stack->pc++;
	id = (int16_t)*(vm->stack->code+vm->stack->pc);
	id = id << 8;
	vm->stack->pc++;
	id+=*(vm->stack->code+vm->stack->pc);
	class_id = vm->stack->current_class->constant_pool[id]->info[0];
	class_id = class_id << 8;
	class_id += vm->stack->current_class->constant_pool[id]->info[1];
	method_name_type_id = vm->stack->current_class->constant_pool[id]->info[2];
	method_name_type_id = method_name_type_id << 8;
	method_name_type_id = vm->stack->current_class->constant_pool[id]->info[3];
	class_name_id = vm->stack->current_class->constant_pool[class_id]->info[0];
	class_name_id = class_name_id << 8;
	class_name_id += vm->stack->current_class->constant_pool[class_id]->info[1];

	if((class = get_by_name_heap(vm->stack->current_class->constant_pool[class_name_id]->info, vm->class_heap))==NULL){
	//TODO: add .class to class name !!!!
	//enum v_loader_status load_status = load_file( , class, stderr);		
	} 
	method_name_id = vm->stack->current_class->constant_pool[method_name_type_id]->info[0];
	method_name_id = method_name_type_id << 8;
	method_name_id = vm->stack->current_class->constant_pool[method_name_type_id]->info[1];
	struct v_stack_frame* new_frame;
	new_frame = vm_init_frame(class, vm->stack->current_class->constant_pool[method_name_id]->info);
	method_type_id = vm->stack->current_class->constant_pool[method_name_type_id]->info[2];
	method_type_id = method_name_type_id << 8;
	method_type_id = vm->stack->current_class->constant_pool[method_name_type_id]->info[3];
	uint64_t args_count = parse_args(vm->stack->current_class->constant_pool[method_type_id]->info);
	for(uint64_t i =0; i<args_count; i++){
		new_frame->array[i]= vm_pop(vm->stack);
	}
	new_frame->prev_frame = vm->stack;
	vm->stack = new_frame;
	vm->stack->pc--;
	return 0;
}

uint8_t ireturn(struct v_machine* vm){
	if(vm->stack->prev_frame != 0){
		vm_push(vm->stack->prev_frame, vm_pop(vm->stack));
		vm->stack = vm->stack->prev_frame;
	} else {
		vm->stack->pc = vm->stack->code_length;
	}
	return 0;
}

enum v_interpreter_status interpret(struct v_machine* vm){
	while(vm->stack->pc<vm->stack->code_length){
			switch(*(vm->stack->code+vm->stack->pc)){
			case 0x2: iconst_m1(vm); break;
			case 0x3: iconst_0(vm); break;
			case 0x4: iconst_1(vm); break;
			case 0x5: iconst_2(vm); break;
			case 0x6: iconst_3(vm); break;
			case 0x7: iconst_4(vm); break;
			case 0x8: iconst_5(vm); break;

			case 0x10: bipush(vm, *(vm->stack->code+vm->stack->pc+1)); vm->stack->pc++; break;

			case 0x12: ldc(vm, *(vm->stack->code+vm->stack->pc+1)); vm->stack->pc++; break;

			case 0x1a: iload_0(vm);  break;
			case 0x1b: iload_1(vm);  break;
			case 0x1c: iload_2(vm);  break;
			case 0x1d: iload_3(vm);  break;

			case 0x9f: if_icmpeq(vm); break;
			case 0xa0: if_icmpne(vm); break;
			case 0xa1: if_icmplt(vm); break;
			case 0xa2: if_icmpge(vm); break;
			case 0xa3: if_icmpgt(vm); break;
			case 0Xa4: if_icmple(vm); break;

			case 0xa7: vm_goto(vm);  break;

			case 0xb1: vm_return(vm); break;

			case 0x3b: istore_0(vm); break;
			case 0x3c: istore_1(vm); break;
			case 0x3d: istore_2(vm); break;
			case 0x3e: istore_3(vm); break;

			case 0x60:iadd(vm); break;

			case 0x7e:iand(vm); break;

			case 0x84:iinc(vm); break;

			case 0xac: ireturn(vm); break;

			case 0xb8:if(invokestatic(vm)==1){return INTER_NO_CLASS;} break;
			default: return INTER_NO_WORD;
		}

		vm->stack->pc++;
	}


	return INTER_OK;
}



