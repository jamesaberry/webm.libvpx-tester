#include "vpxt_test_declarations.h"

int test_post_processor(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Allow Drop Frames";
    char *MyDir = "test_post_processor";
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

    std::string PostProcOutFile = CurTestDirStr;
    PostProcOutFile.append(slashCharStr());
    PostProcOutFile.append(MyDir);
    PostProcOutFile.append("_compression");
    vpxt_enc_format_append(PostProcOutFile, EncForm);

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
    int CompressInt = opt.allow_df;

    opt.target_bandwidth = BitRate;

    int n = 0;
    int i = 0;
    double PSNRArr[50];

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress(input.c_str(), PostProcOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, EncForm) == -1)
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

    int deblock_level = 0;
    int flags = 0;
    int noise_level = 0;
    int countme = 0;
    double ssim = 0;

    tprintf(PRINT_BTH, "\nCaculating PSNR: NOFILTERING DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
    PSNRArr[countme] = vpxt_post_proc_psnr(input.c_str(), PostProcOutFile.c_str(), 0, PRINT_BTH, 1, deblock_level, 0, flags, &ssim);
    countme++;

    flags++;
    tprintf(PRINT_BTH, "\nCaculating PSNR: DEBLOCK DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
    PSNRArr[countme] = vpxt_post_proc_psnr(input.c_str(), PostProcOutFile.c_str(), 0, PRINT_BTH, 1, deblock_level, noise_level, flags, &ssim);
    countme++;
    flags++;

    while (deblock_level != 16)
    {
        tprintf(PRINT_BTH, "\nCaculating PSNR: DEMACROBLOCK DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
        PSNRArr[countme] = vpxt_post_proc_psnr(input.c_str(), PostProcOutFile.c_str(), 0, PRINT_BTH, 1, deblock_level, 0, flags, &ssim);
        countme++;
        deblock_level++;
    }

    flags = flags + 2;
    deblock_level = 0;
    noise_level = 0;

    while (noise_level != 8)
    {
        tprintf(PRINT_BTH, "\nCaculating PSNR: ADDNOISE DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
        PSNRArr[countme] = vpxt_post_proc_psnr(input.c_str(), PostProcOutFile.c_str(), 0, PRINT_BTH, 1, deblock_level, noise_level, flags, &ssim);
        countme++;

        noise_level++;
    }

    int x = 0;
    tprintf(PRINT_STD, "\n");

    int TestFail = 0;
    int TenPercent = 0;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (PSNRArr[0] != PSNRArr[1])
    {
        vpxt_formated_print(RESPRT, "DEBLOCK PSNR: %4.2f != NOFILTERING PSNR: %4.2f - Passed", PSNRArr[1], PSNRArr[0]);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "DEBLOCK PSNR: %4.2f == NOFILTERING PSNR: %4.2f - Failed", PSNRArr[1], PSNRArr[0]);
        tprintf(PRINT_BTH, "\n");

        TestFail = 1;
    }

    tprintf(PRINT_BTH, "\n");

    x = 2;

    while (x < 18)
    {
        if (PSNRArr[0] != PSNRArr[x])
        {
            vpxt_formated_print(RESPRT, "DeblockLevel %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f - Passed", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "DeblockLevel %*i PSNR: %4.2f += NOFILTERING PSNR: %4.2f - Failed", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            tprintf(PRINT_BTH, "\n");

            TestFail = 1;
        }

        x++;
    }

    tprintf(PRINT_BTH, "\n");

    while (x < 26)
    {
        if (PSNRArr[0] != PSNRArr[x])
        {
            vpxt_formated_print(RESPRT, "noise_level %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f - Passed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "noise_level %*i PSNR: %4.2f == NOFILTERING PSNR: %4.2f - Failed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            tprintf(PRINT_BTH, "\n");

            TestFail = 1;
        }

        x++;
    }

    /////////////////////////////

    tprintf(PRINT_BTH, "\n");

    float PSNRPercent = vpxt_abs_float(PSNRArr[1] - PSNRArr[0]) / PSNRArr[0];

    if (PSNRPercent < 0.1)
    {
        vpxt_formated_print(RESPRT, "DEBLOCK PSNR: %4.2f within 10%% of %4.2f - Passed", PSNRArr[1], PSNRArr[0]);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "DEBLOCK PSNR: %4.2f not within 10%% of %4.2f - Failed", PSNRArr[1], PSNRArr[0]);
        tprintf(PRINT_BTH, "\n");

        TenPercent = 1;
    }

    tprintf(PRINT_BTH, "\n");

    x = 2;

    while (x < 18)
    {
        PSNRPercent = vpxt_abs_float(PSNRArr[x] - PSNRArr[0]) / PSNRArr[0];

        if (PSNRPercent < 0.1)
        {
            vpxt_formated_print(RESPRT, "DeblockLevel %*i PSNR: %4.2f within 10%% of %4.2f - Passed", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "DeblockLvl %*i PSNR: %4.2f not within 10%% of %4.2f - Failed", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            tprintf(PRINT_BTH, "\n");

            TenPercent = 1;
        }

        x++;
    }

    tprintf(PRINT_BTH, "\n");

    while (x < 26)
    {
        PSNRPercent = vpxt_abs_float(PSNRArr[x] - PSNRArr[0]) / PSNRArr[0];

        if (PSNRPercent < 0.1)
        {
            vpxt_formated_print(RESPRT, "noise_level %*i PSNR: %4.2f within 10%% of %4.2f - Passed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            tprintf(PRINT_BTH, "\n");
        }
        else
        {
            vpxt_formated_print(RESPRT, "noise_level %*i PSNR: %4.2f not within 10%% of %4.2f - Indeterminate", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            tprintf(PRINT_BTH, "\n");

            TenPercent = 1;
        }

        x++;
    }

    if (TestFail == 0)
    {
        if (TenPercent == 0)
        {
            tprintf(PRINT_BTH, "\nPassed\n");

            if (DeleteIVF)
                vpxt_delete_files(1, PostProcOutFile.c_str());

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 1;
        }
        else
        {
            tprintf(PRINT_BTH, "\nMin Passed\n");

            if (DeleteIVF)
                vpxt_delete_files(1, PostProcOutFile.c_str());

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 8;
        }
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(1, PostProcOutFile.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}