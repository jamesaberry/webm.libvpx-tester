#include <string>

#ifndef DRIVER_H_GUARD
#define DRIVER_H_GUARD

int create_working_folder(int argc, char *argv[], char *WorkingDirChar);
std::string date_string();
void vpxt_on_error_output();
void write_32bit_quick_test(std::string WorkingDir);
void write_64bit_quick_test(std::string WorkingDir);
int print_quick_test_files(std::string WorkingDir);
void vpxt_test_help(int argc, char *argv[], std::string WorkingDir);
int  vpxt_tool_help(std::string InputString);
void format_summary(const char *InputFileNameCharAr);
int show_hidden_cmds();

#endif