#include "vpxt_test_declarations.h"

int test_post_processor(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "AllowDF";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  PostProcessorWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "    <Optional Settings File>\n");
        return 0;
    }

    int speed = 0;
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[3]);
    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "PostProcessorWorks";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string PostProcOutFile = WorkingDirString;

    PostProcOutFile.append(slashCharStr());
    PostProcOutFile.append("PostProcOutput.ivf");

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
        TextfileString.append(".txt");
    else
        TextfileString.append("_TestOnly.txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        printf("Cannot open out put file: %s\n", TextfileString.c_str());
        exit(1);
    }

    ////////////////////////////////
    //////////////////////////////////////////////////////////

    if (TestType == 1)
    {
        print_header_full_test(argc, argv, WorkingDir3);
    }

    if (TestType == 2)
    {
        print_header_compression_only(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        print_header_test_only(argc, argv, WorkingDirString);
    }

    tprintf("Post Processor Works Test");
    tprintf("\n");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
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
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress_ivf_to_ivf(input, PostProcOutFile.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        //Compression only run
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 10;
    }

    int deblock_level = 0;
    int flags = 0;
    int noise_level = 0;
    int countme = 0;
    double ssim = 0;

    tprintf("\nCaculating PSNR: NOFILTERING DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
    PSNRArr[countme] = vpxt_post_proc_ivf_psnr(input, PostProcOutFile.c_str(), 0, 0, 1, deblock_level, 0, flags, &ssim);
    countme++;

    flags++;
    tprintf("\nCaculating PSNR: DEBLOCK DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
    PSNRArr[countme] = vpxt_post_proc_ivf_psnr(input, PostProcOutFile.c_str(), 0, 0, 1, deblock_level, noise_level, flags, &ssim);
    countme++;
    flags++;

    while (deblock_level != 16)
    {
        tprintf("\nCaculating PSNR: DEMACROBLOCK DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
        PSNRArr[countme] = vpxt_post_proc_ivf_psnr(input, PostProcOutFile.c_str(), 0, 0, 1, deblock_level, 0, flags, &ssim);
        countme++;
        deblock_level++;
    }

    flags = flags + 2;
    deblock_level = 0;
    noise_level = 0;

    while (noise_level != 8)
    {
        tprintf("\nCaculating PSNR: ADDNOISE DeblockLevel %i noise_level %i \n", deblock_level, noise_level);
        PSNRArr[countme] = vpxt_post_proc_ivf_psnr(input, PostProcOutFile.c_str(), 0, 0, 1, deblock_level, noise_level, flags, &ssim);
        countme++;

        noise_level++;
    }

    int x = 0;
    cout << "\n";

    int TestFail = 0;
    int TenPercent = 0;

    tprintf("\n\nResults:\n\n");

    if (PSNRArr[0] != PSNRArr[1])
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DEBLOCK PSNR: %4.2f != NOFILTERING PSNR: %4.2f - Passed", PSNRArr[1], PSNRArr[0]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DEBLOCK PSNR: %4.2f == NOFILTERING PSNR: %4.2f - Failed", PSNRArr[1], PSNRArr[0]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        TestFail = 1;
    }

    tprintf("\n");

    x = 2;

    while (x < 18)
    {
        if (PSNRArr[0] != PSNRArr[x])
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DeblockLevel %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f - Passed", 2, x - 2, PSNRArr[x], PSNRArr[0]);;
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DeblockLevel %*i PSNR: %4.2f += NOFILTERING PSNR: %4.2f - Failed", 2, x - 2, PSNRArr[x], PSNRArr[0]);;
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");

            TestFail = 1;
        }

        x++;
    }

    tprintf("\n");

    while (x < 26)
    {
        if (PSNRArr[0] != PSNRArr[x])
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "noise_level %*i PSNR: %4.2f != NOFILTERING PSNR: %4.2f - Passed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "noise_level %*i PSNR: %4.2f == NOFILTERING PSNR: %4.2f - Failed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");

            TestFail = 1;
        }

        x++;
    }

    /////////////////////////////

    tprintf("\n");

    float PSNRPercent = vpxt_abs_float(PSNRArr[1] - PSNRArr[0]) / PSNRArr[0];

    if (PSNRPercent < 0.1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DEBLOCK PSNR: %4.2f within 10%% of %4.2f - Passed", PSNRArr[1], PSNRArr[0]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DEBLOCK PSNR: %4.2f not within 10%% of %4.2f - Failed", PSNRArr[1], PSNRArr[0]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        TenPercent = 1;
    }

    tprintf("\n");

    x = 2;

    while (x < 18)
    {
        PSNRPercent = vpxt_abs_float(PSNRArr[x] - PSNRArr[0]) / PSNRArr[0];

        if (PSNRPercent < 0.1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DeblockLevel %*i PSNR: %4.2f within 10%% of %4.2f - Passed", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DeblockLvl %*i PSNR: %4.2f not within 10%% of %4.2f - Failed", 2, x - 2, PSNRArr[x], PSNRArr[0]);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");

            TenPercent = 1;
        }

        x++;
    }

    tprintf("\n");

    while (x < 26)
    {
        PSNRPercent = vpxt_abs_float(PSNRArr[x] - PSNRArr[0]) / PSNRArr[0];

        if (PSNRPercent < 0.1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "noise_level %*i PSNR: %4.2f within 10%% of %4.2f - Passed", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "noise_level %*i PSNR: %4.2f not within 10%% of %4.2f - Indeterminate", 2, x - 18, PSNRArr[x], PSNRArr[0]);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");

            TenPercent = 1;
        }

        x++;
    }

    if (TestFail == 0)
    {
        if (TenPercent == 0)
        {
            tprintf("\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            tprintf("\nMin Passed\n");

            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 8;
        }
    }
    else
    {
        tprintf("\nFailed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    fclose(fp);
    string File1Str = File1;
    record_test_complete(MainDirString, File1Str, TestType);
    return 6;
}