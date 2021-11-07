#ifndef _PROCESSING_H
#define _PROCESSING_H

#include <unistd.h>
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

typedef matrix_t shared_matrix_t;

#endif // _PROCESSING_H
