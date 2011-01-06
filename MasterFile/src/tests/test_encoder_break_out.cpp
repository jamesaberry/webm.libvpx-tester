#include "vpxt_test_declarations.h"

int test_encoder_break_out(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType)
{
    char *CompressString = "Encoder Break Out";
    char *MyDir = "test_encoder_break_out";

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

    std::string CurTestDirStr = ""; // <- All Options need to set a value for this

    std::string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string EncBreakOut0 = CurTestDirStr;
    EncBreakOut0.append(slashCharStr());
    EncBreakOut0.append(MyDir);
    EncBreakOut0.append("_compression_0.ivf");

    std::string EncBreakOut100 = CurTestDirStr;
    EncBreakOut100.append(slashCharStr());
    EncBreakOut100.append(MyDir);
    EncBreakOut100.append("_compression_100.ivf");

    std::string EncBreakOut500 = CurTestDirStr;
    EncBreakOut500.append(slashCharStr());
    EncBreakOut500.append(MyDir);
    EncBreakOut500.append("_compression_500.ivf");

    std::string EncBreakOut1000 = CurTestDirStr;
    EncBreakOut1000.append(slashCharStr());
    EncBreakOut1000.append(MyDir);
    EncBreakOut1000.append("_compression_1000.ivf");

    std::string EncBreakOut0_Dec = CurTestDirStr;
    EncBreakOut0_Dec.append(slashCharStr());
    EncBreakOut0_Dec.append(MyDir);
    EncBreakOut0_Dec.append("_decompression_0.ivf");

    std::string EncBreakOut100_Dec = CurTestDirStr;
    EncBreakOut100_Dec.append(slashCharStr());
    EncBreakOut100_Dec.append(MyDir);
    EncBreakOut100_Dec.append("_decompression_100.ivf");

    std::string EncBreakOut500_Dec = CurTestDirStr;
    EncBreakOut500_Dec.append(slashCharStr());
    EncBreakOut500_Dec.append(MyDir);
    EncBreakOut500_Dec.append("_decompression_500.ivf");

    std::string EncBreakOut1000_Dec = CurTestDirStr;
    EncBreakOut1000_Dec.append(slashCharStr());
    EncBreakOut1000_Dec.append(MyDir);
    EncBreakOut1000_Dec.append("_decompression_1000.ivf");

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

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.encode_breakout = 0;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), EncBreakOut0.c_str(), speed, BitRate, opt, CompressString, 0, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.encode_breakout = 100;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), EncBreakOut100.c_str(), speed, BitRate, opt, CompressString, 100, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.encode_breakout = 500;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), EncBreakOut500.c_str(), speed, BitRate, opt, CompressString, 500, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.encode_breakout = 1000;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), EncBreakOut1000.c_str(), speed, BitRate, opt, CompressString, 1000, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_BTH, "\nDecoding EncBreakOut0");

        if (vpxt_decompress_ivf_to_ivf(EncBreakOut0.c_str(), EncBreakOut0_Dec.c_str()) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_BTH, "\n\nDecoding EncBreakOut100");

        if (vpxt_decompress_ivf_to_ivf(EncBreakOut100.c_str(), EncBreakOut100_Dec.c_str()) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_BTH, "\n\nDecoding EncBreakOut500");

        if (vpxt_decompress_ivf_to_ivf(EncBreakOut500.c_str(), EncBreakOut500_Dec.c_str()) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_BTH, "\n\nDecoding EncBreakOut1000");

        if (vpxt_decompress_ivf_to_ivf(EncBreakOut1000.c_str(), EncBreakOut1000_Dec.c_str()) == -1)
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

    long SourceFileSize = 0;

    long EncBreakOut0_Dec_FileSize = 0;
    long EncBreakOut100_Dec_FileSize = 0;
    long EncBreakOut500_Dec_FileSize = 0;
    long EncBreakOut1000_Dec_FileSize = 0;

    tprintf(PRINT_BTH, "\n\n");
    SourceFileSize = vpxt_file_size(input.c_str(), 1);
    tprintf(PRINT_BTH, "\n");
    EncBreakOut0_Dec_FileSize = vpxt_file_size(EncBreakOut0_Dec.c_str(), 1);
    tprintf(PRINT_BTH, "\n");
    EncBreakOut100_Dec_FileSize = vpxt_file_size(EncBreakOut100_Dec.c_str(), 1);
    tprintf(PRINT_BTH, "\n");
    EncBreakOut500_Dec_FileSize = vpxt_file_size(EncBreakOut500_Dec.c_str(), 1);
    tprintf(PRINT_BTH, "\n");
    EncBreakOut1000_Dec_FileSize = vpxt_file_size(EncBreakOut1000_Dec.c_str(), 1);
    tprintf(PRINT_BTH, "\n");

    double PSNR0;
    double PSNR100;
    double PSNR500;
    double PSNR1000;

    PSNR0 = vpxt_ivf_psnr(input.c_str(), EncBreakOut0.c_str(), 0, 0, 1, NULL);
    PSNR100 = vpxt_ivf_psnr(input.c_str(), EncBreakOut100.c_str(), 0, 0, 1, NULL);
    PSNR500 = vpxt_ivf_psnr(input.c_str(), EncBreakOut500.c_str(), 0, 0, 1, NULL);
    PSNR1000 = vpxt_ivf_psnr(input.c_str(), EncBreakOut1000.c_str(), 0, 0, 1, NULL);

    double dB1 = vpxt_abs_double(10 * log(PSNR0 / PSNR100));
    double dB2 = vpxt_abs_double(10 * log(PSNR100 / PSNR500));
    double dB3 = vpxt_abs_double(10 * log(PSNR500 / PSNR1000));

    tprintf(PRINT_BTH, "\ndB1: %f \n", dB1);
    tprintf(PRINT_BTH, "dB2: %f \n", dB1);
    tprintf(PRINT_BTH, "dB3: %f \n", dB1);
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int Pass = 1;
    int IndCount = 0;

    if (SourceFileSize != EncBreakOut0_Dec_FileSize)
    {
        vpxt_formated_print(RESPRT, "EncBreakOut0 was not properly decoded.");
        tprintf(PRINT_BTH, "\n");
        Pass = 0;
    }
    else
    {
        vpxt_formated_print(RESPRT, "EncBreakOut0 was properly decoded.");
        tprintf(PRINT_BTH, "\n");
    }

    if (SourceFileSize != EncBreakOut100_Dec_FileSize)
    {
        vpxt_formated_print(RESPRT, "EncBreakOut100 was not properly decoded.");
        tprintf(PRINT_BTH, "\n");
        Pass = 0;
    }
    else
    {
        vpxt_formated_print(RESPRT, "EncBreakOut100 was properly decoded.");
        tprintf(PRINT_BTH, "\n");
    }

    if (SourceFileSize != EncBreakOut500_Dec_FileSize)
    {
        vpxt_formated_print(RESPRT, "EncBreakOut500 was not properly decoded.");
        tprintf(PRINT_BTH, "\n");
        Pass = 0;
    }
    else
    {
        vpxt_formated_print(RESPRT, "EncBreakOut500 was properly decoded.");
        tprintf(PRINT_BTH, "\n");
    }

    if (SourceFileSize != EncBreakOut1000_Dec_FileSize)
    {
        vpxt_formated_print(RESPRT, "EncBreakOut1000 was not properly decoded.");
        tprintf(PRINT_BTH, "\n");
        Pass = 0;
    }
    else
    {
        vpxt_formated_print(RESPRT, "EncBreakOut1000 was properly decoded.");
        tprintf(PRINT_BTH, "\n");
    }

    if (dB1 <= 2)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 0 PSNR and 100 PSNR are within 2 dB: %4f dB - Passed", dB1);
        tprintf(PRINT_BTH, "\n");
    }

    if (dB1 >= 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 0 PSNR and 100 PSNR not within 5 dB: %4f dB - Failed", dB1);
        tprintf(PRINT_BTH, "\n");
        Pass = 0;
    }

    if (dB1 > 2 && dB1 < 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 0 PSNR and 100 PSNR greater than 2 dB but less than 5 dB: %4f dB - Indeterminate", dB1);
        tprintf(PRINT_BTH, "\n");
        IndCount++;
    }

    if (dB2 <= 2)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 100 PSNR and 500 PSNR are within 2 dB: %4f dB - Passed", dB2);
        tprintf(PRINT_BTH, "\n");
    }

    if (dB2 >= 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 100 PSNR and 500 PSNR not within 5 dB: %4f dB - Failed", dB2);
        tprintf(PRINT_BTH, "\n");
        Pass = 0;
    }

    if (dB2 > 2 && dB2 < 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 100 PSNR and 500 PSNR greater than 2 dB but less than 5 dB: %4f dB - Indeterminate", dB2);
        tprintf(PRINT_BTH, "\n");
        IndCount++;
    }

    if (dB3 <= 2)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 500 PSNR and 1000 PSNR are within 2 dB: %4f dB - Passed", dB3);
        tprintf(PRINT_BTH, "\n");
    }

    if (dB3 >= 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 500 PSNR and 1000 PSNR not within 5 dB: %4f dB - Failed", dB3);
        tprintf(PRINT_BTH, "\n");
        Pass = 0;
    }

    if (dB3 > 2 && dB3 < 5)
    {
        vpxt_formated_print(RESPRT, "Encoder Breakout 500 PSNR and 1000 PSNR greater than 2 dB but less than 5 dB: %4f dB - Indeterminate", dB3);
        tprintf(PRINT_BTH, "\n");
        IndCount++;
    }

    if (Pass == 1)
    {
        if (IndCount == 0)
        {
            tprintf(PRINT_BTH, "\nPassed\n");

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 1;
        }
        else
        {
            tprintf(PRINT_BTH, "\nIndeterminate\n");

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
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