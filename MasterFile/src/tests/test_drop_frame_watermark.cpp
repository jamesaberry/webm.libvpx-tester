#include "vpxt_test_declarations.h"

int test_drop_frame_watermark(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Drop Frames Watermark";
    char *MyDir = "test_drop_frame_watermark";

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

    std::string DFWMOutFileBase = CurTestDirStr;
    DFWMOutFileBase.append(slashCharStr());
    DFWMOutFileBase.append(MyDir);
    DFWMOutFileBase.append("_compression_");

    std::string DFWMOutFile0   = DFWMOutFileBase;
    DFWMOutFile0.append("0.ivf");
    std::string DFWMOutFile20  = DFWMOutFileBase;
    DFWMOutFile20.append("20.ivf");
    std::string DFWMOutFile40  = DFWMOutFileBase;
    DFWMOutFile40.append("40.ivf");
    std::string DFWMOutFile60  = DFWMOutFileBase;
    DFWMOutFile60.append("60.ivf");
    std::string DFWMOutFile80  = DFWMOutFileBase;
    DFWMOutFile80.append("80.ivf");
    std::string DFWMOutFile100 = DFWMOutFileBase;
    DFWMOutFile100.append("100.ivf");

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
    opt.worst_allowed_q = 15;
    opt.allow_df = 1;
    opt.allow_spatial_resampling = 0;
    opt.end_usage = 0;

    int n = 100;
    int PSNRToggle = 0;

    int i = 0;
    long DMFW[6];

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        while (n >= 0)
        {
            ///////Update File Name///////
            char num[20];
            vpxt_itoa_custom(n, num, 10);
            std::string DFWMOutFile = DFWMOutFileBase;
            DFWMOutFile.append(num);
            DFWMOutFile.append(".ivf");

            tprintf(PRINT_STD, "\n");
            fprintf(stderr, "\n");
            DMFW[i] = vpxt_file_size(DFWMOutFile.c_str(), 1);
            tprintf(PRINT_STD, "\n");
            fprintf(stderr, "\n");

            i++;
            n = n - 20;

            DFWMOutFile.clear();
        }
    }
    else
    {
        while (n >= 0)
        {
            opt.drop_frames_water_mark = n;

            ///////Update File Name///////
            char num[20];
            vpxt_itoa_custom(n, num, 10);
            std::string DFWMOutFile = DFWMOutFileBase;
            DFWMOutFile.append(num);
            DFWMOutFile.append(".ivf");

            opt.Mode = Mode;

            if (vpxt_compress_ivf_to_ivf(input.c_str(), DFWMOutFile.c_str(), speed, BitRate, opt, CompressString, n, 0) == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                return 2;
            }

            if (TestType != 2)
            {
                tprintf(PRINT_STD, "\n");
                fprintf(stderr, "\n");
                DMFW[i] = vpxt_display_visible_frames(DFWMOutFile.c_str(), 1);
                tprintf(PRINT_STD, "\n");
                fprintf(stderr, "\n");
            }

            i++;
            n = n - 20;

            DFWMOutFile.clear();
        }
    }

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    i = 0;
    n = 100;
    int EqualBool = 0;
    int testBool = 1;

    tprintf(PRINT_STD, "\n\n");
    fprintf(stderr, "\n\n");

    while (i < 6)
    {
        tprintf(PRINT_STD, "DFWM%4i Visible Frames: %4i\n", n, DMFW[i]);//cout << "DFWM" << n << " Size " << DMFW[i] << "\n";
        fprintf(stderr, "DFWM%4i Visible Frames: %4i\n", n, DMFW[i]);//cerr << "DFWM" << n << " Size " << DMFW[i] << "\n";
        i++;
        n = n - 20;
    }

    n = 100;
    i = 0;

    tprintf(PRINT_STD, "\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    while (i < 5)
    {
        if (DMFW[i+1] == DMFW[i])
        {
            EqualBool++;

            vpxt_formated_print(RESPRT, "DFWM%4i: %4i = DFWM%4i: %4i - Indeterminate", n - 20, DMFW[i+1], n, DMFW[i]);
            tprintf(PRINT_STD, "\n");
            fprintf(stderr, "\n");
        }

        if (DMFW[i+1] > DMFW[i])
        {
            vpxt_formated_print(RESPRT, "DFWM%4i: %4i > DFWM%4i: %4i - Passed", n - 20, DMFW[i+1], n, DMFW[i]);
            tprintf(PRINT_STD, "\n");
            fprintf(stderr, "\n");
        }

        if (DMFW[i+1] < DMFW[i])
        {
            testBool = 0;

            vpxt_formated_print(RESPRT, "DFWM%4i: %4i < DFWM%4i: %4i - Failed", n - 20, DMFW[i+1], n, DMFW[i]);
            tprintf(PRINT_STD, "\n");
            fprintf(stderr, "\n");
        }

        i++;
        n = n - 20;
    }

    if (testBool == 0)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(6, DFWMOutFile0.c_str(), DFWMOutFile20.c_str(), DFWMOutFile40.c_str(), DFWMOutFile60.c_str(), DFWMOutFile80.c_str(), DFWMOutFile100.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }
    else
    {
        if (EqualBool == 5)
        {
            tprintf(PRINT_BTH, "\n\nUnknown: Drop-Frames-Watermark has no effect, try different parameters \n");

            if (DeleteIVF)
                vpxt_delete_files(6, DFWMOutFile0.c_str(), DFWMOutFile20.c_str(), DFWMOutFile40.c_str(), DFWMOutFile60.c_str(), DFWMOutFile80.c_str(), DFWMOutFile100.c_str());

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
        else
        {
            tprintf(PRINT_BTH, "\nPassed\n");

            if (DeleteIVF)
                vpxt_delete_files(6, DFWMOutFile0.c_str(), DFWMOutFile20.c_str(), DFWMOutFile40.c_str(), DFWMOutFile60.c_str(), DFWMOutFile80.c_str(), DFWMOutFile100.c_str());

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 1;
        }
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}