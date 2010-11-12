#include "vpxt_test_declarations.h"

int test_frame_size(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Frame Size";

    int StartingWidth = 0;
    int StartingHeight = 0;

    if (!(argc == 8 || argc == 7))
    {
        printf(
            "  FrameSizeTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Starting Width-must be a mult of 16>\n"
            "    <Starting Height-must be a mult of 16>\n"
            "    <Optional Settings File>\n"
        );

        return 0;
    }

    StartingWidth = atoi(argv[5]);
    StartingHeight = atoi(argv[6]);



    char *input = argv[2];

    ////////////Formatting Test Specific Directory////////////

    string WorkingDirString = ""; // <- All Options need to set a value for this

    string MainDirString = "";
    char *MyDir = "FrameSizeTest";

    char WorkingDir3[255] = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    char InputFileName[256] = "";
    vpxt_file_name(input, InputFileName, 1);

    string WorkingDir4 = WorkingDirString;
    WorkingDir4.append(slashCharStr());
    WorkingDir4.append(InputFileName);

    char NewWidth[20];
    char NewHeight[20];
    string RawCrop[47];

    //height
    int counter = 0;
    int FileNum = 1;

    while (counter < 16)
    {
        RawCrop[FileNum] = WorkingDir4;
        RawCrop[FileNum].append("_");
        vpx_itoa_custom(StartingWidth, NewWidth, 10); //width
        RawCrop[FileNum].append(NewWidth);
        RawCrop[FileNum].append("x");
        vpx_itoa_custom(StartingHeight - (counter), NewHeight, 10); //height
        RawCrop[FileNum].append(NewHeight);
        RawCrop[FileNum].append("_raw.ivf");

        counter++;
        FileNum++;
    }

    //width
    counter = 1;

    while (counter < 16)
    {
        RawCrop[FileNum] = WorkingDir4;
        RawCrop[FileNum].append("_");
        vpx_itoa_custom(StartingWidth - (counter), NewWidth, 10); //width
        RawCrop[FileNum].append(NewWidth);
        RawCrop[FileNum].append("x");
        vpx_itoa_custom(StartingHeight, NewHeight, 10); //height
        RawCrop[FileNum].append(NewHeight);
        RawCrop[FileNum].append("_raw.ivf");

        counter++;
        FileNum++;
    }

    //width and height
    counter = 1;

    while (counter < 16)
    {
        RawCrop[FileNum] = WorkingDir4;
        RawCrop[FileNum].append("_");
        vpx_itoa_custom(StartingWidth - (counter), NewWidth, 10); //width
        RawCrop[FileNum].append(NewWidth);
        RawCrop[FileNum].append("x");
        vpx_itoa_custom(StartingHeight - (counter), NewHeight, 10); //height
        RawCrop[FileNum].append(NewHeight);
        RawCrop[FileNum].append("_raw.ivf");

        counter++;
        FileNum++;
    }

    //encoded file names
    FileNum = 1;
    string EncCrop[47];

    while (FileNum < 47)
    {
        EncCrop[FileNum] = RawCrop[FileNum];
        EncCrop[FileNum].erase(EncCrop[FileNum].end() - 7, EncCrop[FileNum].end());
        EncCrop[FileNum].append("enc.ivf");
        FileNum++;
    }

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

    //Make sure starting width and height are mults of 16
    if ((StartingWidth % 16 != 0) && (StartingHeight % 16 != 0))
    {
        tprintf("\nError: Starting width and height are not multiples of 16\n");

        tprintf("\nFailed\n");
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (StartingHeight % 16 != 0)
    {
        tprintf("\nError: Starting height is not a multiple of 16\n");

        tprintf("\nFailed\n");
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (StartingWidth % 16 != 0)
    {
        tprintf("\nError: Starting width is not a multiple of 16\n");

        tprintf("\nFailed\n");
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    int speed = 0;
    int BitRate = atoi(argv[4]);

    int Mode = atoi(argv[3]);

    tprintf("Frame Size Test");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 8)
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

    //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
    //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
    //Test Type 3 = Mode 2 = Run Test Compressions

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {

        //Create Raw Crops
        int x = 0;
        int RawCropNum = 1;

        while (x < 16)
        {
            tprintf("\nCroping to %i %i", StartingWidth, StartingHeight - x);
            vpxt_crop_raw_ivf(input, RawCrop[RawCropNum].c_str(), 0, 0, StartingWidth, StartingHeight - x, 1, 1);
            x++;
            RawCropNum++;
            //return 0;
        }

        x = 1;

        while (x < 16)
        {
            tprintf("\nCroping to %i %i", StartingWidth - x, StartingHeight);
            vpxt_crop_raw_ivf(input, RawCrop[RawCropNum].c_str(), 0, 0, StartingWidth - x, StartingHeight, 1, 1);
            x++;
            RawCropNum++;
        }

        x = 1;

        while (x < 16)
        {
            tprintf("\nCroping to %i %i", StartingWidth - x, StartingHeight - x);
            vpxt_crop_raw_ivf(input, RawCrop[RawCropNum].c_str(), 0, 0, StartingWidth - x, StartingHeight - x, 1, 1);
            x++;
            RawCropNum++;
        }

        opt.Mode = Mode;

        //Create Compressions
        RawCropNum = 1;

        while (RawCropNum < 47)
        {
            char FileNameChar[256];
            char FileNameChar2[256];
            snprintf(FileNameChar, 256, RawCrop[RawCropNum].c_str());
            vpxt_file_name(FileNameChar, FileNameChar2, 1);

            tprintf("\nCompressing %s", FileNameChar2);

            if (vpxt_compress_ivf_to_ivf(RawCrop[RawCropNum].c_str(), EncCrop[RawCropNum].c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 2;
            }

            RawCropNum++;
        }
    }

    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 10;
    }

    int PercentFail = 0;
    int MinPSNRFail = 0;
    double PSNRAr[46];

    int RawCropNum = 1;

    while (RawCropNum < 47)
    {
        PSNRAr[RawCropNum-1] = vpxt_ivf_psnr(RawCrop[RawCropNum].c_str(), EncCrop[RawCropNum].c_str(), 0, 0, 1, NULL);
        RawCropNum++;
    }

    RawCropNum = 1;
    double FivePercentPSNR = (5 * PSNRAr[0]) / 100;

    while (RawCropNum < 47)
    {
        char FileNameChar[256];
        char FileNameChar2[256];
        snprintf(FileNameChar, 256, EncCrop[RawCropNum].c_str());
        vpxt_file_name(FileNameChar, FileNameChar2, 0);

        if (RawCropNum == 1)
        {
            printf("\n\n PSNR %s: %.2f", FileNameChar2, PSNRAr[RawCropNum-1]);
        }
        else if (PSNRAr[RawCropNum-1] <  PSNRAr[0] + FivePercentPSNR && PSNRAr[RawCropNum-1] >  PSNRAr[0] - FivePercentPSNR)
        {
            tprintf("\n PSNR %s: %.2f within 5%% of %.2f - Passed", FileNameChar2, PSNRAr[RawCropNum-1], PSNRAr[0]);
        }
        else
        {
            tprintf("\n PSNR %s: %.2f not within 5%% of %.2f - Failed", FileNameChar2, PSNRAr[RawCropNum-1], PSNRAr[0]);
            PercentFail = 1;
        }

        RawCropNum++;
    }

    tprintf("\n\n");

    RawCropNum = 1;

    while (RawCropNum < 47)
    {
        char FileNameChar[256];
        char FileNameChar2[256];
        snprintf(FileNameChar, 256, EncCrop[RawCropNum].c_str());
        vpxt_file_name(FileNameChar, FileNameChar2, 0);

        if (PSNRAr[RawCropNum-1] > 25.0)
        {
            tprintf("\n PSNR %s: %.2f > %.2f - Passed", FileNameChar2, PSNRAr[RawCropNum-1], 25.0);
        }
        else
        {
            tprintf("\n PSNR %s: %.2f < %.2f - Failed", FileNameChar2, PSNRAr[RawCropNum-1], 25.0);
            MinPSNRFail = 1;
        }

        RawCropNum++;
    }


    printf("\n");
    tprintf("\n\nResults:\n\n");

    int fail = 0;

    if (PercentFail == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All PSNRs are within 3%% of %.2f - Passed", PSNRAr[0]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Not all PSNRs are within 3%% of %.2f - Failed", PSNRAr[0]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (MinPSNRFail == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All PSNRs are greater than 25.0 - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Not all PSNRs are greater than 25.0 - Failed");
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