#include "vpxt_test_declarations.h"

int test_win_lin_mac_match(int argc,
                           const char *const *argv,
                           const std::string &WorkingDir,
                           std::string FilesAr[],
                           int TestType,
                           int DeleteIVF)
{
    char *CompressString = "WinLinMacMatch";
    char *MyDir = "test_win_lin_mac_match";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    std::string basefolder = argv[5];
    std::string ParameterFile = argv[6];
    std::string EncForm = argv[7];
    std::string DecForm = argv[8];

    int speed = 0;

    std::string arch = "Unknown";
#if ARCH_X86
    arch = "-32Bit";
#elif ARCH_X86_64
    arch = "-64Bit";
#elif HAVE_ARMV6
    arch = "-Arm6";
#elif HAVE_ARMV7
    arch = "-Arm7";
#endif

    basefolder.append(slashCharStr().c_str());
    std::string versionstring = vpx_codec_iface_name(&vpx_codec_vp8_cx_algo);
    size_t versionPos = versionstring.find("v");
    versionstring.substr(versionPos);
    basefolder.append(versionstring.substr(versionPos));
    basefolder.append(arch.c_str());
    basefolder.append(slashCharStr().c_str());
    basefolder.append("Mode");
    basefolder.append(argv[3]);
    basefolder.append(slashCharStr().c_str());
    vpxt_make_dir(basefolder);
    int TestMode = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir,
        CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar,
        FilesAr) == 11)
        return 11;

    std::string FiletoEnc = "";
    std::string FiletoDec = "";

    std::string WinEnc = basefolder;
    WinEnc.append(MyDir);
    WinEnc.append("_compression_win");
    vpxt_enc_format_append(WinEnc, EncForm);

    std::string LinEnc = basefolder;
    LinEnc.append(MyDir);
    LinEnc.append("_compression_lin");
    vpxt_enc_format_append(LinEnc, EncForm);

    std::string MacEnc = basefolder;
    MacEnc.append(MyDir);
    MacEnc.append("_compression_mac");
    vpxt_enc_format_append(MacEnc, EncForm);

    std::string WinDec = basefolder;
    WinDec.append(MyDir);
    WinDec.append("_decompression_win");
    vpxt_dec_format_append(WinDec, DecForm);

    std::string LinDec = basefolder;
    LinDec.append(MyDir);
    LinDec.append("_decompression_lin");
    vpxt_dec_format_append(LinDec, DecForm);

    std::string MacDec = basefolder;
    MacDec.append(MyDir);
    MacDec.append("_decompression_mac");
    vpxt_dec_format_append(MacDec, DecForm);

#if defined(linux)
    FiletoEnc = LinEnc;
    FiletoDec = LinDec;

    if (vpxt_file_exists_check(WinEnc) && vpxt_file_exists_check(WinDec) &&
        vpxt_file_exists_check(MacEnc) && vpxt_file_exists_check(MacDec))
    {
        TestMode = 1;
    }

#elif defined(_WIN32)
    FiletoEnc = WinEnc;
    FiletoDec = WinDec;

    if (vpxt_file_exists_check(LinEnc) && vpxt_file_exists_check(LinDec) &&
        vpxt_file_exists_check(MacEnc) && vpxt_file_exists_check(MacDec))
    {
        TestMode = 1;
    }

