#include "vpxt_test_declarations.h"

int test_win_lin_mac_match(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "WinLinMacMatch";


    if (!(argc == 8 || argc == 7))
    {
        printf(
            "  WinLinMacMatch \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "          (2)One Pass Best Quality\n"
            "          (3)Two Pass - First Pass\n"
            "          (4)Two Pass\n"
            "          (5)Two Pass Best Quality\n"
            "    <Target Bit Rate>\n"
            "    <Common Base Folder>\n"
            "    <Common Settings File>\n"
        );
        return 0;
    }

    string arch = "Unknown";

#if ARCH_X86
    arch = "-32Bit";
#elif ARCH_X86_64
    arch = "-64Bit";
#elif HAVE_ARMV6
    arch = "-Arm6";
#elif HAVE_ARMV7
    arch = "-Arm7";
#endif
    char *input = argv[2];
    string basefolder = argv[5];
    basefolder.append(slashCharStr().c_str());
    string versionstring = vpx_codec_iface_name(&vpx_codec_vp8_cx_algo);
    size_t versionPos = versionstring.find("v");
    versionstring.substr(versionPos);
    basefolder.append(versionstring.substr(versionPos));
    basefolder.append(arch.c_str());
    basefolder.append(slashCharStr().c_str());
    basefolder.append("Mode");
    basefolder.append(argv[3]);
    basefolder.append(slashCharStr().c_str());
    vpxt_make_dir(basefolder);
    int TestMode = 0;

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "WinLinMacMatch";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string FiletoEnc = "";
    string FiletoDec = "";

    string WinEnc = basefolder;
    string LinEnc = basefolder;
    string MacEnc = basefolder;

    string WinDec = basefolder;
    string LinDec = basefolder;
    string MacDec = basefolder;

    string ParameterFile = argv[6];

    WinEnc.append("WinEnc.ivf");
    LinEnc.append("LinEnc.ivf");
    MacEnc.append("MacEnc.ivf");

    WinDec.append("WinDec.ivf");
    LinDec.append("LinDec.ivf");
    MacDec.append("MacDec.ivf");


#if defined(linux)
    FiletoEnc = LinEnc;
    FiletoDec = LinDec;

    if (vpxt_file_exists_check(WinEnc) && vpxt_file_exists_check(WinDec) && vpxt_file_exists_check(MacEnc) && vpxt_file_exists_check(MacDec))
    {
        TestMode = 1;
    }

#elif defined(_WIN32)
    FiletoEnc = WinEnc;
    FiletoDec = WinDec;

    if (vpxt_file_exists_check(LinEnc) && vpxt_file_exists_check(LinDec) && vpxt_file_exists_check(MacEnc) && vpxt_file_exists_check(MacDec))
    {
        TestMode = 1;
    }

#elif defined(__APPLE__)
    FiletoEnc = MacEnc;
    FiletoDec = MacDec;

    if (vpxt_file_exists_check(WinEnc) && vpxt_file_exists_check(WinDec) && vpxt_file_exists_check(LinEnc) && vpxt_file_exists_check(LinDec))
    {
        TestMode = 1;
    }

#endif

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
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);

    tprintf("Win Lin Mac Match");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);
    ///////////////////Use Custom Settings///////////////////
    //If Mode == 0 (Create Files) Then if input settings is listed use it
    //If Mode == 1 (run Test) Then input settings from test file

    BitRate = opt.target_bandwidth;

    if (TestType == 3)
    {

    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress_ivf_to_ivf(input, FiletoEnc.c_str(), speed, BitRate, opt, "Mode", Mode, 0) == -1)
        {
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 2;
        }

        printf("\n");
        fprintf(stderr, "\n\nDecompressing VP8 IVF File to IVF File: \n");

        if (vpxt_decompress_ivf_to_ivf(FiletoEnc.c_str(), FiletoDec.c_str()) == -1)
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

    if (TestMode == 0)
    {
        tprintf("\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Test files created.");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);

        tprintf("\n");
        tprintf("\nIndeterminate\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 2;
    }

    int ENCFAIL = 0;
    int DECFAIL = 0;

    tprintf("\n\nComparing %s and %s", WinEnc.c_str(), LinEnc.c_str());
    int WinEncVsLinEnc = vpxt_compare_ivf(WinEnc.c_str(), LinEnc.c_str());

    if (WinEncVsLinEnc == -1)
    {
        tprintf("\n\nFiles are Identical\n");
    }

    if (WinEncVsLinEnc >= 0)
    {
        tprintf("\n\nFail: Encoded files differ at frame: %i", WinEncVsLinEnc);
        ENCFAIL++;
    }

    if (WinEncVsLinEnc == -2)
    {
        tprintf("\n\nFail: File 2 ends before File 1.\n");
        ENCFAIL++;
    }

    if (WinEncVsLinEnc == -3)
    {
        tprintf("\n\nFail: File 1 ends before File 2.\n");
        ENCFAIL++;
    }

    tprintf("\n\nComparing %s and %s", LinEnc.c_str(), MacEnc.c_str());
    int LinEncVsMacEnc = vpxt_compare_ivf(LinEnc.c_str(), MacEnc.c_str());

    if (LinEncVsMacEnc == -1)
    {
        tprintf("\n\nFiles are Identical\n");
    }

    if (LinEncVsMacEnc >= 0)
    {
        tprintf("\n\nFail: Encoded files differ at frame: %i", LinEncVsMacEnc);
        ENCFAIL++;
    }

    if (LinEncVsMacEnc == -2)
    {
        tprintf("\n\nFail: File 2 ends before File 1.\n");
        ENCFAIL++;
    }

    if (LinEncVsMacEnc == -3)
    {
        tprintf("\n\nFail: File 1 ends before File 2.\n");
        ENCFAIL++;
    }

    tprintf("\n\nComparing %s and %s", WinEnc.c_str(), MacEnc.c_str());
    int WinEncVsMacEnc = vpxt_compare_ivf(WinEnc.c_str(), MacEnc.c_str());

    if (WinEncVsMacEnc == -1)
    {
        tprintf("\n\nFiles are Identical\n");
    }

    if (WinEncVsMacEnc >= 0)
    {
        tprintf("\n\nFail: Encoded files differ at frame: %i", WinEncVsMacEnc);
        ENCFAIL++;
    }

    if (WinEncVsMacEnc == -2)
    {
        tprintf("\n\nFail: File 2 ends before File 1.\n");
        ENCFAIL++;
    }

    if (WinEncVsMacEnc == -3)
    {
        tprintf("\n\nFail: File 1 ends before File 2.\n");
        ENCFAIL++;
    }

    tprintf("\n\nComparing %s and %s", WinDec.c_str(), LinDec.c_str());
    int WinDecVsLinDec = vpxt_compare_ivf(WinDec.c_str(), LinDec.c_str());

    if (WinDecVsLinDec == -1)
    {
        tprintf("\n\nFiles are Identical\n");
    }

    if (WinDecVsLinDec >= 0)
    {
        tprintf("\n\nFail: Decoded files differ at frame: %i", WinDecVsLinDec);
        DECFAIL++;
    }

    if (WinDecVsLinDec == -2)
    {
        tprintf("\n\nFail: File 2 ends before File 1.\n");
        DECFAIL++;
    }

    if (WinDecVsLinDec == -3)
    {
        tprintf("\n\nFail: File 1 ends before File 2.\n");
        DECFAIL++;
    }

    tprintf("\n\nComparing %s and %s", LinDec.c_str(), MacDec.c_str());
    int LinDecVsMacDec = vpxt_compare_ivf(LinDec.c_str(), MacDec.c_str());

    if (LinDecVsMacDec == -1)
    {
        tprintf("\n\nFiles are Identical\n");
    }

    if (LinDecVsMacDec >= 0)
    {
        tprintf("\n\nFail: Decoded files differ at frame: %i", LinDecVsMacDec);
        DECFAIL++;
    }

    if (LinDecVsMacDec == -2)
    {
        tprintf("\n\nFail: File 2 ends before File 1.\n");
        DECFAIL++;
    }

    if (LinDecVsMacDec == -3)
    {
        tprintf("\n\nFail: File 1 ends before File 2.\n");
        DECFAIL++;
    }

    tprintf("\n\nComparing %s and %s", WinDec.c_str(), MacDec.c_str());
    int WinDecVsMacDec = vpxt_compare_ivf(WinDec.c_str(), MacDec.c_str());

    if (WinDecVsMacDec == -1)
    {
        tprintf("\n\nFiles are Identical\n");
    }

    if (WinDecVsMacDec >= 0)
    {
        tprintf("\n\nFail: Decoded files differ at frame: %i", WinDecVsMacDec);
        DECFAIL++;
    }

    if (WinDecVsMacDec == -2)
    {
        tprintf("\n\nFail: File 2 ends before File 1.\n");
        DECFAIL++;
    }

    if (WinDecVsMacDec == -3)
    {
        tprintf("\n\nFail: File 1 ends before File 2.\n");
        DECFAIL++;
    }

    tprintf("\n\nResults:\n\n");
    int fail = 0;

    if (ENCFAIL == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All encoded files are identical - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (ENCFAIL > 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Not all encoded files are identical - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (DECFAIL == 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "All decoded files are identical - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
    }

    if (DECFAIL > 0)
    {
        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Not all decoded files are identical - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");
        fail = 1;
    }

    if (fail == 1)
    {
        tprintf("\nFailed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }
    else
    {
        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }

    fclose(fp);
    string File1Str = File1;
    record_test_complete(MainDirString, File1Str, TestType);
    return 6;
}
