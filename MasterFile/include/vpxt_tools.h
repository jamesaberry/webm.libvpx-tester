
#ifndef TOOLS_H_GUARD
#define TOOLS_H_GUARD

int RawDataIVF(const char *input, const char *output);
int IVF2Raw(const char *inputFile, const char *outputDir);
int DecoderCheck(int argc, const char *const *argv);
int IVFParseandDelete(const char *DirName);
int TxtParseandCopy(const char *DirName, const char *BaseOutputDir, int BaseInputStrLength);
int tool_array_cov_fail_list_to_full_list(int argc, const char *const *argv);
int tool_array_cov_summary_file(int argc, const char *const *argv);
int tool_api_compress(int argc, const char *const *argv);
int tool_api_decompress(int argc, const char *const *argv);
int tool_combine_indv_frames(int argc, const char *const *argv);
int tool_compare_ivf(int argc, const char *const *argv);
int tool_compare_code_coverage(int argc, const char *const *argv);
int tool_compression_equiv(int argc, const char *const *argv, std::string WorkingDir);
int tool_compr_ivf_to_ivf(int argc, const char *const *argv, std::string WorkingDir);
int tool_comp_matches_ivfenc(int argc, const char *const *argv);
int tool_convert_par_file_to_ivfenc(int argc, const char *const *argv);
int tool_convert_par_file_to_vpxenc(int argc, const char *const *argv);
int tool_create_rand_par_file(int argc, const char *const *argv);
int tool_crop_raw_ivf(int argc, const char *const *argv);
int tool_pad_raw_ivf(int argc, const char *const *argv);
int tool_copy_all_txt_files(int argc, const char *const *argv);
int tool_cut_ivf(int argc, const char *const *argv);
int tool_dec_ivf_to_ivf(int argc, const char *const *argv);
int tool_dec_ivf_to_raw(int argc, const char *const *argv);
int tool_delete_all_ivf_files(int argc, const char *const *argv);
int tool_disp_alt_ref_frames(int argc, const char *const *argv);
int tool_disp_droped_frames(int argc, const char *const *argv);
int tool_disp_frame_data(int argc, const char *const *argv);
int tool_disp_key_frames(int argc, const char *const *argv);
int tool_disp_resized_frames(int argc, const char *const *argv);
int tool_disp_visible_frames(int argc, const char *const *argv);
int tool_format_code_coverage_file(int argc, const char *const *argv);
int tool_format_summary(int argc, const char *const *argv);
int tool_ivf_check_pbm_run(int argc, const char *const *argv);
int tool_ivf_data_rate(int argc, const char *const *argv);
int tool_ivf_dec_test_vector_check(int argc, const char *const *argv);
int tool_ivf_psnr_run(int argc, const char *const *argv);
int tool_ivf_to_raw(int argc, const char *const *argv);
int tool_ivf_to_raw_frames(int argc, const char *const *argv);
int tool_paste_ivf(int argc, const char *const *argv);
int tool_play_comp_ivf(int argc, const char *const *argv);
int tool_play_dec_ivf(int argc, const char *const *argv);
int tool_print_cpu_info();
int tool_random_multi_test(int argc, const char *const *argv);
int tool_raw_to_ivf(int argc, const char *const *argv);
int tool_run_ivfdec(int argc, const char *const *argv);
int tool_run_ivfenc(int argc, const char *const *argv);
int tool_run_thresh(int argc, const char *const *argv);
int tool_side_by_side_text(int argc, const char *const *argv);
int tool_solve_quad();
int tool_test_print();
int tool_test_vector_index(int argc, const char *const *argv);
int tool_win_mem_mon_format(int argc, const char *const *argv);
int tool_win_mem_mon_graph(int argc, const char *const *argv);
#endif