#include "vpxt_test_declarations.h"

int test_auto_key_frame(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Auto Key Frame";
    char *MyDir = "test_auto_key_frame";

    if (!(argc == 7 || argc == 6))
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
                "    <Key Frame Frequency>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int AutoKeyFramingInt = atoi(argv[5]);

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    std::string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string AutoKeyFramingWorks1 = CurTestDirStr;
    AutoKeyFramingWorks1.append(slashCharStr());
    AutoKeyFramingWorks1.append(MyDir);
    AutoKeyFramingWorks1.append("_compression_1.ivf");

    std::string AutoKeyFramingWorks2 = CurTestDirStr;
    AutoKeyFramingWorks2.append(slashCharStr());
    AutoKeyFramingWorks2.append(MyDir);
    AutoKeyFramingWorks2.append("_compression_2.ivf");

    std::string KeyFrameTxtOut1 = CurTestDirStr;
    KeyFrameTxtOut1.append(slashCharStr());
    KeyFrameTxtOut1.append(MyDir);
    KeyFrameTxtOut1.append("_compression_1_key_frames.txt");

    std::string KeyFrameTxtOut2 = CurTestDirStr;
    KeyFrameTxtOut2.append(slashCharStr());
    KeyFrameTxtOut2.append(MyDir);
    KeyFrameTxtOut2.append("_compression_2_key_frames.txt");

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
    if (argc == 7)
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
    opt.auto_key = 1;
    opt.key_freq = AutoKeyFramingInt;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), AutoKeyFramingWorks1.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        if (vpxt_compress_ivf_to_ivf(input.c_str(), AutoKeyFramingWorks2.c_str(), speed, BitRate, opt, CompressString, AutoKeyFramingInt, 0) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
    }

    //Create Compression only stop test short.
    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    vpxt_display_key_frames(AutoKeyFramingWorks1.c_str(), 1);
    vpxt_display_key_frames(AutoKeyFramingWorks2.c_str(), 1);

    std::ifstream inFileIndexOutputChar(KeyFrameTxtOut1.c_str());
    std::ifstream infile2(KeyFrameTxtOut2.c_str());

    if (!inFileIndexOutputChar.good())
    {
        tprintf(PRINT_BTH, "\nKey Frame File 1 Not Found: %s\n", KeyFrameTxtOut1.c_str());
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    if (!infile2.good())
    {
        tprintf(PRINT_BTH, "\nKey Frame File 2 Not Found: %s\n", KeyFrameTxtOut2.c_str());
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    int fail = 0;
    int x;
    int y;

    while (!inFileIndexOutputChar.eof() && !infile2.eof())
    {
        inFileIndexOutputChar >> x;
        infile2 >> y;

        if (x != y)
        {
            fail = 1;
        }

    }

    if (!inFileIndexOutputChar.eof())
    {
        fail = 1;
    }

    if (!infile2.eof())
    {
        fail = 1;
    }

    inFileIndexOutputChar.close();
    infile2.close();

    char AutoKeyFramingWorks1FileName[255];
    vpxt_file_name(AutoKeyFramingWorks1.c_str(), AutoKeyFramingWorks1FileName, 0);
    char AutoKeyFramingWorks2FileName[255];
    vpxt_file_name(AutoKeyFramingWorks2.c_str(), AutoKeyFramingWorks2FileName, 0);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    //////////////////////////////////////////////////////////////////////////////////
    int fail2 = 0;
    std::ifstream infile(KeyFrameTxtOut1.c_str());

    if (!infile.good())
    {
        tprintf(PRINT_BTH, "\nKey Frame File Not Present - Failed");
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
            vpxt_formated_print(RESPRT, "Key Frames do not occur at least as frequently as Auto Key Frame dictates: %i No key frames between %i and %i - Failed", AutoKeyFramingInt, x2, y2);
            tprintf(PRINT_BTH, "\n");
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
        vpxt_formated_print(RESPRT, "Key Frames do not occur at least as frequently as Auto Key Frame dictates: %i No key frames between %i and %i - Failed", AutoKeyFramingInt, maxKeyFrame, NumberofFrames - 1);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    ///////////////////////////////////////////////////////////////////////////
    if (fail2 == 0)
    {
        vpxt_formated_print(RESPRT, "Key Frames occur at least as frequently as Auto Key Frame dictates: %i - Passed", AutoKeyFramingInt);
        tprintf(PRINT_BTH, "\n");
    }

    if (fail == 0)
    {
        vpxt_formated_print(RESPRT, "Key Frames occur at the same locations for %s and %s - Passed", AutoKeyFramingWorks1FileName, AutoKeyFramingWorks2FileName);
        tprintf(PRINT_BTH, "\n");

        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, AutoKeyFramingWorks1.c_str(), AutoKeyFramingWorks2.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Key Frames do not occur at the same locations for %s and %s - Failed", AutoKeyFramingWorks1FileName, AutoKeyFramingWorks2FileName);
        tprintf(PRINT_BTH, "\n");

        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(2, AutoKeyFramingWorks1.c_str(), AutoKeyFramingWorks2.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}