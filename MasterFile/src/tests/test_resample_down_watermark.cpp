#include "vpxt_test_declarations.h"

#include "vpxt_test_declarations.h"

int test_resample_down_watermark(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "ResampleDownWaterMark";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  ResampleDownWaterMark \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n "
            "    <Optional Settings File>\n"
        );
        return 0;
    }

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "ResampleDownWaterMark";
    string MainDirString = "";
    char File1[255] = "";


    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string DownWaterSamp90OutFile = WorkingDirString;
    string DownWaterSamp10OutFile = WorkingDirString;

    DownWaterSamp90OutFile.append(slashCharStr());
    DownWaterSamp90OutFile.append("DownWaterSamp90Output.ivf");
    DownWaterSamp10OutFile.append(slashCharStr());
    DownWaterSamp10OutFile.append("DownWaterSamp10Output.ivf");

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
        print_header_full_test(argc, argv, WorkingDirString);
    }

    if (TestType == 2)
    {
        print_header_compression_only(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        print_header_test_only(argc, argv, WorkingDirString);
    }

    tprintf("Resample Down WaterMark Test");

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

    int speed = 0;

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
    opt.allow_lag = 0;
    opt.allow_spatial_resampling = 1;
    opt.end_usage = 0;
    opt.target_bandwidth = BitRate;

    int ResampleDownWaterMarkHigh = 90;
    int ResampleDownWaterMarkLow = 20;

    int n = 0;
    double PSNRArr[2];

    opt.resample_up_water_mark = 100;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.resample_down_water_mark = ResampleDownWaterMarkHigh;

        if (vpxt_compress_ivf_to_ivf(input, DownWaterSamp90OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.resample_down_water_mark = ResampleDownWaterMarkLow;

        if (vpxt_compress_ivf_to_ivf(input, DownWaterSamp10OutFile.c_str(), speed, BitRate, opt, CompressString, opt.resample_down_water_mark, 0) == -1)
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

    char DownWaterSamp10Filename[255];
    vpxt_file_name(DownWaterSamp10OutFile.c_str(), DownWaterSamp10Filename, 0);
    char DownWaterSamp90Filename[255];
    vpxt_file_name(DownWaterSamp90OutFile.c_str(), DownWaterSamp90Filename, 0);

    int DispKeyFrames10int = vpxt_display_key_frames(DownWaterSamp10OutFile.c_str(), 1);
    int DispResized10int = vpxt_display_resized_frames(DownWaterSamp10OutFile.c_str(), 1);
    int DispCheckPBMThr10int = vpxt_ivf_check_pbm_threshold(DownWaterSamp10OutFile.c_str(), opt.target_bandwidth, opt.maximum_buffer_size * 1000, opt.starting_buffer_level * 1000, opt.optimal_buffer_level * 1000, ResampleDownWaterMarkLow);
    int RDWMCheck10int = 0;

    if (DispResized10int > 0)
    {
        printf("\nChecking %s buffer for correct resize frame placement:\n\n", DownWaterSamp10Filename);
        RDWMCheck10int = vpxt_dfwm_check(DownWaterSamp10OutFile.c_str(), 1);
    }
    else
    {
        RDWMCheck10int = -2;
    }

    int DispKeyFrames90int = vpxt_display_key_frames(DownWaterSamp90OutFile.c_str(), 1);
    int DispResized90int = vpxt_display_resized_frames(DownWaterSamp90OutFile.c_str(), 1);
    int DispCheckPBMThr90int = vpxt_ivf_check_pbm_threshold(DownWaterSamp90OutFile.c_str(), opt.target_bandwidth, opt.maximum_buffer_size * 1000, opt.starting_buffer_level * 1000, opt.optimal_buffer_level * 1000, ResampleDownWaterMarkHigh);
    int RDWMCheck90int = -3;

    if (DispResized90int > 0)
    {
        printf("\nChecking %s buffer for correct resize frame placement:\n\n", DownWaterSamp90Filename);
        RDWMCheck90int = vpxt_dfwm_check(DownWaterSamp90OutFile.c_str(), 1);
    }
    else
    {
        RDWMCheck90int = -2;
    }

    /*cout << "\n";
    cout << "DispKeyFrames90int: " <<  DispKeyFrames90int << "\n";
    cout << "DispResized90int: " <<  DispResized90int << "\n";
    cout << "DispCheckPBMThr90int: " <<  DispCheckPBMThr90int << "\n";
    cout << "RDWMCheck90int: " << RDWMCheck90int << "\n";
    cout << "\n";
    cout << "DispKeyFrames10int: " <<  DispKeyFrames10int << "\n";
    cout << "DispResized10int: " <<  DispResized10int << "\n";
    cout << "DispCheckPBMThr10int: " <<  DispCheckPBMThr10int << "\n";
    cout << "RDWMCheck90int: " << RDWMCheck90int << "\n";*/

    int fail = 0; //1 = failed // 2 = indt // 3 = track resize for 10 // track resize for 90
    tprintf("\n\nResults:\n\n");

    if (DispResized10int > 0 && DispResized90int > 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Both DWMS 10 and 90 returned resized frames - Indeterminate");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (DispResized10int == 0 && DispResized90int > 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DWMS 10 returned no resized frames; DWMS 90 returned resized frames - Indeterminate");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 4;
        //indt
    }

    if (DispResized10int > 0 && DispResized90int == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DWMS 90 returned no resized frames; DWMS 10 returned resized frames - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
        //fail
    }

    if (DispResized10int == 0 && DispResized90int == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Both DWMS 10 and 90 returned  no resized frames - Indeterminate");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 2;
    }

    if (RDWMCheck10int == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DWMS 10 resizes first frame at correct buffer location - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        //fail
    }

    if (RDWMCheck10int == 1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DWMS 10 does not resize first frame at correct buffer location - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
        //fail
    }

    if (RDWMCheck10int == -3)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DWMS 10 buffer threshold never reached - Indeterminate");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 2;
        //fail
    }

    if (RDWMCheck90int == 0)
    {
        if (fail == 4)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DWMS 90 resizes first frame at correct buffer location - Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            fail = 3;
            //fail
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "DWMS 90 resizes first frame at correct buffer location - Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            //fail
        }
    }

    if (RDWMCheck90int == 1)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DWMS 90 does not resize first frame at correct buffer location - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 2;
        //fail
    }

    if (RDWMCheck90int == -3)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "DWMS 90 buffer threshold never reached - Indeterminate");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 2;
        //fail
    }

    if (fail == 0)
    {
        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (fail == 3)
    {
        tprintf("\nMin Passed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 8;
    }

    if (fail == 2)
    {
        tprintf("\nIndeterminate\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
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