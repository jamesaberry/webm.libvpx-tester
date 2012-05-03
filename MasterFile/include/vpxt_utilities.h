#include "onyx.h"
#include "ivf.h"
#include <string>
#include <vector>

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
std::string slashCharStr();
char slashChar();
//-----------------------------VP8 Settings-------------------------------------
void vpxt_default_parameters(VP8_CONFIG &opt);
void vpxt_determinate_parameters(VP8_CONFIG &opt);
int vpxt_core_config_to_api_config(VP8_CONFIG coreCfg,
                                   vpx_codec_enc_cfg_t *cfg);
VP8_CONFIG vpxt_random_parameters(VP8_CONFIG &opt,
                                  const char *inputfile,
                                  int display);
VP8_CONFIG vpxt_input_settings(const char *inputFile);
int vpxt_output_settings(const char *outputFile, VP8_CONFIG opt);
int vpxt_output_compatable_settings(const char *outputFile,
                                    VP8_CONFIG opt,
                                    int ParVersionNum);
int vpxt_output_settings_api(const char *outputFile, vpx_codec_enc_cfg_t cfg);
int vpxt_input_settings_api(const char *inputFile, vpx_codec_enc_cfg_t &cfg);
int vpxt_output_settings_ivfenc(const char *outputFile, VP8_CONFIG opt);
int vpxt_convert_par_file_to_ivfenc(const char *input, const char *output);
int vpxt_convert_par_file_to_vpxenc(const char *input_core,
                                    const char *input_api,
                                    char *vpxenc_parameters,
                                    int vpxenc_parameters_sz);
int vpxt_file_is_yv12(const char *in_fn);
//------------------------IVF Header Data---------------------------------------
int vpxt_print_ivf_file_header(IVF_HEADER ivf);
int vpxt_format_ivf_header_read(IVF_HEADER *ivf);
int vpxt_format_ivf_header_write(IVF_HEADER &ivf);
int vpxt_format_frame_header_read(IVF_FRAME_HEADER &ivf_fh);
int vpxt_format_frame_header_write(IVF_FRAME_HEADER &ivf_fh);
//------------------------------File Management---------------------------------
long vpxt_file_size(const char *inFile, int printbool);
void vpxt_file_name(const char *input, char *FileName, int removeExt);
void vpxt_folder_name(const char *input, std::string *output_str);
int  vpxt_get_number_of_frames(const char *inputFile);
int  vpxt_get_multi_res_width_height(const char *inputFile,
                                     int FileNumber,
                                     unsigned int &width,
                                     unsigned int &height);
int  vpxt_remove_file_extension(const char *In, std::string &Out);
int  vpxt_get_file_extension(const char *In, std::string &Out);
int  vpxt_enc_format_append(std::string &InputString, std::string EncFormat);
int  vpxt_dec_format_append(std::string &InputString, std::string DecFormat);
int  vpxt_raw_file_size(const char *inputFile);
std::string vpxt_extract_date_time(const std::string InputStr);
int vpxt_timestamp_compare(const std::string TimeStampNow,
                           const std::string TimeStampPrevious);
int get_test_name(int TestNumber, std::string &TestName);
int vpxt_identify_test(const char *test_char);
int vpxt_run_multiple_tests_input_check(const char *input, int MoreInfo);
int vpxt_file_exists_check(const std::string input);
int vpxt_folder_exist_check(const std::string FolderName);
void vpxt_subfolder_name(const char *input, char *FileName);
void vpxt_test_name(char *input, char *FileName);
int  vpxt_init_new_vs_old_log(const char *input, std::string TestIDStr);
int  vpxt_sync_new_vs_old_log(const char *timelog,
                              const char *gitlog,
                              const char *newtimelog,
                              const char *updateinfo,
                              std::string TestIDStr,
                              std::string testName);
double vpxt_get_new_vs_old_val(std::string fileline);
int  vpxt_eval_new_vs_old_log(const char *logfile,
                              std::string TestIDStr,
                              int printvar,
                              std::vector<double> &ValueList,
                              std::string testName);
