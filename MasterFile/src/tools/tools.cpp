#define _CRT_SECURE_NO_WARNINGS
#include "vpxt_test_definitions.h"
#include "vpxt_utilities.h"
#include "vpxt_driver.h"
#include "comp_ivf.h"
#include "onyx.h"
#include "ivf.h"
#include <sstream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#if defined(ARM)
#include "arm.h"
#else
#if !defined(_PPC)
#include "x86.h"
#endif
#endif

#if defined(_WIN32)
#include "on2vpplugin.h"
#define snprintf _snprintf
#else
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
     ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif

#ifdef _PPC
# define make_endian_16(a) \
    (((unsigned int)(a & 0xff)) << 8) | (((unsigned int)(a & 0xff00)) >> 8)
# define make_endian_32(a)                                                                  \
    (((unsigned int)(a & 0xff)) << 24)    | (((unsigned int)(a & 0xff00)) << 8) |   \
    (((unsigned int)(a & 0xff0000)) >> 8) | (((unsigned int)(a & 0xff000000)) >> 24)
# define make_endian_64(a)  \
    ((a & 0xff) << 56           |   \
     ((a >>  8) & 0xff) << 48   |   \
     ((a >> 16) & 0xff) << 40   |   \
     ((a >> 24) & 0xff) << 32   |   \
     ((a >> 32) & 0xff) << 24   |   \
     ((a >> 40) & 0xff) << 16   |   \
     ((a >> 48) & 0xff) <<  8   |   \
     ((a >> 56) & 0xff))
# define MAKEFOURCC(ch0, ch1, ch2, ch3)                                 \
    ((DWORD)(BYTE)(ch0) << 24 | ((DWORD)(BYTE)(ch1) << 16) |    \
     ((DWORD)(BYTE)(ch2) << 8) | ((DWORD)(BYTE)(ch3)))
# define swap4(d)\
    ((d&0x000000ff)<<24) |  \
    ((d&0x0000ff00)<<8)  |  \
    ((d&0x00ff0000)>>8)  |  \
    ((d&0xff000000)>>24)
#else
# define make_endian_16(a)  a
# define make_endian_32(a)  a
# define make_endian_64(a)  a
# define MAKEFOURCC(ch0, ch1, ch2, ch3)                                 \
    ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |           \
     ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
# define swap4(d) d
#endif

//ivfdec ivfenc
extern int ivfdec(int argc, const char **argv_);
extern int ivfenc(int argc, const char **argv_);
extern void out_put(void *out,
                    const uint8_t *buf,
                    unsigned int len,
                    int do_md5);
extern int vp8_multi_resolution_encoder(int argc, char **argv);
extern int vp8_scalable_patterns(int argc, char **argv);

enum video_file_type
{
    FILE_TYPE_RAW,
    FILE_TYPE_IVF,
    FILE_TYPE_Y4M
};

int IVFParseandDelete(const char *dir_name)
{

#if defined(linux)

    unsigned char is_file = 0x8;
    unsigned char is_folder = 0x4;

    std::string input_str = dir_name;
    char get_first_file_char[255];
    snprintf(get_first_file_char, 255, "%s", input_str.c_str());
    // strcpy(get_first_file_char,input_str.c_str());

    std::string double_dot = "..";
    std::string single_dot = ".";

    DIR *find_file_data_a;
    struct dirent *h_find_a;
    std::string file_name_a;
    std::vector<std::string> SourceFileVectorA;

    find_file_data_a = opendir(dir_name);

    if (find_file_data_a == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (h_find_a = readdir(find_file_data_a))
        {
            if (h_find_a->d_type == is_folder && double_dot.compare(h_find_a->d_name)
                != 0 && single_dot.compare(h_find_a->d_name) != 0)
            {
                std::string full_path_name = dir_name;
                full_path_name += "/";
                full_path_name += h_find_a->d_name;
                IVFParseandDelete(full_path_name.c_str());
            }

            if (h_find_a->d_type == is_file)
            {
                std::string output_str = dir_name;
                output_str += "/";
                output_str += h_find_a->d_name;

                std::string file_name = h_find_a->d_name;
                std::string extention = file_name.substr(file_name.length() - 4,
                    4);

                if (extention.compare(".ivf") == 0)
                {
                    if (remove(output_str.c_str()) == 0)
                    {
                        tprintf(PRINT_STD, "%s Successfully Deleted\n\n",
                            output_str.c_str());
                    }
                    else
                    {
                        tprintf(PRINT_STD, "Error: %s Not Deleted\n\n",
                            output_str.c_str());
                    }
                }
            }


        }
    }

    return 0;

#endif

#if defined(_WIN32)
    std::string DirNameStr = dir_name;
    DirNameStr += "\\*";

    WIN32_FIND_DATA find_file_data;
    HANDLE h_find;

    std::string double_dot = "..";
    std::string single_dot = ".";

    h_find = FindFirstFile(DirNameStr.c_str(), &find_file_data);

    if (h_find == INVALID_HANDLE_VALUE)
    {
        tprintf(PRINT_STD, "FindFirstFile failed (%d)\n", GetLastError());
        return 0;
    }
    else
    {
        if (find_file_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY &&
            double_dot.compare(find_file_data.cFileName) != 0 &&
            single_dot.compare(find_file_data.cFileName) != 0)
        {
            std::string full_path_name = dir_name;
            full_path_name += "\\";
            full_path_name += find_file_data.cFileName;
            IVFParseandDelete(full_path_name.c_str());
        }
        else
        {
            if (double_dot.compare(find_file_data.cFileName) != 0 &&
                single_dot.compare(find_file_data.cFileName) != 0)
            {
                std::string output_str = dir_name;
                output_str += "\\";
                output_str += find_file_data.cFileName;

                std::string file_name = find_file_data.cFileName;
                std::string extention = file_name.substr(file_name.length() - 4,
                    4);

                if (extention.compare(".ivf") == 0)
                {
                    if (remove(output_str.c_str()) == 0)
                    {
                        tprintf(PRINT_STD, "%s Successfully Deleted\n\n",
                            output_str.c_str());
                    }
                    else
                    {
                        tprintf(PRINT_STD, "Error: %s Not Deleted\n\n",
                            output_str.c_str());
                    }
                }
            }
        }

        while (FindNextFile(h_find, &find_file_data) != 0)
        {
            if (find_file_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY &&
                double_dot.compare(find_file_data.cFileName) != 0 &&
                single_dot.compare(find_file_data.cFileName) != 0)
            {
                std::string full_path_name = dir_name;
                full_path_name += "\\";
                full_path_name += find_file_data.cFileName;
                IVFParseandDelete(full_path_name.c_str());
            }
            else
            {
                if (double_dot.compare(find_file_data.cFileName) != 0 &&
                    single_dot.compare(find_file_data.cFileName) != 0)
                {
                    std::string output_str = dir_name;
                    output_str += "\\";
                    output_str += find_file_data.cFileName;

                    std::string file_name = find_file_data.cFileName;
                    std::string extention = file_name.substr(file_name.length() -
                        4, 4);

                    if (extention.compare(".ivf") == 0)
                    {
                        if (remove(output_str.c_str()) == 0)
                        {
                            tprintf(PRINT_STD, "%s Successfully Deleted\n\n",
                                output_str.c_str());
                        }
                        else
                        {
                            tprintf(PRINT_STD, "Error: %s Not Deleted\n\n",
                                output_str.c_str());
                        }
                    }
                }
            }
        }

        FindClose(h_find);
    }

    return 0;
#endif
    return 0;
}
int txt_parse_and_copy(const char *dir_name,
                    const char *BaseOutputDir,
                    int base_input_str_length)
{
#if defined(linux)

    unsigned char is_file = 0x8;
    unsigned char is_folder = 0x4;

    std::string input_str = dir_name;

    std::string double_dot = "..";
    std::string single_dot = ".";

    DIR *find_file_data_a;
    struct dirent *h_find_a;
    std::string file_name_a;

    find_file_data_a = opendir(dir_name);

    if (find_file_data_a == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (h_find_a = readdir(find_file_data_a))
        {
            if (h_find_a->d_type == is_folder && double_dot.compare(h_find_a->d_name)
                != 0 && single_dot.compare(h_find_a->d_name) != 0)
            {
                std::string full_path_name = dir_name;
                full_path_name += "/";
                full_path_name += h_find_a->d_name;
                txt_parse_and_copy(full_path_name.c_str(), BaseOutputDir,
                    base_input_str_length);
            }

            if (h_find_a->d_type == is_file)
            {
                std::string output_str = dir_name;
                output_str += "/";
                output_str += h_find_a->d_name;

                std::string file_name = h_find_a->d_name;
                std::string extention = file_name.substr(file_name.length() - 4,
                    4);

                if (extention.compare(".txt") == 0)
                {
                    // if extention matches txt
                    std::string file_name_part_2 =
                        output_str.substr(base_input_str_length + 1,
                        output_str.length() - base_input_str_length - 1);
                    std::string file_name_part_1 = BaseOutputDir;
                    file_name_part_1 += "/";
                    file_name_part_1 += file_name_part_2;
                    std::string copy_cmd_str = "cp \"" + output_str +
                        "\" \"" + file_name_part_1 + "\"";

                    std::string dir_name =
                        file_name_part_1.substr(0,
                        file_name_part_1.length() - file_name.length());
                    std::string mk_dir_str = "mkdir \"" + dir_name + "\"";

                    system(mk_dir_str.c_str());
                    system(copy_cmd_str.c_str());
                }
            }
        }
    }

    return 0;

#elif defined(_PPC)

    unsigned char is_file = 0x8;
    unsigned char is_folder = 0x4;

    std::string input_str = dir_name;

    std::string double_dot = "..";
    std::string single_dot = ".";

    DIR *find_file_data_a;
    struct dirent *h_find_a;
    std::string file_name_a;

    find_file_data_a = opendir(dir_name);

    if (find_file_data_a == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (h_find_a = readdir(find_file_data_a))
        {
            if (h_find_a->d_type == is_folder && double_dot.compare(h_find_a->d_name)
                != 0 && single_dot.compare(h_find_a->d_name) != 0)
            {
                std::string full_path_name = dir_name + "/" + h_find_a->d_name;
                txt_parse_and_copy(full_path_name.c_str(), BaseOutputDir,base_input_str_length);

            }

            if (h_find_a->d_type == is_file)
            {
                std::string output_str = dir_name + "/" + h_find_a->d_name;

                std::string file_name = h_find_a->d_name;
                std::string extention = file_name.substr(file_name.length() - 4,
                    4);

                if (extention.compare(".txt") == 0)
                {
                    // if extention matches txt
                    std::string file_name_part_2 =
                        output_str.substr(base_input_str_length + 1,
                        output_str.length() - base_input_str_length - 1);
                    std::string file_name_part_1 = BaseOutputDir + "/" +
                        file_name_part_2;
                    std::string copy_cmd_str = "cp \"" + output_str +
                        "\" \"" + file_name_part_1 + "\"";

                    std::string dir_name =
                        file_name_part_1.substr(0,
                        file_name_part_1.length() - file_name.length());
                    std::string mk_dir_str = "mkdir \"" + dir_name + "\"";

                    system(mk_dir_str.c_str());
                    system(copy_cmd_str.c_str());
                }
            }
        }
    }

    return 0;

#elif defined(__APPLE__)

    unsigned char is_file = 0x8;
    unsigned char is_folder = 0x4;

    std::string input_str = dir_name;

    std::string double_dot = "..";
    std::string single_dot = ".";

    DIR *find_file_data_a;
    struct dirent *h_find_a;
    std::string file_name_a;

    find_file_data_a = opendir(dir_name);

    if (find_file_data_a == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (h_find_a = readdir(find_file_data_a))
        {
            if (h_find_a->d_type == is_folder && double_dot.compare(h_find_a->d_name)
                != 0 && single_dot.compare(h_find_a->d_name) != 0)
            {
                std::string full_path_name = dir_name + "/" + h_find_a->d_name;
                txt_parse_and_copy(full_path_name.c_str(), BaseOutputDir,
                    base_input_str_length);

            }

            if (h_find_a->d_type == is_file)
            {
                std::string output_str = dir_name + "/" h_find_a->d_name;

                std::string file_name = h_find_a->d_name;
                std::string extention = file_name.substr(file_name.length() - 4,
                    4);

                if (extention.compare(".txt") == 0)
                {
                    // if extention matches txt
                    std::string file_name_part_2 =
                        output_str.substr(base_input_str_length + 1,
                        output_str.length() - base_input_str_length - 1);
                    std::string file_name_part_1 = BaseOutputDir + "/" +
                        file_name_part_2;
                    std::string copy_cmd_str = "cp \"" + output_str +
                        "\" \"" + file_name_part_1 + "\"";

                    std::string dir_name =
                        file_name_part_1.substr(0,
                        file_name_part_1.length() - file_name.length());
                    std::string mk_dir_str = "mkdir \"" + dir_name + "\"";

                    system(mk_dir_str.c_str());
                    system(copy_cmd_str.c_str());
                }
            }
        }
    }

    return 0;

#elif defined(_WIN32)
    std::string DirNameStr = dir_name;
    DirNameStr += "\\*";

    WIN32_FIND_DATA find_file_data;
    HANDLE h_find;

    std::string double_dot = "..";
    std::string single_dot = ".";

    h_find = FindFirstFile(DirNameStr.c_str(), &find_file_data);

    if (h_find == INVALID_HANDLE_VALUE)
    {
        tprintf(PRINT_STD, "FindFirstFile failed (%d)\n", GetLastError());
        return 0;
    }
    else
    {
        if (find_file_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY &&
            double_dot.compare(find_file_data.cFileName) != 0 &&
            single_dot.compare(find_file_data.cFileName) != 0)
        {
            std::string full_path_name = dir_name;
            full_path_name += "\\";
            full_path_name += find_file_data.cFileName;
            txt_parse_and_copy(full_path_name.c_str(), BaseOutputDir, base_input_str_length);
        }
        else
        {
            if (double_dot.compare(find_file_data.cFileName) != 0 &&
                single_dot.compare(find_file_data.cFileName) != 0)
            {
                std::string output_str = dir_name;
                output_str += "\\";
                output_str += find_file_data.cFileName;

                std::string file_name = find_file_data.cFileName;
                std::string extention = file_name.substr(file_name.length() - 4,
                    4);

                if (extention.compare(".txt") == 0)
                {
                    // if extention matches txt
                    std::string file_name_part_2 =
                        output_str.substr(base_input_str_length + 1,
                        output_str.length() - base_input_str_length - 1);
                    std::string file_name_part_1 = BaseOutputDir;
                    file_name_part_1 += "\\";
                    file_name_part_1 += file_name_part_2;
                    std::string copy_cmd_str = "copy \"" + output_str +
                        "\" \"" + file_name_part_1 + "\"";

                    std::string dir_name =
                        file_name_part_1.substr(0,
                        file_name_part_1.length() - file_name.length());
                    std::string mk_dir_str = "mkdir \"" + dir_name + "\"";

                    system(mk_dir_str.c_str());
                    system(copy_cmd_str.c_str());
                }
            }
        }

        while (FindNextFile(h_find, &find_file_data) != 0)
        {
            if (find_file_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY &&
                double_dot.compare(find_file_data.cFileName) != 0 &&
                single_dot.compare(find_file_data.cFileName) != 0)
            {
                std::string full_path_name = dir_name;
                full_path_name += "\\";
                full_path_name += find_file_data.cFileName;
                txt_parse_and_copy(full_path_name.c_str(), BaseOutputDir, base_input_str_length);
            }
            else
            {
                if (double_dot.compare(find_file_data.cFileName) != 0 &&
                    single_dot.compare(find_file_data.cFileName) != 0)
                {
                    std::string output_str = dir_name;
                    output_str += "\\";
                    output_str += find_file_data.cFileName;

                    std::string file_name = find_file_data.cFileName;
                    std::string extention = file_name.substr(file_name.length() -
                        4, 4);

                    if (extention.compare(".txt") == 0)
                    {
                        // if extention matches txt
                        std::string file_name_part_2 =
                            output_str.substr(base_input_str_length + 1,
                            output_str.length() - base_input_str_length - 1);
                        std::string file_name_part_1 = BaseOutputDir;
                        file_name_part_1 += "\\";
                        file_name_part_1 += file_name_part_2;
                        std::string copy_cmd_str = "copy \"" + output_str +
                            "\" \"" + file_name_part_1 + "\"";

                        std::string dir_name =
                            file_name_part_1.substr(0,
                            file_name_part_1.length() - file_name.length());
                        std::string mk_dir_str = "mkdir \"" + dir_name + "\"";

                        system(mk_dir_str.c_str());
                        system(copy_cmd_str.c_str());

                    }
                }
            }
        }

        FindClose(h_find);
    }

    return 0;
#endif
    return 0;
}


