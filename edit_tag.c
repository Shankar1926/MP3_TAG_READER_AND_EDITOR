/***********************************************************************
 *  File Name   : edit_tag.c
 *  Description : Provides functionality to edit ID3v2 tag frames in
 *                MP3 files.
 *
 *                Responsibilities:
 *                - Validate and parse edit arguments.
 *                - Locate and modify specific tag frame content.
 *                - Reconstruct updated MP3 with modified tag frame.
 *                - Display edit summary after update.
 *
 *  Author      : Shankar K
 *  Roll No     : 25008_008
 *  Date        : 08-Aug-2025
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit_tag.h"
#include "types.h"

int convert_to_littleEndian(const char *buffer)
{
    return ((unsigned char)buffer[0] << 24) |
           ((unsigned char)buffer[1] << 16) |
           ((unsigned char)buffer[2] << 8) |
           (unsigned char)buffer[3];
}

Status read_and_validate_edit(int argc, char *argv[], EditTag *editTag)
{
    if (argc < 5)
    {
        printf("Error: Missing arguments.\n");
        return e_failure;
    }
    // Check if file is .mp3
    if (strstr(argv[4], ".mp3") == NULL)
    {
        printf("Error: File must be in .mp3 format\n");
        return e_failure;
    }
    strcpy(editTag->file_name, argv[4]);

    // Validate tag option
    if (argv[2][0] != '-' || argv[2][2] != '\0')
    {
        printf("Error: Invalid tag option '%s'. Use one of the following:\n", argv[2]);
        printf("  -t : Title\n  -a : Artist\n  -A : Album\n  -y : Year\n  -m : Genre\n  -c : Comment\n");
        return e_failure;
    }

    strcpy(editTag->tag_option, argv[2]);
    strcpy(editTag->new_content, argv[3]);

    // Map tag to frame ID
    switch (argv[2][1])
    {
        case 't': strcpy(editTag->frame_id, "TIT2"); break; // Title
        case 'a': strcpy(editTag->frame_id, "TPE1"); break; // Artist
        case 'A': strcpy(editTag->frame_id, "TALB"); break; // Album
        case 'y': strcpy(editTag->frame_id, "TYER"); break; // Year
        case 'm': strcpy(editTag->frame_id, "TCON"); break; // Genre
        case 'c': strcpy(editTag->frame_id, "COMM"); break; // Comment
        default:
            printf("Error: Unsupported tag option '%s'\n", argv[2]);
            return e_failure;
    }
    return e_success;
}

Status edit_tag_data(EditTag *editTag)
{
    FILE *in_fp = fopen(editTag->file_name, "rb");
    if (!in_fp)
    {
        perror("Error opening input file");
        return e_failure;
    }

    FILE *out_fp = fopen("output.mp3", "wb");
    if (!out_fp)
    {
        perror("Error opening output file");
        fclose(in_fp);
        return e_failure;
    }

    // Copy ID3v2 header
    char header[10];
    fread(header, 1, 10, in_fp);
    fwrite(header, 1, 10, out_fp);

    int edited = 0;

    while (1)
    {
        char frame_id[5] = {0};
        char size_buf[4], flags_and_encoding[3];

        // Read frame ID
        if (fread(frame_id, 1, 4, in_fp) != 4 || frame_id[0] == 0x00)
            break;

        // Read size and flags
        if (fread(size_buf, 1, 4, in_fp) != 4 || fread(flags_and_encoding, 1, 3, in_fp) != 3)
            break;

        int frame_size = convert_to_littleEndian(size_buf);

        char *data = malloc(frame_size);
        if (!data)
        {
            fclose(in_fp);
            fclose(out_fp);
            return e_failure;
        }

        if (fread(data, 1, frame_size - 1, in_fp) != frame_size - 1)
        {
            free(data);
            fclose(in_fp);
            fclose(out_fp);
            return e_failure;
        }

        if (strncmp(frame_id, editTag->frame_id, 4) == 0)
        {
            int new_size = strlen(editTag->new_content)+1;

            unsigned char new_size_buf[4] = {
                (new_size >> 24) & 0xFF,
                (new_size >> 16) & 0xFF,
                (new_size >> 8) & 0xFF,
                new_size & 0xFF
            };

            fwrite(frame_id, 1, 4, out_fp);
            fwrite(new_size_buf, 1, 4, out_fp);
            fwrite(flags_and_encoding, 1, 3, out_fp);
            fwrite(editTag->new_content, 1, new_size - 1, out_fp);

            edited = 1;
        }
        else
        {
            fwrite(frame_id, 1, 4, out_fp);
            fwrite(size_buf, 1, 4, out_fp);
            fwrite(flags_and_encoding, 1, 3, out_fp);
            fwrite(data, 1, frame_size -1, out_fp);
        }
        free(data);
    }
    
    char ch;
    while( ( fread(&ch , 1, 1, in_fp) == 1))
    {
        fwrite(&ch , 1, 1, out_fp);
    }

    fclose(in_fp);
    fclose(out_fp);

    if (edited)
{
    remove(editTag->file_name);
    rename("output.mp3", editTag->file_name);

    const char *tag_name = NULL;

    switch (editTag->tag_option[1])
    {
        case 't': tag_name = "Title"; break;
        case 'a': tag_name = "Artist"; break;
        case 'A': tag_name = "Album"; break;
        case 'y': tag_name = "Year"; break;
        case 'm': tag_name = "Genre"; break;
        case 'c': tag_name = "Comment"; break;
        default: tag_name = "Tag"; break;
    }
    printf("----------------------------------------------\n");
    printf("%-8s: %s\n", tag_name, editTag->new_content);
    printf("%s changed successfully\n", tag_name);
    printf("----------------------------------------------\n");
    }
    else
    {
        remove("output.mp3");
        printf("⚠️  Frame '%s' not found. No changes made.\n", editTag->frame_id);
    }

    return e_success;
}
