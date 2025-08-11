/***********************************************************************
 *  File Name   : edit_tag.h
 *  Description : Header file for editing ID3v2 tag frames in MP3 files.
 *
 *                Responsibilities:
 *                - Defines the EditTag structure to hold editing info.
 *                - Declares validation and editing function prototypes.
 *
 *  Author      : Shankar K
 *  Roll No     : 25008_008
 *  Date        : 08-Aug-2025
 ***********************************************************************/

#ifndef EDIT_TAG_H
#define EDIT_TAG_H

#include "types.h"

typedef struct
{
    char tag_option[3];        // e.g. -t
    char new_content[100];     // New value to insert
    char frame_id[5];          // Mapped frame ID (TIT2, etc)
    char file_name[100];       // MP3 file name
} EditTag;

Status read_and_validate_edit(int argc,char *argv[], EditTag *editTag);
Status edit_tag_data(EditTag *editTag);

#endif
