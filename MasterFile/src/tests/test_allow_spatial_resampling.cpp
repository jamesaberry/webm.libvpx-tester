#include "vpxt_test_declarations.h"

int test_allow_spatial_resampling(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "AllowSpatialResampling";

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  AllowSpatialResampling \n\n"
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
    string WorkingDirString = ""; // <- All Options need to set a value for this
    char *MyDir = "AllowSpatialResampling";
    char WorkingDir3[255] = "";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string Spatialon = WorkingDirString;
    string Spatialoff = WorkingDirString;

    Spatialon.append(slashCharStr());
    Spatialon.append("SpatialOnOutput.ivf");
    Spatialoff.append(slashCharStr());
    Spatialoff.append("SpatialOffOutput.ivf");

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

    char *input = argv[2];

    int speed = 0;
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[3]);

    tprintf("Allow Spatial Resampling Test");

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

    opt.target_bandwidth = BitRate;
    opt.end_usage = 0;
    opt.resample_down_water_mark = 60;
    opt.resample_up_water_mark = 80;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;
        opt.allow_spatial_resampling = 0;

        if (vpxt_compress_ivf_to_ivf(input, Spatialoff.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.allow_spatial_resampling = 1;

        if (vpxt_compress_ivf_to_ivf(input, Spatialon.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 10;
    }

    double SpatialResampPSNR = vpxt_ivf_psnr(input, Spatialon.c_str(), 0, 0, 1, NULL);

    char SpatialonFileName[255];
    vpxt_file_name(Spatialon.c_str(), SpatialonFileName, 0);
    char SpatialoffFileName[255];
    vpxt_file_name(Spatialoff.c_str(), SpatialoffFileName, 0);

    tprintf("\nChecking: %s for resized frames\n", SpatialonFileName);
    int AllowSpatResampleONFramesResized = vpxt_display_resized_frames(Spatialon.c_str(), 1);

    tprintf("Checking: %s for resized frames\n", SpatialoffFileName);
    int AllowSpatResampleOFFFramesResized = vpxt_display_resized_frames(Spatialoff.c_str(), 1);

    int fail = 0;
    tprintf("\n\nResults:\n\n");

    if (AllowSpatResampleONFramesResized > 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResampleOn Frames Resized %i > 0 - Passed", AllowSpatResampleONFramesResized);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResampleOn Frames Resized %i <= 0 - Failed", AllowSpatResampleONFramesResized);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (AllowSpatResampleOFFFramesResized == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResampleOff Frames Resized %i == 0 - Passed", AllowSpatResampleOFFFramesResized);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResampleOff Frames Resized %i != 0 - Failed", AllowSpatResampleOFFFramesResized);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (SpatialResampPSNR > 15.0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResample PSNR: %f > 15.00 - Passed", SpatialResampPSNR);;
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "AllowSpatialResample On PSNR: %f < 15.00 - Failed", SpatialResampPSNR);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf("\nPassed\n");
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
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
