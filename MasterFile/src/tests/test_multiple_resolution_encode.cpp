#include "vpxt_test_declarations.h"

int test_multiple_resolution_encode(int argc,
                                    const char *const *argv,
                                    const std::string &WorkingDir,
                                    std::string FilesAr[],
                                    int TestType,
                                    int DeleteIVF)
{
    char *CompressString = "Allow Spatial Resampling";
    char *MyDir = "test_multiple_resolution_encode";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int BitRate = atoi(argv[3]);
    std::string EncForm = argv[4];

    unsigned int width1;
    unsigned int height1;
    unsigned int width2;
    unsigned int height2;
    unsigned int width3;
    unsigned int height3;

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    std::string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir,
        CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar,
        FilesAr) == 11)
        return 11;

    std::string MultResEnc = CurTestDirStr;
    MultResEnc.append(slashCharStr());
    MultResEnc.append(MyDir);
    
    std::string MultResEnc1 = MultResEnc;
    vpxt_get_multi_res_width_height(input.c_str(), 1, width1, height1);
    char Width1Char[10];
    vpxt_itoa_custom(width1, Width1Char, 10);
    char Height1Char[10];
    vpxt_itoa_custom(height1, Height1Char, 10);
    MultResEnc1.append("-");
    MultResEnc1.append(Width1Char);
    MultResEnc1.append("x");
    MultResEnc1.append(Height1Char);
    vpxt_enc_format_append(MultResEnc1, EncForm);
    
    std::string MultResEnc2 = MultResEnc;
    vpxt_get_multi_res_width_height(input.c_str(), 2, width2, height2);
    char Width2Char[10];
    vpxt_itoa_custom(width2, Width2Char, 10);
    char Height2Char[10];
    vpxt_itoa_custom(height2, Height2Char, 10);
    MultResEnc2.append("-");
    MultResEnc2.append(Width2Char);
    MultResEnc2.append("x");
    MultResEnc2.append(Height2Char);
    vpxt_enc_format_append(MultResEnc2, EncForm);

    std::string MultResEnc3 = MultResEnc;
    vpxt_get_multi_res_width_height(input.c_str(), 3, width3, height3);
    char Width3Char[10];
    vpxt_itoa_custom(width3, Width3Char, 10);
    char Height3Char[10];
    vpxt_itoa_custom(height3, Height3Char, 10);
    MultResEnc3.append("-");
    MultResEnc3.append(Width3Char);
    MultResEnc3.append("x");
    MultResEnc3.append(Height3Char);
    vpxt_enc_format_append(MultResEnc3, EncForm);

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

        if (vpxt_compress_multi_resolution(input.c_str(), MultResEnc.c_str(),
            speed, BitRate, opt, CompressString, 1, 0, EncForm) == -1)
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

    double psnr1 = vpxt_psnr(input.c_str(), MultResEnc1.c_str(), 0, PRINT_BTH,
        1, NULL);
    double psnr2 = vpxt_psnr(input.c_str(), MultResEnc2.c_str(), 0, PRINT_BTH,
        1, NULL);
    double psnr3 = vpxt_psnr(input.c_str(), MultResEnc3.c_str(), 0, PRINT_BTH,
        1, NULL);

    char MultResEnc1FN[256];
    char MultResEnc2FN[256];
    char MultResEnc3FN[256];

    vpxt_file_name(MultResEnc1.c_str(), MultResEnc1FN, 0);
    vpxt_file_name(MultResEnc2.c_str(), MultResEnc2FN, 0);
    vpxt_file_name(MultResEnc3.c_str(), MultResEnc3FN, 0);

    int fail = 0;

    if (psnr1 > 25.0)
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f > 25 - Passed",
            MultResEnc1FN, psnr1);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f < 25 - Failed",
            MultResEnc1FN, psnr1);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    if (psnr2 > 25.0)
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f > 25 - Passed",
            MultResEnc2FN, psnr2);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f < 25 - Failed",
            MultResEnc2FN, psnr2);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    if (psnr3 > 25.0)
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f > 25 - Passed",
            MultResEnc3FN, psnr3);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "%s PSNR: %.2f < 25 - Failed",
            MultResEnc3FN, psnr3);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (psnr1 >= psnr2 && psnr2 >= psnr3)
    {
        vpxt_formated_print(RESPRT, "%s: %.2f >= %s: %.2f >= %s: %.2f - Passed"
            , MultResEnc1FN, psnr1, MultResEnc2FN, psnr2, MultResEnc3FN, psnr3);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        if(psnr1 < psnr2 &&  psnr2 < psnr3)
        {
            vpxt_formated_print(RESPRT, "%s: %.2f < %s: %.2f < %s: %.2f - "
                "Failed", MultResEnc1FN, psnr1, MultResEnc2FN, psnr2,
                MultResEnc3FN, psnr3);
            tprintf(PRINT_BTH, "\n");
        }
        else if(psnr1 < psnr2)
        {
            vpxt_formated_print(RESPRT, "%s: %.2f < %s: %.2f >= %s: %.2f - "
               "Failed", MultResEnc1FN, psnr1, MultResEnc2FN, psnr2,
               MultResEnc3FN, psnr3);
            tprintf(PRINT_BTH, "\n");
        }
        else if(psnr2 < psnr3)
        {
            vpxt_formated_print(RESPRT, "%s: %.2f >= %s: %.2f < %s: %.2f - "
                "Failed", MultResEnc1FN, psnr1, MultResEnc2FN, psnr2,
                MultResEnc3FN, psnr3);
            tprintf(PRINT_BTH, "\n");
        }
        
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(3, MultResEnc1.c_str(), MultResEnc2.c_str(),
            MultResEnc3.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(3, MultResEnc1.c_str(), MultResEnc2.c_str(),
            MultResEnc3.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}
