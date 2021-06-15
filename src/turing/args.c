#include <libturing/checks.h>
#include <libturing/process_file.h>

#include <turing/args.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_args(Params* params, int argc, char** argv)
{
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help_message();
            exit(EXIT_SUCCESS);
        }
    }
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'f':
                if (params->input != NULL) {
                    raise_and_exit_argparse();
                }
                params->input = argv[++i];
                break;
            case 'o':
                if (params->output != NULL) {
                    raise_and_exit_argparse();
                }
                params->output = argv[++i];
                break;
            case 's':
                if (params->startfile != NULL) {
                    raise_and_exit_argparse();
                }
                params->startfile = argv[++i];
                break;
            case 't':
                if (params->startstate != NULL) {
                    raise_and_exit_argparse();
                }
                params->startstate = argv[++i];
                break;
            case '-':
                if (strcmp("--silent", argv[i]) == 0) {
                    params->silent = 1;
                }
                break;
            default:
                fprintf(stderr, "Wrong parameter: %s\n", argv[i]);
                raise_and_exit_argparse();
            }
        }
    }
#ifdef DEBUG
    print_params(*params);
#endif
}

char* get_input_filename()
{
    char* str_dest = calloc(MAX_FILENAME, sizeof(char));
    check_allocated(str_dest);
    fgets(str_dest, MAX_FILENAME, stdin);
    str_dest = get_word(str_dest, NULL);
    return (str_dest == NULL || str_dest[0] == '\0') ? NULL : str_dest;
}

void get_missing_params(Params* params)
{
    if (params->input == NULL) {
        printf("Input filename: [default: %s]\n", INPUT_DEFAULT);
        params->input = get_input_filename();
        if (params->input == NULL) {
#ifdef DEBUG
            printf("Using default path: %s\n\n", (char*)INPUT_DEFAULT);
#endif
            params->input = INPUT_DEFAULT;
        }
    }

    if (params->output == NULL) {
        printf("Output filename: [default: %s]\n",
               OUTPUT_DEFAULT ? OUTPUT_DEFAULT : "None");
        params->output = get_input_filename();
        if (params->output == NULL && OUTPUT_DEFAULT) {
#ifdef DEBUG
            printf("Using default path: %s\n\n", (char*)OUTPUT_DEFAULT);
#endif
            params->output = OUTPUT_DEFAULT;
        }
    }

    if (params->startstate == NULL) {
        printf("Startstate name: [default: `%s`] (you can enter it "
               "manually later)\n",
               STARTSTATE_DEFAULT ? STARTSTATE_DEFAULT : "None");
        params->startstate = get_line(stdin);
        params->startstate = strip(params->startstate);
        if (params->startstate[0] == '\0')
            params->startstate = NULL;
        if (params->startstate == NULL && STARTSTATE_DEFAULT) {
#ifdef DEBUG
            printf("Using default name: `%s`\n\n", (char*)STARTSTATE_DEFAULT);
#endif
            params->startstate = STARTSTATE_DEFAULT;
        } else {
            check_statename(params->startstate);
        }
    }

    if (params->startfile == NULL) {
        printf("Startline filename: [default: %s] (you can enter it "
               "manually later)\n",
               STARTFILE_DEFAULT ? STARTFILE_DEFAULT : "None");
        params->startfile = get_input_filename();
        if (params->startfile == NULL && STARTFILE_DEFAULT) {
#ifdef DEBUG
            printf("Using default path: %s\n\n", (char*)STARTFILE_DEFAULT);
#endif
            params->startfile = STARTFILE_DEFAULT;
        }
    }
}

void print_params(Params params)
{
    printf("---\n");
    printf("Input name:\t%s\n", params.input);
    printf("Output name:\t%s\n", params.output);
    printf("Startfile name:\t%s\n", params.startfile);
    printf("Startstate:\t%s\n", params.startstate);
    printf("Silentmode:\t%s\n", params.silent ? "on" : "off");
    printf("---\n\n");
}

char* get_startline(Params params)
{
    char* line;
    if (params.startfile) {
        check_file_exists(params.startfile);
        FILE* fin = fopen(params.startfile, "r");
        line = get_line(fin);
        fclose(fin);
    } else {
        printf("Enter startline:\n");
        do {
            line = get_line(stdin);
            line = strip(line);
            if (line[0] == '\0')
                fprintf(stderr,
                        "You can't use empty string, please use `_` as "
                        "placeholder!\n");
        } while (line[0] == '\0');
    }
    line = strip(line);
    check_startline(line);
    return line;
}