int  vpxt_check_arg_input(const char *testName, int argNum);
int vpxt_remove_char_spaces(const char *input, char *output, int maxsize);
//---------------------------Math-----------------------------------------------
int vpxt_decimal_places(int InputNumber);
int vpxt_gcd(int a, int b);
int vpxt_abs_int(int input);
float vpxt_abs_float(float input);
double vpxt_abs_double(double input);
int vpxt_solve_quadratic(double X1,
                         double X2,
                         double X3,
                         double Y1,
                         double Y2,
                         double Y3,
                         double &A,
                         double &B,
                         double &C);
double vpxt_area_under_quadratic(double A, double B, double C, double X1,
								 double X2);
char *vpxt_itoa_custom(int value, char *result, int base);
//---------------------------Cross Plat-----------------------------------------
void vpxt_delete_files(int argcount, ...);
void vpxt_delete_files_quiet(int argcount, ...);
void vpxt_copy_file(const char *input, const char *output);
unsigned int vpxt_get_high_res_timer_tick();
unsigned int vpxt_get_time_in_micro_sec(unsigned int startTick,
                                        unsigned int stopTick);
unsigned int vpxt_get_cpu_tick();
unsigned int vpxt_get_time();
int vpxt_get_cur_dir(std::string &CurrentDir);
int vpxt_make_dir(std::string CreateDir);
int vpxt_make_dir_vpx(std::string CreateDir2);
void vpxt_run_exe(std::string RunExe);
int vpxt_list_files_in_dir(std::vector<std::string> &file_name_vector,
                           std::string directory);
int vpxt_add_dir_files_to_ignore(std::vector<std::string> &IgnoredFiles,
                                 std::string directory);
int vpxt_find_non_ignored_files_in_dir(std::vector<std::string> IgnoredFiles,
                                       std::vector<std::string> &FilesFound,
                                       std::string directory);
//---------------------------IVF------------------------------------------------
int image2yuvconfig(const vpx_image_t   *img, YV12_BUFFER_CONFIG  *yv12);
double vpxt_psnr(const char *inputFile1,
                 const char *inputFile2,
                 int forceUVswap,
                 int frameStats,
                 int printvar,
                 double *SsimOut);
double vpxt_psnr_dec(const char *inputFile1,
                     const char *inputFile2,
                     int forceUVswap,
                     int frameStats,
                     int printvar,
                     double *SsimOut,
                     int width,
                     int height);
double vpxt_post_proc_psnr(const char *inputFile1,
                           const char *inputFile2,
                           int forceUVswap,
                           int frameStats,
                           int printvar,
                           int deblock_level,
                           int noise_level,
                           int flags,
                           double *SsimOut);
double vpxt_psnr_resize_frames(const char *inputFile1,
                               const char *inputFile2,
                               int forceUVswap,
                               int frameStats,
                               int printvar,
                               double *SsimOut,
                               int resizeHeight,
                               int resizeWidth);
double vpxt_data_rate(const char *inputFile, int DROuputSel);
int vpxt_check_pbm(const char *inputFile,
                   int bitRate,
                   int64_t maxBuffer,
                   int64_t preBuffer);
int vpxt_check_pbm_threshold(const char *inputFile,
                             double bitRate,
                             int64_t maxBuffer,
                             int64_t preBuffer,
                             int64_t optimalbuffer,
                             int Threshold);
int vpxt_faux_compress();
int vpxt_faux_decompress(const char *inputChar);
//----------------------------Test Functions------------------------------------
int initialize_test_directory(int argc,
                              const char *const *argv,
                              int test_type,
                              const std::string &working_dir,
                              const char *MyDir,
                              std::string &CurTestDirStr,
                              std::string &FileIndexStr,
                              char MainTestDirChar[255],
                              char FileIndexOutputChar[255],
                              const std::string sub_folder_str);
void record_test_complete(const std::string MainDirstring,
                          const char *FileIndexOutputChar,
                          int test_type);
int print_version();
void print_header_info();
void print_header_info_to_file(const char *FileName);
void print_header_full_test(int argc,
                            const char *const *argv,
                            std::string WorkingDir3);
