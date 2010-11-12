#include "vpxt_test_declarations.h"

int test_allow_drop_frames(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Allow Drop Frames";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  AllowDF \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "    <Optional Settings File>\n"
        );

        return 0;
    }

    ////////////Formatting Test Specific Directory////////////

    string WorkingDirString = "";
    string MainDirString = "";
    char *MyDir = "AllowDF";
    char WorkingDir3[255] = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string AllowDFon = WorkingDirString;
    string AllowDFoff = WorkingDirString;

    AllowDFon.append(slashCharStr());
    AllowDFon.append("AllowDFOnOutput.ivf");
    AllowDFoff.append(slashCharStr());
    AllowDFoff.append("AllowDFOffOutput.ivf");

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
        print_header_full_test(argc, argv, WorkingDir3);

    if (TestType == 2)
        print_header_compression_only(argc, argv, WorkingDir3);

    if (TestType == 3)
        print_header_test_only(argc, argv, WorkingDirString);

    int speed = 0;
    int BitRate = atoi(argv[4]);;
    int Mode = atoi(argv[3]);

    tprintf("Allow Drop Frames Test");

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

    //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
    //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
    //Test Type 3 = Mode 2 = Run Test Compressions

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;
        opt.allow_df = 0;

        if (vpxt_compress_ivf_to_ivf(input, AllowDFoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.allow_df = 1;

        if (vpxt_compress_ivf_to_ivf(input, AllowDFon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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

    int AllowDFonFrames = vpxt_display_visible_frames(AllowDFon.c_str(), 1);
    int AllowDFoffFrames = vpxt_display_visible_frames(AllowDFoff.c_str(), 1);

    tprintf("\n\nResults:\n\n");

    int fail = 0;

    if (AllowDFonFrames < AllowDFoffFrames)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DF on frames = %i < DF off frames = %i - Passed", AllowDFonFrames, AllowDFoffFrames);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (AllowDFoffFrames == AllowDFonFrames)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DF on frames = %i == DF off frames = %i No effect - Failed", AllowDFonFrames, AllowDFoffFrames);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (AllowDFonFrames > AllowDFoffFrames)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DF on frames = %i > DF off frames = %i - Failed", AllowDFonFrames, AllowDFoffFrames);
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