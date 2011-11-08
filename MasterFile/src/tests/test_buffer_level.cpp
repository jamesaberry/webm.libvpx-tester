#include "vpxt_test_declarations.h"

int test_buffer_level(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Allow Drop Frames";
    char *MyDir = "test_buffer_level";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    std::string EncForm = argv[5];

    int speed = 0;
    int StartingBufferLvl;
    int MaximumBufferLevel;

    char CharCalcBufferSize[32];
    char CharCalcPrebuffer[32];
    char CharBitRate[32];

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string BufferLevelWorksOut = CurTestDirStr;
    BufferLevelWorksOut.append(slashCharStr());
    BufferLevelWorksOut.append(MyDir);
    BufferLevelWorksOut.append("_compression");
    vpxt_enc_format_append(BufferLevelWorksOut, EncForm);

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
    opt.allow_df = 1;
    int CompressInt = opt.allow_df;

    opt.target_bandwidth = BitRate;
    StartingBufferLvl = opt.starting_buffer_level;
    MaximumBufferLevel = opt.maximum_buffer_size;

    vpxt_itoa_custom(MaximumBufferLevel, CharCalcBufferSize, 10);
    vpxt_itoa_custom(StartingBufferLvl, CharCalcPrebuffer, 10);
    vpxt_itoa_custom(BitRate, CharBitRate, 10);

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress(input.c_str(), BufferLevelWorksOut.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
    }

    if (TestType == COMP_ONLY) //Create Compression only stop test short.
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    int PassFail = vpxt_check_pbm(BufferLevelWorksOut.c_str(), BitRate, MaximumBufferLevel, StartingBufferLvl);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (PassFail == -11)
    {
        vpxt_formated_print(RESPRT, "No buffer under run detected - Passed");
        tprintf(PRINT_BTH, "\n");

        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(1, BufferLevelWorksOut.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Buffer under run at frame: %i - Failed", PassFail);
        tprintf(PRINT_BTH, "\n");

        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(1, BufferLevelWorksOut.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}