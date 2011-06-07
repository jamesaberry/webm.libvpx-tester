#define RTFFINUM  0 //RunTestsFromFile
#define AlWDFNUM  1 //test_allow_drop_frames
#define ALWLGNUM  2 //test_allow_lag
#define ALWSRNUM  3 //test_allow_spatial_resampling
#define ARNRTNUM  4 //test_arnr
#define AUTKFNUM  5 //test_auto_key_frame
#define BUFLVNUM  6 //test_buffer_level
#define CPUDENUM  7 //test_change_cpu_dec
#define CPUENNUM  8 //test_change_cpu_enc
#define CONQUNUM  9 //test_constrained_quality
#define COPSRNUM 10 //test_copy_set_reference
#define DTARTNUM 11 //test_data_rate
#define DBMRLNUM 12 //test_debug_matches_release
#define DFWMWNUM 13 //test_drop_frame_watermark
#define ENCBONUM 14 //test_encoder_break_out
#define ERRCONUM 15 //test_error_concealment
#define ERRMWNUM 16 //test_error_resolution
#define EXTFINUM 17 //test_extra_file
#define FIXDQNUM 18 //test_fixed_quantizer
#define FKEFRNUM 19 //test_force_key_frame
#define FRSZTNUM 20 //test_frame_size
#define GQVBQNUM 21 //test_good_vs_best
#define LGIFRNUM 22 //test_lag_in_frames
#define MAXQUNUM 23 //test_max_quantizer
#define MEML1NUM 24 //test_mem_leak
#define MEML2NUM 25 //test_mem_leak2
#define MINQUNUM 26 //test_min_quantizer
#define MULTDNUM 27 //test_multithreaded_dec
#define MULTENUM 28 //test_multithreaded_enc
#define NVOECPTK 29 //test_new_vs_old_enc_cpu_tick
#define NVOPSNUM 30 //test_new_vs_old_psnr
#define NOISENUM 31 //test_noise_sensitivity
#define OV2PSNUM 32 //test_one_pass_vs_two_pass
#define PLYALNUM 33 //test_play_alternate
#define POSTPNUM 34 //test_post_processor
#define RECBFNUM 35 //test_reconstruct_buffer
#define RSDWMNUM 36 //test_resample_down_watermark
#define SPEEDNUM 37 //test_speed
#define TVECTNUM 38 //test_test_vector
#define TTVSFNUM 39 //test_thirtytwo_vs_sixtyfour
#define TV2BTNUM 40 //test_two_pass_vs_two_pass_best
#define UNDSHNUM 41 //test_undershoot
#define VERSINUM 42 //test_version
#define VPXMINUM 43 //test_vpx_matches_int
#define WMLMMNUM 44 //test_win_lin_mac_match

#define MAXTENUM 44

/*Removed Tests
#define PREPRNUM 32 //PreProcessorWorks        //removed 07 15 2010 - Combined with NoiseSensitivity
#define ALWS2NUM  4 //AllowSpatialResampling2  //removed 07 15 2010 - Combined with AllowSpatialResampling
#define CHGCPNUM  8 //ChangeCPUDec             //removed 07 15 2010 - Combined with CPUDecOnlyWorks
#define ALTFRNUM  4 //AltFreqTest              //removed 08 12 2010 - AltFreq no longer accessable via API
*/