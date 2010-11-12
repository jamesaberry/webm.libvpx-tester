#include "vpxt_test_declarations.h"

int test_undershoot(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "UnderShootPct";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  UnderShoot \n\n"
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
    char *MyDir = "UnderShoot";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string UnderShoot10 = WorkingDirString;
    string UnderShoot100 = WorkingDirString;

    UnderShoot10.append(slashCharStr());
    UnderShoot10.append("UnderShoot10Output.ivf");
    UnderShoot100.append(slashCharStr());
    UnderShoot100.append("UnderShoot100Output.ivf");

    //char UnderShoot10[255];
    //char UnderShoot100[255];

    //snprintf(UnderShoot10, 255, "%s", WorkingDir4.c_str());
    //snprintf(UnderShoot100, 255, "%s", WorkingDir5.c_str());

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

    tprintf("Undershoot Test");

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

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.under_shoot_pct = 10;

        if (vpxt_compress_ivf_to_ivf(input, UnderShoot10.c_str(), speed, BitRate, opt, CompressString, 10, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.under_shoot_pct = 100;

        if (vpxt_compress_ivf_to_ivf(input, UnderShoot100.c_str(), speed, BitRate, opt, CompressString, 100, 0) == -1)
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

    tprintf("\n");

    tprintf("\n");
    long File1bytes = vpxt_file_size(UnderShoot10.c_str(), 1);
    tprintf("\n");
    long File2bytes = vpxt_file_size(UnderShoot100.c_str(), 1);
    tprintf("\n");

    tprintf("\n\nResults:\n\n");

    if (File1bytes < File2bytes)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "File size 1:%i < File size 2:%i - Passed", File1bytes , File2bytes);;
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);

        tprintf("\n");
        tprintf("\nPassed\n");


        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }

    if (File1bytes == File2bytes)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "No effect try different file - Indeterminate");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);

        tprintf("\n");
        tprintf("\nIndeterminate\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "File size 1:%i > File size 2:%i - Failed", File1bytes , File2bytes);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);

        tprintf("\n");
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