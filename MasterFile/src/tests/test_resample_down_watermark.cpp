#include "vpxt_test_declarations.h"

int test_resample_down_watermark(int argc,
                                 const char *const *argv,
                                 const std::string &WorkingDir,
                                 std::string FilesAr[],
                                 int TestType,
                                 int DeleteIVF)
{
    char *CompressString = "Resample Down Watermark";
    char *MyDir = "test_resample_down_watermark";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    std::string EncForm = argv[5];

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir,
        CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar,
        FilesAr) == 11)
        return 11;

    std::string DownWaterSamp90OutFile = CurTestDirStr;
    DownWaterSamp90OutFile.append(slashCharStr());
    DownWaterSamp90OutFile.append(MyDir);
    DownWaterSamp90OutFile.append("_compression_90");
    vpxt_enc_format_append(DownWaterSamp90OutFile, EncForm);

    std::string DownWaterSamp10OutFile = CurTestDirStr;
    DownWaterSamp10OutFile.append(slashCharStr());
    DownWaterSamp10OutFile.append(MyDir);
    DownWaterSamp10OutFile.append("_compression_20");
    vpxt_enc_format_append(DownWaterSamp10OutFile, EncForm);

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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////
    opt.allow_lag = 0;
    opt.allow_spatial_resampling = 1;
    opt.end_usage = 0;
    opt.target_bandwidth = BitRate;

    int ResampleDownWaterMarkHigh = 90;
    int ResampleDownWaterMarkLow = 20;

    int n = 0;
    double PSNRArr[2];

    opt.resample_up_water_mark = 100;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.resample_down_water_mark = ResampleDownWaterMarkHigh;

        if (vpxt_compress(input.c_str(), DownWaterSamp90OutFile.c_str(), speed,
            BitRate, opt, CompressString, opt.resample_down_water_mark, 0,
            EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.resample_down_water_mark = ResampleDownWaterMarkLow;

        if (vpxt_compress(input.c_str(), DownWaterSamp10OutFile.c_str(), speed,
            BitRate, opt, CompressString, opt.resample_down_water_mark, 0,
            EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
    }

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    char DownWaterSamp10Filename[255];
    vpxt_file_name(DownWaterSamp10OutFile.c_str(), DownWaterSamp10Filename, 0);
    char DownWaterSamp90Filename[255];
    vpxt_file_name(DownWaterSamp90OutFile.c_str(), DownWaterSamp90Filename, 0);

    int DispKeyFrames10int =
        vpxt_display_key_frames(DownWaterSamp10OutFile.c_str(), 1);
    int DispResized10int =
        vpxt_display_resized_frames(DownWaterSamp10OutFile.c_str(), 1);
    int DispCheckPBMThr10int =
        vpxt_check_pbm_threshold(DownWaterSamp10OutFile.c_str(),
        opt.target_bandwidth, opt.maximum_buffer_size * 1000,
        opt.starting_buffer_level * 1000, opt.optimal_buffer_level * 1000,
        ResampleDownWaterMarkLow);
    int RDWMCheck10int = 0;

    if (DispResized10int > 0)
    {
        tprintf(PRINT_STD, "\nChecking %s buffer for correct resize frame "
            "placement:\n\n", DownWaterSamp10Filename);
        RDWMCheck10int = vpxt_dfwm_check(DownWaterSamp10OutFile.c_str(), 1);
    }
    else
    {
        RDWMCheck10int = -2;
    }

    int DispKeyFrames90int =
        vpxt_display_key_frames(DownWaterSamp90OutFile.c_str(), 1);
    int DispResized90int =
        vpxt_display_resized_frames(DownWaterSamp90OutFile.c_str(), 1);
    int DispCheckPBMThr90int =
        vpxt_check_pbm_threshold(DownWaterSamp90OutFile.c_str(),
        opt.target_bandwidth, opt.maximum_buffer_size * 1000,
        opt.starting_buffer_level * 1000, opt.optimal_buffer_level * 1000,
        ResampleDownWaterMarkHigh);
    int RDWMCheck90int = -3;

    if (DispResized90int > 0)
    {
        tprintf(PRINT_STD, "\nChecking %s buffer for correct resize frame "
            "placement:\n\n", DownWaterSamp90Filename);
        RDWMCheck90int = vpxt_dfwm_check(DownWaterSamp90OutFile.c_str(), 1);
    }
    else
    {
        RDWMCheck90int = -2;
    }

    //1 = failed // 2 = indt // 3 = track resize for 10 // track resize for 90
    int fail = 0;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (DispResized10int > 0 && DispResized90int > 0)
    {
        vpxt_formated_print(RESPRT, "Both DWMS 10 and 90 returned resized "
            "frames - Indeterminate");
        tprintf(PRINT_BTH, "\n");
    }

    if (DispResized10int == 0 && DispResized90int > 0)
    {
        vpxt_formated_print(RESPRT, "DWMS 10 returned no resized frames; DWMS "
            "90 returned resized frames - Indeterminate");
        tprintf(PRINT_BTH, "\n");
        fail = 4;
        //indt
    }

    if (DispResized10int > 0 && DispResized90int == 0)
    {
        vpxt_formated_print(RESPRT, "DWMS 90 returned no resized frames; DWMS "
            "10 returned resized frames - Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
        //fail
    }

    if (DispResized10int == 0 && DispResized90int == 0)
    {
        vpxt_formated_print(RESPRT, "Both DWMS 10 and 90 returned  no resized "
            "frames - Indeterminate");
        tprintf(PRINT_BTH, "\n");
        fail = 2;
    }

    if (RDWMCheck10int == 0)
    {
        vpxt_formated_print(RESPRT, "DWMS 10 resizes first frame at correct "
            "buffer location - Passed");
        tprintf(PRINT_BTH, "\n");
        //fail
    }

    if (RDWMCheck10int == 1)
    {
        vpxt_formated_print(RESPRT, "DWMS 10 does not resize first frame at "
            "correct buffer location - Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
        //fail
    }

    if (RDWMCheck10int == -3)
    {
        vpxt_formated_print(RESPRT, "DWMS 10 buffer threshold never reached - "
            "Indeterminate");
        tprintf(PRINT_BTH, "\n");
        fail = 2;
        //fail
    }

    if (RDWMCheck90int == 0)
    {
        if (fail == 4)
        {
            vpxt_formated_print(RESPRT, "DWMS 90 resizes first frame at "
                "correct buffer location - Passed");
            tprintf(PRINT_BTH, "\n");
            fail = 3;
            //fail
        }
        else
        {
            vpxt_formated_print(RESPRT, "DWMS 90 resizes first frame at "
                "correct buffer location - Passed");
            tprintf(PRINT_BTH, "\n");
            //fail
        }
    }

    if (RDWMCheck90int == 1)
    {
        vpxt_formated_print(RESPRT, "DWMS 90 does not resize first frame at "
            "correct buffer location - Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 2;
        //fail
    }

    if (RDWMCheck90int == -3)
    {
        vpxt_formated_print(RESPRT, "DWMS 90 buffer threshold never reached - "
            "Indeterminate");
        tprintf(PRINT_BTH, "\n");
        fail = 2;
        //fail
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, DownWaterSamp90OutFile.c_str(),
            DownWaterSamp10OutFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }

    if (fail == 3)
    {
        tprintf(PRINT_BTH, "\nMin Passed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, DownWaterSamp90OutFile.c_str(),
            DownWaterSamp10OutFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 8;
    }

    if (fail == 2)
    {
        tprintf(PRINT_BTH, "\nIndeterminate\n");

        if (DeleteIVF)
            vpxt_delete_files(2, DownWaterSamp90OutFile.c_str(),
            DownWaterSamp10OutFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, DownWaterSamp90OutFile.c_str(),
            DownWaterSamp10OutFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}