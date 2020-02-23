#ifndef EXE_FILEPATH
#error "You must define EXE_FILEPATH macro before include this file"
#endif

#define PARM_NODE_CLASS Canny
#define PARM_NODE_NAME canny
#include EXE_FILEPATH

#ifndef SKIP_FILE_NODE
#define PARM_NODE_CLASS FileInput
#define PARM_NODE_NAME file_input
#include EXE_FILEPATH
#endif // SKIP_FILE_NODE