/***********************************************************************
 *  File Name   : main.c
 *  Description : Entry point for MP3 Tag Editor & Viewer.
 *
 *                Responsibilities:
 *                - Parses command-line arguments.
 *                - Dispatches to view or edit functions based on flags.
 *                - Provides help information when needed.
 *
 *  Author      : Shankar K
 *  Roll No     : 25008_008
 *  Date        : 08-Aug-2025
 ***********************************************************************/

#include <stdio.h>
#include <string.h>
#include "types.h"
#include "view_tag.h"
#include "edit_tag.h"

/* Function to display brief help for view and edit */
void help_info_edit()
{
    printf("USAGE :\n");
    printf("  To view : ./a.out -v <file.mp3>\n");
    printf("  To edit : ./a.out -e -t/-a/-A/-y/-m/-c \"value\" <file.mp3>\n\n");
}

/* Function to display detailed help */
void help_info()
{
    printf("Mp3tag --Help\n");
    printf("Usage:\n");
    printf("  For view : mp3tag -v <file.mp3>\n");
    printf("  For edit : mp3tag -e -[tag] \"value\" <file.mp3>\n\n");

    printf("Options:\n");
    printf("%15s  %s\n", "-t", "Modifies Title tag");
    printf("%15s  %s\n", "-a", "Modifies Artist tag");
    printf("%15s  %s\n", "-y", "Modifies Year tag");
    printf("%15s  %s\n", "-A", "Modifies Album tag");
    printf("%15s  %s\n", "-m", "Modifies Content tag");
    printf("%15s  %s\n", "-c", "Modifies Comment tag");
}

/* Main function to handle argument parsing and call appropriate actions */
int main(int argc, char *argv[])
{
    TagInfo tagInfo;
    EditTag editTag;

    OperationType operation = check_operation_type(argv);

    switch (operation)
    {
        case e_view:
            if (read_and_validate_args(argc, argv, &tagInfo) == e_success)
                return view_tag_data(&tagInfo);
            else
            {
                fprintf(stderr, "❌ Invalid view command format.\n");
                help_info_edit();
                return -1;
            }

        case e_edit:
            if (read_and_validate_edit(argc, argv, &editTag) == e_success)
                return edit_tag_data(&editTag);
            else
            {
                fprintf(stderr, "❌ Invalid edit command format.\n");
                help_info_edit();
                return -1;
            }

        case e_help:
            help_info();
            break;

        default:
            fprintf(stderr, "❌ Invalid arguments.\n");
            help_info_edit();
            return -1;
    }

    return 0;
}
