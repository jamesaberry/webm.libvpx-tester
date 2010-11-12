#include "vpxt_test_declarations.h"

int test_play_alternate(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "PlayAlternate";

    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  PlayAlternate \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Optional Settings File>\n"
        );

        return 0;
    }

    ////////////Formatting Test Specific Directory////////////

    string WorkingDirString = ""; // <- All Options need to set a value for this

    string MainDirString = "";
    char *MyDir = "PlayAlternate";

    char WorkingDir3[255] = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string PlayAlternate1 = WorkingDirString;
    string PlayAlternate2 = WorkingDirString;

    PlayAlternate1.append(slashCharStr());
    PlayAlternate1.append("PlayAlternate1Output.ivf");
    PlayAlternate2.append(slashCharStr());
    PlayAlternate2.append("PlayAlternate2Output.ivf");

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");


    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    //////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        print_header_full_test(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        print_header_compression_only(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        print_header_test_only(argc, argv, WorkingDirString);
    }

    int speed = 0;
    int BitRate = atoi(argv[4]);;

    int Mode = atoi(argv[3]);

    int PlayAlternate1Val = 0;
    int PlayAlternate2Val = 1;

    tprintf("Play Alternate Test");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
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
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.play_alternate = PlayAlternate1Val;

        if (vpxt_compress_ivf_to_ivf(input, PlayAlternate1.c_str(), speed, BitRate, opt, CompressString, PlayAlternate1Val, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.play_alternate = PlayAlternate2Val;

        if (vpxt_compress_ivf_to_ivf(input, PlayAlternate2.c_str(), speed, BitRate, opt, CompressString, PlayAlternate2Val, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }
    }

    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
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

    tprintf("\n\nResults:\n\n");

    if (PlayAlternateOnAltRefCount > 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Alternate reference frames exist for %s - Passed", PlayAlternateOnFilename);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Alternate reference frames do not exist for %s - Failed", PlayAlternateOnFilename);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (PlayAlternateOffAltRefCount > 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Alternate reference frames exist for %s - Failed", PlayAlternateOffFilename);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Alternate reference frames do not exist for %s - Passed", PlayAlternateOffFilename);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (VisibleFrameONCount == VisibleFrameOFFCount)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Visible Frames for %s = %i == Visible Frames for %s = %i - Passed", PlayAlternateOnFilename, VisibleFrameONCount, PlayAlternateOffFilename, VisibleFrameOFFCount);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Visible Frames for %s = %i != Visible Frames for %s = %i - Failed", PlayAlternateOnFilename, VisibleFrameONCount, PlayAlternateOffFilename, VisibleFrameOFFCount);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (lngRC >= 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Files are not identical - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (lngRC == -1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Files are identical - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        tprintf("\nFailed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    fclose(fp);
    string File1Str = File1;
    record_test_complete(MainDirString, File1Str, TestType);
    return 6;
}