void print_header_compression_only(int argc,
                                   const char *const *argv,
                                   std::string WorkingDir3);
void print_header_test_only(int argc,
                            const char *const *argv,
                            std::string  WorkingDir3);
void check_time_stamp(int SelectorArInt,
                      std::string *SelectorAr,
                      std::string *SelectorAr2,
                      std::string TimeStampPrevious,
                      int &identicalFileVar,
                      std::string *TimeStampAr2);
void vpxt_formated_print(int selector, const char *fmt, ...);
void vpxt_cap_string_print(int selector, const char *fmt, ...);
int  vpxt_lower_case_string(std::string &input);
//------------------------------IVF API-----------------------------------------
#ifdef API
int vpxt_compress(const char *inputFile,
                  const char *outputFile2,
                  int speed,
                  int BitRate,
                  VP8_CONFIG &oxcf,
                  const char *CompressString,
                  int CompressInt,
                  int RunQCheck,
                  std::string EncFormat);
int vpxt_compress_no_error_output(const char *inputFile,
                                  const char *outputFile2,
                                  int speed,
                                  int BitRate,
                                  VP8_CONFIG &oxcf,
                                  const char *CompressString,
                                  int CompressInt,
                                  int RunQCheck,
                                  std::string EncFormat);
unsigned int vpxt_time_compress(const char *inputFile,
                                const char *outputFile2,
                                int speed,
                                int BitRate,
                                VP8_CONFIG &oxcf,
                                const char *CompressString,
                                int CompressInt,
                                int RunQCheck,
                                unsigned int &CPUTick,
                                std::string EncFormat);
int vpxt_compress_force_key_frame(const char *inputFile,
                                  const char *outputFile2,
                                  int speed,
                                  int BitRate,
                                  VP8_CONFIG &oxcf,
                                  const char *CompressString,
                                  int CompressInt,
                                  int RunQCheck,
                                  int forceKeyFrame,
                                  std::string EncFormat);
int vpxt_compress_recon_buffer_check(const char *inputFile,
                                     const char *outputFile2,
                                     int speed,
                                     int BitRate,
                                     VP8_CONFIG &oxcf,
                                     const char *CompressString,
                                     int CompressInt,
                                     int RunQCheck,
                                     int OutputRaw,
                                     std::string EncFormat);
unsigned int vpxt_compress_multi_resolution(const char *inputFile,
                                            const char *outputFile2,
                                            int speed,
                                            int BitRate,
                                            VP8_CONFIG &oxcf,
                                            const char *CompressString,
                                            int CompressInt,
                                            int RunQCheck,
                                            std::string EncFormat);
unsigned int vpxt_compress_scalable_patterns(const char *inputFile,
                                             const char *outputFile2,
                                             int speed,
                                             VP8_CONFIG &oxcf,
                                             const char *CompressString,
                                             int CompressInt,
                                             int RunQCheck,
                                             std::string EncFormat,
                                             int ScaleMode,
                                             int ScaleBitRate0,
                                             int ScaleBitRate1,
                                             int ScaleBitRate2,
                                             int ScaleBitRate3,
                                             int ScaleBitRate4);
int vpxt_decompress(const char *inputchar,
                    const char *outputchar,
                    std::string DecFormat,
                    int threads);
int vpxt_decompress_copy_set(const char *inputchar,
                             const char *outputchar,
                             const char *outfile2,
                             std::string DecFormat,
                             int threads,
                             int firstClone,
                             int printVar);
int vpxt_decompress_partial_drops(const char *inputchar,
                                  const char *outputchar,
                                  std::string DecFormat,
                                  int threads,
                                  int n,
                                  int m,
                                  int mode,
                                  int printVar,
                                  int outputParDropEnc);
int vpxt_decompress_resize(const char *inputchar,
                           const char *outputchar,
                           std::string DecFormat,
                           int threads);
int vpxt_decompress_to_raw(const char *inputchar,
                           const char *outputchar,
                           int threads);
int vpxt_decompress_to_raw_no_error_output(const char *inputchar,
                                           const char *outputchar,
                                           int threads);
