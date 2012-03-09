#include "vpxt_test_declarations.h"

int test_temporal_scalability(int argc,
                              const char *const *argv,
                              const std::string &WorkingDir,
                              std::string FilesAr[],
                              int TestType,
                              int DeleteIVF)
{
    char *CompressString = "Allow Drop Frames";
    char *MyDir = "test_temporal_scalability";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int LayerMode = atoi(argv[3]);
    int TempBitRate0 = atoi(argv[4]);
    int TempBitRate1 = atoi(argv[5]);
    int TempBitRate2 = atoi(argv[6]);
    int TempBitRate3 = atoi(argv[7]);
    int TempBitRate4 = atoi(argv[8]);
    std::string EncForm = argv[9];

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    std::string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir,
            CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar,
            FilesAr) == 11)
                return 11;

    std::string TimeCompress = CurTestDirStr;
    TimeCompress.append(slashCharStr());
    TimeCompress.append(MyDir);
    TimeCompress.append("_time_compare");
    vpxt_enc_format_append(TimeCompress, EncForm);

    std::string TempScaleBase = CurTestDirStr;
    TempScaleBase.append(slashCharStr());
    TempScaleBase.append(MyDir);
    TempScaleBase.append("_compression");

    std::string TempScale0 = TempScaleBase;
    TempScale0.append("_0");
    std::string TempScale0FS = TempScale0.c_str();
    TempScale0FS.append("_FrameStats.txt");
    std::string Enc0 = TempScale0.c_str();
    Enc0.append("_compare");
    vpxt_enc_format_append(Enc0, EncForm);
    vpxt_enc_format_append(TempScale0, EncForm);

    std::string TempScale1 = TempScaleBase;
    TempScale1.append("_1");
    std::string TempScale1FS = TempScale1.c_str();
    TempScale1FS.append("_FrameStats.txt");
    std::string Enc1 = TempScale1.c_str();
    Enc1.append("_compare");
    vpxt_enc_format_append(Enc1, EncForm);
    vpxt_enc_format_append(TempScale1, EncForm);

    std::string TempScale2 = TempScaleBase;
    TempScale2.append("_2");
    std::string TempScale2FS = TempScale2.c_str();
    TempScale2FS.append("_FrameStats.txt");
    std::string Enc2 = TempScale2.c_str();
    Enc2.append("_compare");
    vpxt_enc_format_append(Enc2, EncForm);
    vpxt_enc_format_append(TempScale2, EncForm);

    std::string TempScale3 = TempScaleBase;
    TempScale3.append("_3");
    std::string TempScale3FS = TempScale3.c_str();
    TempScale3FS.append("_FrameStats.txt");
    std::string Enc3 = TempScale3.c_str();
    Enc3.append("_compare");
    vpxt_enc_format_append(Enc3, EncForm);
    vpxt_enc_format_append(TempScale3, EncForm);

    std::string TempScale4 = TempScaleBase;
    TempScale4.append("_4");
    std::string TempScale4FS = TempScale4.c_str();
    TempScale4FS.append("_FrameStats.txt");
    std::string Enc4 = TempScale4.c_str();
    Enc4.append("_compare");
    vpxt_enc_format_append(Enc4, EncForm);
    vpxt_enc_format_append(TempScale4, EncForm);

    /////////////OutPutfile////////////
    std::string TextfileString = CurTestDirStr;
    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);

    if (TestType == COMP_ONLY || TestType == TEST_AND_COMP)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
            TextfileString.c_str());
                exit(1);
    }

    ////////////////////////////////
    //////////////////////////////////////////////////////////

    if (TestType == TEST_AND_COMP)
        print_header_full_test(argc, argv, MainTestDirChar);

    if (TestType == COMP_ONLY)
        print_header_compression_only(argc, argv, MainTestDirChar);

    if (TestType == TEST_ONLY)
        print_header_test_only(argc, argv, CurTestDirStr);

    vpxt_cap_string_print(PRINT_BTH, "%s", MyDir);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);
    unsigned int scale_compress_time = 0;
    unsigned int enc_compress_time = 0;

    ///////////////////Use Custom Settings///////////////////
    if (inputCheck == 2)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf(PRINT_BTH, "\nInput Settings file %s does not exist\n",
                argv[argc-1]);

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
    }

    /////////////////////////////////////////////////////////

    //Run Test only (Runs Test, Sets up test to be run, or skips
    //compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        //opt.Mode = Mode;
        opt.Mode = 0;
        //opt.target_bandwidth = 256;

        scale_compress_time = vpxt_compress_scalable_patterns(input.c_str(),
            TempScaleBase.c_str(), speed, opt, CompressString, 0, 0, EncForm,
            LayerMode, TempBitRate0, TempBitRate1, TempBitRate2, TempBitRate3,
            TempBitRate4);

        if (scale_compress_time == -1){
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
    }

    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    std::vector<double> TempScalePSNR;
    std::vector<double> EncPSNR;
    std::vector<double> temp_scale_fs_results;

    int delete_files_num = 0;

    if((LayerMode >= 0 && LayerMode < 2) || LayerMode == 7)
    {
        delete_files_num = 4;

        unsigned int CPUTick = 0;
        opt.target_bandwidth = TempBitRate0;
        enc_compress_time = vpxt_time_compress(input.c_str(), Enc0.c_str(), 0,
            TempBitRate0, opt, CompressString, 0, 0, CPUTick, EncForm);

        if (enc_compress_time == -1){
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        CPUTick = 0;
        opt.target_bandwidth = TempBitRate1;
        enc_compress_time = vpxt_time_compress(input.c_str(), Enc1.c_str(), 0,
            TempBitRate1, opt, CompressString, 0, 0, CPUTick, EncForm);

        if (enc_compress_time == -1){
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        EncPSNR.push_back(vpxt_psnr(input.c_str(), Enc0.c_str(), 0, PRINT_BTH,
            1, NULL));
        EncPSNR.push_back(vpxt_psnr(input.c_str(), Enc1.c_str(), 0, PRINT_BTH,
            1, NULL));

        TempScalePSNR.push_back(vpxt_psnr(input.c_str(), TempScale0.c_str(), 0,
            PRINT_BTH, 1, NULL));
        TempScalePSNR.push_back(vpxt_psnr(input.c_str(), TempScale1.c_str(), 0,
            PRINT_BTH, 1, NULL));

        vpxt_print_frame_statistics(input.c_str(), TempScale0.c_str(),
            TempScale0FS.c_str(), 0, NULL, 1, 1, 1, 1, 1, 1);
        vpxt_print_frame_statistics(input.c_str(), TempScale1.c_str(),
            TempScale1FS.c_str(), 0, NULL, 1, 1, 1, 1, 1, 1);

        temp_scale_fs_results.push_back(
            vpxt_eval_frame_stats_temp_scale(TempScale0FS.c_str(),
            (LayerMode == 0 || LayerMode == 7)? 1 : 2));
        temp_scale_fs_results.push_back(
            vpxt_eval_frame_stats_temp_scale(TempScale1FS.c_str(), 0));
    }
    if((LayerMode >= 2 && LayerMode < 6) || LayerMode == 8)
    {
        delete_files_num = 6;

        unsigned int CPUTick = 0;
        opt.target_bandwidth = TempBitRate0;
        enc_compress_time = vpxt_time_compress(input.c_str(), Enc0.c_str(),
            0, TempBitRate0, opt, CompressString, 0, 0, CPUTick, EncForm);

        if (enc_compress_time == -1){
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

         CPUTick = 0;
         opt.target_bandwidth = TempBitRate1;
        enc_compress_time = vpxt_time_compress(input.c_str(), Enc1.c_str(), 0,
            TempBitRate1, opt, CompressString, 0, 0, CPUTick, EncForm);

        if (enc_compress_time == -1){
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        CPUTick = 0;
        opt.target_bandwidth = TempBitRate2;
        enc_compress_time = vpxt_time_compress(input.c_str(), Enc2.c_str(), 0,
            TempBitRate2, opt, CompressString, 0, 0, CPUTick, EncForm);

        if (enc_compress_time == -1){
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        EncPSNR.push_back(vpxt_psnr(input.c_str(), Enc0.c_str(), 0, PRINT_BTH,
            1, NULL));
        EncPSNR.push_back(vpxt_psnr(input.c_str(), Enc1.c_str(), 0, PRINT_BTH,
            1, NULL));
        EncPSNR.push_back(vpxt_psnr(input.c_str(), Enc2.c_str(), 0, PRINT_BTH,
            1, NULL));

        TempScalePSNR.push_back(vpxt_psnr(input.c_str(), TempScale0.c_str(), 0,
            PRINT_BTH, 1, NULL));
        TempScalePSNR.push_back(vpxt_psnr(input.c_str(), TempScale1.c_str(), 0,
            PRINT_BTH, 1, NULL));
        TempScalePSNR.push_back(vpxt_psnr(input.c_str(), TempScale2.c_str(), 0,
            PRINT_BTH, 1, NULL));

        vpxt_print_frame_statistics(input.c_str(), TempScale0.c_str(),
            TempScale0FS.c_str(), 0, NULL, 1, 1, 1, 1, 1, 1);
        vpxt_print_frame_statistics(input.c_str(), TempScale1.c_str(),
            TempScale1FS.c_str(), 0, NULL, 1, 1, 1, 1, 1, 1);
        vpxt_print_frame_statistics(input.c_str(), TempScale2.c_str(),
            TempScale2FS.c_str(), 0, NULL, 1, 1, 1, 1, 1, 1);

        temp_scale_fs_results.push_back(
            vpxt_eval_frame_stats_temp_scale(TempScale0FS.c_str(),
            LayerMode == 2 ? 5 : 3));
        temp_scale_fs_results.push_back(
            vpxt_eval_frame_stats_temp_scale(TempScale1FS.c_str(),
            LayerMode == 2 ? 2 : 1));
        temp_scale_fs_results.push_back(
            vpxt_eval_frame_stats_temp_scale(TempScale2FS.c_str(), 0));
    }
    if(LayerMode == 6)
    {
        delete_files_num = 10;

        unsigned int CPUTick = 0;
        opt.target_bandwidth = TempBitRate0;
        enc_compress_time = vpxt_time_compress(input.c_str(), Enc0.c_str(), 0,
            TempBitRate0, opt, CompressString, 0, 0, CPUTick, EncForm);

        if (enc_compress_time == -1){
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

         CPUTick = 0;
         opt.target_bandwidth = TempBitRate1;
        enc_compress_time = vpxt_time_compress(input.c_str(), Enc1.c_str(), 0,
            TempBitRate1, opt, CompressString, 0, 0, CPUTick, EncForm);

        if (enc_compress_time == -1){
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        CPUTick = 0;
        opt.target_bandwidth = TempBitRate2;
        enc_compress_time = vpxt_time_compress(input.c_str(), Enc2.c_str(), 0,
            TempBitRate2, opt, CompressString, 0, 0, CPUTick, EncForm);

        if (enc_compress_time == -1){
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        CPUTick = 0;
        opt.target_bandwidth = TempBitRate3;
        enc_compress_time = vpxt_time_compress(input.c_str(), Enc3.c_str(), 0,
            TempBitRate3, opt, CompressString, 0, 0, CPUTick, EncForm);

        if (enc_compress_time == -1){
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        CPUTick = 0;
        opt.target_bandwidth = TempBitRate4;
        enc_compress_time = vpxt_time_compress(input.c_str(), Enc4.c_str(), 0,
            TempBitRate4, opt, CompressString, 0, 0, CPUTick, EncForm);

        if (enc_compress_time == -1){
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        EncPSNR.push_back(vpxt_psnr(input.c_str(), Enc0.c_str(), 0, PRINT_BTH,
            1, NULL));
        EncPSNR.push_back(vpxt_psnr(input.c_str(), Enc1.c_str(), 0, PRINT_BTH,
            1, NULL));
        EncPSNR.push_back(vpxt_psnr(input.c_str(), Enc2.c_str(), 0, PRINT_BTH,
            1, NULL));
        EncPSNR.push_back(vpxt_psnr(input.c_str(), Enc3.c_str(), 0, PRINT_BTH,
            1, NULL));
        EncPSNR.push_back(vpxt_psnr(input.c_str(), Enc4.c_str(), 0, PRINT_BTH,
            1, NULL));

        TempScalePSNR.push_back(vpxt_psnr(input.c_str(), TempScale0.c_str(), 0,
            PRINT_BTH, 1, NULL));
        TempScalePSNR.push_back(vpxt_psnr(input.c_str(), TempScale1.c_str(), 0,
            PRINT_BTH, 1, NULL));
        TempScalePSNR.push_back(vpxt_psnr(input.c_str(), TempScale2.c_str(), 0,
            PRINT_BTH, 1, NULL));
        TempScalePSNR.push_back(vpxt_psnr(input.c_str(), TempScale3.c_str(), 0,
            PRINT_BTH, 1, NULL));
        TempScalePSNR.push_back(vpxt_psnr(input.c_str(), TempScale4.c_str(), 0,
            PRINT_BTH, 1, NULL));

        vpxt_print_frame_statistics(input.c_str(), TempScale0.c_str(),
            TempScale0FS.c_str(), 0, NULL, 1, 1, 1, 1, 1, 1);
        vpxt_print_frame_statistics(input.c_str(), TempScale1.c_str(),
            TempScale1FS.c_str(), 0, NULL, 1, 1, 1, 1, 1, 1);
        vpxt_print_frame_statistics(input.c_str(), TempScale2.c_str(),
            TempScale2FS.c_str(), 0, NULL, 1, 1, 1, 1, 1, 1);
        vpxt_print_frame_statistics(input.c_str(), TempScale3.c_str(),
            TempScale3FS.c_str(), 0, NULL, 1, 1, 1, 1, 1, 1);
        vpxt_print_frame_statistics(input.c_str(), TempScale4.c_str(),
            TempScale4FS.c_str(), 0, NULL, 1, 1, 1, 1, 1, 1);

        temp_scale_fs_results.push_back(
            vpxt_eval_frame_stats_temp_scale(TempScale0FS.c_str(), 15));
        temp_scale_fs_results.push_back(
            vpxt_eval_frame_stats_temp_scale(TempScale1FS.c_str(), 7));
        temp_scale_fs_results.push_back(
            vpxt_eval_frame_stats_temp_scale(TempScale2FS.c_str(), 3));
        temp_scale_fs_results.push_back(
            vpxt_eval_frame_stats_temp_scale(TempScale3FS.c_str(), 1));
        temp_scale_fs_results.push_back(
            vpxt_eval_frame_stats_temp_scale(TempScale4FS.c_str(), 0));
    }

    tprintf(PRINT_BTH, "\n");

    int it;

    char TempScaleChar[5][256];
    vpxt_file_name(TempScale0.c_str(), TempScaleChar[0], 0);
    vpxt_file_name(TempScale1.c_str(), TempScaleChar[1], 0);
    vpxt_file_name(TempScale2.c_str(), TempScaleChar[2], 0);
    vpxt_file_name(TempScale3.c_str(), TempScaleChar[3], 0);
    vpxt_file_name(TempScale4.c_str(), TempScaleChar[4], 0);

    char EncChar[5][256];
    vpxt_file_name(Enc0.c_str(), EncChar[0], 0);
    vpxt_file_name(Enc1.c_str(), EncChar[1], 0);
    vpxt_file_name(Enc2.c_str(), EncChar[2], 0);
    vpxt_file_name(Enc3.c_str(), EncChar[3], 0);
    vpxt_file_name(Enc4.c_str(), EncChar[4], 0);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    //make sure that decemation occurs correctly
    //make sure speed is comperable to normal encode
    //make sure psnr values increase as # decimation dec
    //make sure psnr values obtain min quality assurence
    //make suer psnr values compared to standard encodes are with in range

    int fail = 0;
    int increase_fail = 0;
    int drop_frame_fail = 0;

    for ( it=0 ; it < TempScalePSNR.size()-1; it++ )
        if(TempScalePSNR[it] > TempScalePSNR[it+1])
            increase_fail = 1;

    for ( it=0 ; it < temp_scale_fs_results.size()-1; it++ )
        if(!temp_scale_fs_results[it])
            drop_frame_fail = 1;

    //psnr min threashold check
    for ( it=0 ; it < TempScalePSNR.size(); it++ )
    {
        if(TempScalePSNR[it] < 15.0)
        {
            vpxt_formated_print(RESPRT, "%s PSNR: %.2f < 15.0 - Failed",
                TempScaleChar[it], TempScalePSNR[it]);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
        if(TempScalePSNR[it] >= 15.0)
        {
            vpxt_formated_print(RESPRT, "%s PSNR: %.2f >= 15.0 - Passed",
                TempScaleChar[it], TempScalePSNR[it]);
            tprintf(PRINT_BTH, "\n");
        }
    }

    //increasing psnr check
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
        fail = 1;
    }
    //evaluate psnrs vs normal encode
    for ( it=TempScalePSNR.size() ; it > 0; it-- )
    {
        double range = 1.0;
        char *TempScaleFileChar = "";
        char *EncFileChar = "";

        if(it == TempScalePSNR.size()){
            range = 0.10;
            TempScaleFileChar = TempScaleChar[it-1];
            EncFileChar = EncChar[it-1];
        }
        if(it == TempScalePSNR.size()-1){
            range = 0.35;
            TempScaleFileChar = TempScaleChar[it-1];
            EncFileChar = EncChar[it-1];
        }
        if(it == TempScalePSNR.size()-2){
            range = 0.40;
            TempScaleFileChar = TempScaleChar[it-1];
            EncFileChar = EncChar[it-1];
        }
        if(it == TempScalePSNR.size()-3){
            range = 0.45;
            TempScaleFileChar = TempScaleChar[it-1];
            EncFileChar = EncChar[it-1];
        }
        if(it == TempScalePSNR.size()-4){
            range = 0.50;
            TempScaleFileChar = TempScaleChar[it-1];
            EncFileChar = EncChar[it-1];
        }
        // if within x% - pass
        if (TempScalePSNR[it-1] >= (EncPSNR[it-1] - (EncPSNR[it-1] * range)))
        {
            vpxt_formated_print(RESPRT, "%s PSNR: %.2f is within at least "
                "%.0f%% of %s PSNR: %.2f - Passed", TempScaleFileChar,
                TempScalePSNR[it-1], range*100, EncFileChar, EncPSNR[it-1]);
        }
        else
        {
            vpxt_formated_print(RESPRT, "%s PSNR: %.2f is not within at least "
                "%.0f%% of %s PSNR: %.2f - Failed", TempScaleFileChar,
                TempScalePSNR[it-1], range*100, EncFileChar, EncPSNR[it-1]);
            fail = 1;
        }
        tprintf(PRINT_BTH, "\n");
    }

    //frame decimation check
    if (!drop_frame_fail)
    {
        vpxt_formated_print(RESPRT, "Decemation occurs correctly for all files"
            " - Passed");
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        for ( it=0 ; it < temp_scale_fs_results.size()-1; it++ ){
            if(!temp_scale_fs_results[it]){
                vpxt_formated_print(RESPRT, "Decemation does not occur"
                    " correctly for %s - Failed", TempScaleChar[it]);
                tprintf(PRINT_BTH, "\n");
            }
        }

        fail = 1;
    }
    //if scale compress time is less than or within 20% of normal compression
    //pass otherwise fail.
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
            fail = 1;
        }

        tprintf(PRINT_BTH, "\n");
    }

    tprintf(PRINT_BTH, "\n");

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            if(delete_files_num == 4)
                vpxt_delete_files(delete_files_num, TempScale0.c_str(),
                    TempScale1.c_str(), Enc0.c_str(), Enc1.c_str());
            else if(delete_files_num == 6)
                vpxt_delete_files(delete_files_num, TempScale0.c_str(),
                    TempScale1.c_str(), TempScale2.c_str(), Enc0.c_str(),
                    Enc1.c_str(), Enc2.c_str());
            else if (delete_files_num == 10)
                vpxt_delete_files(delete_files_num, TempScale0.c_str(),
                    TempScale1.c_str(), TempScale2.c_str(), TempScale3.c_str(),
                    TempScale4.c_str(), Enc0.c_str(), Enc1.c_str(),
                    Enc2.c_str(), Enc3.c_str(), Enc4.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            if(delete_files_num == 4)
                vpxt_delete_files(delete_files_num, TempScale0.c_str(),
                    TempScale1.c_str(), Enc0.c_str(), Enc1.c_str());
            else if(delete_files_num == 6)
                vpxt_delete_files(delete_files_num, TempScale0.c_str(),
                    TempScale1.c_str(), TempScale2.c_str(), Enc0.c_str(),
                    Enc1.c_str(), Enc2.c_str());
            else if (delete_files_num == 10)
                vpxt_delete_files(delete_files_num, TempScale0.c_str(),
                    TempScale1.c_str(), TempScale2.c_str(), TempScale3.c_str(),
                    TempScale4.c_str(), Enc0.c_str(), Enc1.c_str(),
                    Enc2.c_str(), Enc3.c_str(), Enc4.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);

    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}
