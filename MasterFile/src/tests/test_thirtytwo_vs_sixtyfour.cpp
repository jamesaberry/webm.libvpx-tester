#include "vpxt_test_declarations.h"

int test_thirtytwo_vs_sixtyfour(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "ThirtyTwoVsSixtyFour";
    char *MyDir = "test_thirtytwo_vs_sixtyfour";
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

    basefolder.append(slashCharStr().c_str());
    std::string versionstring = vpx_codec_iface_name(&vpx_codec_vp8_cx_algo);
    size_t versionPos = versionstring.find("v");
    versionstring.substr(versionPos);
    basefolder.append(versionstring.substr(versionPos));
    basefolder.append("-32v64");
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

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string FiletoEnc = "";
    std::string FiletoDec = "";

    std::string Enc32 = basefolder;
    Enc32.append(MyDir);
    Enc32.append("_compression_thirty_two");
    vpxt_enc_format_append(Enc32, EncForm);

    std::string Enc64 = basefolder;
    Enc64.append(MyDir);
    Enc64.append("_compression_sixty_four");
    vpxt_enc_format_append(Enc64, EncForm);

    std::string Dec32 = basefolder;
    Dec32.append(MyDir);
    Dec32.append("_decompression_thirty_two");
    vpxt_dec_format_append(Dec32, DecForm);

    std::string Dec64 = basefolder;
    Dec64.append(MyDir);
    Dec64.append("_decompression_sixty_four");
    vpxt_dec_format_append(Dec64, DecForm);

#if ARCH_X86
    FiletoEnc = Enc32;
    FiletoDec = Dec32;

    if (vpxt_file_exists_check(Enc64) && vpxt_file_exists_check(Dec64))
    {
        TestMode = 1;
    }

#else if ARCH_X86_64
    FiletoEnc = Enc64;
    FiletoDec = Dec64;

    if (vpxt_file_exists_check(Enc32) && vpxt_file_exists_check(Dec32))
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

    BitRate = opt.target_bandwidth;

    if (TestType == TEST_ONLY)
    {

    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress(input.c_str(), FiletoEnc.c_str(), speed, BitRate, opt, "Mode", Mode, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_STD, "\n");
        fprintf(stderr, "\n\nDecompressing VP8 IVF File to IVF File: \n");

        if (vpxt_decompress(FiletoEnc.c_str(), FiletoDec.c_str(), DecForm, 1) == -1)
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

    tprintf(PRINT_BTH, "\n\nComparing %s and %s", Enc32.c_str(), Enc64.c_str());
    int Enc32VsEnc64 = vpxt_compare_enc(Enc32.c_str(), Enc64.c_str());

    if (Enc32VsEnc64 == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (Enc32VsEnc64 >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Encoded files differ at frame: %i", Enc32VsEnc64);
        ENCFAIL++;
    }

    if (Enc32VsEnc64 == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        ENCFAIL++;
    }

    if (Enc32VsEnc64 == -3)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 1 ends before File 2.\n");
        ENCFAIL++;
    }

    tprintf(PRINT_BTH, "\n\nComparing %s and %s", Dec32.c_str(), Dec64.c_str());
    int Dec32VsDec64 = vpxt_compare_dec(Dec32.c_str(), Dec64.c_str());

    if (Dec32VsDec64 == -1)
    {
        tprintf(PRINT_BTH, "\n\nFiles are Identical\n");
    }

    if (Dec32VsDec64 >= 0)
    {
        tprintf(PRINT_BTH, "\n\nFail: Decoded files differ at frame: %i", Dec32VsDec64);
        DECFAIL++;
    }

    if (Dec32VsDec64 == -2)
    {
        tprintf(PRINT_BTH, "\n\nFail: File 2 ends before File 1.\n");
        DECFAIL++;
    }

    if (Dec32VsDec64 == -3)
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
        vpxt_formated_print(RESPRT, "Not all encoded files are identical - Failed");
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
        vpxt_formated_print(RESPRT, "Not all decoded files are identical - Failed");
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
