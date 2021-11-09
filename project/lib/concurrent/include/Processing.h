#ifndef _PROCESSING_H
#define _PROCESSING_H

#include <unistd.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>
#include <sys/mman.h>
#include "Matrix.h"

enum processing_errors{
    PROC_OK = 0,
    PROC_ISCHILD = 0,
    PROC_ERR_NULLPTR,
    PROC_ERR,
    PROC_EOMATRIX = -1,
    PROC_BAD_FORK = -1
};

// imply that this implementation
// uses shared memory
typedef matrix_t shared_matrix_t;

#endif // _PROCESSING_H
