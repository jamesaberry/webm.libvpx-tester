#include "vpxt_test_declarations.h"

int test_noise_sensitivity(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    //This test looks a lot like NoiseSensitivityWorks but has a different purpose.  This test ensures
    //That different noise sensitivities have an effect for each possible noise sensitivity value and
    //passes only if that is the case.
    char *CompressString = "NoiseSensitivity";
    char *input = argv[2];

    if (!(argc == 6 || argc == 5))
    {
        printf(
            "  PreProcessorWorks \n\n"
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

    int speed = 0;
    int BitRate = atoi(argv[4]);
    int Mode = atoi(argv[3]);



    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "NoiseSensitivityWorks";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string NoiseSenseBase = WorkingDirString;
    NoiseSenseBase.append(slashCharStr());
    NoiseSenseBase.append("NoiseSenseOut");

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

    tprintf("Noise Sensitivity Works Test");

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

    int Noise = 0;
    long File2bytes[7];
    double PSNRArr[7];
    int doOnce = 1;
    int PSNRToggle;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        while (Noise != 7)
        {
            char num[20];
            vpx_itoa_custom(Noise, num, 10);

            string NoiseSenseOut = NoiseSenseBase;
            NoiseSenseOut.append(num);
            NoiseSenseOut.append(".ivf");

            //char NoiseSenseOut[255];
            //snprintf(NoiseSenseOut, 255, "%s", WorkingDir5.c_str());

            if (doOnce == 1)
            {
                doOnce = 0;
            }

            tprintf("\n");
            PSNRArr[Noise] = vpxt_ivf_psnr(input, NoiseSenseOut.c_str(), 0, 0, 1, NULL);
            tprintf("\n");
            File2bytes[Noise] = vpxt_file_size(NoiseSenseOut.c_str(), 1);
            tprintf("\n");

            Noise++;
        }
    }
    else
    {
        while (Noise != 7)
        {

            char num[20];
            vpx_itoa_custom(Noise, num, 10);

            string NoiseSenseOut = NoiseSenseBase;
            NoiseSenseOut.append(num);
            NoiseSenseOut.append(".ivf");

            //char NoiseSenseOut[255];
            //snprintf(NoiseSenseOut, 255, "%s", WorkingDir5.c_str());

            opt.Mode = Mode;

            opt.noise_sensitivity = Noise;

            if (vpxt_compress_ivf_to_ivf(input, NoiseSenseOut.c_str(), speed, BitRate, opt, CompressString, Noise, 0) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 2;
            }

            if (TestType != 2)
            {

                tprintf("\n");
                PSNRArr[Noise] = vpxt_ivf_psnr(input, NoiseSenseOut.c_str(), 0, 0, 1, NULL);
                tprintf("\n");
                File2bytes[Noise] = vpxt_file_size(NoiseSenseOut.c_str(), 1);
                tprintf("\n");
            }

            Noise++;
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

    //checks 0v1 | 1v2 | 2v3 | 3v4 | 4v5 | 5v6 Could be modified to check all against eachother if wanted.
    int n = 0;
    int fail = 0;

    tprintf("\n\nResults:\n\n");

    while (n != 6)
    {
        if (PSNRArr[n] == PSNRArr[n+1] && File2bytes[n] == File2bytes[n+1])
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Noise %i PSNR %.2f == Noise %i PSNR %.2f - Failed", n, PSNRArr[n], n + 1, PSNRArr[n+1]);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            fail = 1;

        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Noise %i PSNR %.2f != Noise %i PSNR %.2f - Passed", n, PSNRArr[n], n + 1, PSNRArr[n+1]);
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }

        n++;
    }

    if (PSNRArr[0] <= PSNRArr[6])
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Noise 0 PSNR: %.2f <= Noise 6 PSNR: %.2f - Failed", PSNRArr[0], PSNRArr[6]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }
    else
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Noise 0 PSNR: %.2f > Noise 6 PSNR: %.2f - Passed", PSNRArr[0], PSNRArr[6]);
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
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