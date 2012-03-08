$(OBJDIR)/driver.o: $(SrcDir)/driver.cpp
	-mkdir $(OBJDIR)
	$(GPP) -c $(SrcDir)/driver.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/driver.o

$(OBJDIR)/psnr.o: $(PsnrDir)/psnr.cpp
	$(GPP) -c $(PsnrDir)/psnr.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/psnr.o

$(OBJDIR)/ssim.o: $(PsnrDir)/ssim.cpp
	$(GPP) -c $(PsnrDir)/ssim.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/ssim.o

$(OBJDIR)/test_allow_drop_frames.o: $(TestsDir)/test_allow_drop_frames.cpp
	$(GPP) -c $(TestsDir)/test_allow_drop_frames.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_allow_drop_frames.o

$(OBJDIR)/test_allow_lag.o: $(TestsDir)/test_allow_lag.cpp
	$(GPP) -c $(TestsDir)/test_allow_lag.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_allow_lag.o

$(OBJDIR)/test_allow_spatial_resampling.o: $(TestsDir)/test_allow_spatial_resampling.cpp
	$(GPP) -c $(TestsDir)/test_allow_spatial_resampling.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_allow_spatial_resampling.o

$(OBJDIR)/test_arnr.o: $(TestsDir)/test_arnr.cpp
	$(GPP) -c $(TestsDir)/test_arnr.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_arnr.o

$(OBJDIR)/test_auto_key_frame.o: $(TestsDir)/test_auto_key_frame.cpp
	$(GPP) -c $(TestsDir)/test_auto_key_frame.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_auto_key_frame.o

$(OBJDIR)/test_buffer_level.o: $(TestsDir)/test_buffer_level.cpp
	$(GPP) -c $(TestsDir)/test_buffer_level.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_buffer_level.o

$(OBJDIR)/test_change_cpu_dec.o: $(TestsDir)/test_change_cpu_dec.cpp
	$(GPP) -c $(TestsDir)/test_change_cpu_dec.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_change_cpu_dec.o

$(OBJDIR)/test_change_cpu_enc.o: $(TestsDir)/test_change_cpu_enc.cpp
	$(GPP) -c $(TestsDir)/test_change_cpu_enc.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_change_cpu_enc.o

$(OBJDIR)/test_constrained_quality.o: $(TestsDir)/test_constrained_quality.cpp
	$(GPP) -c $(TestsDir)/test_constrained_quality.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_constrained_quality.o

$(OBJDIR)/test_copy_set_reference.o: $(TestsDir)/test_copy_set_reference.cpp
	$(GPP) -c $(TestsDir)/test_copy_set_reference.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_copy_set_reference.o

$(OBJDIR)/test_data_rate.o: $(TestsDir)/test_data_rate.cpp
	$(GPP) -c $(TestsDir)/test_data_rate.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_data_rate.o

$(OBJDIR)/test_debug_matches_release.o: $(TestsDir)/test_debug_matches_release.cpp
	$(GPP) -c $(TestsDir)/test_debug_matches_release.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_debug_matches_release.o

$(OBJDIR)/test_drop_frame_watermark.o: $(TestsDir)/test_drop_frame_watermark.cpp
	$(GPP) -c $(TestsDir)/test_drop_frame_watermark.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_drop_frame_watermark.o

$(OBJDIR)/test_encoder_break_out.o: $(TestsDir)/test_encoder_break_out.cpp
	$(GPP) -c $(TestsDir)/test_encoder_break_out.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_encoder_break_out.o

$(OBJDIR)/test_error_concealment.o: $(TestsDir)/test_error_concealment.cpp
	$(GPP) -c $(TestsDir)/test_error_concealment.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_error_concealment.o

$(OBJDIR)/test_error_resolution.o: $(TestsDir)/test_error_resolution.cpp
	$(GPP) -c $(TestsDir)/test_error_resolution.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_error_resolution.o

$(OBJDIR)/test_extra_file.o: $(TestsDir)/test_extra_file.cpp
	$(GPP) -c $(TestsDir)/test_extra_file.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_extra_file.o

$(OBJDIR)/test_fixed_quantizer.o: $(TestsDir)/test_fixed_quantizer.cpp
	$(GPP) -c $(TestsDir)/test_fixed_quantizer.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_fixed_quantizer.o

$(OBJDIR)/test_force_key_frame.o: $(TestsDir)/test_force_key_frame.cpp
	$(GPP) -c $(TestsDir)/test_force_key_frame.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_force_key_frame.o

$(OBJDIR)/test_frame_size.o: $(TestsDir)/test_frame_size.cpp
	$(GPP) -c $(TestsDir)/test_frame_size.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_frame_size.o

