#include "vpxt_test_declarations.h"
#include "vpxt_test_list.h"
#include "vpxt_driver.h"

int run_multiple_tests(int argc,
                       const char** argv,
                       std::string working_dir,
                       int number_of_tests,
                       int delete_ivf_files,
                       int artifact_detection)
{
    if (argc < 4)
        return vpxt_test_help(argv[1], 0);

    if (working_dir.length() > 175)
    {
        tprintf(PRINT_BTH, "\nError: Folder Path Length too long.\n");
        return kTestFailed;
    }

    char buffer[512] = "";
    char buffer2[512] = "";

    int *pass_fail_arr = new int[number_of_tests+2];

    std::string summ_comp_and_test;
    std::string summ_comp;
    std::string summ_test;

    char input[256] = "";
    strncpy(input, argv[3], 256);

    std::string string_arr[20];
    std::string time_stamp_arr_2[2];
    std::string selector_arr[999];
    std::string selector_arr_2[999];

    ////////////////////// Record time it took to run test /////////////////////
    int record_run_times = 0; // If set to one will record run times of tests
    unsigned int run_time_1 = 0;
    unsigned int run_time_2 = 0;
    unsigned long run_time_rec_arr[999];
    ////////////////////////////////////////////////////////////////////////////

    int y = 0;
    int buf_1_var = 0;
    int test_run = 0;
    int print_me = 1;
    int dummy_arg_var = 1;
    int pass_fail_int = 0;
    int selector_arr_int = 0;
    int tests_run_catch = 0;
    int make_new_test_run = 1;
    int identical_file_cnt = 1;
    int number_of_tests_run = 1;
    int test_type = atoi(argv[2]);
    int lines_skipped_cnt = 0;

    long file_pos_track = 0;
    long file_pos_track_2 = 0;

    std::string prev_time_stamp;

    std::string work_dir_file_str;
    std::string working_text_file_str;
    char working_dir_2[255] = "";
    char working_dir_3[255] = "";
    char *test_dir = "Summary";

    int (*vpxt_test_funct_ptr[MAXTENUM+1])(int, const char** argv,
        const std::string &, const std::string, int, int, int) = {NULL};
    vpxt_test_funct_ptr[kTestAllowDropFrames] = &test_allow_drop_frames;
    vpxt_test_funct_ptr[kTestAllowLag] = &test_allow_lag;
    vpxt_test_funct_ptr[kTestAllowSpatialResampling] = &test_allow_spatial_resampling;
    vpxt_test_funct_ptr[kTestArnr] = &test_arnr;
    vpxt_test_funct_ptr[kTestAutoKeyFrame] = &test_auto_key_frame;
    vpxt_test_funct_ptr[kTestBufferLevel] = &test_buffer_level;
    vpxt_test_funct_ptr[kTestChangeCpuDec] = &test_change_cpu_dec;
    vpxt_test_funct_ptr[kTestChangeCpuEnc] = &test_change_cpu_enc;
    vpxt_test_funct_ptr[kTestConstrainedQuality] = &test_constrained_quality;
    vpxt_test_funct_ptr[kTestCopySetReference] = &test_copy_set_reference;
    vpxt_test_funct_ptr[kTestDataRate] = &test_data_rate;
    vpxt_test_funct_ptr[kTestDebugMatchesRelease] = &test_debug_matches_release;
    vpxt_test_funct_ptr[kTestDropFrameWaterMark] = &test_drop_frame_watermark;
    vpxt_test_funct_ptr[kTestEncoderBreakout] = &test_encoder_break_out;
    vpxt_test_funct_ptr[kTestErrorConcealment] = &test_error_concealment;
    vpxt_test_funct_ptr[kTestErrorResolution] = &test_error_resolution;
    vpxt_test_funct_ptr[kTestExtraFile] = &test_extra_file;
    vpxt_test_funct_ptr[kTestFixedQuantizer] = &test_fixed_quantizer;
    vpxt_test_funct_ptr[kTestForcedKeyFrame] = &test_force_key_frame;
    vpxt_test_funct_ptr[kTestFrameSize] = &test_frame_size;
    vpxt_test_funct_ptr[kTestGoodVsBest] = &test_good_vs_best;
    vpxt_test_funct_ptr[kTestLagInFrames] = &test_lag_in_frames;
    vpxt_test_funct_ptr[kTestMaxQuantizer] = &test_max_quantizer;
    vpxt_test_funct_ptr[kTestMemLeak] = &test_mem_leak;
    vpxt_test_funct_ptr[kTestMemLeak2] = &test_mem_leak2;
    vpxt_test_funct_ptr[kTestMinQuantizer] = &test_min_quantizer;
    vpxt_test_funct_ptr[kTestMultiResolutionEncode] = &test_multiple_resolution_encode;
    vpxt_test_funct_ptr[kTestMultiThreadedDec] = &test_multithreaded_dec;
    vpxt_test_funct_ptr[kTestMultiThreadedEnc] = &test_multithreaded_enc;
    vpxt_test_funct_ptr[kTestNewVsOldEncCpuTick] = &test_new_vs_old_enc_cpu_tick;
    vpxt_test_funct_ptr[kTestNewVsOldPsnr] = &test_new_vs_old_psnr;
    vpxt_test_funct_ptr[kTestNewVsOldTempScale] = &test_new_vs_old_temp_scale;
    vpxt_test_funct_ptr[kTestNoiseSensitivity] = &test_noise_sensitivity;
    vpxt_test_funct_ptr[kTestOnePassVsTwoPass] = &test_one_pass_vs_two_pass;
    vpxt_test_funct_ptr[kTestPlayAlternate] = &test_play_alternate;
    vpxt_test_funct_ptr[kTestPostProcessor] = &test_post_processor;
    vpxt_test_funct_ptr[kTestPostProcessorMfqe] = &test_post_processor_mfqe;
    vpxt_test_funct_ptr[kTestReconstructBuffer] = &test_reconstruct_buffer;
    vpxt_test_funct_ptr[kTestResampleDownWatermark] = &test_resample_down_watermark;
    vpxt_test_funct_ptr[kTestSpeed] = &test_speed;
    vpxt_test_funct_ptr[kTestTemporalScalability] = &test_temporal_scalability;
    vpxt_test_funct_ptr[kTestTestVector] = &test_test_vector;
    vpxt_test_funct_ptr[kTestThirtytwoVsSixtyfour] = &test_thirtytwo_vs_sixtyfour;
    vpxt_test_funct_ptr[kTestTwoPassVsTwoPassBest] = &test_two_pass_vs_two_pass_best;
    vpxt_test_funct_ptr[kTestUndershoot] = &test_undershoot;
    vpxt_test_funct_ptr[kTestVersion] = &test_version;
    vpxt_test_funct_ptr[kTestVpxMatchesInt] = &test_vpx_matches_int;
    vpxt_test_funct_ptr[kTestWinLinMacMatch] = &test_win_lin_mac_match;

    char* vpxt_test_name_list[MAXTENUM+1]={
        "RunTestsFromFile",
        "test_allow_drop_frames",
        "test_allow_lag",
        "test_allow_spatial_resampling",
        "test_arnr",
        "test_auto_key_frame",
        "test_buffer_level",
        "test_change_cpu_dec",
        "test_change_cpu_enc",
        "test_constrained_quality",
        "test_copy_set_reference",
        "test_data_rate",
        "test_debug_matches_release",
        "test_drop_frame_watermark",
        "test_encoder_break_out",
        "test_error_concealment",
        "test_error_resolution",
        "test_extra_file",
        "test_fixed_quantizer",
        "test_force_key_frame",
        "test_frame_size",
        "test_good_vs_best",
        "test_lag_in_frames",
        "test_max_quantizer",
        "test_mem_leak",
        "test_mem_leak2",
        "test_min_quantizer",
        "test_multiple_resolution_encode",
        "test_multithreaded_dec",
        "test_multithreaded_enc",
        "test_new_vs_old_enc_cpu_tick",
        "test_new_vs_old_psnr",
        "test_new_vs_old_temp_scale",
        "test_noise_sensitivity",
        "test_one_pass_vs_two_pass",
        "test_play_alternate",
        "test_post_processor",
        "test_post_processor_mfqe",
        "test_reconstruct_buffer",
        "test_resample_down_watermark",
        "test_speed",
        "test_temporal_scalability",
        "test_test_vector",
        "test_thirtytwo_vs_sixtyfour",
        "test_two_pass_vs_two_pass_best",
        "test_undershoot",
        "test_version",
        "test_vpx_matches_int",
        "test_win_lin_mac_match"};

        //////////////////// Sets Stage for Resume Mode //////////////////////
        if (test_type == kResumeTest)
        {
            // Read in whats been done already from the summary updating all
            // variables needed start running the test from TestRuns.txt file
            // since it is auto commented it will resume with the last test not
            // completed So long as all arrays are updated properly and
            // variables are as well it should work seemlessly.

            // location of main dir in as workingdir
            working_dir = argv[3];

            // test_run.txt original file ided
            working_text_file_str = working_dir + slashCharStr() +
                "tests_run.txt";

            // Determines which test is in the process of being run and sets the
            // correct text file as input file
            summ_comp_and_test = working_dir + slashCharStr() +
                "test_results.txt"; // Mode 1
            summ_comp = working_dir + slashCharStr() +
                "compression_results.txt"; // Mode 2
            summ_test = working_dir + slashCharStr() +
                "compression_test_results.txt"; // Mode 3

            int test_running = kNoTest;

            std::fstream sum_comp_and_test_file;
            sum_comp_and_test_file.open(summ_comp_and_test.c_str(),
                std::fstream::in);

            if (sum_comp_and_test_file.good())
                test_running = kFullTest;

            std::fstream sum_test_file;
            sum_test_file.open(summ_test.c_str());

            if (sum_test_file.good())
            {
                test_running = kTestOnly;
            }

            std::fstream sum_comp_file;
            sum_comp_file.open(summ_comp.c_str());

            if (sum_comp_file.good())
                if (test_running != kTestOnly)
                    test_running = kCompOnly;

            sum_comp_and_test_file.close();
            sum_test_file.close();
            sum_comp_file.close();

            // exits with error if it cant find a summary file
            if (test_running == kNoTest)
            {
                tprintf(PRINT_STD, "\nError: No Summary File found\n");
                delete [] pass_fail_arr;
                return kTestFailed;
            }

            /// Sets Correct input file
            std::string prior_result_in;

            if (test_running == kFullTest)
                prior_result_in = summ_comp_and_test;

            if (test_running == kCompOnly)
                prior_result_in =  summ_comp;

            if (test_running == kTestOnly)
                prior_result_in = summ_test;

            std::fstream prior_result_input_file;
            prior_result_input_file.open(prior_result_in.c_str());

            int tests_run_cnt = 0;
            int var_to_input = 0;
            int header_line = 0;

            // read in and throw away header
            while (header_line < 7)
            {
                prior_result_input_file.getline(buffer, 1024);
                header_line++;
            }

            // read in data and parse values
            while (!prior_result_input_file.eof())
            {
                prior_result_input_file.getline(buffer, 1024);
                std::string bufferStr = buffer;
                unsigned int u = 0;

                // 0 number not read yet|1 test_type not read yet|2 TestFolder
                // not read yet|3 Status not read yet
                while (buffer[u] != '\0' && buffer[u] != '\r' && u
                    <= bufferStr.length())
                {
                    if (buffer[u] != ' ')
                    {
                        if (var_to_input == 5)
                            var_to_input = 0;

                        if (var_to_input == 3)
                        {
                            char buffer3[1024];

                            int a = 0;

                            while (buffer[u] != '\0' && buffer[u] != '\r')
                            {
                                buffer3[a] = buffer[u];
                                u++;
                                a++;

                            }

                            buffer[u] = '\0';
                            std::string pass_fail_str = buffer3;

                            if (pass_fail_str.compare("Passed") == 0)
                                pass_fail_arr[tests_run_cnt] = kTestPassed;

                            if (pass_fail_str.compare("Failed") == 0)
                                pass_fail_arr[tests_run_cnt] = kTestFailed;

                            if (pass_fail_str.compare("Indeterminate") == 0)
                                pass_fail_arr[tests_run_cnt] = kTestIndeterminate;

                            if (pass_fail_str.compare("SeeComboRunLog") == 0)
                                pass_fail_arr[tests_run_cnt] = kTestComboLog;

                            if (pass_fail_str.compare("SeePSNRLog") == 0)
                                pass_fail_arr[tests_run_cnt] = kTestPsnrLog;

                            if (pass_fail_str.compare("RandomTestCompleted") ==
                                0)
                                pass_fail_arr[tests_run_cnt] = kTestRandomComplete;

                            if (pass_fail_str.compare("MinTestPassed") == 0)
                                pass_fail_arr[tests_run_cnt] = kTestMinPassed;

                            if (pass_fail_str.compare("CompressionMade") == 0)
                                pass_fail_arr[tests_run_cnt] = kTestEncCreated;

                            if (pass_fail_str.compare("ErrorFileMismatch") == 0)
                                pass_fail_arr[tests_run_cnt] = kTestErrFileMismatch;

                            if (pass_fail_str.compare("TestNotSupported") == 0)
                                pass_fail_arr[tests_run_cnt] = kTestNotSupported;

                            var_to_input = 4;
                            memset(buffer3, 0, sizeof(buffer3));
                        }

                        if (var_to_input == 2)
                        {
                            char buffer3[1024];

                            int a = 0;

                            while (a < 24)
                            {
                                buffer3[a] = buffer[u];
                                u++;
                                a++;
                            }

                            if (buffer[u] == '_')
                            {
                                buffer3[a] = buffer[u];
                                u++;
                                a++;
                                buffer3[a] = buffer[u];
                            }

                            selector_arr_2[tests_run_cnt] = buffer3;
                            selector_arr_2[tests_run_cnt] += "\"";

                            var_to_input = 3;
                            memset(buffer3, 0, sizeof(buffer3));
                        }

                        if (var_to_input == 1)
                        {
                            char buffer3[1024];

                            int a = 0;

                            while (buffer[u] != ' ')
                            {
                                buffer3[a] = buffer[u];
                                u++;
                                a++;
                            }

                            selector_arr[tests_run_cnt] = buffer3;
                            var_to_input = 2;
                            memset(buffer3, 0, sizeof(buffer3));
                        }

                        if (var_to_input == 0)
                        {
                            char buffer3[1024];

                            int a = 0;

                            while (buffer[u] != ' ')
                            {
                                buffer3[a] = buffer[u];
                                u++;
                                a++;
                            }

                            var_to_input = 1;
                            memset(buffer3, 0, sizeof(buffer3));
                        }

                        // resets var at end
                        if (var_to_input == 4)
                        {
                            int a = 0;

                            while (buffer[u] != '\0' && buffer[u] != '\r')
                            {
                                u++;
                                a++;
                            }

                            var_to_input = 0;
                        }
                    }

                    u++;
                }

                if (buffer[0] != '\0' && buffer[u] != '\r')
                {
                    // if the first character of a line after the initial input
                    // is read happens to be a dash it signifies the end of
                    // Tests being run and the start of the summary, meaning
                    // that all operations have already been run and the tests
                    // should teminate.
                    if (buffer[0] == '-')
                    {
                        if (test_running == kFullTest)
                        {
                            tprintf(PRINT_STD, "\nAll tests and compressions "
                                "have already been created and run.\n");
                            prior_result_input_file.close();
                            exit(0);
                        }

                        if (test_running == kCompOnly)
                        {
                            std::cout << "\nAll compressions have already been "
                                "created.\n";
                            prior_result_input_file.close();
                            exit(0);
                        }

                        if (test_running == kTestOnly)
                        {
                            std::cout << "\nAll tests on compressions have "
                                "already been run.\n";
                            prior_result_input_file.close();
                            exit(0);
                        }
                    }

                    tests_run_cnt++;
                }

            }

            test_type = test_running;
            selector_arr_int = tests_run_cnt;
            number_of_tests_run = tests_run_cnt;
            pass_fail_int = tests_run_cnt;
            test_run = tests_run_cnt;
            tests_run_catch = tests_run_cnt;

            if (test_type == kTestOnly)
                number_of_tests_run++;

            prior_result_input_file.close();
            working_dir += "\"";
            print_me = 0;
            make_new_test_run = 0;
        }

        ////////////////////////////////////////////////////////////////////
        //////////////////////////// Format Output ///////////////////////////


        if (test_type == kFullTest)
        {
            snprintf(working_dir_2, 255, "%s", working_dir.c_str());
            int v = 0;

            while (working_dir_2[v] != '\"')
            {
                working_dir_3[v] = working_dir_2[v];
                v++;
            }

            working_dir_3[v] = slashChar();
            working_dir_3[v+1] = '\0';
            work_dir_file_str = working_dir_3;
            work_dir_file_str += "test_results.txt";
        }

        if (test_type == kCompOnly)
        {
            snprintf(working_dir_2, 255, "%s", working_dir.c_str());
            int v = 0;

            while (working_dir_2[v] != '\"')
            {
                working_dir_3[v] = working_dir_2[v];
                v++;
            }

            working_dir_3[v] = slashChar();
            working_dir_3[v+1] = '\0';
            work_dir_file_str = working_dir_3;
            work_dir_file_str += "compression_results.txt";
        }

        if (test_type == kTestOnly)
        {
            work_dir_file_str = argv[3] + slashCharStr() +
                "compression_test_results.txt";
        }

        if (test_type != kResumeTest)
        {
            ////////////Formatting New Input TextFile///////////
            snprintf(working_dir_2, 255, "%s", working_dir.c_str());
            int v = 0;

            while (working_dir_2[v] != '\"')
            {
                working_dir_3[v] = working_dir_2[v];
                v++;
            }

            working_dir_3[v] = slashChar();
            working_dir_3[v+1] = '\0';
            working_text_file_str = working_dir_3;
            working_text_file_str += "tests_run.txt";
            ///////////////////////////////////////////////////

        }

        ////////////////////////////////////////////////////////////////////
        ///////////////// Makes New Working Text File From Input /////////////
        if (test_type != kTestOnly && make_new_test_run == 1)
        {
            // This copies original driving text file over to a new formated one
            // not used for Mode 4 or 3
            std::fstream infile;
            infile.open(input);

            if (!infile.good())
            {
                tprintf(PRINT_STD, "\nInput File does not exist\n");
                delete [] pass_fail_arr;
                return kTestFailed;
            }

            std::string create_dir_2(working_dir);
            create_dir_2.insert(0, "md \"");
            create_dir_2.erase(create_dir_2.end() - 1);
            vpxt_make_dir_vpx(create_dir_2);

            std::ofstream outfile(working_text_file_str.c_str());

            while (!infile.eof())
            {
                infile.getline(buffer, 512);

                if (buffer[0] == '%' || buffer[0] == '\0' || buffer[0] == '+' ||
                    buffer[0] == '\r')
                    outfile << buffer << "\n";
                else
                    if (buffer[0] != ' ')
                        outfile << " " << buffer << "\n";
                    else
                        outfile << buffer << "\n";
            }

            infile.close();
            outfile.close();
        }

        if (test_type == kTestOnly)
        {
            // location of main dir in as workingdir
            working_dir = argv[3];
            // test_run.txt original file ided
            working_text_file_str = working_dir + slashCharStr() =
                "tests_run.txt";
            working_dir += "\"";

            // Need to go through and replace all '+' from input file with '-'
            //  so that test can keep track of "Tests Only" run
            std::fstream working_text_file_prep;
            working_text_file_prep.open(working_text_file_str.c_str());

            while (!working_text_file_prep.eof())
            {
                file_pos_track = working_text_file_prep.tellg();
                working_text_file_prep.getline(buffer, 1024);
                file_pos_track_2 = working_text_file_prep.tellg();

                std::string bufferstring = buffer;
                bufferstring.erase(0);

                if (buffer[0] == '+')
                {
                    bufferstring.insert(0, " ");
                    working_text_file_prep.seekg(file_pos_track);
                    working_text_file_prep << bufferstring.c_str() ;
                    working_text_file_prep.seekg(file_pos_track_2);
                }
            }

            working_text_file_prep.close();

            std::string summ_comp = argv[3] + slashCharStr() +
                "compression_results.txt";

            std::fstream prior_results_input_file_2;
            prior_results_input_file_2.open(summ_comp.c_str());

            if (!prior_results_input_file_2.good())
            {
                tprintf(PRINT_STD, "\nError opening file: %s\n",
                    summ_comp.c_str());
                delete [] pass_fail_arr;
                return kTestFailed;
            }

            int tests_run_cnt = 0;
            int var_to_input = 0;
            int p = 0;

            // read in and throw away header
            while (p < 7)
            {
                prior_results_input_file_2.getline(buffer, 1024);
                p++;
            }

            // read in data
            int run_once = 1;
            char buffer3[255];

            while (!prior_results_input_file_2.eof())
            {
                prior_results_input_file_2.getline(buffer, 1024);
                run_once = 1;
                int u = 0;
                // 0 number not read yet|1 test_type not read yet|2 TestFolder
                // not read yet|3 Status not read yet

                int q = 35;

                while (q < 35 + 24)
                {
                    buffer3[u] = buffer[q];
                    q++;
                    u++;
                }

                selector_arr_2[tests_run_cnt] = buffer3;
                selector_arr_2[tests_run_cnt] += "\"";

                if (buffer[0] != '\0' && buffer[u] != '\r')
                    tests_run_cnt++;

                memset(buffer3, 0, sizeof(buffer3));
            }

            prior_results_input_file_2.close();
        }

        //////////////////////////////////////////////////////////////////////
        std::fstream working_text_file;
        working_text_file.open(working_text_file_str.c_str(), std::fstream::in | std::fstream::out);

        if (!working_text_file.good())
        {
            tprintf(PRINT_STD, "\nInput File does not exist\n");
            delete [] pass_fail_arr;
            return kTestFailed;
        }

        ///////////////// Files are actualy processed and run here /////////////
        while (!working_text_file.eof())
        {
            memset(buffer, 0, sizeof(buffer));

            const char *dummy_argv[999];
            dummy_argv[0] = argv[0];
            dummy_arg_var = 1;
            file_pos_track = working_text_file.tellg();
            working_text_file.getline(buffer, 1024);
            file_pos_track_2 = working_text_file.tellg();

            std::string bufferString = buffer;

            // skips over any line starting with a % in the input file to allow
            // for comenting
            if (buffer[0] == '%' || buffer[0] == '\0' || buffer[0] == '+' ||
                buffer[0] == '-' || buffer[0] == '\r')
                lines_skipped_cnt++;
            else
            {
                buf_1_var = 0;

                // parses through gotline and seperates commands out
                while (buffer[buf_1_var] != '\0' && buffer[buf_1_var] != '\r')
                {
                    int buf_2_var = 0;

                    while (buffer[buf_1_var] != 64 && buffer[buf_1_var] != '\0'
                        && buffer[buf_1_var] != '\r')
                    {
                        buffer2[buf_2_var] = buffer[buf_1_var];
                        buf_1_var++;
                        buf_2_var++;
                    }

                    buffer2[buf_2_var] = '\0';

                    if (buffer[buf_1_var] != '\0' && buffer[buf_1_var] != '\r')
                    {
                        buf_1_var++;
                    }

                    string_arr[dummy_arg_var] = buffer2;
                    dummy_arg_var++;
                }

                y = 1;

                while (y < dummy_arg_var)
                {
                    dummy_argv[y] = string_arr[y].c_str();
                    y++;
                }

                char test_run_cnt_char[4];
                dummy_argv[y] = vpxt_itoa_custom(number_of_tests_run,
                    test_run_cnt_char, 10);

                tprintf(PRINT_STD, "\n");

                int selector = vpxt_identify_test(dummy_argv[1]);

                if (selector >= 0 && selector < MAXTENUM)
                    number_of_tests_run++;

                prev_time_stamp = time_stamp_arr_2[0];
                time_stamp_arr_2[0] = date_string();

                // set the correct tester type dir string input
                std::string TestDir = working_dir;

                if (test_type == kTestOnly)
                    TestDir = argv[3];

                if (selector == 0)
                    if (test_type == kFullTest)
                        tprintf(PRINT_STD, "Not Implemented Yet.\n");

                selector_arr[selector_arr_int] = vpxt_test_name_list[selector];
                check_time_stamp(selector_arr_int, selector_arr, selector_arr_2,
                    prev_time_stamp, identical_file_cnt, time_stamp_arr_2);
                selector_arr_2[selector_arr_int] = time_stamp_arr_2[0];

                if (record_run_times == 1)
                    run_time_1 = vpxt_get_time();

                pass_fail_arr[pass_fail_int] = vpxt_test_funct_ptr[selector]
                (dummy_arg_var, (const char **)dummy_argv, TestDir,
                    time_stamp_arr_2[0], test_type, delete_ivf_files,
                    artifact_detection);

                if (record_run_times == 1){
                    run_time_2 = vpxt_get_time();
                    run_time_rec_arr[selector_arr_int] =
                        vpxt_get_time_in_micro_sec(run_time_1, run_time_2);
                }

                test_run++;

                /////////////////////////// On Fly out put /////////////////////
                FILE *fp;
                FILE *fp_html;

                if ((fp = freopen(work_dir_file_str.c_str(), "a", stderr)) ==
                    NULL)
                {
                    tprintf(PRINT_STD, "Cannot open summary out put file.\n");
                    delete [] pass_fail_arr;
                    return kTestFailed;
                }

                std::string test_results_html;
                vpxt_remove_file_extension(work_dir_file_str.c_str(),
                    test_results_html);
                test_results_html.erase(test_results_html.length() - 1, 1);
                test_results_html += ".html";

                fp_html = fopen(test_results_html.c_str() , "a");

                if (print_me)
                {
                    fprintf(fp_html, "\n<html>\n<body\n><p>\n<PRE><META "
                        "HTTP-EQUIV=\"REFRESH\" CONTENT=\"60\">");
                    fclose(fp_html);
                    print_header_info_to_file(test_results_html.c_str());
                    fp_html = fopen(test_results_html.c_str() , "a");

                    if (test_type == kFullTest)
                    {
                        fprintf(stderr, "\n------------------------------------"
                            "-------------------------------------------\n\n"
                            "                                  Test Results"
                            " \n");
                        fprintf(fp_html, "\n-----------------------------------"
                            "--------------------------------------------\n\n"
                            "                                  Test Results"
                            " \n");
                    }

                    if (test_type == kCompOnly)
                    {
                        fprintf(stderr, "\n------------------------------------"
                            "-------------------------------------------\n\n"
                            "                                Test "
                            "Compressions \n");
                        fprintf(fp_html, "\n<html>\n<body\n><p>\n<PRE>\n-------"
                            "--------------------------------------------------"
                            "----------------------\n\n"
                            "                                Test "
                            "Compressions \n");
                    }

                    if (test_type == kTestOnly)
                    {

                        fprintf(stderr, "\n------------------------------------"
                            "-------------------------------------------\n\n"
                            "                                Test Only "
                            "Results \n");
                        fprintf(fp_html, "\n<html>\n<body\n><p>\n<PRE>\n-------"
                            "--------------------------------------------------"
                            "----------------------\n\n"
                            "                                Test Only "
                            "Results \n");
                    }

                    fprintf(stderr, "\n\n%4s %-32s%-25s%s\n\n", "#", "Test Name"
                        , "Date and time", "Status");
                    fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#", "Test "
                        "Name" , "Date and time", "Status");

                    print_me = 0;
                }

                selector_arr_2[selector_arr_int].erase(
                    selector_arr_2[selector_arr_int].end() - 1);
                fprintf(stderr, "%4i %-32s%-25s", selector_arr_int,
                    selector_arr[selector_arr_int].c_str(),
                    selector_arr_2[selector_arr_int].c_str());

                ////////////////////////////////////////////////////////////////
                std::string lower_case_name =
                    selector_arr[selector_arr_int].c_str();
                vpxt_lower_case_string(lower_case_name);

                std::string html_name =
                    "<a style=\"color: #800000; text-decoration: none;\" href="
                    "\"" + lower_case_name + "/" +
                    selector_arr_2[selector_arr_int] + "/" + lower_case_name +
                    ".txt\" color=\"green\">" + selector_arr[selector_arr_int] +
                    "</a>";

                std::string html_status =
                    "<a style=\"color: #800000; text-decoration: none;\" href="
                    "\"" + lower_case_name + "/" +
                    selector_arr_2[selector_arr_int] + "/" + lower_case_name;

                std::string html_folder =
                    "<a style=\"color: #800000; text-decoration: none;\" href="
                    "\"" + lower_case_name + "/" +
                    selector_arr_2[selector_arr_int] + "\" color=\"green\">" +
                    selector_arr_2[selector_arr_int] + "</a>";

                fprintf(fp_html, "%4i", selector_arr_int);
                fprintf(fp_html, " %s", html_name.c_str());

                int number_of_spaces = 0;

                while (selector_arr[selector_arr_int].size() + number_of_spaces
                    < 31)
                {
                    fprintf(fp_html, " ");
                    number_of_spaces = number_of_spaces + 1;
                }

                fprintf(fp_html, " %s", html_folder.c_str());
                number_of_spaces = 0;

                while (selector_arr_2[selector_arr_int].size() +
                    number_of_spaces < 25)
                {
                    fprintf(fp_html, " ");
                    number_of_spaces = number_of_spaces + 1;
                }

                ////////////////////////////////////////////////////////////////

                selector_arr_2[selector_arr_int] += "\"";

                if (pass_fail_arr[selector_arr_int] == kTestPassed)
                {
                    tprintf(PRINT_ERR, "Passed\n");

                    html_status += ".txt\" color=\"green\">";
                    html_status += "Passed";
                    html_status += "</a>";
                    fprintf(fp_html, "%s \n", html_status.c_str());
                }

                if (pass_fail_arr[selector_arr_int] == kTestFailed)
                {
                    tprintf(PRINT_ERR, "Failed\n");

                    html_status += ".txt\" color=\"red\">";
                    html_status += "Failed";
                    html_status += "</a>";
                    fprintf(fp_html, "%s \n", html_status.c_str());
                }

                if (pass_fail_arr[selector_arr_int] == kTestIndeterminate)
                {
                    tprintf(PRINT_ERR, "Indeterminate\n");

                    html_status += ".txt\" color=\"yellow\">";
                    html_status += "Indeterminate";
                    html_status += "</a>";
                    fprintf(fp_html, "%s \n", html_status.c_str());
                }

                if (pass_fail_arr[selector_arr_int] == kTestComboLog)
                {
                    tprintf(PRINT_ERR, "SeeComboRunLog\n");
                    fprintf(fp_html, "SeeComboRunLog\n");
                }

                if (pass_fail_arr[selector_arr_int] == kTestPsnrLog)
                {
                    tprintf(PRINT_ERR, "SeePSNRLog\n");
                    fprintf(fp_html, "SeePSNRLog\n");
                }

                if (pass_fail_arr[selector_arr_int] == kTestRandomComplete)
                {
                    tprintf(PRINT_ERR, "RandomTestCompleted\n");
                    fprintf(fp_html, "RandomTestCompleted\n");
                }

                if (pass_fail_arr[selector_arr_int] == kTestMinPassed)
                {
                    tprintf(PRINT_ERR, "MinTestPassed\n");

                    html_status += ".txt\" color=\"yellow\">";
                    html_status += "MinTestPassed";
                    html_status += "</a>";
                    fprintf(fp_html, "%s \n", html_status.c_str());
                }

                if (pass_fail_arr[selector_arr_int] == kTestEncCreated)
                {
                    tprintf(PRINT_ERR, "CompressionMade\n");
                    fprintf(fp_html, "CompressionMade\n");
                }

                if (pass_fail_arr[selector_arr_int] == kTestErrFileMismatch)
                {
                    tprintf(PRINT_ERR, "ErrorFileMismatch\n");
                    fprintf(fp_html, "ErrorFileMismatch\n");
                }

                if (pass_fail_arr[selector_arr_int] == kTestNotSupported)
                {
                    tprintf(PRINT_ERR, "TestNotSupported\n");
                    fprintf(fp_html, "TestNotSupported\n");
                }
                if (pass_fail_arr[selector_arr_int] == kTestPossibleArtifact)
                {
                    tprintf(PRINT_ERR, "PossibleArtifact\n");

                    html_status.append(".txt\" color=\"yellow\">");
                    html_status.append("PossibleArtifact");
                    html_status.append("</a>");
                    fprintf(fp_html, "%s \n", html_status.c_str());
                }

                fclose(fp);
                fclose(fp_html);

                working_text_file.seekg(file_pos_track);
                std::string bufferstring = buffer;
                bufferstring.erase(0);

                if (test_type == kTestOnly)
                    bufferstring.insert(0, "-");
                else
                    bufferstring.insert(0, "+");

                working_text_file << bufferstring.c_str() ;
                working_text_file.seekg(file_pos_track_2);
                ////////////////////////////////////////////////////////////////

                pass_fail_int++;
                selector_arr_int++;
            }
        }

        working_text_file.close();
        // outputs a summary of the test results

        if (test_run == 0)
        {
            std::cout << "Test file specified contains no tests.\n";
            delete [] pass_fail_arr;
            return kTestFailed;
        }

        if (test_run == tests_run_catch)
        {
            if (test_type == kFullTest)
            {
                tprintf(PRINT_STD, "\nAll tests and compressions have already "
                    "been created and run.\n");
                delete [] pass_fail_arr;
                return kTestFailed;
            }

            if (test_type == kCompOnly)
            {
                tprintf(PRINT_STD, "\nAll compressions have already been "
                    "created.\n");
                delete [] pass_fail_arr;
                return kTestFailed;
            }

            if (test_type == kTestOnly)
            {
                tprintf(PRINT_STD, "\nAll tests on compressions have already "
                    "been run.\n");
                delete [] pass_fail_arr;
                return kTestFailed;
            }
        }

        FILE *fp;
        FILE *fp_html;

        if ((fp = freopen(work_dir_file_str.c_str(), "w", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open summary out put file.\n");
            delete [] pass_fail_arr;
            return kTestFailed;
        }

        std::string test_results_html;
        vpxt_remove_file_extension(work_dir_file_str.c_str(),test_results_html);
        test_results_html.erase(test_results_html.length() - 1, 1);
        test_results_html += ".html";

        fp_html = fopen(test_results_html.c_str() , "w");
        fprintf(fp_html, "\n<html>\n<body\n><p>\n<PRE>\n");
        fclose(fp_html);

        tprintf(PRINT_STD, "\n\n\n");
        print_header_info();
        print_header_info_to_file(test_results_html.c_str());

        fp_html = fopen(test_results_html.c_str() , "a");

        y = 0;

        while (y < test_run)
        {
            selector_arr_2[y].erase(selector_arr_2[y].end() - 1);
            y++;
        }

        int test_indicator = 0;
        y = 0;

        if (test_type == kTestOnly || test_type == kFullTest)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                "Test Results - Failed \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                "Test Results - Failed \n");

            test_indicator = 0;
            y = 0;

            while (y < test_run)
            {
                if (pass_fail_arr[y] == kTestFailed)
                {
                    if (test_indicator == 0)
                    {
                        tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
                            "Test Name" , "Date and time", "Status");
                        fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
                            "Test Name" , "Date and time", "Status");
                    }

                    test_indicator = 1;
                    tprintf(PRINT_BTH, "%4i %-32s%-25s", y,
                        selector_arr[y].c_str(), selector_arr_2[y].c_str());
                    ////////////////////////////////////////////////////////////
                    std::string lower_case_name = selector_arr[y].c_str();
                    vpxt_lower_case_string(lower_case_name);

                    std::string html_name =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "/" + lower_case_name + ".txt\" color=\"green\">" +
                        selector_arr[y] + "</a>";

                    std::string html_folder =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "\" color=\"green\">" + selector_arr_2[y] + "</a>";

                    fprintf(fp_html, "%4i", y);
                    fprintf(fp_html, " %s", html_name.c_str());

                    int number_of_spaces = 0;

                    while (selector_arr[y].size() + number_of_spaces < 31)
                    {
                        fprintf(fp_html, " ");
                        number_of_spaces = number_of_spaces + 1;
                    }

                    fprintf(fp_html, " %s", html_folder.c_str());
                    number_of_spaces = 0;

                    while (selector_arr_2[y].size() + number_of_spaces < 25)
                    {
                        fprintf(fp_html, " ");
                        number_of_spaces = number_of_spaces + 1;
                    }

                    ////////////////////////////////////////////////////////////
                    tprintf(PRINT_BTH, "Failed\n");

                    std::string html_status =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "/" + lower_case_name + ".txt\" color=\"green\">" +
                        "Failed" + "</a>";
                    fprintf(fp_html, "%s \n", html_status.c_str());
                }

                y++;
            }

            if (test_indicator == 0)
            {
                tprintf(PRINT_BTH, "\n    NONE\n\n");
                fprintf(fp_html, "\n    NONE\n\n");
            }

            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                "Test Results - Indeterminate \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                "Test Results - Indeterminate \n");

            test_indicator = 0;
            y = 0;

            while (y < test_run)
            {
                if (pass_fail_arr[y] == kTestIndeterminate)
                {
                    if (test_indicator == 0)
                    {
                        tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
                            "Test Name" , "Date and time", "Status");
                        fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
                            "Test Name" , "Date and time", "Status");
                    }

                    test_indicator = 1;
                    tprintf(PRINT_BTH, "%4i %-32s%-25s", y,
                        selector_arr[y].c_str(), selector_arr_2[y].c_str());
                    ////////////////////////////////////////////////////////////
                    std::string lower_case_name = selector_arr[y].c_str();
                    vpxt_lower_case_string(lower_case_name);

                    std::string html_name =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "/" + lower_case_name + ".txt\" color=\"green\">" +
                        selector_arr[y] + "</a>";

                    std::string html_folder =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "\" color=\"green\">" + selector_arr_2[y] + "</a>";

                    fprintf(fp_html, "%4i", y);
                    fprintf(fp_html, " %s", html_name.c_str());

                    int number_of_spaces = 0;

                    while (selector_arr[y].size() + number_of_spaces < 31)
                    {
                        fprintf(fp_html, " ");
                        number_of_spaces = number_of_spaces + 1;
                    }

                    fprintf(fp_html, " %s", html_folder.c_str());
                    number_of_spaces = 0;

                    while (selector_arr_2[y].size() + number_of_spaces < 25)
                    {
                        fprintf(fp_html, " ");
                        number_of_spaces = number_of_spaces + 1;
                    }

                    ////////////////////////////////////////////////////////////
                    tprintf(PRINT_BTH, "Indeterminate\n");

                    std::string html_status =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "/" + lower_case_name + ".txt\" color=\"green\">" +
                        "Indeterminate" + "</a>";
                    fprintf(fp_html, "%s \n", html_status.c_str());
                }

                y++;
            }

            if (test_indicator == 0)
            {
                tprintf(PRINT_BTH, "\n    NONE\n\n");
                fprintf(fp_html, "\n    NONE\n\n");
            }

            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                "Test Results - MinPassed \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                "Test Results - MinPassed \n");

            test_indicator = 0;
            y = 0;

            while (y < test_run)
            {
                if (pass_fail_arr[y] == kTestMinPassed)
                {
                    if (test_indicator == 0)
                    {
                        tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
                            "Test Name" , "Date and time", "Status");
                        fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
                            "Test Name" , "Date and time", "Status");
                    }

                    test_indicator = 1;
                    tprintf(PRINT_BTH, "%4i %-32s%-25s", y,
                        selector_arr[y].c_str(), selector_arr_2[y].c_str());
                    ////////////////////////////////////////////////////////////
                    std::string lower_case_name = selector_arr[y].c_str();
                    vpxt_lower_case_string(lower_case_name);

                    std::string html_name =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "/" + lower_case_name + ".txt\" color=\"green\">" +
                        selector_arr[y] + "</a>";

                    std::string html_folder =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "\" color=\"green\">" + selector_arr_2[y] + "</a>";

                    fprintf(fp_html, "%4i", y);
                    fprintf(fp_html, " %s", html_name.c_str());

                    int number_of_spaces = 0;

                    while (selector_arr[y].size() + number_of_spaces < 31)
                    {
                        fprintf(fp_html, " ");
                        number_of_spaces = number_of_spaces + 1;
                    }

                    fprintf(fp_html, " %s", html_folder.c_str());
                    number_of_spaces = 0;

                    while (selector_arr_2[y].size() + number_of_spaces < 25)
                    {
                        fprintf(fp_html, " ");
                        number_of_spaces = number_of_spaces + 1;
                    }

                    ////////////////////////////////////////////////////////////
                    tprintf(PRINT_BTH, "MinTestPassed\n");

                    std::string html_status =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "/" + lower_case_name + ".txt\" color=\"green\">" +
                        "MinTestPassed" + "</a>";
                    fprintf(fp_html, "%s \n", html_status.c_str());
                }

                y++;
            }

            if (test_indicator == 0)
            {
                tprintf(PRINT_BTH, "\n    NONE\n\n");
                fprintf(fp_html, "\n    NONE\n\n");
            }

            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                "Test Results - Passed \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                "Test Results - Passed \n");

            test_indicator = 0;
            y = 0;

            while (y < test_run)
            {
                if (pass_fail_arr[y] == kTestPassed)
                {
                    if (test_indicator == 0)
                    {
                        tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
                            "Test Name" , "Date and time", "Status");
                        fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
                            "Test Name" , "Date and time", "Status");
                    }

                    test_indicator = 1;
                    tprintf(PRINT_BTH, "%4i %-32s%-25s", y,
                        selector_arr[y].c_str(), selector_arr_2[y].c_str());

                    ////////////////////////////////////////////////////////////
                    std::string lower_case_name = selector_arr[y].c_str();
                    vpxt_lower_case_string(lower_case_name);

                    std::string html_name =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "/" + lower_case_name + ".txt\" color=\"green\">" +
                        selector_arr[y] + "</a>";

                    std::string html_folder =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "\" color=\"green\">" + selector_arr_2[y] + "</a>";

                    fprintf(fp_html, "%4i", y);
                    fprintf(fp_html, " %s", html_name.c_str());

                    int number_of_spaces = 0;

                    while (selector_arr[y].size() + number_of_spaces < 31)
                    {
                        fprintf(fp_html, " ");
                        number_of_spaces = number_of_spaces + 1;
                    }

                    fprintf(fp_html, " %s", html_folder.c_str());
                    number_of_spaces = 0;

                    while (selector_arr_2[y].size() + number_of_spaces < 25)
                    {
                        fprintf(fp_html, " ");
                        number_of_spaces = number_of_spaces + 1;
                    }

                    ////////////////////////////////////////////////////////////

                    tprintf(PRINT_BTH, "Passed\n");

                    std::string html_status =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "/" + lower_case_name + ".txt\" color=\"green\">" +
                        "Passed" + "</a>";
                    fprintf(fp_html, "%s \n", html_status.c_str());
                }

                y++;
            }

            if (test_indicator == 0)
            {
                tprintf(PRINT_BTH, "\n    NONE\n\n");
                fprintf(fp_html, "\n    NONE\n\n");
            }

            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                "Test Results - Other \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                "Test Results - Other \n");

            test_indicator = 0;
            y = 0;

            while (y < test_run)
            {
                if (pass_fail_arr[y] != kTestFailed && pass_fail_arr[y] !=
                    kTestPassed && pass_fail_arr[y] != kTestMinPassed &&
                    pass_fail_arr[y] != kTestIndeterminate)
                {
                    if (test_indicator == 0)
                    {
                        tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
                            "Test Name" , "Date and time", "Status");
                        fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
                            "Test Name" , "Date and time", "Status");
                    }

                    test_indicator = 1;
                    tprintf(PRINT_BTH, "%4i %-32s%-25s", y,
                        selector_arr[y].c_str(), selector_arr_2[y].c_str());
                    ////////////////////////////////////////////////////////////
                    std::string lower_case_name = selector_arr[y].c_str();
                    vpxt_lower_case_string(lower_case_name);

                    std::string html_name =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "/" + lower_case_name + ".txt\" color=\"green\">" +
                        selector_arr[y] + "</a>";

                    std::string html_folder =
                        "<a style=\"color: #800000; text-decoration: none;\" hr"
                        "ef=\"" + lower_case_name + "/" + selector_arr_2[y] +
                        "\" color=\"green\">" + selector_arr_2[y] + "</a>";

                    fprintf(fp_html, "%4i", y);
                    fprintf(fp_html, " %s", html_name.c_str());

                    int number_of_spaces = 0;

                    while (selector_arr[y].size() + number_of_spaces < 31)
                    {
                        fprintf(fp_html, " ");
                        number_of_spaces = number_of_spaces + 1;
                    }

                    fprintf(fp_html, " %s", html_folder.c_str());
                    number_of_spaces = 0;

                    while (selector_arr_2[y].size() + number_of_spaces < 25)
                    {
                        fprintf(fp_html, " ");
                        number_of_spaces = number_of_spaces + 1;
                    }

                    ////////////////////////////////////////////////////////////

                    if (pass_fail_arr[y] == kTestComboLog)
                    {
                        tprintf(PRINT_BTH, "SeeComboRunLog\n");
                        fprintf(fp_html, "SeeComboRunLog\n");
                    }

                    if (pass_fail_arr[y] == kTestPsnrLog)
                    {
                        tprintf(PRINT_BTH, "SeePSNRLog\n");
                        fprintf(fp_html, "SeePSNRLog\n");
                    }

                    if (pass_fail_arr[y] == kTestRandomComplete)
                    {
                        tprintf(PRINT_BTH, "RandomTestCompleted\n");
                        fprintf(fp_html, "RandomTestCompleted\n");
                    }

                    if (pass_fail_arr[y] == kTestEncCreated)
                    {
                        tprintf(PRINT_BTH, "CompressionMade.\n");
                        fprintf(fp_html, "CompressionMade.\n");
                    }

                    if (pass_fail_arr[y] == kTestErrFileMismatch)
                    {
                        tprintf(PRINT_BTH, "ErrorFileMismatch\n");
                        fprintf(fp_html, "ErrorFileMismatch\n");
                    }

                    if (pass_fail_arr[y] == kTestNotSupported)
                    {
                        tprintf(PRINT_BTH, "TestNotSupported\n");
                        fprintf(fp_html, "TestNotSupported\n");
                    }

                    if (pass_fail_arr[y] == kTestPossibleArtifact)
                    {
                        tprintf(PRINT_BTH, "PossibleArtifact\n");
                        fprintf(fp_html, "PossibleArtifact\n");
                    }
                }
                y++;
            }

            if (test_indicator == 0)
            {
                tprintf(PRINT_BTH, "\n    NONE\n\n");
                fprintf(fp_html, "\n    NONE\n\n");
            }

            /////////////////////////// Output time it took to run test ////////
            if (record_run_times == 1)
            {
                std::string time_output_str;
                vpxt_remove_file_extension(work_dir_file_str.c_str(),
                    time_output_str);
                time_output_str += "Times.txt";

                FILE *output_time;
                output_time = fopen(time_output_str.c_str() , "w");

                tprintf(PRINT_STD, "\n-----------------------------------------"
                    "--------------------------------------\n\n"
                    "                       time to Run Tests\n");
                fprintf(output_time, "\n---------------------------------------"
                    "----------------------------------------\n\n"
                    "                       time to Run Tests\n");

                tprintf(PRINT_STD, "\n\n%4s %-28s%s\n\n", "",
                    " Test Name", "            time");
                fprintf(output_time, "\n\n%4s %-28s%s\n\n", "",
                    " Test Name", "            time");

                y = 0;

                while (y < test_run)
                {
                    tprintf(PRINT_STD, "   %s", selector_arr[y].c_str());
                    fprintf(output_time, "   %s", selector_arr[y].c_str());

                    int z = selector_arr[y].size();

                    while (z <= 25)
                    {
                        tprintf(PRINT_STD, " ");
                        fprintf(output_time, " ");
                        z++;
                    }

                    tprintf(PRINT_STD, "%20u ms\n", run_time_rec_arr[y]);
                    fprintf(output_time, "%20u ms\n", run_time_rec_arr[y]);
                    y++;
                }
            }

            ////////////////////////////////////////////////////////////////////
        }

        if (test_type == kFullTest)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                "Chronological Test Results \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                "Chronological Test Results \n");
        }

        if (test_type == kCompOnly)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                "Chronological Test Compressions \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                "Chronological Test Compressions \n");
        }

        if (test_type == kTestOnly)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                "Chronological Test Only Results \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                "Chronological Test Only Results \n");
        }

        y = 0;

        tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
            "Test Name" , "Date and time", "Status");
        fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
            "Test Name" , "Date and time", "Status");

        while (y < test_run)
        {
            tprintf(PRINT_BTH, "%4i %-32s%-25s", y, selector_arr[y].c_str(),
                selector_arr_2[y].c_str());

            ////////////////////////////////////////////////////////////////////
            std::string lower_case_name = selector_arr[y].c_str();
            vpxt_lower_case_string(lower_case_name);

            std::string html_name =
                "<a style=\"color: #800000; text-decoration: none;\" href=\"" +
                lower_case_name + "/" + selector_arr_2[y] + "/" +
                lower_case_name + ".txt\" color=\"green\">" + selector_arr[y] +
                "</a>";

            std::string html_status =
                "<a style=\"color: #800000; text-decoration: none;\" href=\"" +
                lower_case_name + "/" + selector_arr_2[y] + "/" +
                lower_case_name;

            std::string html_folder =
                "<a style=\"color: #800000; text-decoration: none;\" href=\"" +
                lower_case_name + "/" + selector_arr_2[y] +
                "\" color=\"green\">" + selector_arr_2[y] + "</a>";

            fprintf(fp_html, "%4i", y);
            fprintf(fp_html, " %s", html_name.c_str());

            int number_of_spaces = 0;

            while (selector_arr[y].size() + number_of_spaces < 31)
            {
                fprintf(fp_html, " ");
                number_of_spaces = number_of_spaces + 1;
            }

            fprintf(fp_html, " %s", html_folder.c_str());
            number_of_spaces = 0;

            while (selector_arr_2[y].size() + number_of_spaces < 25)
            {
                fprintf(fp_html, " ");
                number_of_spaces = number_of_spaces + 1;
            }

            ////////////////////////////////////////////////////////////////////

            if (pass_fail_arr[y] == kTestPassed)
            {
                tprintf(PRINT_BTH, "Passed\n");

                html_status += ".txt\" color=\"green\">";
                html_status += "Passed";
                html_status += "</a>";
                fprintf(fp_html, "%s \n", html_status.c_str());
            }

            if (pass_fail_arr[y] == kTestFailed)
            {
                tprintf(PRINT_BTH, "Failed\n");

                html_status += ".txt\" color=\"red\">";
                html_status += "Failed";
                html_status += "</a>";
                fprintf(fp_html, "%s \n", html_status.c_str());
            }

            if (pass_fail_arr[y] == kTestIndeterminate)
            {
                tprintf(PRINT_BTH, "Indeterminate\n");

                html_status += ".txt\" color=\"yellow\">";
                html_status += "Indeterminate";
                html_status += "</a>";
                fprintf(fp_html, "%s \n", html_status.c_str());
            }

            if (pass_fail_arr[y] == kTestComboLog)
            {
                tprintf(PRINT_BTH, "SeeComboRunLog\n");
                fprintf(fp_html, "SeeComboRunLog\n");
            }

            if (pass_fail_arr[y] == kTestPsnrLog)
            {
                tprintf(PRINT_BTH, "SeePSNRLog\n");
                fprintf(fp_html, "SeePSNRLog\n");
            }

            if (pass_fail_arr[y] == kTestRandomComplete)
            {
                tprintf(PRINT_BTH, "RandomTestCompleted\n");
                fprintf(fp_html, "RandomTestCompleted\n");
            }

            if (pass_fail_arr[y] == kTestMinPassed)
            {
                tprintf(PRINT_BTH, "MinTestPassed\n");

                html_status += ".txt\" color=\"yellow\">";
                html_status += "MinTestPassed";
                html_status += "</a>";
                fprintf(fp_html, "%s \n", html_status.c_str());
            }

            if (pass_fail_arr[y] == kTestEncCreated)
            {
                tprintf(PRINT_BTH, "CompressionMade\n");
                fprintf(fp_html, "CompressionMade\n");
            }

            if (pass_fail_arr[y] == kTestErrFileMismatch)
            {
                tprintf(PRINT_BTH, "ErrorFileMismatch\n");
                fprintf(fp_html, "ErrorFileMismatch\n");
            }

            if (pass_fail_arr[y] == kTestNotSupported)
            {
                tprintf(PRINT_BTH, "TestNotSupported\n");
                fprintf(fp_html, "TestNotSupported\n");
            }

            if (pass_fail_arr[y] == kTestPossibleArtifact)
            {
                tprintf(PRINT_BTH, "PossibleArtifact\n");

                html_status.append(".txt\" color=\"yellow\">");
                html_status.append("PossibleArtifact");
                html_status.append("</a>");
                fprintf(fp_html, "%s \n", html_status.c_str());
            }

            y++;
        }

        fclose(fp);
        fclose(fp_html);
        delete [] pass_fail_arr;
        return kTestFailed;
}