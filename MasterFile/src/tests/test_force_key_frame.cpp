#include "vpxt_test_declarations.h"

int test_force_key_frame(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Key Frame Frequency";
    char *MyDir = "test_force_key_frame";

    if (!(argc == 6 || argc == 7))
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
                "    <ForceKeyFrame>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int ForceKeyFrameInt = atoi(argv[5]);

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string ForceKeyFrame = CurTestDirStr;
    ForceKeyFrame.append(slashCharStr());
    ForceKeyFrame.append(MyDir);
    ForceKeyFrame.append("_compression.ivf");

    std::string KeyFrameoutputfile = CurTestDirStr;
    KeyFrameoutputfile.append(slashCharStr());
    KeyFrameoutputfile.append(MyDir);
    KeyFrameoutputfile.append("_compression_key_frames.txt");

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
    if (argc == 7)
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
    opt.auto_key = 0;//1;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;
        opt.key_freq = 0;//ForceKeyFrameInt;

        if (vpxt_compress_ivf_to_ivf_force_key_frame(input.c_str(), ForceKeyFrame.c_str(), speed, BitRate, opt, CompressString, ForceKeyFrameInt, 0, ForceKeyFrameInt) == -1)
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

    vpxt_display_key_frames(ForceKeyFrame.c_str(), 1);

    int fail = vpxt_check_force_key_frames(KeyFrameoutputfile.c_str(), ForceKeyFrameInt, ForceKeyFrame.c_str());

    if (fail == 1)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, ForceKeyFrame.c_str(), KeyFrameoutputfile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Key Frames occur only when Force Key Frame dictates: %i - Passed", ForceKeyFrameInt);
        tprintf(PRINT_BTH, "\n");

        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, ForceKeyFrame.c_str(), KeyFrameoutputfile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;

}