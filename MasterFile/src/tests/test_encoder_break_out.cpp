#include "vpxt_test_declarations.h"

int test_encoder_break_out(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "EncoderBreakOut";

    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  EncoderBreakOut \n\n"
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

    string WorkingDirString = ""; // <- All Options need to set a value for this

    string MainDirString = "";
    char *MyDir = "EncoderBreakOut";

    char WorkingDir3[255] = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string EncBreakOut0 = WorkingDirString;
    string EncBreakOut100 = WorkingDirString;
    string EncBreakOut500 = WorkingDirString;
    string EncBreakOut1000 = WorkingDirString;

    string EncBreakOut0_Dec = WorkingDirString;
    string EncBreakOut100_Dec = WorkingDirString;
    string EncBreakOut500_Dec = WorkingDirString;
    string EncBreakOut1000_Dec = WorkingDirString;

    EncBreakOut0.append(slashCharStr());
    EncBreakOut0.append("EncBreakOut0.ivf");
    EncBreakOut100.append(slashCharStr());
    EncBreakOut100.append("EncBreakOut100.ivf");
    EncBreakOut500.append(slashCharStr());
    EncBreakOut500.append("EncBreakOut500.ivf");
    EncBreakOut1000.append(slashCharStr());
    EncBreakOut1000.append("EncBreakOut1000.ivf");

    EncBreakOut0_Dec.append(slashCharStr());
    EncBreakOut0_Dec.append("EncBreakOut0_Dec.ivf");
    EncBreakOut100_Dec.append(slashCharStr());
    EncBreakOut100_Dec.append("EncBreakOut100_Dec.ivf");
    EncBreakOut500_Dec.append(slashCharStr());
    EncBreakOut500_Dec.append("EncBreakOut500_Dec.ivf");
    EncBreakOut1000_Dec.append(slashCharStr());
    EncBreakOut1000_Dec.append("EncBreakOut1000_Dec.ivf");

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
    int BitRate = atoi(argv[4]);;

    int Mode = atoi(argv[3]);


    tprintf("Encoder Break Out Test");

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

    //Test Type 1 = Mode 1 = Run Test Compressions and Tests.
    //Test Type 2 = Mode 3 = Run tests from Pre-existing Compressed file
    //Test Type 3 = Mode 2 =Run Test Compressions

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.encode_breakout = 0;

        if (vpxt_compress_ivf_to_ivf(input, EncBreakOut0.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.encode_breakout = 100;

        if (vpxt_compress_ivf_to_ivf(input, EncBreakOut100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.encode_breakout = 500;

        if (vpxt_compress_ivf_to_ivf(input, EncBreakOut500.c_str(), speed, BitRate, opt, CompressString, 500, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.encode_breakout = 1000;

        if (vpxt_compress_ivf_to_ivf(input, EncBreakOut1000.c_str(), speed, BitRate, opt, CompressString, 1000, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        tprintf("\nDecoding EncBreakOut0");

        if (vpxt_decompress_ivf_to_ivf(EncBreakOut0.c_str(), EncBreakOut0_Dec.c_str()) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        tprintf("\n\nDecoding EncBreakOut100");

        if (vpxt_decompress_ivf_to_ivf(EncBreakOut100.c_str(), EncBreakOut100_Dec.c_str()) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        tprintf("\n\nDecoding EncBreakOut500");

        if (vpxt_decompress_ivf_to_ivf(EncBreakOut500.c_str(), EncBreakOut500_Dec.c_str()) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        tprintf("\n\nDecoding EncBreakOut1000");

        if (vpxt_decompress_ivf_to_ivf(EncBreakOut1000.c_str(), EncBreakOut1000_Dec.c_str()) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }
    }

    if (TestType == 2)
    {
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 10;
    }

    long SourceFileSize = 0;

    long EncBreakOut0_Dec_FileSize = 0;
    long EncBreakOut100_Dec_FileSize = 0;
    long EncBreakOut500_Dec_FileSize = 0;
    long EncBreakOut1000_Dec_FileSize = 0;

    tprintf("\n\n");
    SourceFileSize = vpxt_file_size(input, 1);
    tprintf("\n");
    EncBreakOut0_Dec_FileSize = vpxt_file_size(EncBreakOut0_Dec.c_str(), 1);
    tprintf("\n");
    EncBreakOut100_Dec_FileSize = vpxt_file_size(EncBreakOut100_Dec.c_str(), 1);
    tprintf("\n");
    EncBreakOut500_Dec_FileSize = vpxt_file_size(EncBreakOut500_Dec.c_str(), 1);
    tprintf("\n");
    EncBreakOut1000_Dec_FileSize = vpxt_file_size(EncBreakOut1000_Dec.c_str(), 1);
    tprintf("\n");

    double PSNR0;
    double PSNR100;
    double PSNR500;
    double PSNR1000;

    PSNR0 = vpxt_ivf_psnr(input, EncBreakOut0.c_str(), 0, 0, 1, NULL);
    PSNR100 = vpxt_ivf_psnr(input, EncBreakOut100.c_str(), 0, 0, 1, NULL);
    PSNR500 = vpxt_ivf_psnr(input, EncBreakOut500.c_str(), 0, 0, 1, NULL);
    PSNR1000 = vpxt_ivf_psnr(input, EncBreakOut1000.c_str(), 0, 0, 1, NULL);

    double dB1 = vpxt_abs_double(10 * log(PSNR0 / PSNR100));
    double dB2 = vpxt_abs_double(10 * log(PSNR100 / PSNR500));
    double dB3 = vpxt_abs_double(10 * log(PSNR500 / PSNR1000));

    tprintf("\ndB1: %f \n", dB1);
    tprintf("dB2: %f \n", dB1);
    tprintf("dB3: %f \n", dB1);
    tprintf("\n\nResults:\n\n");

    int Pass = 1;
    int IndCount = 0;

    if (SourceFileSize != EncBreakOut0_Dec_FileSize)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut0 was not properly decoded.");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        Pass = 0;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut0 was properly decoded.");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (SourceFileSize != EncBreakOut100_Dec_FileSize)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut100 was not properly decoded.");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        Pass = 0;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut100 was properly decoded.");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (SourceFileSize != EncBreakOut500_Dec_FileSize)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut500 was not properly decoded.");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        Pass = 0;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut500 was properly decoded.");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (SourceFileSize != EncBreakOut1000_Dec_FileSize)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut1000 was not properly decoded.");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        Pass = 0;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "EncBreakOut1000 was properly decoded.");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (dB1 <= 2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 0 PSNR and 100 PSNR are within 2 dB: %4f dB - Passed", dB1);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (dB1 >= 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 0 PSNR and 100 PSNR not within 5 dB: %4f dB - Failed", dB1);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        Pass = 0;
    }

    if (dB1 > 2 && dB1 < 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 0 PSNR and 100 PSNR greater than 2 dB but less than 5 dB: %4f dB - Indeterminate", dB1);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        IndCount++;
    }

    if (dB2 <= 2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 100 PSNR and 500 PSNR are within 2 dB: %4f dB - Passed", dB2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (dB2 >= 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 100 PSNR and 500 PSNR not within 5 dB: %4f dB - Failed", dB2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        Pass = 0;
    }

    if (dB2 > 2 && dB2 < 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 100 PSNR and 500 PSNR greater than 2 dB but less than 5 dB: %4f dB - Indeterminate", dB2);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        IndCount++;
    }

    if (dB3 <= 2)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 500 PSNR and 1000 PSNR are within 2 dB: %4f dB - Passed", dB3);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (dB3 >= 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 500 PSNR and 1000 PSNR not within 5 dB: %4f dB - Failed", dB3);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        Pass = 0;
    }

    if (dB3 > 2 && dB3 < 5)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Encoder Breakout 500 PSNR and 1000 PSNR greater than 2 dB but less than 5 dB: %4f dB - Indeterminate", dB3);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        IndCount++;
    }

    if (Pass == 1)
    {
        if (IndCount == 0)
        {
            tprintf("\nPassed\n");
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 1;
        }
        else
        {
            tprintf("\nIndeterminate\n");
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
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