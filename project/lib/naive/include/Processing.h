#ifndef _PROCESSING_H
#define _PROCESSING_H

#include "Matrix.h"

enum processing_errors{
    PROC_OK = 0,
    PROC_ERR_NULLPTR,
    PROC_ERR
};

int mirror_by_side_diagonal(matrix_t *const mx);

#endif // _PROCESSING_H
