#include "vpxt_test_declarations.h"

int test_allow_spatial_resampling(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Allow Spatial Resampling";
    char *MyDir = "test_allow_spatial_resampling";
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
    std::string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string Spatialon = CurTestDirStr;
    Spatialon.append(slashCharStr());
    Spatialon.append(MyDir);
    Spatialon.append("_compression_1");
    vpxt_enc_format_append(Spatialon, EncForm);

    std::string Spatialoff = CurTestDirStr;
    Spatialoff.append(slashCharStr());
    Spatialoff.append(MyDir);
    Spatialoff.append("_compression_0");
    vpxt_enc_format_append(Spatialoff, EncForm);

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
        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
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
            tprintf(PRINT_BTH, "\nInput Settings file %s does not exist\n", argv[argc-1]);

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    opt.end_usage = 0;
    opt.resample_down_water_mark = 60;
    opt.resample_up_water_mark = 80;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;
        opt.allow_spatial_resampling = 0;

        if (vpxt_compress(input.c_str(), Spatialoff.c_str(), speed, BitRate, opt, CompressString, 0, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.allow_spatial_resampling = 1;

        if (vpxt_compress(input.c_str(), Spatialon.c_str(), speed, BitRate, opt, CompressString, 1, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
    }

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    double SpatialResampPSNR = vpxt_psnr(input.c_str(), Spatialon.c_str(), 0, 0, 1, NULL);

    char SpatialonFileName[255];
    vpxt_file_name(Spatialon.c_str(), SpatialonFileName, 0);
    char SpatialoffFileName[255];
    vpxt_file_name(Spatialoff.c_str(), SpatialoffFileName, 0);

    tprintf(PRINT_BTH, "\nChecking: %s for resized frames\n", SpatialonFileName);
    int AllowSpatResampleONFramesResized = vpxt_display_resized_frames(Spatialon.c_str(), 1);

    tprintf(PRINT_BTH, "Checking: %s for resized frames\n", SpatialoffFileName);
    int AllowSpatResampleOFFFramesResized = vpxt_display_resized_frames(Spatialoff.c_str(), 1);

    int fail = 0;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (AllowSpatResampleONFramesResized > 0)
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResampleOn Frames Resized %i > 0 - Passed", AllowSpatResampleONFramesResized);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResampleOn Frames Resized %i <= 0 - Failed", AllowSpatResampleONFramesResized);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (AllowSpatResampleOFFFramesResized == 0)
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResampleOff Frames Resized %i == 0 - Passed", AllowSpatResampleOFFFramesResized);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResampleOff Frames Resized %i != 0 - Failed", AllowSpatResampleOFFFramesResized);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (SpatialResampPSNR > 15.0)
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResample PSNR: %f > 15.00 - Passed", SpatialResampPSNR);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "AllowSpatialResample On PSNR: %f < 15.00 - Failed", SpatialResampPSNR);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, Spatialon.c_str(), Spatialoff.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, Spatialon.c_str(), Spatialoff.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}
