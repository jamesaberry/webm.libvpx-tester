#include "vpxt_test_declarations.h"

int test_version(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Version";
    char *MyDir = "test_version";
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

    std::string CurTestDirStr = ""; // <- All Options need to set a value for this
    std::string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string Version0 = CurTestDirStr;
    Version0.append(slashCharStr());
    Version0.append(MyDir);
    Version0.append("_compression_0");
    vpxt_enc_format_append(Version0, EncForm);

    std::string Version1 = CurTestDirStr;
    Version1.append(slashCharStr());
    Version1.append(MyDir);
    Version1.append("_compression_1");
    vpxt_enc_format_append(Version1, EncForm);

    std::string Version2 = CurTestDirStr;
    Version2.append(slashCharStr());
    Version2.append(MyDir);
    Version2.append("_compression_2");
    vpxt_enc_format_append(Version2, EncForm);

    std::string Version3 = CurTestDirStr;
    Version3.append(slashCharStr());
    Version3.append(MyDir);
    Version3.append("_compression_3");
    vpxt_enc_format_append(Version3, EncForm);

    std::string Version0_Dec = CurTestDirStr;
    Version0_Dec.append(slashCharStr());
    Version0_Dec.append(MyDir);
    Version0_Dec.append("_decompression_0");
    vpxt_dec_format_append(Version0_Dec, DecForm);

    std::string Version1_Dec = CurTestDirStr;
    Version1_Dec.append(slashCharStr());
    Version1_Dec.append(MyDir);
    Version1_Dec.append("_decompression_1");
    vpxt_dec_format_append(Version1_Dec, DecForm);

    std::string Version2_Dec = CurTestDirStr;
    Version2_Dec.append(slashCharStr());
    Version2_Dec.append(MyDir);
    Version2_Dec.append("_decompression_2");
    vpxt_dec_format_append(Version2_Dec, DecForm);

    std::string Version3_Dec = CurTestDirStr;
    Version3_Dec.append(slashCharStr());
    Version3_Dec.append(MyDir);
    Version3_Dec.append("_decompression_3");
    vpxt_dec_format_append(Version3_Dec, DecForm);

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

    float PSNRArr[4];
    unsigned int Deccpu_tick[4];

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        Deccpu_tick[0] = vpxt_cpu_tick_return(Version0_Dec.c_str(), 1);
        Deccpu_tick[1] = vpxt_cpu_tick_return(Version1_Dec.c_str(), 1);
        Deccpu_tick[2] = vpxt_cpu_tick_return(Version2_Dec.c_str(), 1);
        Deccpu_tick[3] = vpxt_cpu_tick_return(Version3_Dec.c_str(), 1);
    }
    else
    {
        opt.Mode = Mode;

        opt.Version = 0;

        if (vpxt_compress(input.c_str(), Version0.c_str(), speed, BitRate, opt, CompressString, 0, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.Version = 1;

        if (vpxt_compress(input.c_str(), Version1.c_str(), speed, BitRate, opt, CompressString, 1, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.Version = 2;

        if (vpxt_compress(input.c_str(), Version2.c_str(), speed, BitRate, opt, CompressString, 2, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.Version = 3;

        if (vpxt_compress(input.c_str(), Version3.c_str(), speed, BitRate, opt, CompressString, 3, 0, EncForm) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_STD, "\n\n");
        //fprintf(stderr, "\n\nDecompressing VP8 IVF File to IVF File: \n");
        unsigned int Time1 = vpxt_time_decompress(Version0.c_str(), Version0_Dec.c_str(), Deccpu_tick[0], DecForm);

        if (Time1 == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_STD, "\n");
        //fprintf(stderr, "\nDecompressing VP8 IVF File to IVF File: \n");
        unsigned int Time2 = vpxt_time_decompress(Version1.c_str(), Version1_Dec.c_str(), Deccpu_tick[1], DecForm);

        if (Time2 == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_STD, "\n");
        //fprintf(stderr, "\nDecompressing VP8 IVF File to IVF File: \n");
        unsigned int Time3 = vpxt_time_decompress(Version2.c_str(), Version2_Dec.c_str(), Deccpu_tick[2], DecForm);

        if (Time3 == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        tprintf(PRINT_STD, "\n");
        //fprintf(stderr, "\nDecompressing VP8 IVF File to IVF File: \n");
        unsigned int Time4 = vpxt_time_decompress(Version3.c_str(), Version3_Dec.c_str(), Deccpu_tick[3], DecForm);

        if (Time4 == -1)
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

    PSNRArr[0] = vpxt_psnr(input.c_str(), Version0.c_str(), 0, 0, 1, NULL);
    PSNRArr[1] = vpxt_psnr(input.c_str(), Version1.c_str(), 0, 0, 1, NULL);
    PSNRArr[2] = vpxt_psnr(input.c_str(), Version2.c_str(), 0, 0, 1, NULL);
    PSNRArr[3] = vpxt_psnr(input.c_str(), Version3.c_str(), 0, 0, 1, NULL);

    tprintf(PRINT_BTH, "\n");

    int PSNRFail = 0;
    int TIMEFail = 0;
    int i = 0;

    while (i < 4)
    {

        int t = i + 1;

        while (t < 4)
        {
            //i should always have Higher PSNR than t
            //i should always have a higher Deccpu_tick as well.
            if (PSNRArr[i] < PSNRArr[t])
            {
                if (Deccpu_tick[i] < Deccpu_tick[t])
                {
                    tprintf(PRINT_BTH, "\nFailed Version %i Decode Tick: %d >= Version %i Decode Tick: %d\n"
                            "Failed Version %i PSNR: %f <= Version %i PSNR: %f\n", i, Deccpu_tick[i], t, Deccpu_tick[t], i, PSNRArr[i], t, PSNRArr[t]);
                    TIMEFail++;
                    PSNRFail++;
                }
                else
                {
                    tprintf(PRINT_BTH, "\n       Version %i Decode Tick: %d >= Version %i Decode Tick: %d\n"
                            "Failed Version %i PSNR: %f <= Version %i PSNR: %f\n", i, Deccpu_tick[i], t, Deccpu_tick[t], i, PSNRArr[i], t, PSNRArr[t]);
                    PSNRFail++;
                }
            }
            else
            {
                if (Deccpu_tick[i] < Deccpu_tick[t])
                {
                    tprintf(PRINT_BTH, "\nFailed Version %i Decode Tick: %d <= Version %i Decode Tick: %d\n"
                            "       Version %i PSNR: %f >= Version %i PSNR: %f\n", i, Deccpu_tick[i], t, Deccpu_tick[t], i, PSNRArr[i], t, PSNRArr[t]);
                    TIMEFail++;
                }
                else
                {
                    tprintf(PRINT_BTH, "\n       Version %i Decode Tick: %d >= Version %i Decode Tick: %d\n"
                            "       Version %i PSNR: %f >= Version %i PSNR: %f\n", i, Deccpu_tick[i], t, Deccpu_tick[t], i, PSNRArr[i], t, PSNRArr[t]);
                }
            }

            t++;
        }

        i++;
    }

    //Fail tracks PSNR Fails
    //Fail2 tracks Time Fails

    int fail = 0;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (PSNRFail == 0)// && TIMEFail == 0)
    {
        vpxt_formated_print(RESPRT, "All PSNRs decrease as version numbers increase - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (PSNRFail < 2 && PSNRFail != 0)// && TIMEFail == 0)
    {
        vpxt_formated_print(RESPRT, "All but one PSNR Decreases as version numbers increase - Min Passed");
        tprintf(PRINT_BTH, "\n");
        fail = 2;
    }

    if (PSNRFail >= 2)
    {
        vpxt_formated_print(RESPRT, "Not all PSNRs decrease as version numbers increase - Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (TIMEFail == 0)
    {
        vpxt_formated_print(RESPRT, "All Decode ticks decrease as version numbers increase - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (TIMEFail < 2 && TIMEFail != 0)
    {
        vpxt_formated_print(RESPRT, "All but one Decode ticks decrease as version numbers increase - Min Passed");
        tprintf(PRINT_BTH, "\n");
        fail = 2;
    }

    if (TIMEFail >= 2)
    {
        vpxt_formated_print(RESPRT, "Not all Decode ticks increase as version numbers increase - Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (fail == 2)
    {
        tprintf(PRINT_BTH, "\nMin Passed\n");

        if (DeleteIVF)
            vpxt_delete_files(8, Version0.c_str(), Version1.c_str(), Version2.c_str(), Version3.c_str(), Version0_Dec.c_str(), Version1_Dec.c_str(), Version2_Dec.c_str(), Version3_Dec.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 8;
    }

    if (fail == 1)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(8, Version0.c_str(), Version1.c_str(), Version2.c_str(), Version3.c_str(), Version0_Dec.c_str(), Version1_Dec.c_str(), Version2_Dec.c_str(), Version3_Dec.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }
    else
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(8, Version0.c_str(), Version1.c_str(), Version2.c_str(), Version3.c_str(), Version0_Dec.c_str(), Version1_Dec.c_str(), Version2_Dec.c_str(), Version3_Dec.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}