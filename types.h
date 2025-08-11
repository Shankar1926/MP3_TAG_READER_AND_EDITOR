/*
 * File Name   : types.h
 * Description : Contains common macros and enumerations used 
 *               across the MP3 Tag Reader and Editor project.
*/
#ifndef TYPES_H
#define TYPES_H
#define MAX_FRAME_COUNT 10
typedef enum
{
    e_success,
    e_failure
}Status;

typedef enum
{
    e_view,
    e_edit,
    e_help,
    e_unsupported
}OperationType;

#endif


