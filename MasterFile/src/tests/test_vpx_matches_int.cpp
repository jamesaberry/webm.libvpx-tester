#include "vpxt_test_declarations.h"

int test_vpx_matches_int(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Allow Drop Frames";
    char *MyDir = "test_vpx_matches_int";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    std::string vpxencPath = argv[5];
    std::string vpxdecPath = argv[6];
    std::string EncForm = argv[7];
    std::string DecForm = argv[8];

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    std::string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string InternalComp = CurTestDirStr;
    InternalComp.append(slashCharStr());
    InternalComp.append(MyDir);
    InternalComp.append("_internal_compression");
    vpxt_enc_format_append(InternalComp, EncForm);

    std::string VpxEncComp = CurTestDirStr;
    VpxEncComp.append(slashCharStr());
    VpxEncComp.append(MyDir);
    VpxEncComp.append("_vpxenc_compression");
    vpxt_enc_format_append(VpxEncComp, EncForm);

    std::string InternalDec = CurTestDirStr;
    InternalDec.append(slashCharStr());
    InternalDec.append(MyDir);
    InternalDec.append("_internal_decompression");
    vpxt_dec_format_append(InternalDec, DecForm);

    std::string VpxDec = CurTestDirStr;
    VpxDec.append(slashCharStr());
    VpxDec.append(MyDir);
    VpxDec.append("_vpxenc_decompression");
    vpxt_dec_format_append(VpxDec, DecForm);

    std::string VpxencProgramString = vpxencPath;
    VpxencProgramString.append(" ");
    VpxencProgramString.append(input.c_str());
    VpxencProgramString.append(" -o ");
    VpxencProgramString.append(VpxEncComp.c_str());
    VpxencProgramString.append(" ");

    std::string VpxdecProgramString = vpxdecPath;
    VpxdecProgramString.append(" ");
    VpxdecProgramString.append(VpxEncComp.c_str());
    VpxdecProgramString.append(" -o ");
    VpxdecProgramString.append(VpxDec.c_str());

#if !defined(_WIN32)
    VpxencProgramString.insert(0, "./");
    VpxdecProgramString.insert(0, "./");
#endif

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

    /////////////////Make Sure Exe File Exists///////////////
    if (!vpxt_file_exists_check(vpxencPath.c_str()))
    {
        tprintf(PRINT_BTH, "\nInput executable %s does not exist\n", vpxencPath.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }

    /////////////////Make Sure Exe File Exists///////////////
    if (!vpxt_file_exists_check(vpxdecPath.c_str()))
    {
        tprintf(PRINT_BTH, "\nInput executable %s does not exist\n", vpxdecPath.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }

    /////////////////Make Sure ivf File Exists///////////////
    if (!vpxt_file_exists_check(input.c_str()))
    {
        tprintf(PRINT_BTH, "\nInput encode file %s does not exist\n", input.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
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
        vpxt_determinate_parameters(opt);

        if (vpxt_compress(input.c_str(), InternalComp.c_str(), speed, BitRate, opt, CompressString, 0, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        //Get internal parameter file strings
        std::string InternalCompPar1;
        int extLength = vpxt_remove_file_extension(InternalComp.c_str(), InternalCompPar1);
        InternalCompPar1.append("parameters_core.txt");
        std::string InternalCompPar2 = InternalCompPar1;
        InternalCompPar2.erase(InternalCompPar2.length() - (15 + extLength), 15 + extLength);
        InternalCompPar2.append("_parameters_vpx.txt");

        //convert internal parameters to vpxenc
        char VpxencParameters[1024];
        vpxt_convert_par_file_to_vpxenc(InternalCompPar1.c_str(), InternalCompPar2.c_str(), VpxencParameters, 1024);
        VpxencProgramString.append(VpxencParameters);

        if (EncForm.compare("ivf") == 0)
            VpxencProgramString.append("--ivf");

        //run vpxenc comp

        tprintf(PRINT_ERR, "\nAttempting to run: %s\n\n", VpxencProgramString.c_str());
        //tprintf(PRINT_STD, "\n");

        fclose(fp);

        vpxt_run_exe(VpxencProgramString);
        vpxt_run_exe(VpxdecProgramString);

        if ((fp = freopen(TextfileString.c_str(), "a", stderr)) == NULL)
        {
            tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
            exit(1);
        }

        tprintf(PRINT_ERR, "\nAttempting to run: %s\n\n", VpxdecProgramString.c_str());
        vpxt_decompress(VpxEncComp.c_str(), InternalDec.c_str(), DecForm, 1);
    }

    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    tprintf(PRINT_BTH, "\n\nComparing Compression Files: ");
    int lngRCEnc = vpxt_compare_enc(InternalComp.c_str(), VpxEncComp.c_str(), 0);
    int EncMatch = vpxt_print_compare_ivf_results(lngRCEnc, 1);

    tprintf(PRINT_BTH, "Comparing Decompression Files: ");
    int lngRCDec = vpxt_compare_dec(VpxDec.c_str(), InternalDec.c_str());
    int DecMatch = vpxt_print_compare_ivf_results(lngRCDec, 0);

    int fail = 0;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (EncMatch == 1)
    {
        vpxt_formated_print(RESPRT, "Internal Compression identical to Vpxenc Compression - Passed");
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Internal Compression not identical to Vpxenc Compression - Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (DecMatch == 1)
    {
        vpxt_formated_print(RESPRT, "Internal Decompression identical to Vpxenc Decompression - Passed");
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Internal Decompression not identical to Vpxenc Decompression - Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(4, InternalComp.c_str(), VpxEncComp.c_str(), VpxDec.c_str(), InternalDec.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(4, InternalComp.c_str(), VpxEncComp.c_str(), VpxDec.c_str(), InternalDec.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;

}