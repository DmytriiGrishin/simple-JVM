#include "launcher.h"
#include "execute_engine.h"


int main (int argc, char const* const* argv){
	if(argc != 2) {fprintf(stderr, "Usage: vm Classfile\n"); return 2;}
	struct v_machine vm;
	struct class_header* class = malloc(sizeof(struct class_header)); 
	argv++;
	enum v_loader_status load_status = load_file(*argv, class, stderr);
	if (load_status == 	LOADER_IO_ERROR) return LAUNCH_FILE_ERROR;
	if (load_status == LOADER_FORMAT_ERROR || load_status == LOADER_VERSION_ERROR) return LAUNCH_FORMAT_ERROR;
	struct class_heap_t* class_heap = init_heap(class);
	vm_init(class_heap, class, &vm);
	if(interpret(&vm)==1){
		printf("No such word: 0x%x\n", *(vm.stack->code+vm.stack->pc));
		return LAUNCH_FILE_ERROR;
	}
	return LAUNCH_OK;
}



