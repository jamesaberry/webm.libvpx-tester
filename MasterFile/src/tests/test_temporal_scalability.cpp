#include "vpxt_test_declarations.h"

int test_temporal_scalability(int argc,
                              const char** argv,
                              const std::string &working_dir,
                              const std::string sub_folder_str,
                              int test_type,
                              int delete_ivf,
                              int artifact_detection)
{
    char *comp_out_str = "Allow Drop Frames";
    char *test_dir = "test_temporal_scalability";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    // layer_mode is rate_num as used in vp8_scalable_patterns.exe
    int layer_mode = atoi(argv[3]);
    int temp_scale_br_0 = atoi(argv[4]);
    int temp_scale_br_1 = atoi(argv[5]);
    int temp_scale_br_2 = atoi(argv[6]);
    int temp_scale_br_3 = atoi(argv[7]);
    int temp_scale_br_4 = atoi(argv[8]);
    std::string enc_format = argv[9];

    int speed = 0;

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
            cur_test_dir_str, file_index_str, main_test_dir_char,
            file_index_output_char, sub_folder_str) == 11)
                return kTestErrFileMismatch;

    int number_of_encodes = 0;
    std::string temp_scale_out_base = cur_test_dir_str;
    temp_scale_out_base += slashCharStr();
    temp_scale_out_base += test_dir;
    temp_scale_out_base += "_compression";

    std::vector<int> enc_vec_art_det;
    std::vector<int>::iterator int_it;

    std::vector<std::string> enc_vec;
    std::vector<std::string> temp_scale_vec;
    std::vector<std::string> temp_scale_fs_vec;

    std::vector<std::string>::iterator str_it;
    std::vector<std::string>::iterator str_it2;


    if((layer_mode >= 0 && layer_mode < 2) || layer_mode == 7)
        number_of_encodes = 2;
    if((layer_mode >= 2 && layer_mode < 6) || layer_mode == 8)
        number_of_encodes = 3;
    if(layer_mode == 6)
        number_of_encodes = 5;

    for(int i = 0; i < number_of_encodes; ++i){
        char i_char[8];
        vpxt_itoa_custom(i, i_char, 10);

        std::string temp_scale_str = cur_test_dir_str;
        temp_scale_str += slashCharStr();
        temp_scale_str += test_dir;
        temp_scale_str += "_compression_";
        temp_scale_str += i_char;

        std::string temp_scale_fs_str = cur_test_dir_str;
        temp_scale_fs_str += slashCharStr();
        temp_scale_fs_str += test_dir;
        temp_scale_fs_str += "_compression_";
        temp_scale_fs_str += i_char;
        temp_scale_fs_str +="_FrameStats.txt";

        std::string enc_str = cur_test_dir_str;
        enc_str += slashCharStr();
        enc_str += test_dir;
        enc_str += "_compression_";
        enc_str += i_char;
        enc_str += "_compare";

        vpxt_enc_format_append(enc_str, enc_format);
        vpxt_enc_format_append(temp_scale_str, enc_format);

        temp_scale_vec.push_back(temp_scale_str);
        temp_scale_fs_vec.push_back(temp_scale_fs_str);
        enc_vec.push_back(enc_str);

        enc_vec_art_det.push_back(artifact_detection);
    }

    ///////////// OutPutfile ////////////
    std::string text_file_str = cur_test_dir_str;
    text_file_str += slashCharStr();
    text_file_str += test_dir;

    if (test_type == kCompOnly || test_type == kFullTest)
        text_file_str += ".txt";
    else
        text_file_str += "_TestOnly.txt";

    FILE *fp;

    if ((fp = freopen(text_file_str.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
            text_file_str.c_str());
                exit(1);
    }

    ////////////////////////////////
    //////////////////////////////////////////////////////////

    if (test_type == kFullTest)
        print_header_full_test(argc, argv, main_test_dir_char);

    if (test_type == kCompOnly)
        print_header_compression_only(argc, argv, main_test_dir_char);

    if (test_type == kTestOnly)
        print_header_test_only(argc, argv, cur_test_dir_str);

    vpxt_cap_string_print(PRINT_BTH, "%s", test_dir);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);
    unsigned int scale_compress_time = 0;
    unsigned int enc_compress_time = 0;

    /////////////////// Use Custom Settings ///////////////////
    if (input_ver == 2)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf(PRINT_BTH, "\nInput Settings file %s does not exist\n",
                argv[argc-1]);

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt = vpxt_input_settings(argv[argc-1]);
    }

    /////////////////////////////////////////////////////////

    // Run Test only (Runs Test, Sets up test to be run, or skips
    // compresion of files)
    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = 0;

        scale_compress_time = vpxt_compress_scalable_patterns(input.c_str(),
            temp_scale_out_base.c_str(), speed, opt, comp_out_str, 0, 0,
            enc_format, layer_mode, temp_scale_br_0, temp_scale_br_1,
            temp_scale_br_2, temp_scale_br_3, temp_scale_br_4);

        if (scale_compress_time == -1){
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }
    }

    if (test_type == kCompOnly)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    std::vector<double> enc_psnr;
    std::vector<double> temp_scale_psnr;
    std::vector<double> temp_scale_fs_results;
    std::vector<int> eval_drop_vec;

    int temp_bitrate_arr[5] =  {temp_scale_br_0, temp_scale_br_1,
        temp_scale_br_2, temp_scale_br_3, temp_scale_br_4};

    if(layer_mode == kRealTime || layer_mode == 7){
        eval_drop_vec.push_back(1);
        eval_drop_vec.push_back(0);
    }
    else if(layer_mode == kOnePassGoodQuality){
        eval_drop_vec.push_back(2);
        eval_drop_vec.push_back(0);
    }
    else if(layer_mode == kOnePassBestQuality){
        eval_drop_vec.push_back(5);
        eval_drop_vec.push_back(2);
        eval_drop_vec.push_back(0);
    }
    else if(layer_mode == 6){
        eval_drop_vec.push_back(15);
        eval_drop_vec.push_back(7);
        eval_drop_vec.push_back(3);
        eval_drop_vec.push_back(1);
        eval_drop_vec.push_back(0);
    }
    else{
        eval_drop_vec.push_back(3);
        eval_drop_vec.push_back(1);
        eval_drop_vec.push_back(0);
    }

    int j = 0;
    int delete_files_num = 0;

    // encode standard compressions and do psnrs
    int_it = enc_vec_art_det.begin();
    for(str_it = enc_vec.begin(); str_it < enc_vec.end(); ++str_it)
    {
        unsigned int cpu_tick = 0;
        opt.target_bandwidth = temp_bitrate_arr[j];
        enc_compress_time = vpxt_time_compress(input.c_str(), (*str_it).c_str(),
            0, temp_bitrate_arr[j], opt, comp_out_str, 0, 0, cpu_tick,
            enc_format);

        if (enc_compress_time == -1){
            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        enc_psnr.push_back(vpxt_psnr(input.c_str(), (*str_it).c_str(), 0,
            PRINT_BTH, 1, 0, 0, 0, NULL, (*int_it)));

        ++j;
        ++int_it;
    }

    // run psnrs for temp scale compressions
    int_it = enc_vec_art_det.begin();
    for(str_it = temp_scale_vec.begin(); str_it < temp_scale_vec.end();++str_it)
    {
        temp_scale_psnr.push_back(vpxt_psnr(input.c_str(), (*str_it).c_str(), 0,
        PRINT_BTH, 1, 0, 0, 0, NULL, (*int_it)));
        ++int_it;
    }

    // gather frame statistics for temp scale compressions
    str_it2 = temp_scale_vec.begin();
    for(str_it = temp_scale_fs_vec.begin(); str_it<temp_scale_fs_vec.end();
        ++str_it){
        vpxt_print_frame_statistics(input.c_str(), (*str_it2).c_str(),
            (*str_it).c_str(), 0, NULL, 1, 1, 1, 1, 1, 1);

        ++str_it2;
    }

    // evaluate frame satistics
    std::vector<int>::iterator eval_drop_it = eval_drop_vec.begin();
    for(str_it = temp_scale_fs_vec.begin(); str_it < temp_scale_fs_vec.end();
        ++str_it){
        temp_scale_fs_results.push_back(
            vpxt_eval_frame_stats_temp_scale((*str_it).c_str(), *eval_drop_it));

        ++eval_drop_it;
    }

    tprintf(PRINT_BTH, "\n");

    int i = 0;

    // temp scale file name only vector
    std::vector<std::string> temp_scale_fn_vec;
    for(str_it = temp_scale_vec.begin();str_it < temp_scale_vec.end();++str_it){
        char file_name_char[256];
        vpxt_file_name((*str_it).c_str(), file_name_char, 0);
        temp_scale_fn_vec.push_back(file_name_char);
    }

    // encode comp file name only vector
    std::vector<std::string> enc_fn_vec;
    for(str_it = enc_vec.begin(); str_it < enc_vec.end(); ++str_it){
        char file_name_char[256];
        vpxt_file_name((*str_it).c_str(), file_name_char, 0);
        enc_fn_vec.push_back(file_name_char);
    }

    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    // make sure that decemation occurs correctly
    // make sure speed is comperable to normal encode
    // make sure psnr values increase as # decimation dec
    // make sure psnr values obtain min quality assurence
    // make sure psnr values compared to standard encodes are with in range

    int increase_fail = 0;
    int drop_frame_fail = 0;
    std::vector<double>::iterator double_it;
    std::vector<double>::iterator double_it2;

    // psnr min threashold check
    str_it = temp_scale_fn_vec.begin();
    for(double_it = temp_scale_psnr.begin(); double_it < temp_scale_psnr.end();
        ++double_it){
        if(*double_it < 15.0){
            vpxt_formated_print(RESPRT, "%s PSNR: %.2f < 15.0 - Failed",
                (*str_it).c_str(), *double_it);
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
        else{
            vpxt_formated_print(RESPRT, "%s PSNR: %.2f >= 15.0 - Passed",
                (*str_it).c_str(), *double_it);
            tprintf(PRINT_BTH, "\n");
        }

        // increase psnr check
        if(double_it < temp_scale_psnr.end()-1)
            if(*double_it > *(double_it+1))
                increase_fail = 1;

        ++str_it;
    }

    if (!increase_fail)
    {
        vpxt_formated_print(RESPRT, "All PSNRs increase correctly - Passed");
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT,
            "Not all PSNRs increase correctly - Failed");
        tprintf(PRINT_BTH, "\n");
        test_state = kTestFailed;
    }

    // evaluate temp scale psnrs vs normal encode psnrs
    double_it2 = enc_psnr.begin();
    str_it = temp_scale_fn_vec.begin();
    str_it2 = enc_fn_vec.begin();
    int comp_num = 0;
    int temp_scale_psnr_size = temp_scale_psnr.size();
    for(double_it = temp_scale_psnr.begin(); double_it < temp_scale_psnr.end();
        ++double_it)
    {
        double range = 1.0;

        // adjust target bitrate range
        if(comp_num == temp_scale_psnr_size - 1)
            range = 0.10;
        if(comp_num == temp_scale_psnr_size - 2)
            range = 0.35;
        if(comp_num == temp_scale_psnr_size - 3)
            range = 0.40;
        if(comp_num == temp_scale_psnr_size - 4)
            range = 0.45;
        if(comp_num == temp_scale_psnr_size - 5)
            range = 0.50;

        // if wihin x% - pass
        if (*double_it >= (*double_it2 - (*double_it2 * range)))
        {
            vpxt_formated_print(RESPRT, "%s PSNR: %.2f is within at least "
                "%.0f%% of %s PSNR: %.2f - Passed", (*str_it).c_str(),
                *double_it, range*100, (*str_it2).c_str(), *double_it2);
        }
        else
        {
            vpxt_formated_print(RESPRT, "%s PSNR: %.2f is not within at least "
                "%.0f%% of %s PSNR: %.2f - Failed", (*str_it).c_str(),
                *double_it, range*100, (*str_it2).c_str(), *double_it2);
            test_state = kTestFailed;
        }

        ++comp_num;
        ++str_it;
        ++str_it2;
        ++double_it2;

        tprintf(PRINT_BTH, "\n");
    }

    // frame decimation check
    for(double_it = temp_scale_fs_results.begin(); double_it <
        temp_scale_fs_results.end(); ++double_it){
        if(!*double_it)
            drop_frame_fail = 1;
    }
    if (!drop_frame_fail)
    {
        vpxt_formated_print(RESPRT, "Decemation occurs correctly for all files"
            " - Passed");
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        str_it = temp_scale_fn_vec.begin();
        for(double_it = temp_scale_fs_results.begin();
            double_it < temp_scale_fs_results.end(); ++double_it)
        {
            if(!*double_it){
                vpxt_formated_print(RESPRT, "Decemation does not occur"
                    " correctly for %s - Failed", (*str_it).c_str());
                tprintf(PRINT_BTH, "\n");
            }

            ++str_it;
        }

        test_state = kTestFailed;
    }

    // if scale compress time is less than or within 20% of normal compression
    // pass otherwise fail.
    if(scale_compress_time < enc_compress_time)
    {
        vpxt_formated_print(RESPRT, "Scaled compression time: %i is less than "
            "standard encode compression time: %i. - Passed",
            scale_compress_time, enc_compress_time);
    }
    else
    {
        // if greater than but within 20% - pass
        if (scale_compress_time <= (enc_compress_time +
            (enc_compress_time * 0.20)))
        {
            vpxt_formated_print(RESPRT, "Scaled compression time: %i is within"
                " 20%% of standard encode compression time: %i. - Passed",
                scale_compress_time, enc_compress_time);
        }
        else
        {
            vpxt_formated_print(RESPRT, "Scaled compression time: %i is not "
                "within 20%% of standard encode compression time: %i. - Failed",
                scale_compress_time, enc_compress_time);
            test_state = kTestFailed;
        }

        tprintf(PRINT_BTH, "\n");
    }

    tprintf(PRINT_BTH, "\n");

    // handle possible artifact
    for(int_it = enc_vec_art_det.begin(); int_it < enc_vec_art_det.end();
        ++int_it)
    {
        if((*int_it) == kPossibleArtifactFound)
        {
            tprintf(PRINT_BTH, "\nPossible Artifact\n");

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestPossibleArtifact;
        }
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf){
        for(str_it = temp_scale_vec.begin(); str_it < temp_scale_vec.end();
            ++str_it)
            vpxt_delete_files(1, (*str_it).c_str());
        for(str_it = enc_vec.begin(); str_it < enc_vec.end(); ++str_it)
            vpxt_delete_files(1, (*str_it).c_str());
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}
