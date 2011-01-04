#include "vpxt_test_declarations.h"

int test_error_resolution(int argc, const char *const *argv, const string &WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Error Resilient Mode";
    char *MyDir = "test_error_resolution";

    if (argc != 5)
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
                "\n"
               );
        return 0;
    }

    string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    string ErrorOnOutFile = CurTestDirStr;
    ErrorOnOutFile.append(slashCharStr());
    ErrorOnOutFile.append(MyDir);
    ErrorOnOutFile.append("_compression_1.ivf");

    string ErrorOffOutFile = CurTestDirStr;
    ErrorOffOutFile.append(slashCharStr());
    ErrorOffOutFile.append(MyDir);
    ErrorOffOutFile.append("_compression_0.ivf");

    /////////////OutPutfile////////////
    string TextfileString = CurTestDirStr;
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

    vpxt_cap_string_print(PRINT_BTH, "%s\n", MyDir);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt.target_bandwidth = BitRate;

    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.error_resilient_mode = 1;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), ErrorOnOutFile.c_str(), speed, BitRate, opt, CompressString, 0, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt.error_resilient_mode = 0;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), ErrorOffOutFile.c_str(), speed, BitRate, opt, CompressString, 1, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
    }

    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    tprintf(PRINT_BTH, "\n");

    double PSNRon;
    double PSNRoff;

    PSNRon = vpxt_ivf_psnr(input.c_str(), ErrorOnOutFile.c_str(), 0, 0, 1, NULL);
    PSNRoff = vpxt_ivf_psnr(input.c_str(), ErrorOffOutFile.c_str(), 0, 0, 1, NULL);

    float PSRNPerc = 100 * vpxt_abs_float((PSNRon - PSNRoff) / PSNRoff);

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (PSRNPerc < 10.00)
    {
        vpxt_formated_print(RESPRT, "ErrorRes on PSNR is within 10%% of Error Res off PSNR: %.2f%% - Passed", PSRNPerc);
        tprintf(PRINT_BTH, "\n");

        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "ErrorRes on PSNR is not within 10%% of Error Res off PSNR: %.2f%% - Failed", PSRNPerc);
        tprintf(PRINT_BTH, "\n");

        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}