$(OBJDIR)/test_good_vs_best.o: $(TestsDir)/test_good_vs_best.cpp
	$(GPP) -c $(TestsDir)/test_good_vs_best.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_good_vs_best.o

$(OBJDIR)/test_lag_in_frames.o: $(TestsDir)/test_lag_in_frames.cpp
	$(GPP) -c $(TestsDir)/test_lag_in_frames.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_lag_in_frames.o

$(OBJDIR)/test_max_quantizer.o: $(TestsDir)/test_max_quantizer.cpp
	$(GPP) -c $(TestsDir)/test_max_quantizer.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_max_quantizer.o

$(OBJDIR)/test_mem_leak.o: $(TestsDir)/test_mem_leak.cpp
	$(GPP) -c $(TestsDir)/test_mem_leak.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_mem_leak.o

$(OBJDIR)/test_mem_leak2.o: $(TestsDir)/test_mem_leak2.cpp
	$(GPP) -c $(TestsDir)/test_mem_leak2.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_mem_leak2.o

$(OBJDIR)/test_min_quantizer.o: $(TestsDir)/test_min_quantizer.cpp
	$(GPP) -c $(TestsDir)/test_min_quantizer.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_min_quantizer.o

$(OBJDIR)/test_multiple_resolution_encode.o: $(TestsDir)/test_multiple_resolution_encode.cpp
	$(GPP) -c $(TestsDir)/test_multiple_resolution_encode.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_multiple_resolution_encode.o

$(OBJDIR)/test_multithreaded_enc.o: $(TestsDir)/test_multithreaded_enc.cpp
	$(GPP) -c $(TestsDir)/test_multithreaded_enc.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_multithreaded_enc.o

$(OBJDIR)/test_multithreaded_dec.o: $(TestsDir)/test_multithreaded_dec.cpp
	$(GPP) -c $(TestsDir)/test_multithreaded_dec.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_multithreaded_dec.o

$(OBJDIR)/test_new_vs_old_psnr.o: $(TestsDir)/test_new_vs_old_psnr.cpp
	$(GPP) -c $(TestsDir)/test_new_vs_old_psnr.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_new_vs_old_psnr.o

$(OBJDIR)/test_new_vs_old_enc_cpu_tick.o: $(TestsDir)/test_new_vs_old_enc_cpu_tick.cpp
	$(GPP) -c $(TestsDir)/test_new_vs_old_enc_cpu_tick.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_new_vs_old_enc_cpu_tick.o

$(OBJDIR)/test_noise_sensitivity.o: $(TestsDir)/test_noise_sensitivity.cpp
	$(GPP) -c $(TestsDir)/test_noise_sensitivity.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_noise_sensitivity.o

$(OBJDIR)/test_one_pass_vs_two_pass.o: $(TestsDir)/test_one_pass_vs_two_pass.cpp
	$(GPP) -c $(TestsDir)/test_one_pass_vs_two_pass.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_one_pass_vs_two_pass.o

$(OBJDIR)/test_play_alternate.o: $(TestsDir)/test_play_alternate.cpp
	$(GPP) -c $(TestsDir)/test_play_alternate.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_play_alternate.o

$(OBJDIR)/test_post_processor.o: $(TestsDir)/test_post_processor.cpp
	$(GPP) -c $(TestsDir)/test_post_processor.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_post_processor.o

$(OBJDIR)/test_reconstruct_buffer.o: $(TestsDir)/test_reconstruct_buffer.cpp
	$(GPP) -c $(TestsDir)/test_reconstruct_buffer.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_reconstruct_buffer.o

$(OBJDIR)/test_resample_down_watermark.o: $(TestsDir)/test_resample_down_watermark.cpp
	$(GPP) -c $(TestsDir)/test_resample_down_watermark.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_resample_down_watermark.o

$(OBJDIR)/test_temporal_scalability.o: $(TestsDir)/test_temporal_scalability.cpp
	$(GPP) -c $(TestsDir)/test_temporal_scalability.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_temporal_scalability.o

$(OBJDIR)/test_speed.o: $(TestsDir)/test_speed.cpp
	$(GPP) -c $(TestsDir)/test_speed.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_speed.o

$(OBJDIR)/test_test_vector.o: $(TestsDir)/test_test_vector.cpp
	$(GPP) -c $(TestsDir)/test_test_vector.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_test_vector.o

$(OBJDIR)/test_thirtytwo_vs_sixtyfour.o: $(TestsDir)/test_thirtytwo_vs_sixtyfour.cpp
	$(GPP) -c $(TestsDir)/test_thirtytwo_vs_sixtyfour.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_thirtytwo_vs_sixtyfour.o

