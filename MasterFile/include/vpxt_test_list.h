#include <string>
using namespace std;

/////Tests
extern int run_multiple_tests(int argc, char *argv[], string WorkingDir,  int NumberofTests);
extern int test_allow_drop_frames(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_allow_lag(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_allow_spatial_resampling(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_auto_key_frame(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_buffer_level(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_change_cpu_dec(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_change_cpu_enc(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_drop_frame_watermark(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_data_rate(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_debug_matches_release(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_encoder_break_out(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_error_resolution(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_extra_file(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_fixed_quantizer(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_force_key_frame(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_frame_size(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_good_vs_best(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_lag_in_frames(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_max_quantizer(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_mem_leak(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_mem_leak2(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_min_quantizer(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_multithreaded(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_new_vs_old_psnr(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_new_vs_old_enc_cpu_tick(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_noise_sensitivity(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_one_pass_vs_two_pass(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_play_alternate(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_post_processor(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_reconstruct_buffer(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_resample_down_watermark(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_speed(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_vector_test(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_two_pass_vs_two_pass_best(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_undershoot(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_version(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int test_win_lin_mac_match(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);

//Tools
extern int tool_rand_comp(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
extern int tool_graph_psnr(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType);
