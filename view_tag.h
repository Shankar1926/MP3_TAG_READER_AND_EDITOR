/***********************************************************************
 *  File Name   : view_tag.h
 *  Description : Header file for viewing ID3v2 tag information from MP3
 *                files.
 *
 *                Responsibilities:
 *                - Define TagInfo and Frame structures.
 *                - Declare constants, functions, and external variables
 *                  for parsing and displaying tag metadata such as
 *                  Title, Artist, Album, Year, Genre, etc.
 *
 *  Author      : Shankar K
 *  Roll No     : 25008_008
 *  Date        : 08-Aug-2025
 ***********************************************************************/

#ifndef VIEW_TAG_H
#define VIEW_TAG_H

#include <stdio.h>
#include "types.h"

extern const char *tags_name[MAX_FRAME_COUNT];

typedef struct
{
    char frame_id[5];
    int size;
    char *data;
} Frame;

typedef struct
{
    char audio_file_name[100];
    FILE *audio_fp;
    Frame frames[MAX_FRAME_COUNT];
    int frame_count;
} TagInfo;

OperationType check_operation_type(char *argv[]);
Status read_and_validate_args(int argc, char *argv[], TagInfo *tagInfo);
Status view_tag_data(TagInfo *tagInfo);
Status open_audio_file(TagInfo *tagInfo);
Status read_header(TagInfo *tagInfo);
Status read_frame_data(TagInfo *tagInfo);
Status print_frame_data(TagInfo *tagInfo);
int convert_to_little_Endian(const char *buffer);

#endif
