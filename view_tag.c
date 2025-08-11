/***********************************************************************
 *  File Name   : view_tag.c
 *  Description : Contains implementation of functions to view ID3v2
 *                tags in MP3 files.
 *
 *                Responsibilities:
 *                - Identify operation type from command-line arguments.
 *                - Validate arguments and file format.
 *                - Open MP3 file and read ID3v2 header and frames.
 *                - Extract and display metadata such as Title, Artist,
 *                  Album, Year, Genre, etc.
 *
 *  Author      : Shankar K
 *  Roll No     : 25008_008
 *  Date        : 08-Aug-2025
 ***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "view_tag.h"
#include "types.h"

const char *tags_name[MAX_FRAME_COUNT] = {
    "TPE1", "TIT2", "TALB", "TYER", "TCON", "TEXT", "TCOM", "COMM" ,"TDRC"
};

OperationType check_operation_type(char *argv[])
{
    if (!argv[1]) return e_unsupported;
    if (strcmp(argv[1], "-v") == 0) return e_view;
    if (strcmp(argv[1], "-e") == 0) return e_edit;
    if (strcmp(argv[1], "--help") == 0) return e_help;
    return e_unsupported;
}

Status read_and_validate_args(int argc, char *argv[], TagInfo *tagInfo)
{
    if (argc < 3)
    {
        return e_failure;
    }
    char *ext = strrchr(argv[2], '.');
    if (!ext || strcmp(ext, ".mp3") != 0)
    {
        fprintf(stderr, "❌ Error: Audio file should be in .mp3 format.\n");
        return e_failure;
    }

    strcpy(tagInfo->audio_file_name, argv[2]);
    return e_success;
}



Status view_tag_data(TagInfo *tagInfo)
{
    if (open_audio_file(tagInfo) == e_failure) return e_failure;
    if (read_header(tagInfo) == e_failure) return e_failure;
    if (read_frame_data(tagInfo) == e_failure) return e_failure;
    if (print_frame_data(tagInfo) == e_failure) return e_failure;
    return e_success;
}

Status open_audio_file(TagInfo *tagInfo)
{
    tagInfo->audio_fp = fopen(tagInfo->audio_file_name, "rb");
    if (!tagInfo->audio_fp)
    {
        printf("Error: Unable to open file %s\n", tagInfo->audio_file_name);
        return e_failure;
    }
    return e_success;
}

Status read_header(TagInfo *tagInfo)
{
    fseek(tagInfo->audio_fp , 0 , SEEK_SET);
    char header[10];
    fread(header, 1, 10, tagInfo->audio_fp);
    if (strncmp(header, "ID3", 3) != 0)
    {
        printf("Error: ID3 header not found\n");
        return e_failure;
    }
    return e_success;
}

int convert_to_little_Endian(const char *buffer)
{
    return ((unsigned char)buffer[0] << 24) |
           ((unsigned char)buffer[1] << 16) |
           ((unsigned char)buffer[2] << 8) |
           ((unsigned char)buffer[3]);
}

Status read_frame_data(TagInfo *tagInfo)
{
    int frame_index = 0;
    fseek(tagInfo->audio_fp, 10, SEEK_SET); // skip ID3 header

    while (frame_index < MAX_FRAME_COUNT)
    {
        char frame_id[5] = {0};
        char size_buf[4];
        char flags[4];

        if (fread(frame_id, 1, 4, tagInfo->audio_fp) != 4 || frame_id[0] == 0)
            break;
        if (fread(size_buf, 1, 4, tagInfo->audio_fp) != 4)
            break;

        if (fread(flags, 1, 3, tagInfo->audio_fp) != 3)
            break;

        int frame_size = convert_to_little_Endian(size_buf);

        if (frame_size <= 0 || frame_size > 1000) break;
        char *data = malloc(frame_size + 1);
        if (!data) return e_failure;

        if (fread(data, 1, frame_size - 1, tagInfo->audio_fp) != frame_size - 1)
        {
            free(data);
            return e_failure;
        }
        data[frame_size] = '\0';

        strncpy(tagInfo->frames[frame_index].frame_id, frame_id, 4);
        tagInfo->frames[frame_index].frame_id[4] = '\0';
        tagInfo->frames[frame_index].size = frame_size;
        tagInfo->frames[frame_index].data = data;

        frame_index++;
    }

    tagInfo->frame_count = frame_index;
    return e_success;
}

Status print_frame_data(TagInfo *tagInfo)
{
    printf("----------------------------------------------\n");
    printf("Mp3 Tag Reader and Editor fpr ID3v2\n");
    printf("----------------------------------------------\n");

    for (int i = 0; i < tagInfo->frame_count; i++)
    {
        const char *id = tagInfo->frames[i].frame_id;
        const char *value = tagInfo->frames[i].data ;

        if (strcmp(id, "TIT2") == 0) printf("Title    : %s\n", value);
        else if (strcmp(id, "TPE1") == 0) printf("Artist   : %s\n", value);
        else if (strcmp(id, "TALB") == 0) printf("Album    : %s\n", value);
        else if (strcmp(id, "TYER") == 0) printf("Year     : %s\n", value);
        else if (strcmp(id, "TDRC") == 0) printf("Year     : %s\n", value);
        else if (strcmp(id, "TCON") == 0) printf("Genre    : %s\n", value);
        else if (strcmp(id, "TEXT") == 0) printf("Lyricist : %s\n", value);
        else if (strcmp(id, "TCOM") == 0) printf("Composer : %s\n", value);
        else if (strcmp(id, "COMM") == 0) printf("Comment  : %s\n", value);
    }
    printf("----------------------------------------------\n");
    return e_success;
}
