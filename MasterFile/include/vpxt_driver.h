#include <string>

#ifndef DRIVER_H_GUARD
#define DRIVER_H_GUARD

int create_working_folder(int argc, const char *argv[], char *WorkingDirChar);
std::string date_string();
void vpxt_on_error_output();
void write_32bit_quick_test(const std::string WorkingDir);
void write_64bit_quick_test(const std::string WorkingDir);
int print_quick_test_files(const std::string WorkingDir);
int  vpxt_test_help(const char *InputChar, int printSummary);
int  vpxt_tool_help(const char *InputChar, int printSummary);
void format_summary(const char *InputFileNameCharAr);
int show_hidden_cmds();
int  vpxt_run_multi(int argc, const char *argv[], const std::string WorkingDir);

#endif