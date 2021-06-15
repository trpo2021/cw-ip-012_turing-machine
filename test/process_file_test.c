#include <libturing/process_file.h>

#include <ctest.h>
#include <stdlib.h>
#include <string.h>

#define PATH_TEST "test/file_test.txt"
#define PATH_FILL_TEST "test/file_fillprog_test.txt"

CTEST(suite_process_file, get_line)
{
    FILE* fin = fopen(PATH_TEST, "r");
    if (fin == NULL) {
        fprintf(stderr, "Error! Can't open `%s` ", PATH_TEST);
        ASSERT_NOT_NULL(fin);
    }

    char* line = get_line(fin);
    ASSERT_STR("   ", line);

    char* line2 = get_line(fin);
    ASSERT_STR("", line2);

    char* line3 = get_line(fin);
    ASSERT_STR("q1 a b l q2 ", line3);

    char* line4 = get_line(fin);
    ASSERT_STR(
            "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
            "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
            "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
            "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq3 a b r "
            "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
            "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
            "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"
            "qqqqqqqqqqqqqqqqq4",
            line4);

    char* line5 = get_line(fin);
    ASSERT_NULL(line5);

    fclose(fin);
    free(line);
    free(line2);
    free(line3);
    free(line4);
    free(line5);
}

CTEST(suite_process_file, strip)
{
    char* line = calloc(32, sizeof(char));

    strcpy(line, "hello");
    line = strip(line);
    ASSERT_STR("hello", line);

    strcpy(line, " hello");
    line = strip(line);
    ASSERT_STR("hello", line);

    strcpy(line, "    ");
    line = strip(line);
    ASSERT_STR("", line);

    strcpy(line, "");
    line = strip(line);
    ASSERT_STR("", line);

    strcpy(line, "hello ");
    line = strip(line);
    ASSERT_STR("hello", line);

    strcpy(line, "hello\n\0");
    line = strip(line);
    ASSERT_STR("hello", line);

    strcpy(line, "hello\r\n");
    line = strip(line);
    ASSERT_STR("hello", line);

    free(line);
}

CTEST(suite_process_file, get_word)
{
    char* line = calloc(32, sizeof(char));
    char* ret;

    strcpy(line, "hello\r\n");
    char* word = get_word(line, &ret);
    ASSERT_STR("hello", word);
    word = get_word(ret, &ret);
    ASSERT_NULL(word);
    free(word);

    strcpy(line, "hello world ");
    char* word2 = get_word(line, &ret);
    ASSERT_STR("hello", word2);
    word2 = get_word(ret, &ret);
    ASSERT_STR("world", word2);
    word2 = get_word(ret, &ret);
    ASSERT_NULL(word2);
    free(word2);

    strcpy(line, "   hello         ");
    char* word3 = get_word(line, &ret);
    ASSERT_STR("hello", word3);
    word3 = get_word(ret, &ret);
    ASSERT_NULL(word3);
    free(word3);

    strcpy(line, "");
    char* word4 = get_word(line, &ret);
    ASSERT_NULL(word4);
    free(word4);

    strcpy(line, "   hello    world   !  ");
    char* word5 = get_word(line, &ret);
    ASSERT_STR("hello", word5);
    word5 = get_word(ret, &ret);
    ASSERT_STR("world", word5);
    word5 = get_word(ret, &ret);
    ASSERT_STR("!", word5);
    word5 = get_word(ret, &ret);
    ASSERT_NULL(word5);
    free(word5);

    free(line);
}

CTEST(suite_process_file, fill_program)
{
    FILE* fin = fopen(PATH_FILL_TEST, "r");
    if (fin == NULL) {
        fprintf(stderr, "Error! Can't open `%s` ", PATH_TEST);
        ASSERT_NOT_NULL(fin);
    }
    Program prog = create_program();

    prog = fill_program(fin, prog);

    ASSERT_EQUAL(2, prog.length);

    ASSERT_STR("q1", prog.names[0]);
    ASSERT_STR("q1", prog.states[0]->name);
    ASSERT_EQUAL('a', prog.states[0]->actions[0]->symb_old);
    ASSERT_EQUAL('b', prog.states[0]->actions[0]->symb_new);
    ASSERT_EQUAL(MOTION_LEFT, prog.states[0]->actions[0]->motion);
    ASSERT_STR("q2", prog.states[0]->actions[0]->next_state);

    ASSERT_STR(
            "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq3",
            prog.names[1]);
    ASSERT_EQUAL('a', prog.states[1]->actions[0]->symb_old);
    ASSERT_EQUAL('b', prog.states[1]->actions[0]->symb_new);
    ASSERT_EQUAL(MOTION_RIGHT, prog.states[1]->actions[0]->motion);
    ASSERT_STR(
            "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq4",
            prog.states[1]->actions[0]->next_state);

    fclose(fin);
    free_program(prog);
}
