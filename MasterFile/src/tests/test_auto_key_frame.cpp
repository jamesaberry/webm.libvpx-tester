#include "vpxt_test_declarations.h"

int test_auto_key_frame(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "opt.auto_keyFrame";

    if (!(argc == 7 || argc == 6))
    {
        printf(
            "  AutoKeyFramingWorks \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Key Frame Frequency>\n"
            "    <Optional Settings File>\n"
            "\n");
        return 0;
    }

    int speed = 0;
    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int AutoKeyFramingInt = atoi(argv[5]);

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";
    char WorkingDir3[255] = "";
    char *MyDir = "AutoKeyFramingWorks";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string KeyFrameTxtOut1 = WorkingDirString;
    string KeyFrameTxtOut2 = WorkingDirString;
    string AutoKeyFramingWorks1 = WorkingDirString;
    string AutoKeyFramingWorks2 = WorkingDirString;

    KeyFrameTxtOut1.append(slashCharStr());
    KeyFrameTxtOut1.append("AutoKeyFramingWorksOutput1_KeyFrames.txt");
    AutoKeyFramingWorks1.append(slashCharStr());
    AutoKeyFramingWorks1.append("AutoKeyFramingWorksOutput1.ivf");
    KeyFrameTxtOut2.append(slashCharStr());
    KeyFrameTxtOut2.append("AutoKeyFramingWorksOutput2_KeyFrames.txt");
    AutoKeyFramingWorks2.append(slashCharStr());
    AutoKeyFramingWorks2.append("AutoKeyFramingWorksOutput2.ivf");

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

    tprintf("Auto Key Framing Works Test");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 7)
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
    opt.auto_key = 1;
    opt.key_freq = AutoKeyFramingInt;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress_ivf_to_ivf(input, AutoKeyFramingWorks1.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        if (vpxt_compress_ivf_to_ivf(input, AutoKeyFramingWorks2.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
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

    vpxt_display_key_frames(AutoKeyFramingWorks1.c_str(), 1);
    vpxt_display_key_frames(AutoKeyFramingWorks2.c_str(), 1);

    ifstream infile1(KeyFrameTxtOut1.c_str());
    ifstream infile2(KeyFrameTxtOut2.c_str());

    if (!infile1.good())
    {
        tprintf("\nKey Frame File 1 Not Found: %s\n", KeyFrameTxtOut1.c_str());
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (!infile2.good())
    {
        tprintf("\nKey Frame File 2 Not Found: %s\n", KeyFrameTxtOut2.c_str());
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    int fail = 0;
    int x;
    int y;

    while (!infile1.eof() && !infile2.eof())
    {
        infile1 >> x;
        infile2 >> y;

        if (x != y)
        {
            fail = 1;
        }

    }

    if (!infile1.eof())
    {
        fail = 1;
    }

    if (!infile2.eof())
    {
        fail = 1;
    }

    infile1.close();
    infile2.close();

    char AutoKeyFramingWorks1FileName[255];
    vpxt_file_name(AutoKeyFramingWorks1.c_str(), AutoKeyFramingWorks1FileName, 0);
    char AutoKeyFramingWorks2FileName[255];
    vpxt_file_name(AutoKeyFramingWorks2.c_str(), AutoKeyFramingWorks2FileName, 0);

    tprintf("\n\nResults:\n\n");

    //////////////////////////////////////////////////////////////////////////////////
    int fail2 = 0;
    ifstream infile(KeyFrameTxtOut1.c_str());

    if (!infile.good())
    {
        tprintf("\nKey Frame File Not Present - Failed");
        fclose(fp);
        return 0;
    }

    int selector2 = 0;
    int doOnce2 = 0;
    int x2;
    int y2;

    while (!infile.eof())
    {
        if (selector2 == 1)
        {
            infile >> x2;
            selector2 = 2;
        }
        else
        {
            infile >> y2;
            selector2 = 1;
        }

        if (doOnce2 == 0)
        {
            x2 = 0;
            infile >> y2;
            doOnce2 = 1;
            selector2 = 1;
        }

        if (vpxt_abs_int(y2 - x2) > AutoKeyFramingInt)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Key Frames do not occur at least as frequently as Auto Key Frame dictates: %i No key frames between %i and %i - Failed", AutoKeyFramingInt, x2, y2);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            fail2 = 1;
        }
    }

    int maxKeyFrame = 0;

    if (x2 > y2)
    {
        maxKeyFrame = x2;
    }
    else
    {
        maxKeyFrame = y2;
    }

    int NumberofFrames = vpxt_get_number_of_frames(AutoKeyFramingWorks1.c_str());

    if (NumberofFrames - 1 >= (maxKeyFrame + AutoKeyFramingInt))
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Key Frames do not occur at least as frequently as Auto Key Frame dictates: %i No key frames between %i and %i - Failed", AutoKeyFramingInt, maxKeyFrame, NumberofFrames - 1);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    ///////////////////////////////////////////////////////////////////////////
    if (fail2 == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Key Frames occur at least as frequently as Auto Key Frame dictates: %i - Passed", AutoKeyFramingInt);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (fail == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Key Frames occur at the same locations for %s and %s - Passed", AutoKeyFramingWorks1FileName, AutoKeyFramingWorks2FileName);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Key Frames do not occur at the same locations for %s and %s - Failed", AutoKeyFramingWorks1FileName, AutoKeyFramingWorks2FileName);
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