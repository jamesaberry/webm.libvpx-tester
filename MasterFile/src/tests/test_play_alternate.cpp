#include "vpxt_test_declarations.h"

int test_play_alternate(int argc, char *argv[], const string& WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Play Alternate";
    char *MyDir = "test_play_alternate";

    if (!(argc == 6 || argc == 5))
    {
        vpxt_cap_string_print(PRINT_STD, "  %s", MyDir);
        tprintf(PRINT_STD,
                "\n\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

    int speed = 0;
    int PlayAlternate1Val = 0;
    int PlayAlternate2Val = 1;

    ////////////Formatting Test Specific Directory////////////
    string CurTestDirStr = ""; // <- All Options need to set a value for this
    string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    char playaltbuff[255];

    string PlayAlternate1 = CurTestDirStr;
    PlayAlternate1.append(slashCharStr());
    PlayAlternate1.append(MyDir);
    PlayAlternate1.append("_compression_");
    PlayAlternate1.append(vpxt_itoa_custom(PlayAlternate1Val, playaltbuff, 10));
    PlayAlternate1.append(".ivf");

    string PlayAlternate2 = CurTestDirStr;
    PlayAlternate2.append(slashCharStr());
    PlayAlternate2.append(MyDir);
    PlayAlternate2.append("_compression_");
    PlayAlternate2.append(vpxt_itoa_custom(PlayAlternate2Val, playaltbuff, 10));
    PlayAlternate2.append(".ivf");

    /////////////OutPutfile////////////
    string TextfileString = CurTestDirStr;
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
    if (argc == 6)
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
    opt.allow_lag = 1;
    opt.lag_in_frames = 10;
    opt.end_usage = 1;

    //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
    //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
    //Test Type 3 = Mode 2 =Run Test Compressions

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;
        opt.play_alternate = PlayAlternate1Val;

        if (vpxt_compress_ivf_to_ivf(input, PlayAlternate1.c_str(), speed, BitRate, opt, CompressString, PlayAlternate1Val, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.play_alternate = PlayAlternate2Val;

        if (vpxt_compress_ivf_to_ivf(input, PlayAlternate2.c_str(), speed, BitRate, opt, CompressString, PlayAlternate2Val, 0, 0, 3, 3) == -1)
        {
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

    int lngRC = vpxt_compare_ivf(PlayAlternate2.c_str(), PlayAlternate1.c_str());

    int fail = 0;

    int PlayAlternateOnAltRefCount = vpxt_display_alt_ref_frames(PlayAlternate2.c_str(), 1);
    int PlayAlternateOffAltRefCount = vpxt_display_alt_ref_frames(PlayAlternate1.c_str(), 1);
    int VisibleFrameONCount = vpxt_display_visible_frames(PlayAlternate2.c_str(), 1);
    int VisibleFrameOFFCount = vpxt_display_visible_frames(PlayAlternate1.c_str(), 1);

    char PlayAlternateOnFilename[255];
    vpxt_file_name(PlayAlternate2.c_str(), PlayAlternateOnFilename, 0);
    char PlayAlternateOffFilename[255];
    vpxt_file_name(PlayAlternate1.c_str(), PlayAlternateOffFilename, 0);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (PlayAlternateOnAltRefCount > 0)
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames exist for %s - Passed", PlayAlternateOnFilename);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames do not exist for %s - Failed", PlayAlternateOnFilename);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (PlayAlternateOffAltRefCount > 0)
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames exist for %s - Failed", PlayAlternateOffFilename);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Alternate reference frames do not exist for %s - Passed", PlayAlternateOffFilename);
        tprintf(PRINT_BTH, "\n");
    }

    if (VisibleFrameONCount == VisibleFrameOFFCount)
    {
        vpxt_formated_print(RESPRT, "Visible Frames for %s = %i == Visible Frames for %s = %i - Passed", PlayAlternateOnFilename, VisibleFrameONCount, PlayAlternateOffFilename, VisibleFrameOFFCount);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Visible Frames for %s = %i != Visible Frames for %s = %i - Failed", PlayAlternateOnFilename, VisibleFrameONCount, PlayAlternateOffFilename, VisibleFrameOFFCount);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (lngRC >= 0)
    {
        vpxt_formated_print(RESPRT, "Files are not identical - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (lngRC == -1)
    {
        vpxt_formated_print(RESPRT, "Files are identical - Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}