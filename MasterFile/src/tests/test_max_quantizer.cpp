#include "vpxt_test_declarations.h"

int test_max_quantizer(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType)
{
    char *CompressString = "Max Quantizer";
    char *MyDir = "test_max_quantizer";

    if (!(argc == 6 || argc == 5))
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
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

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
    if (argc == 6)
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
    int PSNRToggle = 0;

    int n = 3;
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
            QuantOutFile.append(".ivf");

            tprintf(PRINT_BTH, "\n");

            if (TestType != 2)
            {
                PSNRArr[i] = vpxt_ivf_psnr(input.c_str(), QuantOutFile.c_str(), PSNRToggle, 0, 1, NULL);
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
            QuantOutFile.append(".ivf");

            opt.Mode = Mode;

            if (vpxt_compress_ivf_to_ivf(input.c_str(), QuantOutFile.c_str(), speed, BitRate, opt, CompressString, n, 1, 0, 3, 3) == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                return 2;
            }

            tprintf(PRINT_BTH, "\n");

            if (TestType != 2)
            {
                PSNRArr[i] = vpxt_ivf_psnr(input.c_str(), QuantOutFile.c_str(), PSNRToggle, 0, 1, NULL);
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
    int MaxQDisplayValue = 3;
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
    MaxQDisplayValue = 3;

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

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}