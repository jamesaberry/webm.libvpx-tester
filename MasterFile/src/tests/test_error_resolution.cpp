#include "vpxt_test_declarations.h"

int test_error_resolution(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "ErrorResilientMode";
    char *input = argv[2];

    if (argc != 5)
    {
        printf(
            "  ErrorRes \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n ");

        return 0;
    }

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "ErrorRes";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string ErrorOnOutFile = WorkingDirString;
    string ErrorOffOutFile = WorkingDirString;

    ErrorOnOutFile.append(slashCharStr());
    ErrorOnOutFile.append("ErrorOnOutput.ivf");
    ErrorOffOutFile.append(slashCharStr());
    ErrorOffOutFile.append("ErrorOffOutput.ivf");

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


    tprintf("Error Ressiliancy Test\n");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt.target_bandwidth = BitRate;

    if (TestType == 3)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        opt.error_resilient_mode = 1;

        if (vpxt_compress_ivf_to_ivf(input, ErrorOnOutFile.c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        opt.error_resilient_mode = 0;

        if (vpxt_compress_ivf_to_ivf(input, ErrorOffOutFile.c_str(), speed, BitRate, opt, CompressString, 1, 0) == -1)
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

    tprintf("\n");

    double PSNRon;
    double PSNRoff;

    PSNRon = vpxt_ivf_psnr(input, ErrorOnOutFile.c_str(), 0, 0, 1, NULL);
    PSNRoff = vpxt_ivf_psnr(input, ErrorOffOutFile.c_str(), 0, 0, 1, NULL);

    float PSRNPerc = 100 * vpxt_abs_float((PSNRon - PSNRoff) / PSNRoff);

    tprintf("\n\nResults:\n\n");

    if (PSRNPerc < 10.00)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "ErrorRes on PSNR is within 10%% of Error Res off PSNR: %.2f%% - Passed", PSRNPerc);
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
        snprintf(OutputChar1, 255, "ErrorRes on PSNR is not within 10%% of Error Res off PSNR: %.2f%% - Failed", PSRNPerc);
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