$(OBJDIR)/test_two_pass_vs_two_pass_best.o: $(TestsDir)/test_two_pass_vs_two_pass_best.cpp
	$(GPP) -c $(TestsDir)/test_two_pass_vs_two_pass_best.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_two_pass_vs_two_pass_best.o

$(OBJDIR)/test_undershoot.o: $(TestsDir)/test_undershoot.cpp
	$(GPP) -c $(TestsDir)/test_undershoot.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_undershoot.o

$(OBJDIR)/test_version.o: $(TestsDir)/test_version.cpp
	$(GPP) -c $(TestsDir)/test_version.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_version.o

$(OBJDIR)/test_vpx_matches_int.o: $(TestsDir)/test_vpx_matches_int.cpp
	$(GPP) -c $(TestsDir)/test_vpx_matches_int.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_vpx_matches_int.o

$(OBJDIR)/test_win_lin_mac_match.o: $(TestsDir)/test_win_lin_mac_match.cpp
	$(GPP) -c $(TestsDir)/test_win_lin_mac_match.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/test_win_lin_mac_match.o

$(OBJDIR)/run_multiple_tests.o: $(ToolDir)/run_multiple_tests.cpp
	$(GPP) -c $(ToolDir)/run_multiple_tests.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/run_multiple_tests.o

$(OBJDIR)/rand_comp.o: $(ToolDir)/rand_comp.cpp
	$(GPP) -c $(ToolDir)/rand_comp.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/rand_comp.o

$(OBJDIR)/graph_psnr.o: $(ToolDir)/graph_psnr.cpp
	$(GPP) -c $(ToolDir)/graph_psnr.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/graph_psnr.o

$(OBJDIR)/code_coverage.o: $(ToolDir)/code_coverage.cpp
	$(GPP) -c $(ToolDir)/code_coverage.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/code_coverage.o

$(OBJDIR)/tools.o: $(ToolDir)/tools.cpp
	$(GPP) -c $(ToolDir)/tools.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/tools.o

$(OBJDIR)/md5_utils.o: $(IvfDir)/md5_utils.cpp
	$(GPP) -c $(IvfDir)/md5_utils.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/md5_utils.o

$(OBJDIR)/utilities.o: $(UtlDir)/utilities.cpp
	$(GPP) -c  $(UtlDir)/utilities.cpp $(INCLUDE)  $(LFLAGS) -o $(OBJDIR)/utilities.o

$(OBJDIR)/args.o: $(IvfDir)/args.cpp
	$(GCC) -c $(IvfDir)/args.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/args.o

$(OBJDIR)/cpu_id.o: $(IvfDir)/cpu_id.cpp
	$(GPP) -c $(IvfDir)/cpu_id.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/cpu_id.o

$(OBJDIR)/EbmlWriter.o: $(IvfDir)/EbmlWriter.cpp
	$(GPP) -c $(IvfDir)/EbmlWriter.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/EbmlWriter.o

$(OBJDIR)/halloc.o: $(IvfDir)/halloc.c
	$(GCC) -c $(IvfDir)/halloc.c $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/halloc.o

$(OBJDIR)/ivf.o: $(IvfDir)/ivf.cpp
	$(GCC) -c $(IvfDir)/ivf.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/ivf.o

$(OBJDIR)/nestegg.o: $(IvfDir)/nestegg.c
	$(GCC) -c $(IvfDir)/nestegg.c $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/nestegg.o

$(OBJDIR)/scale.o: $(IvfDir)/scale.cpp
	$(GPP) -c $(IvfDir)/scale.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/scale.o

$(OBJDIR)/tools_common.o: $(IvfDir)/tools_common.cpp
	$(GPP) -c $(IvfDir)/tools_common.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/tools_common.o

$(OBJDIR)/vp8_scalable_patterns.o: $(IvfDir)/vp8_scalable_patterns.cpp
	$(GPP) -c $(IvfDir)/vp8_scalable_patterns.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/vp8_scalable_patterns.o

$(OBJDIR)/vp8_multi_resolution_encoder.o: $(IvfDir)/vp8_multi_resolution_encoder.cpp
	$(GPP) -c $(IvfDir)/vp8_multi_resolution_encoder.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/vp8_multi_resolution_encoder.o

$(OBJDIR)/vpxdec.o: $(IvfDir)/vpxdec.cpp
	$(GPP) -c $(IvfDir)/vpxdec.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/vpxdec.o

$(OBJDIR)/vpxenc.o: $(IvfDir)/vpxenc.cpp
	$(GPP) -c $(IvfDir)/vpxenc.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/vpxenc.o

$(OBJDIR)/y4minput.o: $(IvfDir)/y4minput.cpp
	$(GPP) -c $(IvfDir)/y4minput.cpp $(INCLUDE) $(LFLAGS) -o $(OBJDIR)/y4minput.o
