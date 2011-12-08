#include "vpxt_test_declarations.h"

int test_max_quantizer(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Max Quantizer";
    char *MyDir = "test_max_quantizer";
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
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string QuantOutBase = CurTestDirStr;
    QuantOutBase.append(slashCharStr());
    QuantOutBase.append(MyDir);
    QuantOutBase.append("_compression_");

    std::string QuantOut3 = QuantOutBase;
    QuantOut3.append("4");
    vpxt_enc_format_append(QuantOut3, EncForm);
    std::string QuantOut11 = QuantOutBase;
    QuantOut11.append("12");
    vpxt_enc_format_append(QuantOut11, EncForm);
    std::string QuantOut19 = QuantOutBase;
    QuantOut19.append("20");
    vpxt_enc_format_append(QuantOut19, EncForm);
    std::string QuantOut27 = QuantOutBase;
    QuantOut27.append("28");
    vpxt_enc_format_append(QuantOut27, EncForm);
    std::string QuantOut35 = QuantOutBase;
    QuantOut35.append("36");
    vpxt_enc_format_append(QuantOut35, EncForm);
    std::string QuantOut43 = QuantOutBase;
    QuantOut43.append("44");
    vpxt_enc_format_append(QuantOut43, EncForm);
    std::string QuantOut51 = QuantOutBase;
    QuantOut51.append("52");
    vpxt_enc_format_append(QuantOut51, EncForm);
    std::string QuantOut59 = QuantOutBase;
    QuantOut59.append("60");
    vpxt_enc_format_append(QuantOut59, EncForm);

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
    //make sure constrained quantizer mode is off
    if(opt.end_usage == 2)
        opt.end_usage = 1;
    opt.target_bandwidth = BitRate;

    int PSNRToggle = 0;
    int n = 4;
    double PSNRArr[10];
    int MaxQArr[10];
    int i = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        while (n < 63)
        {
            char num[20];
            vpxt_itoa_custom(n, num, 10);

            std::string QuantOutFile = QuantOutBase;
            QuantOutFile.append(num);
            vpxt_enc_format_append(QuantOutFile, EncForm);

            tprintf(PRINT_BTH, "\n");

            if (TestType != 2)
            {
                PSNRArr[i] = vpxt_psnr(input.c_str(), QuantOutFile.c_str(), PSNRToggle, 0, 1, NULL);
                tprintf(PRINT_BTH, "\n");
                MaxQArr[i] = vpxt_check_max_quantizer(QuantOutFile.c_str(), n);
                tprintf(PRINT_BTH, "\n");
            }

            n = n + 8;
            i++;
        }
    }
    else
    {
        while (n < 63)
        {

            opt.worst_allowed_q = n;

            char num[20];
            vpxt_itoa_custom(n, num, 10);

            std::string QuantOutFile = QuantOutBase;
            QuantOutFile.append(num);
            vpxt_enc_format_append(QuantOutFile, EncForm);

            opt.Mode = Mode;

            if (vpxt_compress(input.c_str(), QuantOutFile.c_str(), speed, BitRate, opt, CompressString, n, 1, EncForm) == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                return 2;
            }

            tprintf(PRINT_BTH, "\n");

            if (TestType != 2)
            {
                PSNRArr[i] = vpxt_psnr(input.c_str(), QuantOutFile.c_str(), PSNRToggle, 0, 1, NULL);
                tprintf(PRINT_BTH, "\n");
                MaxQArr[i] = vpxt_check_max_quantizer(QuantOutFile.c_str(), n);
                tprintf(PRINT_BTH, "\n");
            }

            n = n + 8;
            i++;
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

    tprintf(PRINT_BTH, "\n");

    i = 0;
    int MaxQDisplayValue = 4;
    int fail = 0;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    while (i < 7)
    {
        if (!(PSNRArr[i+1] <= PSNRArr[i]))
        {
            vpxt_formated_print(RESPRT, "MaxQ %2i %.2f > %.2f MaxQ %2i - Failed", MaxQDisplayValue + 8, PSNRArr[i+1], PSNRArr[i], MaxQDisplayValue);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
        else
        {
            vpxt_formated_print(RESPRT, "MaxQ %2i %.2f <= %.2f MaxQ %2i - Passed", MaxQDisplayValue + 8, PSNRArr[i+1], PSNRArr[i], MaxQDisplayValue);
            tprintf(PRINT_BTH, "\n");
        }

        i++;
        MaxQDisplayValue = MaxQDisplayValue + 8;
    }

    i = 0;
    MaxQDisplayValue = 4;

    while (i < 8)
    {
        if (MaxQArr[i] != -1)
        {
            vpxt_formated_print(RESPRT, "MaxQ value exceded for MaxQ %2i - frame %i - Failed", MaxQDisplayValue, MaxQArr[i]);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
        else
        {
            vpxt_formated_print(RESPRT, "MaxQ value not exceded for MaxQ %2i - Passed", MaxQDisplayValue, MaxQArr[i]);
            tprintf(PRINT_BTH, "\n");
        }

        MaxQDisplayValue = MaxQDisplayValue + 8;
        i++;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(8, QuantOut3.c_str(), QuantOut11.c_str(), QuantOut19.c_str(), QuantOut27.c_str(), QuantOut35.c_str(), QuantOut43.c_str(), QuantOut51.c_str(), QuantOut59.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(8, QuantOut3.c_str(), QuantOut11.c_str(), QuantOut19.c_str(), QuantOut27.c_str(), QuantOut35.c_str(), QuantOut43.c_str(), QuantOut51.c_str(), QuantOut59.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}