int vpxt_decompress_no_output(const char *inputchar,
                              const char *outputchar,
                              std::string DecFormat,
                              int threads);
unsigned int vpxt_time_decompress(const char *inputchar,
                                  const char *outputchar,
                                  unsigned int &CPUTick,
                                  std::string DecFormat,
                                  int threads);
unsigned int vpxt_decompress_time_and_output(const char *inputchar,
                                             const char *outputchar,
                                             unsigned int &CPUTick,
                                             std::string DecFormat,
                                             int threads);
int vpxt_dec_compute_md5(const char *inputchar, const char *outputchar);
#endif
//------------------------------IVF Tools---------------------------------------
int vpxt_cut_clip(const char *inputFile,
                  const char *outputFile,
                  int StartingFrame,
                  int EndingFrame);
int vpxt_crop_raw_clip(const char *inputFile,
                       const char *outputFile,
                       int xoffset,
                       int yoffset,
                       int newFrameWidth,
                       int newFrameHeight,
                       int FileIsIVF,
                       int OutputToFile);
int vpxt_pad_raw_clip(const char *inputFile,
                      const char *outputFile,
                      int newFrameWidth,
                      int newFrameHeight,
                      int FileIsIVF,
                      int OutputToFile);
int vpxt_paste_clip(const char *inputFile1,
                    const char *inputFile2,
                    const char *outputFile,
                    int StartingFrame);
int vpxt_formatted_to_raw(const std::string inputFile,
                          const std::string outputDir);
int vpxt_formatted_to_raw_frames(std::string inputFile,
                                 std::string outputDir);
int vpxt_display_header_info(int argc, const char *const *argv);
int vpxt_compare_header_info(int argc, const char *const *argv);
int vpxt_compare_dec(const char *inputFile1, const char *inputFile2);
int vpxt_compare_enc(const char *inputFile1,
                     const char *inputFile2, int fullcheck);
int vpxt_display_droped_frames(const char *inputchar, int PrintSwitch);
int vpxt_display_resized_frames(const char *inputchar, int PrintSwitch);
int vpxt_display_visible_frames(const char *inputFile, int Selector);
int vpxt_display_alt_ref_frames(const char *inputFile, int Selector);
int vpxt_display_key_frames(const char *inputFile, int Selector);
int vpxt_lag_in_frames_check(const char *QuantInChar);
int vpxt_dfwm_check(const char *InputFile, int printselect);
double vpxt_print_frame_statistics(const char *input_file1,
                                   const char *input_file2,
                                   const char *output_file,
                                   int force_uvswap,
                                   int print_out,
                                   int print_embl,
                                   int print_drop_frame,
                                   int print_resized_frame,
                                   int print_key_frame,
                                   int print_non_visible_frame,
                                   int print_frame_size);
int vpxt_eval_frame_stats_temp_scale(const char *input_file, int pattern);
int vpxt_check_min_quantizer(const char *inputFile, int MinQuantizer);
int vpxt_check_max_quantizer(const char *inputFile, int MaxQuantizer);
int vpxt_check_fixed_quantizer(const char *inputFile, int FixedQuantizer);
int vpxt_time_return(const char *infile, int FileType);
int vpxt_cpu_tick_return(const char *infile, int FileType);
int vpxt_check_force_key_frames(const char *KeyFrameoutputfile,
                                int ForceKeyFrameInt,
                                const char *ForceKeyFrame);
int vpxt_check_mem_state(const std::string FileName, std::string &bufferString);
int vpxt_print_compare_ivf_results(int lngRC, int printErr);
double vpxt_get_psnr(const char *compFileName);
//----------------------------------IVF Enc-------------------------------------
int API20Encoder(long width,
                 long height,
                 const char *infilechar,
                 const char *outfilechar);
int API20EncoderIVF2IVF(const char *inputFile,
                        const char *outputFile2,
                        int speed,
                        int BitRate,
                        VP8_CONFIG &oxcf,
                        const char *CompressString,
                        int CompressInt);
int API20Decoder(const char *inputchar, const char *outputchar);
int API20DecoderIVF2IVF(const char *inputchar, const char *outputchar);
#endif
