#ifndef TEST_NAME_LIST_ONCE
#define TEST_NAME_LIST_ONCE

enum TestNamesList {
kTestMultiRun = 0,               // vpxt_run_multi
kTestAllowDropFrames = 1,        // test_allow_drop_frames
kTestAllowLag = 2,               // test_allow_lag
kTestAllowSpatialResampling = 3, // test_allow_spatial_resampling
kTestArnr = 4,                   // test_arnr
kTestAutoKeyFrame = 5,           // test_auto_key_frame
kTestBufferLevel = 6,            // test_buffer_level
kTestChangeCpuDec = 7,            // test_change_cpu_dec
kTestChangeCpuEnc = 8,            // test_change_cpu_enc
kTestConstrainedQuality = 9,     // test_constrained_quality
kTestCopySetReference = 10,      // test_copy_set_reference
kTestDataRate = 11,              // test_data_rate
kTestDebugMatchesRelease = 12,   // test_debug_matches_release
kTestDropFrameWaterMark = 13,    // test_drop_frame_watermark
kTestEncoderBreakout = 14,       // test_encoder_break_out
kTestErrorConcealment = 15,      // test_error_concealment
kTestErrorResolution = 16,       // test_error_resolution
kTestExtraFile = 17,             // test_extra_file
kTestFixedQuantizer = 18,        // test_fixed_quantizer
kTestForcedKeyFrame = 19,        // test_force_key_frame
kTestFrameSize = 20,             // test_frame_size
kTestGoodVsBest = 21,            // test_good_vs_best
kTestLagInFrames = 22,           // test_lag_in_frames
kTestMaxQuantizer = 23,          // test_max_quantizer
kTestMemLeak = 24,               // test_mem_leak
kTestMemLeak2 = 25,              // test_mem_leak2
kTestMinQuantizer = 26,          // test_min_quantizer
kTestMultiResolutionEncode = 27, // test_multiple_resolution_encode
kTestMultiThreadedDec = 28,      // test_multithreaded_dec
kTestMultiThreadedEnc = 29,      // test_multithreaded_enc
kTestNewVsOldEncCpuTick = 30,    // test_new_vs_old_enc_cpu_tick
kTestNewVsOldPsnr = 31,          // test_new_vs_old_psnr
kTestNewVsOldTempScale = 32,     // test_new_vs_old_temp_scale
kTestNoiseSensitivity = 33,      // test_noise_sensitivity
kTestOnePassVsTwoPass = 34,      // test_one_pass_vs_two_pass
kTestPlayAlternate = 35,         // test_play_alternate
kTestPostProcessor = 36,         // test_post_processor
kTestPostProcessorMfqe = 37,     // test_post_processor_mfqe
kTestReconstructBuffer = 38,     // test_reconstruct_buffer
kTestResampleDownWatermark = 39, // test_resample_down_watermark
kTestSpeed = 40,                 // test_speed
kTestTemporalScalability = 41,   // test_temporal_scalability
kTestTestVector = 42,            // test_test_vector
kTestThirtytwoVsSixtyfour = 43,  // test_thirtytwo_vs_sixtyfour
kTestTwoPassVsTwoPassBest = 44,  // test_two_pass_vs_two_pass_best
kTestUndershoot = 45,            // test_undershoot
kTestVersion = 46,               // test_version
kTestVpxMatchesInt = 47,         // test_vpx_matches_int
kTestWinLinMacMatch = 48         // test_win_lin_mac_match
};
#endif

#define MAXTENUM 48
