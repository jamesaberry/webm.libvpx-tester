#include "vpxt_test_declarations.h"

int test_buffer_level(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "AllowDF";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  BufferLevelWorks \n\n"
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


    char WorkingDir3[255] = "";
    char *MyDir = "BufferLevelWorks";
    string MainDirString = "";
    char File1[255] = "";


    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;


    string BufferLevelWorksOut = WorkingDirString;
    //string WorkingDir5 = WorkingDirString;

    BufferLevelWorksOut.append(slashCharStr());
    BufferLevelWorksOut.append("BufferLevelWorksOutput.ivf");

    //char BufferLevelWorksOut[255];
    //snprintf(BufferLevelWorksOut, 255, "%s", WorkingDir4.c_str());

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
        print_header_full_test(argc, argv, WorkingDirString);
    }

    if (TestType == 2)
    {
        print_header_compression_only(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        print_header_test_only(argc, argv, WorkingDirString);
    }

    tprintf("Buffer Level Works Test");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

    int speed = 0;
    int StartingBufferLvl = 4;
    int MaximumBufferLevel = 6;

    //int constInt = 128;

    //int CalcBufferSize = BitRate * MaximumBufferLevel * constInt;
    //int CalcPrebuffer = BitRate * StartingBufferLvl * constInt;

    char CharCalcBufferSize[32];
    char CharCalcPrebuffer[32];
    char CharBitRate[32];

    vpx_itoa_custom(MaximumBufferLevel * 1000, CharCalcBufferSize, 10);
    vpx_itoa_custom(StartingBufferLvl * 100, CharCalcPrebuffer, 10);
    vpx_itoa_custom(BitRate, CharBitRate, 10);

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
    opt.allow_df = 1;
    int CompressInt = opt.allow_df;

    opt.target_bandwidth = BitRate;
    opt.starting_buffer_level = StartingBufferLvl;
    opt.maximum_buffer_size = MaximumBufferLevel;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress_ivf_to_ivf(input, BufferLevelWorksOut.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }
    }

    if (TestType == 2) //Create Compression only stop test short.
    {
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 10;
    }

    int PassFail = vpxt_ivf_check_pbm(BufferLevelWorksOut.c_str(), BitRate, MaximumBufferLevel * 1000, StartingBufferLvl * 1000);

    tprintf("\n\nResults:\n\n");

    if (PassFail == -11)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "No buffer under run detected - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Buffer under run at frame: %i - Failed", PassFail);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

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