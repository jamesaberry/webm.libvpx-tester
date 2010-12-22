#include "vpxt_test_declarations.h"

int test_extra_file(int argc, const char *const *argv, const string &WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Allow Drop Frames";
    char *MyDir = "test_extra_file";

    if (!(argc == 3 || argc == 4))
    {
        vpxt_cap_string_print(PRINT_STD, "  %s", MyDir);
        tprintf(PRINT_STD,
                "\n\n"
                "    <Input File>\n"
                "    <Optional Settings File>\n"
                "\n"
               );
        return 0;
    }

    string input = argv[2];

    ////////////Formatting Test Specific Directory////////////
    string CurTestDirStr = "";
    char MainTestDirChar[255] = "";
    string FileIndexStr = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    ////////compression directory////////
    string CompressionDir = CurTestDirStr;
    CompressionDir.append(slashCharStr());
    string CompressionDirAllFiles = CompressionDir;
#if defined(_WIN32)
    CompressionDirAllFiles.append("*");
#endif

    ////////tester directory////////
    char TesterDirChar[255];
    vpxt_folder_name(argv[0], TesterDirChar);
    string TesterDirAllFiles = TesterDirChar;
#if defined(_WIN32)
    TesterDirAllFiles.append("*");
#endif

    ////////current directory////////
    string CurrentDir;
    vpxt_get_cur_dir(CurrentDir);
    string CurrentDirAllFiles = CurrentDir;
    CurrentDirAllFiles.append(slashCharStr());
#if defined(_WIN32)
    CurrentDirAllFiles.append("*");
#endif

    ////////////////////////////////////////////////////////////////

    string ExtraFileCheckStr = CurTestDirStr;
    ExtraFileCheckStr.append(slashCharStr());
    ExtraFileCheckStr.append(MyDir);
    ExtraFileCheckStr.append("_compression.ivf");

    //This is to record state for compression only run
    string ExtraFileCheckResultStr = CurTestDirStr;
    ExtraFileCheckResultStr.append(slashCharStr());
    ExtraFileCheckResultStr.append(MyDir);
    ExtraFileCheckResultStr.append("_result.txt");

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

    vpxt_cap_string_print(PRINT_BTH, "%s", MyDir);
    tprintf(PRINT_BTH, "\n\n---------------------------Checking for OPSNR Files----------------------------\n\n");

    /////////////////////////////////////////////////Delete opsnr.stt////////////////////////////////////////////

    //check Current Directory
    string CurrentDirOPSNR = CurrentDir;

    CurrentDirOPSNR.append(slashCharStr());
    tprintf(PRINT_BTH, "Checking: %s For opsnr.stt - ", CurrentDirOPSNR.c_str());
    CurrentDirOPSNR.append("opsnr.stt");

    FILE *FileCurrentDirOPSNR = fopen(CurrentDirOPSNR.c_str(), "rb");

    if (FileCurrentDirOPSNR == NULL)
        tprintf(PRINT_BTH, "opsnr.stt File not found. \n\n");
    else
    {
        fclose(FileCurrentDirOPSNR);
        remove(CurrentDirOPSNR.c_str());
        tprintf(PRINT_BTH, "opsnr.stt File found and deleted. \n");
    }

    //check Tester Dirctory
    string TesterDirOPSNR = TesterDirChar;
    tprintf(PRINT_BTH, "Checking: %s For opsnr.stt - ", TesterDirOPSNR.c_str());
    TesterDirOPSNR.append("opsnr.stt");

    FILE *FileTesterOPSNR = fopen(TesterDirOPSNR.c_str(), "rb");

    if (FileTesterOPSNR == NULL)
        tprintf(PRINT_BTH, "opsnr.stt File not found. \n\n");
    else
    {
        fclose(FileTesterOPSNR);
        remove(TesterDirOPSNR.c_str());
        tprintf(PRINT_BTH, "opsnr.stt File found and deleted. \n");
    }

    //check Compression Directory
    string CompDirOPSNR = CompressionDir;
    tprintf(PRINT_BTH, "Checking: %s For opsnr.stt - ", CompDirOPSNR.c_str());
    CompDirOPSNR.append("opsnr.stt");

    FILE *FileCompDirOPSNR = fopen(CompDirOPSNR.c_str(), "rb");

    if (FileCompDirOPSNR == NULL)
        tprintf(PRINT_BTH, "opsnr.stt File not found. \n\n");
    else
    {
        fclose(FileCompDirOPSNR);
        remove(CompDirOPSNR.c_str());
        tprintf(PRINT_BTH, "opsnr.stt File found and deleted. \n");
    }

    tprintf(PRINT_BTH, "------------------------------------------------------------------------------\n");
    ////////////////////////////////////////////////////////////////

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 4)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf(PRINT_BTH, "\nInput Settings file %s does not exist\n", argv[argc-1]);

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }

        opt = vpxt_input_settings(argv[argc-1]);
    }

    /////////////////////////////////////////////////////////
    int CompressInt = opt.allow_df;
    int BitRate = opt.target_bandwidth;
    int Fail = 0;

    //Record all files in the executable directory
    vector<string> IgnoreTesterDir;
    vpxt_add_dir_files_to_ignore(IgnoreTesterDir, TesterDirAllFiles);

    //Record all files in the current directory.
    vector<string> IgnoreCurrentDir;
    vpxt_add_dir_files_to_ignore(IgnoreCurrentDir, CurrentDirAllFiles);

    //There are no files in comp dir but we need to ignore compression files
    vector<string> IgnoreCompressionDir;
    IgnoreCompressionDir.push_back(".");
    IgnoreCompressionDir.push_back("..");
    IgnoreCompressionDir.push_back("test_extra_file.txt");
    IgnoreCompressionDir.push_back("test_extra_file_compression.ivf");
    IgnoreCompressionDir.push_back("test_extra_file_compression_paramaters_core.txt");
    IgnoreCompressionDir.push_back("test_extra_file_compression_paramaters_vpx.txt");

    if (TestType == TEST_ONLY)
    {
        tprintf(PRINT_BTH, "\nExtraFileCheck Test Run Previously Retreiveing Result\n");

        ifstream infile(ExtraFileCheckResultStr.c_str());

        if (!infile.is_open())
        {
            tprintf(PRINT_BTH, "File: %s not opened", ExtraFileCheckResultStr.c_str());
            return 0;
        }

        infile >> Fail;
        infile.close();

        if (Fail == 1)
        {
            tprintf(PRINT_BTH, "\n\nExtra files found - Test Failed\n");

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 0;
        }

        if (Fail == 0)
        {
            tprintf(PRINT_BTH, "\n\n No extra files found - Test Passed\n");

            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 1;
        }
    }
    else
    {
        opt.Mode = MODE_SECONDPASS_BEST;

        if (vpxt_compress_ivf_to_ivf(input.c_str(), ExtraFileCheckStr.c_str(), opt.multi_threaded, BitRate, opt, CompressString, 1, 0, 0, 3, 3/*opt.DeleteFirstPassFile*/) == -1)
        {
            fclose(fp);
            record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
            return 2;
        }
    }

    tprintf(PRINT_BTH, "\n\n---------------------------Checking for Extra Files----------------------------\n\n");
    tprintf(PRINT_BTH, "Checking: %s", CompressionDirAllFiles.c_str());
    tprintf(PRINT_BTH, "\n\nChecking: %s", TesterDirAllFiles.c_str());
    tprintf(PRINT_BTH, "\n\nChecking: %s\n\n", CurrentDirAllFiles.c_str());

    vector<string> FoundTesterDir;
    vector<string> FoundCurrentDir;
    vector<string> FoundCompressionDir;

    int CurDirFail = vpxt_find_non_ignored_files_in_dir(IgnoreCurrentDir, FoundCurrentDir, CurrentDirAllFiles);
    int CompDirFail = vpxt_find_non_ignored_files_in_dir(IgnoreCompressionDir, FoundCompressionDir, CompressionDirAllFiles);
    int TesterDirFail = vpxt_find_non_ignored_files_in_dir(IgnoreTesterDir, FoundTesterDir, TesterDirAllFiles);

    tprintf(PRINT_BTH, "------------------------------------------------------------------------------\n");
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (TestType == COMP_ONLY)
    {
        ofstream outfile(ExtraFileCheckResultStr.c_str());

        if (CurDirFail == 0 && CompDirFail == 0 && TesterDirFail == 0)
        {
            tprintf(PRINT_BTH, "\nNo extra files found\n");
            outfile << 0;
        }
        else
        {
            tprintf(PRINT_BTH, "\nExtra files found\n");
            outfile << 1;
        }

        outfile.close();
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    if (TesterDirFail == 0)
    {
        vpxt_formated_print(RESPRT, "No extra files found in tester directory - Passed\n");
    }
    else
    {
        int vecpos = 0;

        while (vecpos < FoundTesterDir.size())
        {
            vpxt_formated_print(RESPRT, "Tester directory extra file found: %s - Failed\n", FoundTesterDir[vecpos].c_str());
            vecpos++;
        }

        Fail = 1;
    }

    if (CurDirFail == 0)
    {
        vpxt_formated_print(RESPRT, "No extra files found in current directory - Passed\n");
    }
    else
    {
        int vecpos = 0;

        while (vecpos < FoundCurrentDir.size())
        {
            vpxt_formated_print(RESPRT, "Current directory extra file found: %s - Failed\n", FoundCurrentDir[vecpos].c_str());
            vecpos++;
        }

        Fail = 1;
    }

    if (CompDirFail == 0)
    {
        vpxt_formated_print(RESPRT, "No extra files found in compression directory - Passed\n");
    }
    else
    {
        int vecpos = 0;

        while (vecpos < FoundCompressionDir.size())
        {
            vpxt_formated_print(RESPRT, "Compression directory extra file found: %s - Failed\n", FoundCompressionDir[vecpos].c_str());
            vecpos++;
        }

        Fail = 1;
    }

    if (Fail == 1)
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    if (Fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;

}