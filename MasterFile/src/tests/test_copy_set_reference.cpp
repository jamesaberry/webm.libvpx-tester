#include "vpxt_test_declarations.h"

int test_copy_set_reference(int argc,
                            const char *const *argv,
                            const std::string &WorkingDir,
                            std::string FilesAr[],
                            int TestType,
                            int DeleteIVF)
{
    char *CompressString = "Error Resilient";
    char *MyDir = "test_copy_set_reference";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int FirstCopyFrame = atoi(argv[5]);
    std::string EncForm = argv[6];
    std::string DecForm = argv[7];

    int speed = 0;
    int pass = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    std::string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir,
        CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar,
        FilesAr) == 11)
        return 11;

    std::string CopySetComp = CurTestDirStr;
    CopySetComp.append(slashCharStr());
    CopySetComp.append(MyDir);
    CopySetComp.append("_compression");
    vpxt_enc_format_append(CopySetComp, EncForm);

    std::string CopySetDec = CurTestDirStr;
    CopySetDec.append(slashCharStr());
    CopySetDec.append(MyDir);
    CopySetDec.append("_decompression");
    vpxt_dec_format_append(CopySetDec, DecForm);

    std::string CopySetDec2 = CurTestDirStr;
    CopySetDec2.append(slashCharStr());
    CopySetDec2.append(MyDir);
    CopySetDec2.append("_decompression_clone");
    vpxt_dec_format_append(CopySetDec2, DecForm);

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

    opt.target_bandwidth = BitRate;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    //files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;
        opt.error_resilient_mode = 1;

        if (vpxt_compress(input.c_str(), CopySetComp.c_str(), speed, BitRate,
            opt, CompressString, opt.error_resilient_mode, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        pass = vpxt_decompress_copy_set(CopySetComp.c_str(), CopySetDec.c_str(),
            CopySetDec2.c_str(), DecForm, 1, FirstCopyFrame, PRINT_BTH);

    }

    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (pass == 0)
    {
        vpxt_formated_print(RESPRT, "Not all cloned decompression frames "
            "identical to non cloned decompression frames - Failed");
        tprintf(PRINT_BTH, "\n");
    }

    if (pass == 1)
    {
        vpxt_formated_print(RESPRT, "All cloned decompression frames identical "
            "to non cloned decompression frames - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (pass == 2)
    {
        vpxt_formated_print(RESPRT, "First clone frame not reached - "
            "Indeterminate");
        tprintf(PRINT_BTH, "\n");
    }

    if (pass == 1)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(3, CopySetComp.c_str(), CopySetDec.c_str(),
            CopySetDec2.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }

    if (pass == 2)
    {
        tprintf(PRINT_BTH, "\nIndeterminate\n");

        if (DeleteIVF)
            vpxt_delete_files(3, CopySetComp.c_str(), CopySetDec.c_str(),
            CopySetDec2.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(3, CopySetComp.c_str(), CopySetDec.c_str(),
            CopySetDec2.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);

    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}
