#include "vpxt_test_declarations.h"

int test_reconstruct_buffer(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Allow Drop Frames";
    char *MyDir = "test_reconstruct_buffer";
    int inputCheck = vpxt_check_arg_input(argv[1], argc);

    if (inputCheck < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    std::string EncForm = argv[5];

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////

    std::string CurTestDirStr = ""; // <- All Options need to set a value for this
    std::string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    std::string ReconBufferCompression = CurTestDirStr;
    ReconBufferCompression.append(slashCharStr());
    ReconBufferCompression.append(MyDir);
    ReconBufferCompression.append("_compression");
    vpxt_enc_format_append(ReconBufferCompression, EncForm);

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
    if (inputCheck == 2)
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
    opt.lag_in_frames = 0;
    opt.play_alternate = 0;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        opt.Mode = Mode;

        if (vpxt_compress_recon_buffer_check(input.c_str(), ReconBufferCompression.c_str(), speed, BitRate, opt, CompressString, 0, 0, DeleteIVF, EncForm) == -1)
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

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int fail = 0;

    std::ifstream ReconOutFile;
    std::string ReconOutStr;
    vpxt_remove_file_extension(ReconBufferCompression.c_str(), ReconOutStr);
    //std::string ReconOutStr = ReconBufferCompression;
    //ReconOutStr.erase(ReconOutStr.length() - 4, 4);
    ReconOutStr.append("ReconFrameState.txt");

    ReconOutFile.open(ReconOutStr.c_str());

    int Frame;
    char BufferLetter;
    int State;

    while (!ReconOutFile.eof())
    {
        ReconOutFile >> Frame;
        ReconOutFile >> BufferLetter;
        ReconOutFile >> State;

        if (State == 0)
        {
            vpxt_formated_print(RESPRT, "Frame: %i Buffer: %c - Preview not identical to Decoded - Failed", Frame, BufferLetter);
            tprintf(PRINT_BTH, "\n");
            fail = 1;
        }
    }

    ReconOutFile.close();

    if (fail == 0)
    {
        vpxt_formated_print(RESPRT, "All preview frames are identical to decoded frames - Passed");
        tprintf(PRINT_BTH, "\n");
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        if (DeleteIVF)
            vpxt_delete_files(1, ReconBufferCompression.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        if (DeleteIVF)
            vpxt_delete_files(1, ReconBufferCompression.c_str());

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}