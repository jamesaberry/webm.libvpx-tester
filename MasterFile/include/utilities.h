#include <string>
#include <vector>
#include "onyx.h"
#include "ivf.h"
using namespace std;

#ifndef UTILITIES_H_GUARD
#define UTILITIES_H_GUARD

#define PRINT_STD 1
#define PRINT_ERR 2
#define PRINT_BTH 3

#define HLPPRT 1
#define TOLPRT 2
#define FUNPRT 3
#define OTRPRT 4
#define RESPRT 5

void tprintf(int PrintSelection, const char *fmt, ...);
string slashCharStr();
char slashChar();
//-----------------------------------------------------VP8 Settings-------------------------------------------------------------------
void vpxt_default_parameters(VP8_CONFIG &opt);
int vpxt_core_config_to_api_config(VP8_CONFIG coreCfg, vpx_codec_enc_cfg_t *cfg);
VP8_CONFIG vpxt_random_parameters(VP8_CONFIG &opt, char *inputfile, int display);
VP8_CONFIG vpxt_input_settings(char *inputFile);
int vpxt_output_settings(const char *outputFile, VP8_CONFIG opt);
int vpxt_output_compatable_settings(const char *outputFile, VP8_CONFIG opt, int ParVersionNum);
int vpxt_output_settings_api(char *outputFile, vpx_codec_enc_cfg_t cfg);
int vpxt_output_settings_ivfenc(char *outputFile, VP8_CONFIG opt);
//---------------------------------------------------IVF Header Data------------------------------------------------------------------
int vpxt_print_ivf_file_header(IVF_HEADER ivf);
int vpxt_format_ivf_header_read(IVF_HEADER *ivf);
int vpxt_format_ivf_header_write(IVF_HEADER &ivf);
int vpxt_format_frame_header_read(IVF_FRAME_HEADER &ivf_fh);
int vpxt_format_frame_header_write(IVF_FRAME_HEADER &ivf_fh);
//---------------------------------------------------File Management------------------------------------------------------------------
long vpxt_file_size(const char *inFile, int printbool);
void vpxt_file_name(const char *input, char *FileName, int removeExt);
void vpxt_folder_name(const char *input, char *output);
void vpxt_remove_file_extension(const char *DirIn, char *DirOut);
string vpxt_extract_date_time(string InputStr);
int vpxt_timestamp_compare(string TimeStampNow, string TimeStampPrevious);
int vpxt_identify_test(const char *test_char);
int vpxt_run_multiple_tests_input_check(const char *input, int MoreInfo);
int vpxt_file_exists_check(string input);
int vpxt_folder_exist_check(string FolderName);
void vpxt_subfolder_name(char *input, char *FileName);
void vpxt_test_name(char *input, char *FileName);
int vpxt_remove_char_spaces(const char *input, char *output, int maxsize);
//--------------------------------------------------------Math------------------------------------------------------------------------
int vpxt_decimal_places(int InputNumber);
int vpxt_gcd(int a, int b);
int vpxt_abs_int(int input);
float vpxt_abs_float(float input);
double vpxt_abs_double(double input);
int vpxt_solve_quadradic(float X1, float X2, float X3, float Y1, float Y2, float Y3, float &A, float &B, float &C);
float vpxt_area_under_quadradic(float A, float B, float C, float X1, float X2);
char *vpxt_itoa_custom(int value, char *result, int base);
//----------------------------------------------------Cross Plat----------------------------------------------------------------------
unsigned int vpxt_get_high_res_timer_tick();
unsigned int vpxt_get_time_in_micro_sec(unsigned int startTick, unsigned int stopTick);
unsigned int vpxt_get_cpu_tick();
unsigned int vpxt_get_time();
int vpxt_get_cur_dir(string &CurrentDir);
int vpxt_make_dir(string CreateDir);
int vpxt_make_dir_vpx(string CreateDir2);
void vpxt_run_exe(string RunExe);
int vpxt_add_dir_files_to_ignore(vector<string> &IgnoredFiles, string Directory);
int vpxt_find_non_ignored_files_in_dir(vector<string> IgnoredFiles, vector<string> &FilesFound, string Directory);
//---------------------------------------------------------IVF------------------------------------------------------------------------
int image2yuvconfig(const vpx_image_t   *img, YV12_BUFFER_CONFIG  *yv12);
double vpxt_ivf_psnr(const char *inputFile1, const char *inputFile2, int forceUVswap, int frameStats, int printvar, double *SsimOut);
double vpxt_post_proc_ivf_psnr(char *inputFile1, const char *inputFile2, int forceUVswap, int frameStats, int printvar, int deblock_level, int noise_level, int flags, double *SsimOut);
double vpxt_ivf_data_rate(const char *inputFile, int DROuputSel);
int vpxt_ivf_check_pbm(const char *inputFile, int bitRate, int maxBuffer, int preBuffer);
int vpxt_ivf_check_pbm_threshold(const char *inputFile, double bitRate, int maxBuffer, int preBuffer, int optimalbuffer, int Threshold);
int vpxt_faux_compress();
int vpxt_faux_decompress(char *inputChar);
//---------------------------------------------------Test Functions-------------------------------------------------------------------
int initialize_test_directory(int argc, char *argv[], int TestType, string WorkingDir, char *MyDir, string &WorkingDirString, string &MainDirString, char WorkingDir3[255], char File1[255], string FilesAr[]);
void record_test_complete(string MainDirString, const char *FileIndexOutputChar, int TestType);
int print_version();
void print_header_info();
void print_header_full_test(int argc, char *argv[], string WorkingDir3);
void print_header_compression_only(int argc, char *argv[], string WorkingDir3);
void print_header_test_only(int argc, char *argv[], string  WorkingDir3);
void check_time_stamp(int SelectorArInt, string *SelectorAr, string *SelectorAr2, string TimeStampPrevious, int &identicalFileVar, string *TimeStampAr2);
void vpxt_formated_print(int selector, const char *fmt, ...);
void vpxt_cap_string_print(int selector, const char *fmt, ...);
//----------------------------------------------------------IVF API-------------------------------------------------------------------------
#ifdef API
int vpxt_compress_ivf_to_ivf(const char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck, int arnr_max_frames, int arnr_strength, int arnr_type);
int vpxt_compress_ivf_to_ivf_no_error_output(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck, int arnr_max_frames, int arnr_strength, int arnr_type);
unsigned int vpxt_time_compress_ivf_to_ivf(char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck, unsigned int &CPUTick, int arnr_max_frames, int arnr_strength, int arnr_type);
int vpxt_compress_ivf_to_ivf_force_key_frame(char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck, int forceKeyFrame, int arnr_max_frames, int arnr_strength, int arnr_type);
int vpxt_compress_ivf_to_ivf_recon_buffer_check(char *inputFile, const char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt, int RunQCheck, int arnr_max_frames, int arnr_strength, int arnr_type);
int vpxt_decompress_ivf_to_ivf(const char *inputchar, const char *outputchar);
int vpxt_decompress_ivf_to_raw(const char *inputchar, const char *outputchar);
int vpxt_decompress_ivf_to_raw_no_error_output(char *inputchar, char *outputchar);
int vpxt_decompress_ivf_to_ivf_no_output(char *inputchar, char *outputchar);
unsigned int vpxt_time_decompress_ivf_to_ivf(const char *inputchar, const char *outputchar, unsigned int &CPUTick);
unsigned int vpxt_decompress_ivf_to_ivf_time_and_output(const char *inputchar, const char *outputchar, unsigned int &CPUTick);
int vpxt_dec_compute_md5(const char *inputchar, const char *outputchar);
#endif
//--------------------------------------------------------IVF Tools-------------------------------------------------------------------------
int vpxt_cut_ivf(char *inputFile, char *outputFile, int StartingFrame, int EndingFrame);
int vpxt_crop_raw_ivf(char *inputFile, const char *outputFile, int xoffset, int yoffset, int newFrameWidth, int newFrameHeight, int FileIsIVF, int OutputToFile);
int vpxt_paste_ivf(char *inputFile1, char *inputFile2, char *outputFile, int StartingFrame);
int vpxt_display_ivf_header_info(int argc, char *argv[]);
int vpxt_compare_ivf_header_info(int argc, char *argv[]);
int vpxt_compare_ivf(const char *inputFile1, const char *inputFile2);
double vpxt_display_droped_frames(char *inputchar, int PrintSwitch);
double vpxt_display_resized_frames(const char *inputchar, int PrintSwitch);
double vpxt_display_visible_frames(const char *inputFile, int Selector);
double vpxt_display_alt_ref_frames(const char *inputFile, int Selector);
double vpxt_display_key_frames(const char *inputFile, int Selector);
int vpxt_lag_in_frames_check(const char *QuantInChar);
int vpxt_dfwm_check(const char *InputFile, int printselect);
int vpxt_check_min_quantizer(const char *inputFile, int MinQuantizer);
int vpxt_check_max_quantizer(const char *inputFile, int MaxQuantizer);
int vpxt_check_fixed_quantizer(const char *inputFile, int FixedQuantizer);
int vpxt_time_return(const char *infile, int FileType);
int vpxt_cpu_tick_return(const char *infile, int FileType);
int vpxt_get_number_of_frames(const char *inputFile);
int vpxt_check_force_key_frames(const char *KeyFrameoutputfile, int ForceKeyFrameInt, const char *ForceKeyFrame);
int vpxt_check_mem_state(string FileName, string &bufferString);
int vpxt_print_compare_ivf_results(int lngRC);
double vpxt_get_psnr(const char *compFileName);
//-----------------------------------------------------------IVF Enc------------------------------------------------------
int API20Encoder(long width, long height, char *infilechar, char *outfilechar);
int API20EncoderIVF2IVF(char *inputFile, char *outputFile2, int speed, int BitRate, VP8_CONFIG &oxcf, char *CompressString, int CompressInt);
int API20Decoder(char *inputchar, char *outputchar);
int API20DecoderIVF2IVF(char *inputchar, char *outputchar);
#endif