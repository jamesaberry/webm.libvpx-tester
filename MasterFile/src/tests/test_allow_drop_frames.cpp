#include "vpxt_test_declarations.h"

int test_allow_drop_frames(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Allow Drop Frames";
    char *MyDir = "test_allow_drop_frames";
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

    std::string AllowDFon = CurTestDirStr;
    AllowDFon.append(slashCharStr());
    AllowDFon.append(MyDir);
    AllowDFon.append("_compression_1");
    vpxt_enc_format_append(AllowDFon, EncForm);

    std::string AllowDFoff = CurTestDirStr;
    AllowDFoff.append(slashCharStr());
    AllowDFoff.append(MyDir);
    AllowDFoff.append("_compression_0");
    vpxt_enc_format_append(AllowDFoff, EncForm);

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

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;
        opt.allow_df = 0;

        if (vpxt_compress(input.c_str(), AllowDFoff.c_str(), speed, BitRate, opt, CompressString, 0, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.allow_df = 1;

        if (vpxt_compress(input.c_str(), AllowDFon.c_str(), speed, BitRate, opt, CompressString, 1, 0, EncForm) == -1)
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

    int AllowDFonFrames = vpxt_display_visible_frames(AllowDFon.c_str(), 1);
    int AllowDFoffFrames = vpxt_display_visible_frames(AllowDFoff.c_str(), 1);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int fail = 0;

    if (AllowDFonFrames < AllowDFoffFrames)
    {
        vpxt_formated_print(RESPRT, "DF on frames = %i < DF off frames = %i - Passed", AllowDFonFrames, AllowDFoffFrames);
        tprintf(PRINT_BTH, "\n");
    }

    if (AllowDFoffFrames == AllowDFonFrames)
    {
        vpxt_formated_print(RESPRT, "DF on frames = %i == DF off frames = %i No effect - Failed", AllowDFonFrames, AllowDFoffFrames);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (AllowDFonFrames > AllowDFoffFrames)
    {
        vpxt_formated_print(RESPRT, "DF on frames = %i > DF off frames = %i - Failed", AllowDFonFrames, AllowDFoffFrames);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, AllowDFon.c_str(), AllowDFoff.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, AllowDFon.c_str(), AllowDFoff.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);

    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}
