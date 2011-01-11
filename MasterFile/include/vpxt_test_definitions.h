#define RTFFINUM  0 //RunTestsFromFile
#define AlWDFNUM  1 //test_allow_drop_frames
#define ALWLGNUM  2 //test_allow_lag
#define ALWSRNUM  3 //test_allow_spatial_resampling
#define AUTKFNUM  4 //test_auto_key_frame
#define BUFLVNUM  5 //test_buffer_level
#define CPUDENUM  6 //test_change_cpu_dec
#define CPUENNUM  7 //test_change_cpu_enc
#define CONQUNUM  8 //test_constrained_quality
#define DTARTNUM  9 //test_data_rate
#define DBMRLNUM 10 //test_debug_matches_release
#define DFWMWNUM 11 //test_drop_frame_watermark
#define ENCBONUM 12 //test_encoder_break_out
#define ERRMWNUM 13 //test_error_resolution
#define EXTFINUM 14 //test_extra_file
#define FIXDQNUM 15 //test_fixed_quantizer
#define FKEFRNUM 16 //test_force_key_frame
#define FRSZTNUM 17 //test_frame_size
#define GQVBQNUM 18 //test_good_vs_best
#define LGIFRNUM 19 //test_lag_in_frames
#define MAXQUNUM 20 //test_max_quantizer
#define MEML1NUM 21 //test_mem_leak
#define MEML2NUM 22 //test_mem_leak2
#define MINQUNUM 23 //test_min_quantizer
#define MULTTNUM 24 //test_multithreaded
#define NVOECPTK 25 //test_new_vs_old_enc_cpu_tick
#define NVOPSNUM 26 //test_new_vs_old_psnr
#define NOISENUM 27 //test_noise_sensitivity
#define OV2PSNUM 28 //test_one_pass_vs_two_pass
#define PLYALNUM 29 //test_play_alternate
#define POSTPNUM 30 //test_post_processor
#define RECBFNUM 31 //test_reconstruct_buffer
#define RSDWMNUM 32 //test_resample_down_watermark
#define SPEEDNUM 33 //test_speed
#define TVECTNUM 34 //test_test_vector
#define TV2BTNUM 35 //test_two_pass_vs_two_pass_best
#define UNDSHNUM 36 //test_undershoot
#define VERSINUM 37 //test_version
#define WMLMMNUM 38 //test_win_lin_mac_match

#define MAXTENUM 38

/*Removed Tests
#define PREPRNUM 32 //PreProcessorWorks        //removed 07 15 2010 - Combined with NoiseSensitivity
#define ALWS2NUM  4 //AllowSpatialResampling2  //removed 07 15 2010 - Combined with AllowSpatialResampling
#define CHGCPNUM  8 //ChangeCPUDec             //removed 07 15 2010 - Combined with CPUDecOnlyWorks
#define ALTFRNUM  4 //AltFreqTest              //removed 08 12 2010 - AltFreq no longer accessable via API
*/