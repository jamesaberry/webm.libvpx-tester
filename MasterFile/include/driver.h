#include <string>
using namespace std;

#ifndef DRIVER_H_GUARD
#define DRIVER_H_GUARD

int create_working_folder(int argc, char *argv[], char *WorkingDirChar);
string date_string();
void vpxt_on_error_output();
void write_32bit_quick_test(string WorkingDir);
void write_64bit_quick_test(string WorkingDir);
void print_quick_test_files(string WorkingDir);
void vpxt_test_help(int argc, char *argv[], string WorkingDir);
int  vpxt_tool_help(string InputString);
void format_summary(char *InputFileNameCharAr);
void show_hidden_cmds();

#endif