#elif defined(__APPLE__)
    FiletoEnc = MacEnc;
    FiletoDec = MacDec;

    if (vpxt_file_exists_check(WinEnc) && vpxt_file_exists_check(WinDec) &&
        vpxt_file_exists_check(LinEnc) && vpxt_file_exists_check(LinDec))
    {
        TestMode = 1;
    }

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

    BitRate = opt.target_bandwidth;

    if (TestType == TEST_ONLY)
    {

    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress(input.c_str(), FiletoEnc.c_str(), speed, BitRate, opt,
            "Mode", Mode, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_STD, "\n");
        fprintf(stderr, "\n\nDecompressing VP8 IVF File to IVF File: \n");

        if (vpxt_decompress(FiletoEnc.c_str(), FiletoDec.c_str(), DecForm, 1) ==
            -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

    }

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        //Compression only run
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    if (TestMode == 0)
    {
        tprintf(PRINT_BTH, "\n\nResults:\n\n");

        vpxt_formated_print(RESPRT, "Test files created.");

        tprintf(PRINT_BTH, "\n\nIndeterminate\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 2;
    }

    int ENCFAIL = 0;
    int DECFAIL = 0;

    tprintf(PRINT_BTH, "\n\nComparing %s and %s",WinEnc.c_str(),LinEnc.c_str());
    int WinEncVsLinEnc = vpxt_compare_enc(WinEnc.c_str(), LinEnc.c_str(), 0);

    if (WinEncVsLinEnc == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (WinEncVsLinEnc >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Encoded files differ at frame: %i",
            WinEncVsLinEnc);
        ENCFAIL++;
    }

    if (WinEncVsLinEnc == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        ENCFAIL++;
    }

    if (WinEncVsLinEnc == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        ENCFAIL++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s",LinEnc.c_str(),MacEnc.c_str());
    int LinEncVsMacEnc = vpxt_compare_enc(LinEnc.c_str(), MacEnc.c_str(), 0);

    if (LinEncVsMacEnc == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (LinEncVsMacEnc >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Encoded files differ at frame: %i",
            LinEncVsMacEnc);
        ENCFAIL++;
    }

    if (LinEncVsMacEnc == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        ENCFAIL++;
    }

    if (LinEncVsMacEnc == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        ENCFAIL++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s",WinEnc.c_str(),MacEnc.c_str());
    int WinEncVsMacEnc = vpxt_compare_enc(WinEnc.c_str(), MacEnc.c_str(), 0);

    if (WinEncVsMacEnc == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (WinEncVsMacEnc >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Encoded files differ at frame: %i",
            WinEncVsMacEnc);
        ENCFAIL++;
    }

    if (WinEncVsMacEnc == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        ENCFAIL++;
    }

    if (WinEncVsMacEnc == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        ENCFAIL++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s",WinDec.c_str(),LinDec.c_str());
    int WinDecVsLinDec = vpxt_compare_enc(WinDec.c_str(), LinDec.c_str(), 0);

    if (WinDecVsLinDec == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (WinDecVsLinDec >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Decoded files differ at frame: %i",
            WinDecVsLinDec);
        DECFAIL++;
    }

    if (WinDecVsLinDec == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        DECFAIL++;
    }

    if (WinDecVsLinDec == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        DECFAIL++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s",LinDec.c_str(),MacDec.c_str());
    int LinDecVsMacDec = vpxt_compare_enc(LinDec.c_str(), MacDec.c_str(), 0);

    if (LinDecVsMacDec == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (LinDecVsMacDec >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Decoded files differ at frame: %i",
            LinDecVsMacDec);
        DECFAIL++;
    }

    if (LinDecVsMacDec == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        DECFAIL++;
    }

    if (LinDecVsMacDec == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        DECFAIL++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s",WinDec.c_str(),MacDec.c_str());
    int WinDecVsMacDec = vpxt_compare_enc(WinDec.c_str(), MacDec.c_str(), 0);

    if (WinDecVsMacDec == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (WinDecVsMacDec >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Decoded files differ at frame: %i",
            WinDecVsMacDec);
        DECFAIL++;
    }

    if (WinDecVsMacDec == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        DECFAIL++;
    }

    if (WinDecVsMacDec == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        DECFAIL++;
    }

    tprintf(PRINT_BTH, "\n\nResults:\n\n");
    int fail = 0;

    if (ENCFAIL == 0)
    {
        vpxt_formated_print(RESPRT, "All encoded files are identical - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (ENCFAIL > 0)
    {
        vpxt_formated_print(RESPRT, "Not all encoded files are identical - "
            "Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (DECFAIL == 0)
    {
        vpxt_formated_print(RESPRT, "All decoded files are identical - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (DECFAIL > 0)
    {
        vpxt_formated_print(RESPRT, "Not all decoded files are identical - "
            "Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (fail == 1)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }
    else
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}
