#include "vpxt_test_declarations.h"

int test_error_concealment(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Error Resilient Mode";
    char *MyDir = "test_error_concealment";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    std::string EncForm = argv[5];
    std::string DecForm = argv[6];

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string ErrConComp = CurTestDirStr;
    ErrConComp.append(slashCharStr());
    ErrConComp.append(MyDir);
    ErrConComp.append("_compression");
    vpxt_enc_format_append(ErrConComp, EncForm);

    std::string ErrConDec = CurTestDirStr;
    ErrConDec.append(slashCharStr());
    ErrConDec.append(MyDir);
    ErrConDec.append("_decompression");
    vpxt_dec_format_append(ErrConDec, DecForm);

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

    vpxt_cap_string_print(PRINT_BTH, "%s\n", MyDir);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt.target_bandwidth = BitRate;

    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.error_resilient_mode = 1;

        if (vpxt_compress(input.c_str(), ErrConComp.c_str(), speed, BitRate, opt, CompressString, opt.error_resilient_mode, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        if (vpxt_decompress_partial_drops(ErrConComp.c_str(), ErrConDec.c_str(), DecForm, 1, 3, 5, 2, PRINT_BTH) == -1)
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

    tprintf(PRINT_BTH, "\n");

    double PSNRClean;
    double PSNRDrops;

    PSNRClean = vpxt_psnr(input.c_str(), ErrConComp.c_str(), 0, 0, 1, NULL);
    PSNRDrops = vpxt_psnr_dec(input.c_str(), ErrConDec.c_str(), 0, 0, 1, NULL, 0, 0);

    //tprintf(PRINT_BTH, "\n PSNRClean: %f PSNRDrops: %f\n",PSNRClean,PSNRDrops);

    float PSRNPerc = 100 * vpxt_abs_float((PSNRClean - PSNRDrops) / PSNRDrops);

    int passed = 1;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (PSRNPerc < 50.00)
    {
        vpxt_formated_print(RESPRT, "No Partial Drop PSNR: %.2f is within 50%% of Partial Drop PSNR: %.2f - %.2f%% - Passed", PSNRClean, PSNRDrops, PSRNPerc);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "No Partial Drop PSNR: %.2f is not within 50%% of Partial Drop PSNR: %.2f - %.2f%% - Failed", PSNRClean, PSNRDrops, PSRNPerc);
        tprintf(PRINT_BTH, "\n");
        passed = 0;
    }

    if (PSNRDrops > 20.00)
    {
        vpxt_formated_print(RESPRT, "Partial Drop PSNR is greater than 20.00: %.2f - Passed", PSNRDrops);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Partial Drop PSNR is not greater than 20.00: %.2f - Failed", PSNRDrops);
        tprintf(PRINT_BTH, "\n");
        passed = 0;
    }

    if (passed == 1)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, ErrConComp.c_str(), ErrConDec.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, ErrConComp.c_str(), ErrConDec.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}