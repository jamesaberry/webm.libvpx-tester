#include "vpxt_test_declarations.h"

int test_drop_frame_watermark(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "DropFramesWaterMark";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  DFWMTest \n\n"
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

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "DropFramesWaterMark";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string DFWMOutFileBase = WorkingDirString;

    DFWMOutFileBase.append(slashCharStr());
    DFWMOutFileBase.append("DFWMOutput");

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

    int speed = 0;
    int BitRate = atoi(argv[4]);

    int Mode = atoi(argv[3]);


    printf("Drop Frame Watermark Works Test");
    fprintf(stderr, "Drop Frame Watermark Works Test");


    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 6)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            printf("\nInput Settings file %s does not exist\n", argv[argc-1]);
            fprintf(stderr, "\nInput Settings file %s does not exist\n", argv[argc-1]);
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = BitRate;
    /////Newly added//////
    opt.worst_allowed_q = 15;
    opt.allow_df = 1;
    opt.allow_spatial_resampling = 0;
    opt.end_usage = 0;
    //////////////////////

    int n = 100;
    int PSNRToggle = 0;

    int i = 0;
    long DMFW[6];

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        while (n >= 0)
        {
            ///////Update File Name///////
            char num[20];
            vpx_itoa_custom(n, num, 10);
            string DFWMOutFile = DFWMOutFileBase;
            DFWMOutFile.append(num);
            DFWMOutFile.append(".ivf");

            printf("\n");
            fprintf(stderr, "\n");
            DMFW[i] = vpxt_file_size(DFWMOutFile.c_str(), 1);
            printf("\n");
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
            vpx_itoa_custom(n, num, 10);
            string DFWMOutFile = DFWMOutFileBase;
            DFWMOutFile.append(num);
            DFWMOutFile.append(".ivf");

            opt.Mode = Mode;

            if (vpxt_compress_ivf_to_ivf(input, DFWMOutFile.c_str(), speed, BitRate, opt, CompressString, n, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 2;
            }

            if (TestType != 2)
            {
                printf("\n");
                fprintf(stderr, "\n");
                DMFW[i] = vpxt_display_visible_frames(DFWMOutFile.c_str(), 1);
                printf("\n");
                fprintf(stderr, "\n");
            }

            i++;
            n = n - 20;

            DFWMOutFile.clear();
        }
    }

    //Create Compression only stop test short.
    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 10;
    }

    i = 0;
    n = 100;
    int EqualBool = 0;
    int testBool = 1;

    printf("\n\n");
    fprintf(stderr, "\n\n");

    while (i < 6)
    {
        printf("DFWM%4i Visible Frames: %4i\n", n, DMFW[i]);//cout << "DFWM" << n << " Size " << DMFW[i] << "\n";
        fprintf(stderr, "DFWM%4i Visible Frames: %4i\n", n, DMFW[i]);//cerr << "DFWM" << n << " Size " << DMFW[i] << "\n";
        i++;
        n = n - 20;
    }

    n = 100;
    i = 0;

    printf("\n\nResults:\n\n");
    fprintf(stderr, "\n\nResults:\n\n");

    while (i < 5)
    {
        if (DMFW[i+1] == DMFW[i])
        {
            EqualBool++;

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DFWM%4i: %4i = DFWM%4i: %4i - Indeterminate", n - 20, DMFW[i+1], n, DMFW[i]);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (DMFW[i+1] > DMFW[i])
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DFWM%4i: %4i > DFWM%4i: %4i - Passed", n - 20, DMFW[i+1], n, DMFW[i]);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        if (DMFW[i+1] < DMFW[i])
        {
            testBool = 0;

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DFWM%4i: %4i < DFWM%4i: %4i - Failed", n - 20, DMFW[i+1], n, DMFW[i]);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            printf("\n");
            fprintf(stderr, "\n");
        }

        i++;
        n = n - 20;
    }

    if (testBool == 0)
    {
        printf("\nFailed\n");
        fprintf(stderr, "\nFailed\n");
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }
    else
    {
        if (EqualBool == 5)
        {
            printf("\n\nUnknown: Drop-Frames-Watermark has no effect, try different parameters \n");
            fprintf(stderr, "\n\nUnknown: Drop-Frames-Watermark has no effect, try different parameters \n");
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }
        else
        {
            printf("\nPassed\n");
            fprintf(stderr, "\nPassed\n");
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 1;
        }
    }

    fclose(fp);
    string File1Str = File1;
    record_test_complete(MainDirString, File1Str, TestType);
    return 6;
}