int tool_combine_indv_frames(int argc, const char** argv)
{
    if (argc != 8)
        return vpxt_tool_help(argv[1], 0);

    std::string input_dir = argv[2];
    std::string name_base = argv[3];
    std::string extension = argv[4];
    int first_frame = atoi(argv[5]);
    int last_frame = atoi(argv[6]);
    std::string output_file = argv[7];


    int x = 0;
    int static_zero_count = 0;

    while (argv[6][x] != NULL)
    {
        char zero_check[2];
        zero_check[0] = argv[6][x];
        zero_check[1] = '\0';

        // std::cout << "\n" << argv[6][x] << "\n";
        if (atoi(zero_check) == 0)
            static_zero_count++;
        else
            break;
        x++;
    }

    // find out how many dec places due to increasing frames
    std::string cur_indv_frame_file_name;
    int input_dec_places = vpxt_decimal_places(last_frame);
    int current_frame = first_frame;

    FILE *out = fopen(output_file.c_str(), "wb");

    if (out == NULL)
    {
        tprintf(PRINT_STD, "\nOutput file does not exist");
        return 0;
    }

    std::cout << "\n\n";

    while (current_frame <= last_frame)
    {
        // printf("%i ",current_frame);
        cur_indv_frame_file_name = input_dir + slashCharStr() + name_base;

        int added_static_zeros = 0;

        while (added_static_zeros < static_zero_count) // add static zeros
        {
            cur_indv_frame_file_name += "0";
            added_static_zeros++;
        }

        int cur_num_dec_places = vpxt_decimal_places(current_frame);

        // add zeros for increasing frames
        while (cur_num_dec_places < input_dec_places)
        {
            cur_indv_frame_file_name += "0";
            cur_num_dec_places++;
        }

        char current_frame_char[512];
        vpxt_itoa_custom(current_frame, current_frame_char, 10);
        cur_indv_frame_file_name += current_frame_char;
        cur_indv_frame_file_name += extension;

        char cur_indv_frame_file_name_char[255];
        char cur_indv_frame_file_name_only[255];
        snprintf(cur_indv_frame_file_name_char, 255, "%s",
            cur_indv_frame_file_name.c_str());
        vpxt_file_name(cur_indv_frame_file_name_char,
            cur_indv_frame_file_name_only, 0);

        int file_size = vpxt_file_size(cur_indv_frame_file_name_char, 0);

        FILE *in = fopen(cur_indv_frame_file_name.c_str(), "rb");

        if (in != NULL && file_size > 0)
        {
            tprintf(PRINT_STD, "%s\n", cur_indv_frame_file_name_only);
            char *input_buffer = new char[file_size*2];
            fread(input_buffer, 1, file_size, in);
            fwrite(input_buffer, 1, file_size, out);
            delete [] input_buffer;
            fclose(in);
        }
        else
            tprintf(PRINT_STD, "%s - Skiped\n", cur_indv_frame_file_name_only);

        current_frame++;
    }

    fclose(out);
    return 0;
}
int tool_compare_enc(int argc, const char** argv)
{
    if (argc < 5)
        return vpxt_tool_help(argv[1], 0);

    std::string input_1 = argv[2];
    std::string input_2 = argv[3];
    int full_check = atoi(argv[4]);

    tprintf(PRINT_STD, "\nComparing Files:\n\n");
    tprintf(PRINT_STD, "%s\n", input_1.c_str());
    tprintf(PRINT_STD, "%s\n", input_2.c_str());

    int comp_ivf_output = vpxt_compare_enc(input_1.c_str(), input_2.c_str(),
        full_check);

    if (comp_ivf_output == -1)
        tprintf(PRINT_STD, "\n Files Contain Identical Video Data\n");

    if (comp_ivf_output == -2)
        tprintf(PRINT_STD, "\n\nFail: File 2 ends before File 1.\n");

    if (comp_ivf_output == -3)
        tprintf(PRINT_STD, "\n\nFail: File 1 ends before File 2.\n");

    if (comp_ivf_output != -1 && comp_ivf_output != -2 && comp_ivf_output != -3 &&
        comp_ivf_output != -5)
    {
        if(full_check)
            tprintf(PRINT_STD, "\nFILES NOT IDENTICAL\n");
        else
            tprintf(PRINT_STD, "\nFILES DIFFER AT FRAME: %i\n", comp_ivf_output);
    }

    return 0;
}
int tool_compare_dec(int argc, const char** argv)
{
    if (argc < 3)
        return vpxt_tool_help(argv[1], 0);

    std::string input_1 = argv[2];
    std::string input_2 = argv[3];

    tprintf(PRINT_STD, "\nComparing Files:\n\n");
    tprintf(PRINT_STD, "%s\n", input_1.c_str());
    tprintf(PRINT_STD, "%s\n", input_2.c_str());

    int comp_ivf_output = vpxt_compare_dec(input_1.c_str(), input_2.c_str());

    if (comp_ivf_output == -1)
        tprintf(PRINT_STD, "\n Files Contain Identical Video Data\n");

    if (comp_ivf_output == -2)
        tprintf(PRINT_STD, "\n\nFail: File 2 ends before File 1.\n");

    if (comp_ivf_output == -3)
        tprintf(PRINT_STD, "\n\nFail: File 1 ends before File 2.\n");

    if (comp_ivf_output != -1 && comp_ivf_output != -2 && comp_ivf_output != -3
        && comp_ivf_output != -5)
        tprintf(PRINT_STD, "\nFILES DIFFER AT FRAME: %i\n", comp_ivf_output);

    return 0;
}
int tool_compare_header_info(int argc, const char** argv)
{
    if (argc < 3)
        return vpxt_tool_help(argv[1], 0);

    vpxt_compare_header_info(argc, argv);

    return 0;
}
int tool_display_header_info(int argc, const char** argv)
{
    if (argc < 3)
        return vpxt_tool_help(argv[1], 0);

    vpxt_display_header_info(argc, argv);

    return 0;
}
int tool_vpxt_enc(int argc, const char** argv, std::string working_dir)
{
    char *comp_out_str = "Allow DF";

    if (argc < 8 || argc > 9)
        return vpxt_tool_help(argv[1], 0);

    int compression_type = atoi(argv[2]);
    std::string input = argv[3];
    std::string output = argv[4];
    int bitrate = atoi(argv[5]);
    int mode = atoi(argv[6]);
    std::string enc_format = argv[7];

    int speed = 0;

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt.target_bandwidth = bitrate;

    if (argc == 9)
        opt = vpxt_input_settings(argv[8]);

    int compress_int = opt.allow_df;

    //////////// Track Mem Usage //////////
    // std::string mem_leak_check_txt_2_str = output + "_MemOut.txt";
    // char MemLeakCheckTXT2[255];
    // snprintf(MemLeakCheckTXT2, 255, "%s", mem_leak_check_txt_2_str.c_str());
    // on2_MemoryTrackerSetLogType(0, MemLeakCheckTXT2);
    // std::cout << "\nMemory Tracking to file: " << MemLeakCheckTXT2 << "\n";
    /////////////////////////////////////

    opt.Mode = mode;

    if (compression_type == 1)
        vpxt_compress_no_error_output(input.c_str(), output.c_str(),
        speed, bitrate, opt, comp_out_str, compress_int, 0, enc_format);

    if (compression_type == 2)
    {
        unsigned int CPUTick = 0;
        vpxt_time_compress(input.c_str(), output.c_str(), speed, bitrate,
            opt, comp_out_str, compress_int, 0, CPUTick, enc_format);
    }

    //////////// Track Mem Usage //////////
    // on2_MemoryTrackerDump();
    /////////////////////////////////////
    return 0;
}
int tool_vpxt_multi_res_enc(int argc, const char** argv)
{
    char *comp_out_str = "Allow DF";

    if (argc < 6)
        return vpxt_tool_help(argv[1], 0);

    std::string input = argv[2];
    std::string output = argv[3];
    int bitrate = atoi(argv[4]);
    std::string enc_format = argv[5];

    int mode = 0;
    int speed = 0;

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt.target_bandwidth = bitrate;

    if (argc == 9)
        opt = vpxt_input_settings(argv[8]);

    int compress_int = opt.allow_df;

    opt.Mode = mode;

    vpxt_compress_multi_resolution(input.c_str(), output.c_str(),
        speed, bitrate, opt, comp_out_str, compress_int, 0, enc_format);

    return 0;
}
int tool_vpxt_temp_scale_enc(int argc, const char** argv)
{
    char *comp_out_str = "Allow DF";

    if (argc < 11)
        return vpxt_tool_help(argv[1], 0);

    std::string input = argv[2];
    std::string output = argv[3];
    std::string enc_format = argv[10];

    int mode = 0;
    int speed = 0;

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    if (argc == 9)
        opt = vpxt_input_settings(argv[8]);

    int compress_int = opt.allow_df;

    opt.Mode = mode;

    vpxt_compress_scalable_patterns(input.c_str(), output.c_str(), speed,
        opt, comp_out_str, compress_int, 0, enc_format, atoi(argv[4]),
        atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]),
        atoi(argv[9]));

    return 0;
}
int tool_multi_res_enc(int argc, const char** argv)
{
    int dummy_argc = argc - 1;

    const char *dummy_argv[999];
    dummy_argv[0] = argv[0];

    int i = 2;
    while (i < argc)
    {
        dummy_argv[i-1] = argv[i];
        i++;
    }

    vp8_multi_resolution_encoder(dummy_argc, (char **) dummy_argv);

    return 0;
}
int tool_vp8_scalable_patterns(int argc, const char** argv)
{
    int dummy_argc = argc - 1;

    const char *dummy_argv[999];
    dummy_argv[0] = argv[0];

    int i = 2;
    while (i < argc)
    {
        dummy_argv[i-1] = argv[i];
        i++;
    }

    vp8_scalable_patterns(dummy_argc, (char **) dummy_argv);

    return 0;
}
int tool_convert_par_file_to_vpxenc(int argc, const char** argv)
{
    if (argc < 4)
        return vpxt_tool_help(argv[1], 0);

    char vpxenc_parameters[1024];
    tprintf(PRINT_STD, "\n\nequivalent vpxenc settings:\n");
    vpxt_convert_par_file_to_vpxenc(argv[2], argv[3], vpxenc_parameters, 1024);
    tprintf(PRINT_STD, "%s\n\n", vpxenc_parameters);

    return 0;
}
int tool_create_rand_par_file(int argc, const char** argv)
{
    if (argc < 3)
        return vpxt_tool_help(argv[1], 0);

    std::string output = argv[2];

    VP8_CONFIG opt = vpxt_random_parameters(opt, "", 0);
    vpxt_output_settings(output.c_str(), opt);

    tprintf(PRINT_STD, "\nSaved to: %s\n", output.c_str());

    return 0;
}
int tool_crop_raw_clip(int argc, const char** argv)
{
    if (argc < 8)
        return vpxt_tool_help(argv[1], 0);

    std::string input_file = argv[2];
    std::string output_file = argv[3];
    int x_offset = atoi(argv[4]);
    int y_offset = atoi(argv[5]);
    int new_frame_width = atoi(argv[6]);
    int new_frame_height = atoi(argv[7]);
    int file_is_ivf  = atoi(argv[8]);

    vpxt_crop_raw_clip(input_file.c_str(), output_file.c_str(), x_offset,
        y_offset, new_frame_width, new_frame_height, file_is_ivf, 0);

    tprintf(PRINT_STD, "\n");
    return 0;
}
int tool_pad_raw_clip(int argc, const char** argv)
{
    if (argc < 6)
        return vpxt_tool_help(argv[1], 0);

    std::string input_file = argv[2];
    std::string output_file = argv[3];
    int new_frame_width = atoi(argv[4]);
    int new_frame_height = atoi(argv[5]);
    int file_is_ivf  = atoi(argv[6]);
    vpxt_pad_raw_clip(input_file.c_str(), output_file.c_str(), new_frame_width,
        new_frame_height, file_is_ivf, 0);
    return 0;
}
int tool_copy_all_txt_files(int argc, const char** argv)
{
    if (argc < 4)
        return vpxt_tool_help(argv[1], 0);

#if defined(linux)

    unsigned char is_file = 0x8;
    unsigned char is_folder = 0x4;

    std::string input_str = argv[2];
    int base_input_str_length = input_str.length();
    char get_first_file_char[255];
    snprintf(get_first_file_char, 255, "%s", input_str.c_str());

    std::string double_dot = "..";
    std::string single_dot = ".";

    DIR *find_file_data_a;
    struct dirent *h_find_a;
    std::string file_name_a;

    find_file_data_a = opendir(argv[2]);

    if (find_file_data_a == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {
        while (h_find_a = readdir(find_file_data_a))
        {
            if (h_find_a->d_type == is_folder && double_dot.compare(
                h_find_a->d_name) != 0 && single_dot.compare(h_find_a->d_name)
                != 0)
            {
                std::string full_path_name = argv[2];
                full_path_name += "/";
                full_path_name += h_find_a->d_name;
                txt_parse_and_copy(full_path_name.c_str(), argv[3],
                    base_input_str_length);
            }

            if (h_find_a->d_type == is_file)
            {
                std::string output_str = argv[2];
                output_str += "/";
                output_str += h_find_a->d_name;

                std::string file_name = h_find_a->d_name;
                std::string extention = file_name.substr(file_name.length() - 4,
                    4);

                if (extention.compare(".txt") == 0)
                {
                    // if extention matches txt
                    std::string file_name_part_2 =
                        output_str.substr(base_input_str_length + 1,
                        output_str.length() - base_input_str_length - 1);
                    std::string file_name_part_1 = argv[3];
                    file_name_part_1 += "/";
                    file_name_part_1 += file_name_part_2;
                    std::string copy_cmd_str = "cp \"" + output_str +
                        "\" \"" + file_name_part_1 + "\"";

                    std::string dir_name =
                        file_name_part_1.substr(0,
                        file_name_part_1.length() - file_name.length());
                    std::string mk_dir_str = "mkdir \"" + dir_name + "\"";

                    system(mk_dir_str.c_str());
                    system(copy_cmd_str.c_str());
                }
            }
        }
    }

    return 0;

#elif defined(_PPC)

    unsigned char is_file = 0x8;
    unsigned char is_folder = 0x4;

    std::string input_str = argv[2];
    int base_input_str_length = input_str.length();
    char get_first_file_char[256];
    strcpy(get_first_file_char, input_str.c_str());

    std::string double_dot = "..";
    std::string single_dot = ".";

    DIR *find_file_data_a;
    struct dirent *h_find_a;
    std::string file_name_a;

    find_file_data_a = opendir(argv[2]);

    if (find_file_data_a == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {

        while (h_find_a = readdir(find_file_data_a))
        {
            if (h_find_a->d_type == is_folder && double_dot.compare(
                h_find_a->d_name) != 0 && single_dot.compare(h_find_a->d_name)
                != 0)
            {
                std::string full_path_name = argv[2] + "/" + h_find_a->d_name;
                txt_parse_and_copy(full_path_name.c_str(), argv[3],
                    base_input_str_length);
            }

            if (h_find_a->d_type == is_file)
            {
                std::string output_str = argv[2] + "/" + h_find_a->d_name;

                std::string file_name = h_find_a->d_name;
                std::string extention = file_name.substr(file_name.length() - 4,
                    4);

                if (extention.compare(".txt") == 0)
                {
                    // if extention matches txt
                    std::string file_name_part_2 =
                        output_str.substr(base_input_str_length + 1,
                        output_str.length() - base_input_str_length - 1);
                    std::string file_name_part_1 = argv[3] + "/" +
                        file_name_part_2;
                    std::string copy_cmd_str = "cp \"" = output_str +
                        "\" \"" + file_name_part_1 + "\"";

                    std::string dir_name = file_name_part_1.substr(0,
                        file_name_part_1.length() - file_name.length());
                    std::string mk_dir_str = "mkdir \"" + dir_name + "\"";

                    system(mk_dir_str.c_str());
                    system(copy_cmd_str.c_str());
                }
            }
        }
    }

    return 0;

#elif defined(__APPLE__)

    unsigned char is_file = 0x8;
    unsigned char is_folder = 0x4;

    std::string input_str = argv[2];
    int base_input_str_length = input_str.length();
    char get_first_file_char[255];
    snprintf(get_first_file_char, 255, "%s", input_str.c_str());

    std::string double_dot = "..";
    std::string single_dot = ".";

    DIR *find_file_data_a;
    struct dirent *h_find_a;
    std::string file_name_a;

    find_file_data_a = opendir(argv[2]);

    if (find_file_data_a == NULL)
    {
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {
        while (h_find_a = readdir(find_file_data_a))
        {

            if (h_find_a->d_type == is_folder && double_dot.compare(
                h_find_a->d_name) != 0 && single_dot.compare(h_find_a->d_name)
                != 0)
            {
                std::string full_path_name = argv[2] + "/" + h_find_a->d_name;
                txt_parse_and_copy(full_path_name.c_str(), argv[3],
                    base_input_str_length);
            }

            if (h_find_a->d_type == is_file)
            {
                std::string output_str = argv[2] + "/" + h_find_a->d_name;

                std::string file_name = h_find_a->d_name;
                std::string extention = file_name.substr(file_name.length() - 4,
                    4);

                if (extention.compare(".txt") == 0)
                {
                    // if extention matches txt
                    std::string file_name_part_2 =
                        output_str.substr(base_input_str_length + 1,
                        output_str.length() - base_input_str_length - 1);
                    std::string file_name_part_1 = argv[3] + "/" +
                        file_name_part_2;
                    std::string copy_cmd_str = "cp \"" + output_str +
                        "\" \"" + file_name_part_1 + "\"";

                    std::string dir_name =
                        file_name_part_1.substr(0,
                        file_name_part_1.length() - file_name.length());
                    std::string mk_dir_str = "mkdir \"" + dir_name + "\"";

                    system(mk_dir_str.c_str());
                    system(copy_cmd_str.c_str());
                }
            }
        }
    }

    return 0;

#elif defined(_WIN32)

    tprintf(PRINT_STD, "\n");

    std::string input_str = argv[2];
    int base_input_str_length = input_str.length();
    input_str += "\\*";
    char get_first_file_char[255];
    snprintf(get_first_file_char, 255, "%s", input_str.c_str());

    std::string double_dot = "..";
    std::string single_dot = ".";

    WIN32_FIND_DATA find_file_data;
    HANDLE h_find;

    h_find = FindFirstFile(get_first_file_char, &find_file_data);

    if (h_find == INVALID_HANDLE_VALUE)
    {
        tprintf(PRINT_STD, "\nFindFirstFile failed (%d)\n", GetLastError());
        tprintf(PRINT_STD, "Could not open first file.\n");
        return 0;
    }
    else
    {
        if (find_file_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY &&
            double_dot.compare(find_file_data.cFileName) != 0 &&
            single_dot.compare(find_file_data.cFileName) != 0)
        {
            std::string full_path_name = argv[2];
            full_path_name += "\\";
            full_path_name += find_file_data.cFileName;
            txt_parse_and_copy(full_path_name.c_str(), argv[3],
                base_input_str_length);
        }
        else
        {
            if (double_dot.compare(find_file_data.cFileName) != 0 &&
                single_dot.compare(find_file_data.cFileName) != 0)
            {
                std::string output_str = argv[2];
                output_str += "\\";
                output_str += find_file_data.cFileName;

                std::string file_name = find_file_data.cFileName;
                std::string extention = file_name.substr(file_name.length() - 4,
                    4);

                if (extention.compare(".txt") == 0)
                {
                    // if extention matches txt
                    std::string file_name_part_2 =
                        output_str.substr(base_input_str_length + 1,
                        output_str.length() - base_input_str_length - 1);
                    std::string file_name_part_1 = argv[3];
                    file_name_part_1 += "\\";
                    file_name_part_1 += file_name_part_2;
                    std::string copy_cmd_str = "copy \"" + output_str +
                        "\" \"" + file_name_part_1 + "\"";

                    std::string dir_name =
                        file_name_part_1.substr(0,
                        file_name_part_1.length() - file_name.length());
                    std::string mk_dir_str = "mkdir \"" + dir_name + "\"";

                    system(mk_dir_str.c_str());
                    system(copy_cmd_str.c_str());
                }
            }
        }

        while (FindNextFile(h_find, &find_file_data) != 0)
        {
            if (find_file_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY &&
                double_dot.compare(find_file_data.cFileName) != 0 &&
                single_dot.compare(find_file_data.cFileName) != 0)
            {
                std::string full_path_name = argv[2];
                full_path_name += "\\";
                full_path_name += find_file_data.cFileName;
                txt_parse_and_copy(full_path_name.c_str(), argv[3],
                    base_input_str_length);
            }
            else
            {
                if (double_dot.compare(find_file_data.cFileName) != 0 &&
                    single_dot.compare(find_file_data.cFileName) != 0)
                {
                    std::string output_str = argv[2];
                    output_str += "\\";
                    output_str += find_file_data.cFileName;

                    std::string file_name = find_file_data.cFileName;
                    std::string extention = file_name.substr(file_name.length() -
                        4, 4);

                    if (extention.compare(".txt") == 0)
                    {
                        // if extention matches txt
                        std::string file_name_part_2 =
                            output_str.substr(base_input_str_length + 1,
                            output_str.length() - base_input_str_length - 1);
                        std::string file_name_part_1 = argv[3];
                        file_name_part_1 += "\\";
                        file_name_part_1 += file_name_part_2;
                        std::string copy_cmd_str = "copy \"" + output_str +
                            "\" \"" + file_name_part_1 + "\"";

                        std::string dir_name =
                            file_name_part_1.substr(0,

                            file_name_part_1.length() - file_name.length());
                        std::string mk_dir_str = "mkdir \"" + dir_name + "\"";

                        system(mk_dir_str.c_str());
                        system(copy_cmd_str.c_str());

                    }
                }
            }
        }

        FindClose(h_find);
    }

    return 0;
#endif
    return 0;
}
int tool_cut_ivf(int argc, const char** argv)
{
    if (argc < 5)
        return vpxt_tool_help(argv[1], 0);

    std::string input_file = argv[2];
    std::string output_file = argv[3];
    int starting_frame = atoi(argv[4]);
    int ending_frame = atoi(argv[5]);

    vpxt_cut_clip(input_file.c_str(), output_file.c_str(), starting_frame,
        ending_frame);

    return 0;
}
int tool_vpxt_dec(int argc, const char** argv)
{
    if (argc < 5)
        return vpxt_tool_help(argv[1], 0);

    std::string input_file = argv[2];
    std::string output_file = argv[3];
    std::string dec_format = argv[4];

    vpxt_decompress_no_output(input_file.c_str(), output_file.c_str(),
        dec_format, 1);

    return 0;
}
int tool_vpxt_dec_part_drop(int argc, const char** argv)
{
    if (argc < 8)
        return vpxt_tool_help(argv[1], 0);

    std::string input_file = argv[2];
    std::string output_file = argv[3];
    std::string dec_format = argv[4];

    vpxt_decompress_partial_drops(input_file.c_str(), output_file.c_str(),
        dec_format, 1, atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), PRINT_STD,
        1);

    tprintf(PRINT_STD, "\n");

    return 0;
}
int tool_vpxt_dec_resize(int argc, const char** argv)
{
    if (argc < 5)
        return vpxt_tool_help(argv[1], 0);

    std::string input_file = argv[2];
    std::string output_file = argv[3];
    std::string dec_format = argv[4];

    vpxt_decompress_resize(input_file.c_str(), output_file.c_str(), dec_format,
        1);

    return 0;
}
int tool_vpxt_dec_to_raw(int argc, const char** argv)
{
    if (argc < 4)
        return vpxt_tool_help(argv[1], 0);

    std::string input_file = argv[2];
    std::string output_file = argv[3];

    vpxt_decompress_to_raw(input_file.c_str(), output_file.c_str(), 1);

    return 0;
}
int tool_vpxt_write_ivf_file_header(int argc, const char** argv)
{
    if (argc < 7)
        return vpxt_tool_help(argv[1], 0);

    const char *output_file = argv[2];
    int width = atoi(argv[3]);
    int height = atoi(argv[4]);
    int scale = atoi(argv[5]);
    int rate = atoi(argv[6]);
    int length = atoi(argv[7]);

    FILE *out_file = fopen(output_file, "a");

    IVF_HEADER ivf;
    strncpy((char *)(ivf.signature), "DKIF", 4);
    ivf.version = 0;
    ivf.headersize = make_endian_16(32);
    ivf.four_cc     = MAKEFOURCC('I', '4', '2', '0');
    ivf.width      = make_endian_16(width);
    ivf.height     = make_endian_16(height);
    ivf.scale      = make_endian_32(scale);
    ivf.rate       = make_endian_32(rate);
    ivf.length     = make_endian_32(length);
    fseek(out_file, 0, 0);
    fwrite((char *)&ivf, sizeof(ivf), 1, out_file);

    fclose(out_file);
    return 0;
}
int tool_vpxt_write_ivf_frame_header(int argc, const char** argv)
{
    if (argc < 4)
        return vpxt_tool_help(argv[1], 0);

    const char *output_file = argv[2];
    int time_stamp = atoi(argv[3]);
    int frame_size = atoi(argv[4]);

    FILE *out_file = fopen(output_file, "a");

    IVF_FRAME_HEADER ivf_fh;

    ivf_fh.timeStamp = make_endian_64(time_stamp);
    ivf_fh.frameSize = make_endian_32(frame_size);

    fwrite((char *)&ivf_fh, sizeof(ivf_fh), 1, out_file);

    fclose(out_file);
    return 0;

}
int tool_vpxt_write_frame_data(int argc, const char** argv)
{
    if (argc < 4)
        return vpxt_tool_help(argv[1], 0);

    const char *input_file = argv[2];
    const char *output_file = argv[3];
    int frame_size = atoi(argv[4]);

    FILE *in_file = fopen(input_file, "r");
    FILE *out_file = fopen(output_file, "a");

    char *read_frame = new char[frame_size];

    fread(read_frame, frame_size, 1, in_file);
    fwrite(read_frame, frame_size, 1, out_file);

    fclose(in_file);
    fclose(out_file);
    delete [] read_frame;

    return 0;
}
int tool_disp_alt_ref_frames(int argc, const char** argv)
{
    if (argc < 4)
        return vpxt_tool_help(argv[1], 0);

    int alt_ref_frames = vpxt_display_alt_ref_frames(argv[2], atoi(argv[3]));
    tprintf(PRINT_STD, "\nAlternate Reference Frames Found: %i\n",
        alt_ref_frames);
    return 0;
}
int tool_disp_droped_frames(int argc, const char** argv)
{
    if (argc < 4)
        return vpxt_tool_help(argv[1], 0);

    int dropped_frames = vpxt_display_droped_frames(argv[2], atoi(argv[3]));
    tprintf(PRINT_STD, "\nDropped Frames Counted: %i \n\n (Num is aprox as any "
        "frame droped after last encoded frame cannot be counted)\n",
        dropped_frames);
    return 0;
}
int tool_disp_frame_data(int argc, const char** argv)
{
    if (argc < 3)
        return vpxt_tool_help(argv[1], 0);

    vpxt_display_alt_ref_frames(argv[2], 0);
    return 0;
}
int tool_disp_key_frames(int argc, const char** argv)
{
    if (argc < 3)
        return vpxt_tool_help(argv[1], 0);

    int key_frame_count = vpxt_display_key_frames(argv[2], atoi(argv[3]));
    tprintf(PRINT_STD, "\nKey Frames Found: %i\n", key_frame_count);
    return 0;
}

int tool_disp_resized_frames(int argc, const char** argv)
{
    if (argc < 4)
        return vpxt_tool_help(argv[1], 0);

    int resized_frames = vpxt_display_resized_frames(argv[2], atoi(argv[3]));
    tprintf(PRINT_STD, "\nResized Frames Found: %i\n", resized_frames);
    return 0;
}
int tool_disp_visible_frames(int argc, const char** argv)
{
    if (argc < 4)
        return vpxt_tool_help(argv[1], 0);

    int vis_frame_num = vpxt_display_visible_frames(argv[2], atoi(argv[3]));
    tprintf(PRINT_STD, "\nVisible Frames Found: %i\n", vis_frame_num);

    return 0;
}
int tool_format_summary(int argc, const char** argv)
{
    if (argc < 3)
    {
        tprintf(PRINT_STD, "\n"
                "  FormatSummaryByTestandResult \n\n"
                "    <Text File to format>\n"
                "\n");

        return 0;
    }

    format_summary(argv[2]);
    return 0;
}

int tool_vpxt_check_pbm_run(int argc, const char** argv)
{
    if (argc < 6)
        return vpxt_tool_help(argv[1], 0);

    std::string input_file = argv[2];
    int bitrate = atoi(argv[3]);
    int max_buffer = atoi(argv[4]);
    int pre_buffer = atoi(argv[5]);
    int check_pbm_val;

    check_pbm_val = vpxt_check_pbm(input_file.c_str(), bitrate, max_buffer,
        pre_buffer);

    if (check_pbm_val == -11)
        tprintf(PRINT_STD, "CheckPBM Passed\n");
    else
        tprintf(PRINT_STD, "CheckPBM failed at: %i\n", check_pbm_val);

    return 0;
}
int tool_vpxt_data_rate(int argc, const char** argv)
{
    if (argc < 3)
        return vpxt_tool_help(argv[1], 0);

    vpxt_data_rate(argv[2], 2);

    return 1;
}
int tool_vpxt_psnr_run(int argc, const char** argv)
{
    if (argc < 4)
        return vpxt_tool_help(argv[1], 0);

    double run_ssim = 1;
    std::string raw = argv[2];
    std::string comp = argv[3];

    if (atoi(argv[4]) == 1)
        vpxt_psnr(raw.c_str(), comp.c_str(), 0, PRINT_STD, 0, &run_ssim);
    else
        vpxt_psnr(raw.c_str(), comp.c_str(), 0, PRINT_STD, 0, NULL);

    tprintf(PRINT_STD, "\n");

    return 0;
}
int tool_vpxt_psnr_run_dec(int argc, const char** argv)
{
    if (argc < 5)
        return vpxt_tool_help(argv[1], 0);

    double run_ssim = 1;
    std::string raw = argv[2];
    std::string comp = argv[3];

    int width = 0;
    int height = 0;
    int uv_swap = 0;

    if (argc > 5)
        width = atoi(argv[5]);

    if (argc > 6)
        height = atoi(argv[6]);

    if (argc > 7)
        uv_swap = atoi(argv[7]);

    if (atoi(argv[4]) == 1)
        vpxt_psnr_dec(raw.c_str(), comp.c_str(), uv_swap, 3, 0, &run_ssim, width
            ,height);
    else
        vpxt_psnr_dec(raw.c_str(), comp.c_str(), uv_swap, 3, 0, NULL, width,
            height);

    tprintf(PRINT_STD, "\n");

    return 0;
}
int tool_formatted_to_raw(int argc, const char** argv)
{
    if (argc < 4)
        return vpxt_tool_help(argv[1], 0);

    vpxt_formatted_to_raw(argv[2], argv[3]);
    return 0;
}
int tool_formatted_to_raw_frames(int argc, const char** argv)
{
    if (argc < 3)
        return vpxt_tool_help(argv[1], 0);

    tprintf(PRINT_STD, "\nConverting to Individual Frames.\n");
    vpxt_formatted_to_raw_frames(argv[2], argv[3]);
    return 0;
}

int tool_paste_clip(int argc, const char** argv)
{
    if (argc < 5)
        return vpxt_tool_help(argv[1], 0);

    std::string input_file_1 = argv[2];
    std::string input_file_2 = argv[3];
    std::string output_file = argv[4];
    int starting_frame = atoi(argv[5]);

    vpxt_paste_clip(input_file_1.c_str(), input_file_2.c_str(),
        output_file.c_str(), starting_frame);

    return 0;
}

int tool_play_comp_ivf(int argc, const char** argv)
{
    if (argc < 3)
        return vpxt_tool_help(argv[1], 0);

#if defined(__APPLE__)
    {
        // printf("\nError - Not Yet Implemented for Mac.\n");
        // return 0;
    }
#endif
#if defined(_PPC)
    {
        // printf("\nError - Not Yet Implemented for Mac.\n");
        // return 0;
    }
#endif
#if defined(linux)
    {
        // if(!vpxt_file_exists_check("/usr/bin/mplayer"))
        // {
        // tprintf(PRINT_STD, "\nError - c:\\bin\\tmnplay.exe not found.\n");
        //  return 0;
        // }
    }
#endif
#if defined(_WIN32)
    {
        if (!vpxt_file_exists_check("c:\\bin\\tmnplay.exe"))
        {
            tprintf(PRINT_STD, "\nError - c:\\bin\\tmnplay.exe not found.\n");
            return 0;
        }
    }
#endif

    std::string input = argv[2];
    std::string output = argv[2];
    output += "_DEC.ivf.raw";

    ///////////////////// Read In Data From IVF File /////////////////////
    FILE *in = fopen(input.c_str(), "rb");

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    IVF_HEADER ivf_h_raw;

    InitIVFHeader(&ivf_h_raw);
    fread(&ivf_h_raw, 1, sizeof(ivf_h_raw), in);

    std::string ivf_check = (char *) ivf_h_raw.signature;

    if (ivf_check.compare("DKIF") != 0)
    {
        tprintf(PRINT_STD, "\nError: Input file is not IVF\n");
        return 0;
    }

    char width_char[256];
    char height_char[256];
    char frame_rate_char[256];
    char yuv_char[256];

    int width = ivf_h_raw.width;
    int height = ivf_h_raw.height;
    int frame_rate = (ivf_h_raw.rate / ivf_h_raw.scale);
    int yuv = 0;

    vpxt_itoa_custom(width, width_char, 10);
    vpxt_itoa_custom(height, height_char, 10);
    vpxt_itoa_custom(frame_rate, frame_rate_char, 10);
    vpxt_itoa_custom(yuv, yuv_char, 10);
    fclose(in);
    ////////////////////////////////////////////////////////////////////

    vpxt_decompress_to_raw_no_error_output(input.c_str(), output.c_str(), 1);

    std::string Program;

#if defined(_WIN32)
    Program = "\"c:\\bin\\tmnplay.exe \"" + output + "\" " +
        width_char + " " + height_char + " " + frame_rate_char + " \"" +
        yuv_char;
#elif defined(linux)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program += width_char;
    Program += ":h=";
    Program += height_char;
    Program += "  \"" + output + "\" -loop 1000";
#elif defined(__APPLE__)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program += width_char;
    Program += ":h=";
    Program += height_char;
    Program += "  \"" + output + "\" -loop 1000";
#elif defined(_PPC)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program += width_char;
    Program += ":h=";
    Program += height_char;
    Program += "  \"" + output + "\" -loop 1000";
#endif

    tprintf(PRINT_STD, "\n\n");

    int SysRet = system(Program.c_str());

    if (!SysRet == 0)
    {
#if defined(_WIN32)
        tprintf(PRINT_STD, "\n"
                "    An error occured when trying to play the file.  Please\n"
                "    Make sure tmnplay is located in your c:\\bin directory.\n"
                "    tmnplay can be found in the TestClips directory.\n"
               );
#else
        tprintf(PRINT_STD, "\n"
                "    An error occured when trying to play the file.  Please\n"
                "    make sure that mplayer is installed correctly.  mplayer\n"
                "    can be installed by typing:\n"
                "\n"
               "    svn checkout svn://svn.mplayerhq.hu/mplayer/trunk mplayer\n"
                "    cd mplayer\n"
                "    ./configure\n"
                "    make\n"
                "    make install\n"
                "\n"
                "    from a command Prompt."
               );
#endif
    }


    if (remove(output.c_str()) != 0)
        tprintf(PRINT_STD, "\nError deleting file: %s\n", output.c_str());
    else
        tprintf(PRINT_STD, "\n\nFile successfully deleted: %s\n",
        output.c_str());

    return 0;

}
int tool_play_dec_ivf(int argc, const char** argv)
{
    if (argc < 3)
        return vpxt_tool_help(argv[1], 0);

#if defined(linux)
    {
        if (!vpxt_file_exists_check("/usr/bin/mplayer"))
        {
            // printf("\nError - /usr/bin/mplayer not found.\n");
            // return 0;
        }
    }
#endif
#if defined(__APPLE__)
    {
        // printf("\nError - Not Yet Implemented for Mac\n");
        // return 0;
    }
#endif
#if defined(_PPC)
    {
        // printf("\nError - Not Yet Implemented for Mac\n");
        // return 0;
    }
#endif
#if defined(_WIN32)
    {
        if (!vpxt_file_exists_check("c:\\bin\\tmnplay.exe"))
        {
            tprintf(PRINT_STD, "\nError - c:\\bin\\tmnplay.exe not found.\n");
            return 0;
        }
    }
#endif

    std::string input = argv[2];
    std::string output = argv[2];
    output += ".raw";

    ///////////////////// Read In Data From IVF File /////////////////////
    FILE *in = fopen(input.c_str(), "rb");

    if (!in)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    IVF_HEADER ivf_h_raw;

    InitIVFHeader(&ivf_h_raw);
    fread(&ivf_h_raw, 1, sizeof(ivf_h_raw), in);

    std::string ivf_check = (char *) ivf_h_raw.signature;

    if (ivf_check.compare("DKIF") != 0)
    {
        tprintf(PRINT_STD, "\nError: Input file is not IVF\n");
        return 0;
    }

    char width_char[256];
    char height_char[256];
    char frame_rate_char[256];
    char yuv_char[256];

    int width = ivf_h_raw.width;
    int height = ivf_h_raw.height;
    int frame_rate = (ivf_h_raw.rate / ivf_h_raw.scale);
    int yuv = 1;

    if (ivf_h_raw.four_cc == 808596553)//I420
    {
        yuv = 0;
    }

    vpxt_itoa_custom(width, width_char, 10);
    vpxt_itoa_custom(height, height_char, 10);
    vpxt_itoa_custom(frame_rate, frame_rate_char, 10);
    vpxt_itoa_custom(yuv, yuv_char, 10);

    fclose(in);
    ////////////////////////////////////////////////////////////////////

    tprintf(PRINT_STD, "\n\nConvereting to raw.");

    vpxt_formatted_to_raw(input.c_str(), output.c_str());

    std::string Program;

#if defined(_WIN32)
    Program = "\"c:\\bin\\tmnplay.exe \"" + output + "\" ";
    Program += width_char;
    Program += " ";
    Program += height_char;
    Program += " ";
    Program += frame_rate_char;
    Program += " \"";
    Program += yuv_char;
#elif defined(linux)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program += width_char;
    Program += ":h=";
    Program += height_char;
    Program += "  \"" + output + "\" -loop 1000";
#elif defined(__APPLE__)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program += width_char;
    Program += ":h=";
    Program += height_char;
    Program += "  \"" + output + "\" -loop 1000";
#elif defined(_PPC)
    Program = "mplayer -demuxer rawvideo -rawvideo w=";
    Program += width_char;
    Program += ":h=";
    Program += height_char;
    Program += "  \"" + output + "\" -loop 1000";
#endif

    tprintf(PRINT_STD, "\n\n");

    int SysRet = system(Program.c_str());

    if (!SysRet == 0)
    {
#if defined(_WIN32)
        tprintf(PRINT_STD, "\n"
                "    An error occured when trying to play the file.  Please\n"
                "    Make sure tmnplay is located in your c:\\bin directory.\n"
                "    tmnplay can be found in the TestClips directory.\n"
               );
#else
        tprintf(PRINT_STD, "\n"
                "    An error occured when trying to play the file.  Please\n"
                "    make sure that mplayer is installed correctly.  mplayer\n"
                "    can be installed by typing:\n"
                "\n"
               "    svn checkout svn://svn.mplayerhq.hu/mplayer/trunk mplayer\n"
                "    cd mplayer\n"
                "    ./configure\n"
                "    make\n"
                "    make install\n"
                "\n"
                "    from a command Prompt."
               );
#endif
    }

    if (remove(output.c_str()) != 0)
        tprintf(PRINT_STD, "\nError deleting file: %s\n", output.c_str());
    else
        tprintf(PRINT_STD, "\n\nFile successfully deleted: %s\n",
        output.c_str());

    return 0;
}

int tool_print_cpu_info()
{
#if defined(ARM)
    printf("\nNOT SUPPORTED FOR ARM.\n");
    return 0;
#endif
#if defined(_PPC)
    printf("\nNOT SUPPORTED FOR PPC.\n");
    return 0;
#else
    int Simd_Caps = x86_simd_caps();

    tprintf(PRINT_STD, "\nDetected CPU capabilities:\n");

    if ((Simd_Caps & HAS_MMX)    == HAS_MMX)
        tprintf(PRINT_STD, "\n     MMX    Detected");

    if ((Simd_Caps & HAS_SSE)    == HAS_SSE)
        tprintf(PRINT_STD, "\n     SSE    Detected");

    if ((Simd_Caps & HAS_SSE2)   == HAS_SSE2)
        tprintf(PRINT_STD, "\n     SSE2   Detected");

    if ((Simd_Caps & HAS_SSE3)   == HAS_SSE3)
        tprintf(PRINT_STD, "\n     SSE3   Detected");

    if ((Simd_Caps & HAS_SSSE3)  == HAS_SSSE3)
        tprintf(PRINT_STD, "\n     SSSE3  Detected");

    if ((Simd_Caps & HAS_SSE4_1) == HAS_SSE4_1)
        tprintf(PRINT_STD, "\n     SSE4_1 Detected");

    tprintf(PRINT_STD, "\n");
#endif

    return 0;
}
int tool_random_stress_test(int argc, const char** argv)
{
    if (argc < 6)
    {
        tprintf(PRINT_STD,
                "\n  Random Stress Test\n\n"
                "    <Input Dir>\n"
                "    <Output Par Dir>\n"
                "    <Output Txt File>\n"
                "    <Input Number of Tests to Run>\n"
               );
        return 0;
    }

    printf("\n");

    std::string input_source_dir = argv[2];
    std::string output_par_dir = argv[3];
    std::string output_test_file = argv[4];
    int num_of_tests_to_run = atoi(argv[5]);

    char output_test_file_name[255];
    vpxt_file_name(output_test_file.c_str(), output_test_file_name, 1);
    std::string par_files_folder = output_par_dir + slashCharStr() +
        output_test_file_name;
    vpxt_make_dir(par_files_folder);
    par_files_folder += slashCharStr();
    std::string test_vector_folder = input_source_dir + slashCharStr() +
        "TestVectors";
    std::vector<std::string> file_names_vec;
    std::vector<std::string> source_file_names_vector;
    vpxt_list_files_in_dir(file_names_vec, input_source_dir.c_str());

    std::string VP8vNewest_PlugIn_RLib_RMode;
    std::string VP8vOldest_PlugIn_RLib_RMode;
    std::string VP8vNewest_PlugIn_DLib_DMode;

#if ARCH_X86
    VP8vNewest_PlugIn_RLib_RMode = "VP8vNewest_PlugIn_RLib_RMode_32Bit.exe";
    VP8vOldest_PlugIn_RLib_RMode = "VP8vOldest_PlugIn_RLib_RMode_32Bit.exe";
    VP8vNewest_PlugIn_DLib_DMode = "VP8vNewest_PlugIn_DLib_DMode_32Bit.exe";
#else if ARCH_X86_64
    VP8vNewest_PlugIn_RLib_RMode = "VP8vNewest_PlugIn_RLib_RMode_64Bit.exe";
    VP8vOldest_PlugIn_RLib_RMode = "VP8vOldest_PlugIn_RLib_RMode_64Bit.exe";
    VP8vNewest_PlugIn_DLib_DMode = "VP8vNewest_PlugIn_DLib_DMode_64Bit.exe";
#endif

    unsigned int i = 0;

    while (i < file_names_vec.size())
    {
        if (file_names_vec[i].length() > 4)
        {
            if (!file_names_vec[i].substr(file_names_vec[i].length() - 4,
                4).compare(".ivf"))
                source_file_names_vector.push_back(file_names_vec[i].c_str());
            else if (!file_names_vec[i].substr(file_names_vec[i].length() - 4,
                4).compare(".y4m"))
                source_file_names_vector.push_back(file_names_vec[i].c_str());
        }

        i++;
    }

    printf("\n");
    i = 0;

    while (i < source_file_names_vector.size())
    {
#if !defined(_WIN32)
        source_file_names_vector[i].insert(0, "/");
        source_file_names_vector[i].insert(0, input_source_dir.c_str());
#endif
        printf("Input file found %i %s\n", i, source_file_names_vector[i].c_str());
        i++;
    }

    if (source_file_names_vector.size() == 0)
    {
        printf("Error - No input files found in directory %s\n",
            input_source_dir.c_str());
        return 0;
    }

    std::string done_str = "done";
    char input_buffer[255];
    std::vector<int> valid_test_numbers;
    int arg_parse = 6;
    int input_done = 0;

    // parse command line if done is found skip manual input
    while(arg_parse < argc)
    {
        int test_num = vpxt_identify_test(argv[arg_parse]);

        if (test_num > 0 && test_num <= MAXTENUM)
            valid_test_numbers.push_back(test_num);
        else
            if (done_str.compare(input_buffer))
                input_done=1;

        arg_parse++;

    }

    if(!input_done)
    {
        vpxt_on_error_output();
        printf("\nPlease input test names or numbers to include "
            "(\"done\" to exit):\n");

        while (done_str.compare(input_buffer) != 0)
        {
            std::cin.getline(input_buffer, 255);
            int test_num = vpxt_identify_test(input_buffer);

            if (test_num > 0 && test_num <= MAXTENUM)
                valid_test_numbers.push_back(test_num);
            else
            {
                if (done_str.compare(input_buffer))
                    printf("Invaild Entry\n");
            }
        }
    }

    printf("output_file: %s\n", output_test_file.c_str());
    std::ofstream out_file(output_test_file.c_str());
    int current_test = 1;

    while (current_test <= num_of_tests_to_run)
    {
        int rand_test_num = rand() % valid_test_numbers.size();
        int rand_source_num = rand() % source_file_names_vector.size();
        char CurrentTestChar[255];
        vpxt_itoa_custom(current_test, CurrentTestChar, 10);
        std::string rand_setting_file = par_files_folder + output_test_file_name
            + "_" + CurrentTestChar + ".txt";
        std::string rand_source_file =
            source_file_names_vector[rand_source_num].c_str();
        VP8_CONFIG opt = vpxt_random_parameters(opt, rand_source_file.c_str(),
            2);
        vpxt_output_settings(rand_setting_file.c_str(), opt);
        int mode_num = 3;

        while (mode_num == 3)
            mode_num = rand() % 6;

        int rand_tb_num = (rand() % 20 + 1) * 128;
        int rand_key_fram_freq = rand() % 120;
        int rand_version = rand() % 4;
        int rand_cq = opt.cq_level;
        int rand_q_1 = rand() % 64;
        int rand_q_2 = rand() % 64;
        int rand_force_key_frame_num = rand() % 120;
        int rand_copy_set_frame = rand() % 120;
        int rand_lag_1 = rand() % 26;
        int rand_lag_2 = rand() % 26;
        int rand_multi_thread = rand() % 4;
        int rand_lag_in_frames = rand() % 26;
        int rand_scale_mode_num = rand() % 9;
        int rand_scale_tb_1 = 0;
        int rand_scale_tb_2 = 0;
        int rand_scale_tb_3 = 0;
        int rand_scale_tb_4 = 0;
        int rand_scale_tb_5 = 0;

        if((rand_scale_mode_num >= 0 && rand_scale_mode_num < 2) ||
            rand_scale_mode_num == 7)
        {
            rand_scale_tb_2 = (rand() % 20 + 4) * 128;
            rand_scale_tb_1 = (rand_scale_tb_2 * 60) / 100;
        }
        if((rand_scale_mode_num >= 2 && rand_scale_mode_num < 6)  ||
            rand_scale_mode_num == 8)
        {
            rand_scale_tb_3 = (rand() % 20 + 4) * 128;
            rand_scale_tb_2 = (rand_scale_tb_3 * 60) / 100;
            rand_scale_tb_1 = (rand_scale_tb_3 * 40) / 100;
        }
        if(rand_scale_mode_num == 6)
        {
            rand_scale_tb_5 = (rand() % 20 + 4) * 128;
            rand_scale_tb_4 = (rand_scale_tb_5 * 75) / 100;
            rand_scale_tb_3 = (rand_scale_tb_5 * 50) / 100;
            rand_scale_tb_2 = (rand_scale_tb_5 * 30) / 100;
            rand_scale_tb_1 = (rand_scale_tb_5 * 20) / 100;
        }

        if (valid_test_numbers[rand_test_num] == kTestAllowDropFrames)
        {
            out_file << "test_allow_drop_frames@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestAllowLag)
        {
            out_file << "test_allow_lag@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestAllowSpatialResampling)
        {
            out_file << "test_allow_spatial_resampling@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestArnr)
        {
            out_file << "test_arnr@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestAutoKeyFrame)
        {
            out_file << "test_auto_key_frame@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << rand_key_fram_freq;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestBufferLevel)
        {
            out_file << "test_buffer_level@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestChangeCpuDec)
        {
            out_file << "test_change_cpu_dec@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << rand_version;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestChangeCpuEnc)
        {
            out_file << "test_change_cpu_enc@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << rand_version;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestConstrainedQuality)
        {
            out_file << "test_constrained_quality@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << rand_cq;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestCopySetReference)
        {
            out_file << "test_copy_set_reference@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << rand_copy_set_frame;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestDataRate)
        {
            out_file << "test_data_rate@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestDebugMatchesRelease)
        {
            out_file << "test_debug_matches_release@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << VP8vNewest_PlugIn_DLib_DMode.c_str();
            out_file << "@";
            out_file << VP8vNewest_PlugIn_RLib_RMode.c_str();
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestDropFrameWaterMark)
        {
            out_file << "test_drop_frame_watermark@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestEncoderBreakout)
        {
            out_file << "test_encoder_break_out@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestErrorConcealment)
        {
            out_file << "test_error_concealment@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestErrorResolution)
        {
            out_file << "test_error_resolution@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestExtraFile)
        {
            out_file << "test_extra_file@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestFixedQuantizer)
        {
            out_file << "test_fixed_quantizer@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << rand_q_1;
            out_file << "@";
            out_file << rand_q_2;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestForcedKeyFrame)
        {
            out_file << "test_force_key_frame@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << rand_force_key_frame_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestFrameSize)
        {
            int RandWidth = opt.Width - (opt.Width % 16) - ((rand() %
                (opt.Width / 16)) * 16);
            int RandHeight = opt.Height - - (opt.Height % 16) - ((rand() %
                (opt.Height / 16)) * 16);
            out_file << "test_frame_size@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << RandWidth;
            out_file << "@";
            out_file << RandHeight;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestGoodVsBest)
        {
            out_file << "test_good_vs_best@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestLagInFrames)
        {
            out_file << "test_lag_in_frames@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << rand_lag_1;
            out_file << "@";
            out_file << rand_lag_2;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestMaxQuantizer)
        {
            out_file << "test_max_quantizer@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestMemLeak)
        {
            out_file << "test_mem_leak@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << VP8vNewest_PlugIn_DLib_DMode.c_str();
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestMemLeak2)
        {
            out_file << "test_mem_leak2@";
            out_file << VP8vNewest_PlugIn_DLib_DMode.c_str();
            out_file << "@";
            out_file << "MemLeakCheck2_Compression.ivf";
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestMinQuantizer)
        {
            out_file << "test_min_quantizer@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestMultiResolutionEncode)
        {
            out_file << "test_multiple_resolution_encode@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }


        if (valid_test_numbers[rand_test_num] == kTestMultiThreadedDec)
        {
            out_file << "test_multithreaded_dec@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << rand_multi_thread;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestMultiThreadedEnc)
        {
            out_file << "test_multithreaded_enc@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << rand_multi_thread;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestNewVsOldEncCpuTick)
        {
            out_file << "test_new_vs_old_enc_cpu_tick@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << VP8vOldest_PlugIn_RLib_RMode.c_str();
            out_file << "@";
            out_file << "2";
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestNewVsOldPsnr)
        {
            out_file << "test_new_vs_old_psnr@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << VP8vOldest_PlugIn_RLib_RMode.c_str();
            out_file << "@";
            out_file << "2";
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestNewVsOldTempScale)
        {
            out_file << "test_new_vs_old_psnr@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << VP8vOldest_PlugIn_RLib_RMode.c_str();
            out_file << "@";
            out_file << "2";
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestNoiseSensitivity)
        {
            out_file << "test_noise_sensitivity@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestOnePassVsTwoPass)
        {
            out_file << "test_one_pass_vs_two_pass@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestPlayAlternate)
        {
            out_file << "test_play_alternate@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestPostProcessor)
        {
            out_file << "test_post_processor@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestPostProcessorMfqe)
        {
            out_file << "test_post_processor@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << 10;
            out_file << "@";
            out_file << 200;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestReconstructBuffer)
        {
            out_file << "test_reconstruct_buffer@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestResampleDownWatermark)
        {
            out_file << "test_resample_down_watermark@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestSpeed)
        {
            out_file << "test_speed@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << rand_lag_in_frames;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestTemporalScalability)
        {
            out_file << "test_temporal_scalability@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << rand_scale_mode_num;
            out_file << "@";
            out_file << rand_scale_tb_1;
            out_file << "@";
            out_file << rand_scale_tb_2;
            out_file << "@";
            out_file << rand_scale_tb_3;
            out_file << "@";
            out_file << rand_scale_tb_4;
            out_file << "@";
            out_file << rand_scale_tb_5;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestTestVector)
        {
            out_file << "test_test_vector@";
            out_file << test_vector_folder.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestThirtytwoVsSixtyfour)
        {
        }

        if (valid_test_numbers[rand_test_num] == kTestTwoPassVsTwoPassBest)
        {
            out_file << "test_two_pass_vs_two_pass_best@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestUndershoot)
        {
            out_file << "test_undershoot@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestVersion)
        {
            out_file << "test_version@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestVpxMatchesInt)
        {
            out_file << "test_vpx_matches_int@";
            out_file << rand_source_file.c_str();
            out_file << "@";
            out_file << mode_num;
            out_file << "@";
            out_file << rand_tb_num;
            out_file << "@";
            out_file << "vpxenc.exe";
            out_file << "@";
            out_file << "vpxdec.exe";
            out_file << "@";
            out_file << "webm";
            out_file << "@";
            out_file << "y4m";
            out_file << "@";
            out_file << rand_setting_file.c_str();
            out_file << "\n";
        }

        if (valid_test_numbers[rand_test_num] == kTestWinLinMacMatch)
        {
        }

        current_test++;
    }

    out_file.close();
    return 0;
}
int tool_raw_to_formatted(int argc, const char** argv)
{
    if (argc < 9)
        return vpxt_tool_help(argv[1], 0);

    std::string input_file = argv[2];
    std::string outputDir = argv[3];
    std::string format = argv[4];
    vpxt_lower_case_string(format);
    int width = atoi(argv[5]);
    int height = atoi(argv[6]);
    int frame_rate = atoi(argv[7]);
    std::string fourcc = argv[8];

    int file_type = -1;

    if (format.compare("ivf") == 0)
        file_type = FILE_TYPE_IVF;

    if (format.compare("y4m") == 0)
        file_type = FILE_TYPE_Y4M;

    if (file_type == -1)
    {
        tprintf(PRINT_STD, "\nError File format %s not recognised\n", argv[4]);
        return 0;
    }

    FILE *in = fopen(input_file.c_str(), "rb");
    FILE *out2 = fopen(outputDir.c_str(), "wb");
    ///////////////////////////////////

    if (in == NULL)
    {
        tprintf(PRINT_BTH, "\nInput file does not exist");
        return 0;
    }

    int current_video_frame = 0;
    int frame_count = 0;
    int byte_rec = 0;

    IVF_HEADER ivf_h_raw;
    InitIVFHeader(&ivf_h_raw);

    if (fourcc.compare("I420") == 0)
        ivf_h_raw.four_cc    = MAKEFOURCC('I', '4', '2', '0');

    if (fourcc.compare("YV12") == 0)
        ivf_h_raw.four_cc    = MAKEFOURCC('Y', 'V', '1', '2');

    ivf_h_raw.headersize  = 32;
    ivf_h_raw.height      = height;
    ivf_h_raw.length      = 0;
    ivf_h_raw.rate        = frame_rate;
    ivf_h_raw.scale       = 1;
    ivf_h_raw.version     = 0;
    ivf_h_raw.width       = width;

    vpxt_format_ivf_header_write(ivf_h_raw);

    if (file_type == FILE_TYPE_Y4M)
    {
        char width_char[32];
        char height_char[32];
        char frame_rate_char[32];
        vpxt_itoa_custom(width, width_char, 10);
        vpxt_itoa_custom(height, height_char, 10);
        vpxt_itoa_custom(frame_rate, frame_rate_char, 10);

        std::string buffer_str = "YUV4MPEG2 C420jpeg W";
        buffer_str += width_char;
        buffer_str += " H";
        buffer_str += height_char;
        buffer_str += " F";
        buffer_str += frame_rate_char;
        buffer_str += ":1 Ip.";

        out_put(out2, (unsigned char *)buffer_str.c_str(),
            strlen(buffer_str.c_str()), 0);
    }

    if (file_type == FILE_TYPE_IVF)
        fwrite(&ivf_h_raw, 1, 32, out2);

    IVF_FRAME_HEADER ivf_fhRaw;
    ivf_fhRaw.frameSize = ivf_h_raw.width * ivf_h_raw.height * 3 / 2;
    ivf_fhRaw.timeStamp = 0;

    long samples = frame_count;
    long rate_num = ivf_h_raw.rate;
    long rate_denom = ivf_h_raw.scale;

    long samples_per_block = 1;

    long bytes = 0;
    long bytes_min = 999999;
    long bytes_max = 0;

    if (file_type == FILE_TYPE_IVF)
        tprintf(PRINT_STD, "\nConvereting raw to IVF:\n");

    if (file_type == FILE_TYPE_Y4M)
        tprintf(PRINT_STD, "\nConvereting raw to Y4M:\n");

    char *inbuff = new char[ivf_h_raw.width * ivf_h_raw.height * 3/2];
    int char_count = 0;

    while (!feof(in))
    {
        if (char_count == 79)
        {
            tprintf(PRINT_STD, "\n");
            char_count = 0;
        }

        tprintf(PRINT_STD, ".");
        memset(inbuff, 0, ivf_h_raw.width * ivf_h_raw.height * 3 / 2);
        fread(inbuff, 1, ivf_fhRaw.frameSize, in);

        if (feof(in))
            break;

        if (file_type == FILE_TYPE_IVF)
        {
            vpxt_format_frame_header_write(ivf_fhRaw);
            fwrite(&ivf_fhRaw, 1, sizeof(ivf_fhRaw), out2);
        }

        if (file_type == FILE_TYPE_Y4M)
            out_put(out2, (unsigned char *)"FRAME\n", 6, 0);

        fwrite(inbuff, 1, ivf_fhRaw.frameSize, out2);

        current_video_frame ++;
    }

    if (file_type == FILE_TYPE_IVF)
    {
        ivf_h_raw.length = current_video_frame;
        fseek(out2 , 0 , SEEK_SET);
        vpxt_format_ivf_header_write(ivf_h_raw);
        fwrite(&ivf_h_raw, 1, 32, out2);
    }

    tprintf(PRINT_STD, "\n");

    fclose(in);
    fclose(out2);

    return 0;
}
int tool_run_ivfdec(int argc, const char** argv)
{
    int dummy_argc = argc - 1;

    const char *dummy_argv[999];
    dummy_argv[0] = argv[0];

    int i = 2;

    while (i < argc)
    {
        dummy_argv[i-1] = argv[i];
        i++;
    }

    ivfdec(dummy_argc, (const char **) dummy_argv);

    return 0;
}
int tool_run_ivfenc(int argc, const char** argv)
{
    int dummy_argc = argc - 1;

    const char *dummy_argv[999];
    dummy_argv[0] = argv[0];

    int i = 2;

    while (i < argc)
    {
        dummy_argv[i-1] = argv[i];
        i++;
    }

    ivfenc(dummy_argc, (const char **) dummy_argv);

    return 0;
}
int tool_run_thresh(int argc, const char** argv)
{
    if (argc < 8)
    {
        tprintf(PRINT_STD, "\n"
                "  DisplayResizedFrames \n\n"
                "    <Input IVF File>\n"
                "    <bitrate>\n"
                "    <max_buffer>\n"
                "    <pre_buffer>\n"
                "    <optBuffer>\n"
                "    <Threshold>\n"
                "\n");
        return 0;
    }

    vpxt_check_pbm_threshold(argv[2], atof(argv[3]), atoi(argv[4]),
        atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));

    return 0;
}



int tool_solve_quad()
{
    float x1 = 0;
    float x2 = 0;
    float x3 = 0;

    float y1 = 0;
    float y2 = 0;
    float y3 = 0;

    float a = 0;
    float b = 0;
    float c = 0;

    tprintf(PRINT_STD, "\nIntput x1: ");
    std::cin >> x1;
    tprintf(PRINT_STD, "Intput x2: ");
    std::cin >> x2;
    tprintf(PRINT_STD, "Intput x3: ");
    std::cin >> x3;

    tprintf(PRINT_STD, "Intput y1: ");
    std::cin >> y1;
    tprintf(PRINT_STD, "Intput y2: ");
    std::cin >> y2;
    tprintf(PRINT_STD, "Intput y3: ");
    std::cin >> y3;


    a = ((y2 - y1) * (x1 - x3) + (y3 - y1) * (x2 - x1)) / ((x1 - x3) *
        ((x2 * x2) - (x1 * x1)) + (x2 - x1) * ((x3 * x3) - (x1 * x1)));
    b = ((y2 - y1) - a * ((x2 * x2) - (x1 * x1))) / (x2 - x1);
    c = y1 - a * (x1 * x1) - b * x1;

    std::cout << "\nA = " << a;
    std::cout << "\nB = " << b;
    std::cout << "\nC = " << c;

    tprintf(PRINT_STD, "\n\ny = %.2fx^2 + %.2fx + %.2f\n\n", a, b, c);

    return 0;
}