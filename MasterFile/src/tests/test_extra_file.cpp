#include "vpxt_test_declarations.h"

int test_extra_file(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
#if defined(linux)

    char *CompressString = "opt.DeleteFirstPassFile";

    if (!(argc == 3 || argc == 4))
    {
        printf(
            "  ExtraFileCheck \n\n"
            "    <inputfile>\n"
            "    <Optional Settings File>\n"
            "\n"
        );
        return 0;
    }

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";

    char WorkingDir3[255] = "";
    char *MyDir = "ExtraFileCheckTest";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string ExtraFileDir = WorkingDirString;
    ////////////////////OutPut Directory location////////////////////
    char EF_DestChar[255];
    snprintf(EF_DestChar, 255, "%s", ExtraFileDir.c_str());
    string EF_DestString = EF_DestChar;
    //////////////////////////////////////////////////////////////////
    //////////////////Program Location///////////////////////////////
    char EF_SourChar[255];
    char EF_SourChar2[255];
    vpxt_folder_name(argv[0], EF_SourChar2);
    snprintf(EF_SourChar, 255, "%s", EF_SourChar2);
    string EF_SourString1 = EF_SourChar;
    EF_SourString1.erase(EF_SourString1.length() - 1, 1);
    ////////////////////////////////////////////////////////////////
    //////////////////Current Dir Locatoin//////////////////////////
    char EF_SourChar3[256];
    getcwd(EF_SourChar3, 256);

    string EF_SourString2 = EF_SourChar3;
    ////////////////////////////////////////////////////////////////

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr());
    WorkingDir4.append("ExtraFileCheck.ivf");

    WorkingDir5.append(slashCharStr());
    WorkingDir5.append("ExtraFileCheckResult.txt");

    char ExtraFileCheck[255];
    char ExtraFileCheckResult[255];

    snprintf(ExtraFileCheck, 255, "%s", WorkingDir4.c_str());
    snprintf(ExtraFileCheckResult, 255, "%s", WorkingDir5.c_str());

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

    tprintf("Extra File Check\n");
    tprintf("---------------------------Checking for OPSNR Files----------------------------\n");

    /////////////////////////////////////////////////Delete opsnr.stt////////////////////////////////////////////
    char CurrentPath[260];
    getcwd(CurrentPath, 260);

    string CurrentPathStr = CurrentPath;

    cout << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";
    cerr << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";

    CurrentPathStr.append(slashCharStr());
    CurrentPathStr.append("opsnr.stt");

    FILE *opsnrExists = fopen(CurrentPathStr.c_str(), "rb");

    if (opsnrExists == NULL)
    {
        cout << "opsnr.stt File not found. \n";
        cerr << "opsnr.stt File not found. \n";

    }
    else
    {
        fclose(opsnrExists);
        remove(CurrentPathStr.c_str());
        cout << "opsnr.stt File found and deleted. \n";
        cerr << "opsnr.stt File found and deleted. \n";
    }

    string EF_DestString2 = EF_DestChar;

    cout << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";
    cerr << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";

    EF_DestString2.append(slashCharStr());
    EF_DestString2.append("opsnr.stt");

    FILE *opsnrExists2 = fopen(EF_DestString2.c_str(), "rb");

    if (opsnrExists2 == NULL)
    {
        cout << "opsnr.stt File not found. \n";
        cerr << "opsnr.stt File not found. \n";
    }
    else
    {
        fclose(opsnrExists2);
        remove(EF_DestString2.c_str());
        cout << "opsnr.stt File found and deleted. \n";
        cerr << "opsnr.stt File found and deleted. \n";
    }

    tprintf("------------------------------------------------------------------------------\n");
    ////////////////////////////////////////////////////////////////

    char *input = argv[2];

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 4)
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
    }

    /////////////////////////////////////////////////////////

    int CompressInt = opt.allow_df;
    int BitRate = opt.target_bandwidth;
    int Fail = 0;

    ///////////////////////////Record all files in the executable + Current directory///////////////////////////
    //Record all files in the executable directory.
    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;
    vector<string> SourceFileVectorA;

    FindFileDataA = opendir(EF_SourString1.c_str());
    hFindA = readdir(FindFileDataA);

    while (FileNameA.compare(hFindA->d_name) != 0)
    {
        FileNameA = hFindA->d_name;
        SourceFileVectorA.push_back(FileNameA);
        hFindA = readdir(FindFileDataA);

        if (hFindA == NULL)
        {
            break;
        }
    }

    closedir(FindFileDataA);

    //Record all files in the current directory.
    DIR *FindFileDataB;
    struct dirent *hFindB;
    string FileNameB;
    vector<string> SourceFileVectorB;

    FindFileDataB = opendir(EF_SourString2.c_str());
    hFindB = readdir(FindFileDataB);

    while (FileNameB.compare(hFindB->d_name) != 0)
    {
        FileNameB = hFindB->d_name;
        SourceFileVectorB.push_back(FileNameB);
        hFindB = readdir(FindFileDataB);

        if (hFindB == NULL)
        {
            break;
        }
    }

    closedir(FindFileDataB);

    //////////////////////////////////////////////////////////////
    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        tprintf("\nExtraFileCheck Test Run Previously Retreiveing Result\n");

        ifstream infile(ExtraFileCheckResult);

        if (!infile.is_open())
        {
            tprintf("File: %s not opened", ExtraFileCheckResult);
            return 0;
        }

        infile >> Fail;
        infile.close();

        if (Fail == 1)
        {
            cout << "\n\nExtra files found - Test Failed\n";
            cerr << "\n\nExtra files found - Test Failed\n";
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Fail == 0)
        {
            cout << "\n\n No extra files found - Test Passed\n";
            cerr << "\n\n No extra files found - Test Passed\n";
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 1;
        }
    }
    else
    {
        opt.Mode = MODE_SECONDPASS_BEST;

        if (vpxt_compress_ivf_to_ivf(input, ExtraFileCheck, opt.multi_threaded, BitRate, opt, CompressString, 0, 0/*opt.DeleteFirstPassFile*/) == -1)
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

    tprintf("\n\n---------------------------Checking for Extra Files----------------------------\n\n");

    cout << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";
    cerr << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";

    cout << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";
    cerr << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";

    ///////////////////////////Check all files in the executable + Current directory///////////////////////////
    //Check all files in the output directory.
    DIR *FindFileData2;
    struct dirent *hFind2;

    FindFileData2 = opendir(EF_DestString.c_str());
    hFind2 = readdir(FindFileData2);

    string FileName2;
    vector<string> DestFileExtraVector;

    while (FileName2.compare(hFind2->d_name) != 0)
    {
        FileName2 = hFind2->d_name;

        if (FileName2.find(".fpf") == string::npos && FileName2.compare("ExtraFileCheck.ivf") != 0 &&
            FileName2.compare(".") != 0 && FileName2.compare("..") != 0 && FileName2.compare("ExtraFileCheckTest.txt")
            && FileName2.compare("Extra File Check Test_TestOnly.txt") && FileName2.compare("ExtraFileCheck_Paramaters.txt")
            && FileName2.compare("ExtraFileCheck_APIParamaters.txt"))
        {
            DestFileExtraVector.push_back(FileName2);
            Fail = 2;
        }

        if (FileName2.compare("opsnr.stt") == 0)
        {
            DestFileExtraVector.push_back(FileName2);
            Fail = 2;
        }

        hFind2 = readdir(FindFileData2);

        if (hFind2 == NULL)
        {
            break;
        }
    }

    closedir(FindFileData2);

    //Check through all files in the executable directory.
    DIR *FindFileData3;
    struct dirent *hFind3;
    string FileName3;

    int filenum = 0;
    vector<int> FileFoundVect;
    vector<string> PostCompressionFileListVect;

    FindFileData3 = opendir(EF_SourString1.c_str());
    hFind3 = readdir(FindFileData3);

    while (FileName3.compare(hFind3->d_name) != 0)
    {
        FileName3 = hFind3->d_name;

        int vectloc = 0;
        int FileFound = 0;
        vector<string>::iterator it;
        it = SourceFileVectorA.begin();

        while (it < SourceFileVectorA.end())
        {
            //checks Exe directory for opsnr.stt file
            if (FileName3.compare("opsnr.stt") == 0)
            {
                FileFound = 0;
                break;
            }

            if (FileName3.compare(SourceFileVectorA[vectloc]) == 0)
            {
                FileFound = 1;
                break;
            }

            if (FileName3.find(".fpf") != string::npos)
            {
                FileFound = 1;
                break;
            }

            vectloc++;
            it++;
        }

        FileFoundVect.push_back(FileFound);
        PostCompressionFileListVect.push_back(FileName3);

        hFind3 = readdir(FindFileData3);

        if (hFind3 == NULL)
        {
            break;
        }
    }

    vector<int>::iterator it2;
    it2 = FileFoundVect.begin();
    int vectorposition = 0;

    while (it2 < FileFoundVect.end())
    {
        if (FileFoundVect[vectorposition] == 0)
        {
            cout << "Extra File Found: " << PostCompressionFileListVect[vectorposition];
            cerr << "Extra File Found: " << PostCompressionFileListVect[vectorposition];

            if (Fail != 2)
            {
                Fail = 1;
            }
        }

        it2++;
        vectorposition++;
    }

    closedir(FindFileData3);

    //Check through all files in the current directory.
    DIR *FindFileData4;
    struct dirent *hFind4;
    string FileName4;

    int filenumB = 0;
    vector<int> FileFoundVectB;
    vector<string> PostCompressionFileListVectB;

    FindFileData4 = opendir(EF_SourString2.c_str());
    hFind4 = readdir(FindFileData4);

    while (FileName4.compare(hFind4->d_name) != 0)
    {
        FileName4 = hFind4->d_name;
        //puts(hFind4->d_name);

        int vectloc = 0;
        int FileFound = 0;
        vector<string>::iterator it;
        it = SourceFileVectorB.begin();

        while (it < SourceFileVectorB.end())
        {
            //checks current directory for opsnr.stt file
            if (FileName4.compare("opsnr.stt") == 0)
            {
                FileFound = 0;
                break;
            }

            if (FileName4.compare(SourceFileVectorB[vectloc]) == 0)
            {
                FileFound = 1;
                break;
            }

            if (FileName4.find(".fpf") != string::npos)
            {
                FileFound = 1;
                break;
            }

            vectloc++;
            it++;
        }

        FileFoundVectB.push_back(FileFound);
        PostCompressionFileListVectB.push_back(FileName4);

        hFind4 = readdir(FindFileData4);

        if (hFind4 == NULL)
        {
            break;
        }
    }

    vector<int>::iterator it2B;
    it2B = FileFoundVectB.begin();
    int vectorpositionB = 0;


    cout << "\n";
    cerr << "\n";

    while (it2B < FileFoundVectB.end())
    {
        if (FileFoundVectB[vectorpositionB] == 0)
        {
            cout << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];
            cerr << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];

            if (Fail != 2)
            {
                Fail = 1;
            }
        }

        it2B++;
        vectorpositionB++;
    }

    closedir(FindFileData4);
    //////////////////////////////////////////////////////////////

    cout << "\n";
    cerr << "\n";

    if (Fail == 2)
    {
        vector<string>::iterator it3;
        it3 = DestFileExtraVector.begin();
        int Varloc = 0;

        while (it3 < DestFileExtraVector.end())
        {
            cout << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
            cerr << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
            Varloc++;
            it3++;
        }

        Fail = 1;
    }

    if (Fail == 1)
    {
        tprintf("\n------------------------------------------------------------------------------\n");

        if (TestType == 2)
        {
            cout << "\nExtra files found\n";
            cerr << "\nExtra files found\n";

            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);

            ofstream outfile(ExtraFileCheckResult);
            outfile << 1;
            outfile.close();

            return 10;
        }

        tprintf("\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Extra Files Found - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        tprintf("\nFailed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (Fail == 0)
    {
        tprintf("------------------------------------------------------------------------------\n");

        if (TestType == 2)
        {
            cout << "\nNo extra files found\n";
            cerr << "\nNo extra files found\n";

            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);

            ofstream outfile(ExtraFileCheckResult);
            outfile << 0;
            outfile.close();

            return 10;
        }

        tprintf("\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "No extra files found - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }

#endif
#if defined(__APPLE__)

    char *CompressString = "opt.DeleteFirstPassFile";

    if (!(argc == 3 || argc == 4))
    {
        printf(
            "  ExtraFileCheck \n\n"
            "    <inputfile>\n"
            "    <Optional Settings File>\n"
            "\n"
        );
        return 0;
    }

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";

    char WorkingDir3[255] = "";
    char *MyDir = "ExtraFileCheckTest";
    string MainDirString = "";
    char File1[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string ExtraFileDir = WorkingDirString;
    ////////////////////OutPut Directory location////////////////////
    char EF_DestChar[255];
    snprintf(EF_DestChar, 255, "%s", ExtraFileDir.c_str());
    string EF_DestString = EF_DestChar;
    //////////////////////////////////////////////////////////////////
    //////////////////Program Location///////////////////////////////
    char EF_SourChar[255];
    char EF_SourChar2[255];
    vpxt_folder_name(argv[0], EF_SourChar2);
    snprintf(EF_SourChar, 255, "%s", EF_SourChar2);
    string EF_SourString1 = EF_SourChar;
    EF_SourString1.erase(EF_SourString1.length() - 1, 1);
    ////////////////////////////////////////////////////////////////
    //////////////////Current Dir Locatoin//////////////////////////
    char EF_SourChar3[256];
    getcwd(EF_SourChar3, 256);

    string EF_SourString2 = EF_SourChar3;
    ////////////////////////////////////////////////////////////////

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr());
    WorkingDir4.append("ExtraFileCheck.ivf");

    WorkingDir5.append(slashCharStr());
    WorkingDir5.append("ExtraFileCheckResult.txt");

    char ExtraFileCheck[255];
    char ExtraFileCheckResult[255];

    snprintf(ExtraFileCheck, 255, "%s", WorkingDir4.c_str());
    snprintf(ExtraFileCheckResult, 255, "%s", WorkingDir5.c_str());

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

    tprintf("Extra File Check\n");
    tprintf("---------------------------Checking for OPSNR Files----------------------------\n");

    /////////////////////////////////////////////////Delete opsnr.stt////////////////////////////////////////////
    char CurrentPath[260];
    getcwd(CurrentPath, 260);

    string CurrentPathStr = CurrentPath;

    cout << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";
    cerr << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";

    CurrentPathStr.append(slashCharStr());
    CurrentPathStr.append("opsnr.stt");

    FILE *opsnrExists = fopen(CurrentPathStr.c_str(), "rb");

    if (opsnrExists == NULL)
    {
        cout << "opsnr.stt File not found. \n";
        cerr << "opsnr.stt File not found. \n";

    }
    else
    {
        fclose(opsnrExists);
        remove(CurrentPathStr.c_str());
        cout << "opsnr.stt File found and deleted. \n";
        cerr << "opsnr.stt File found and deleted. \n";
    }

    string EF_DestString2 = EF_DestChar;

    cout << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";
    cerr << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";

    EF_DestString2.append(slashCharStr());
    EF_DestString2.append("opsnr.stt");

    FILE *opsnrExists2 = fopen(EF_DestString2.c_str(), "rb");

    if (opsnrExists2 == NULL)
    {
        cout << "opsnr.stt File not found. \n";
        cerr << "opsnr.stt File not found. \n";
    }
    else
    {
        fclose(opsnrExists2);
        remove(EF_DestString2.c_str());
        cout << "opsnr.stt File found and deleted. \n";
        cerr << "opsnr.stt File found and deleted. \n";
    }

    tprintf("------------------------------------------------------------------------------\n");
    ////////////////////////////////////////////////////////////////

    char *input = argv[2];

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 4)
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
    }

    /////////////////////////////////////////////////////////

    int CompressInt = opt.allow_df;
    int BitRate = opt.target_bandwidth;
    int Fail = 0;

    ///////////////////////////Record all files in the executable + Current directory///////////////////////////
    //Record all files in the executable directory.
    DIR *FindFileDataA;
    struct dirent *hFindA;
    string FileNameA;
    vector<string> SourceFileVectorA;

    FindFileDataA = opendir(EF_SourString1.c_str());
    hFindA = readdir(FindFileDataA);

    while (FileNameA.compare(hFindA->d_name) != 0)
    {
        FileNameA = hFindA->d_name;
        SourceFileVectorA.push_back(FileNameA);
        hFindA = readdir(FindFileDataA);

        if (hFindA == NULL)
        {
            break;
        }
    }

    closedir(FindFileDataA);

    //Record all files in the current directory.
    DIR *FindFileDataB;
    struct dirent *hFindB;
    string FileNameB;
    vector<string> SourceFileVectorB;

    FindFileDataB = opendir(EF_SourString2.c_str());
    hFindB = readdir(FindFileDataB);

    while (FileNameB.compare(hFindB->d_name) != 0)
    {
        FileNameB = hFindB->d_name;
        SourceFileVectorB.push_back(FileNameB);
        hFindB = readdir(FindFileDataB);

        if (hFindB == NULL)
        {
            break;
        }
    }

    closedir(FindFileDataB);

    //////////////////////////////////////////////////////////////
    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        tprintf("\nExtraFileCheck Test Run Previously Retreiveing Result\n");

        ifstream infile(ExtraFileCheckResult);

        if (!infile.is_open())
        {
            tprintf("File: %s not opened", ExtraFileCheckResult);
            return 0;
        }

        infile >> Fail;
        infile.close();

        if (Fail == 1)
        {
            cout << "\n\nExtra files found - Test Failed\n";
            cerr << "\n\nExtra files found - Test Failed\n";
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Fail == 0)
        {
            cout << "\n\n No extra files found - Test Passed\n";
            cerr << "\n\n No extra files found - Test Passed\n";
            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 1;
        }
    }
    else
    {
        opt.Mode = MODE_SECONDPASS_BEST;

        if (vpxt_compress_ivf_to_ivf(input, ExtraFileCheck, opt.multi_threaded, BitRate, opt, CompressString, 0, 0) == -1)
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

    tprintf("\n\n---------------------------Checking for Extra Files----------------------------\n\n");

    cout << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";
    cerr << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";

    cout << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";
    cerr << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";

    ///////////////////////////Check all files in the executable + Current directory///////////////////////////
    //Check all files in the output directory.
    DIR *FindFileData2;
    struct dirent *hFind2;

    FindFileData2 = opendir(EF_DestString.c_str());
    hFind2 = readdir(FindFileData2);

    string FileName2;
    vector<string> DestFileExtraVector;

    while (FileName2.compare(hFind2->d_name) != 0)
    {
        FileName2 = hFind2->d_name;

        if (FileName2.find(".fpf") == string::npos && FileName2.compare("ExtraFileCheck.ivf") != 0 &&
            FileName2.compare(".") != 0 && FileName2.compare("..") != 0 && FileName2.compare("ExtraFileCheckTest.txt")
            && FileName2.compare("Extra File Check Test_TestOnly.txt") && FileName2.compare("ExtraFileCheck_Paramaters.txt")
            && FileName2.compare("ExtraFileCheck_APIParamaters.txt"))
        {
            DestFileExtraVector.push_back(FileName2);
            Fail = 2;
        }

        if (FileName2.compare("opsnr.stt") == 0)
        {
            DestFileExtraVector.push_back(FileName2);
            Fail = 2;
        }

        hFind2 = readdir(FindFileData2);

        if (hFind2 == NULL)
        {
            break;
        }
    }

    closedir(FindFileData2);

    //Check through all files in the executable directory.
    DIR *FindFileData3;
    struct dirent *hFind3;
    string FileName3;

    int filenum = 0;
    vector<int> FileFoundVect;
    vector<string> PostCompressionFileListVect;

    FindFileData3 = opendir(EF_SourString1.c_str());
    hFind3 = readdir(FindFileData3);

    while (FileName3.compare(hFind3->d_name) != 0)
    {
        FileName3 = hFind3->d_name;

        int vectloc = 0;
        int FileFound = 0;
        vector<string>::iterator it;
        it = SourceFileVectorA.begin();

        while (it < SourceFileVectorA.end())
        {
            //checks Exe directory for opsnr.stt file
            if (FileName3.compare("opsnr.stt") == 0)
            {
                FileFound = 0;
                break;
            }

            if (FileName3.compare(SourceFileVectorA[vectloc]) == 0)
            {
                FileFound = 1;
                break;
            }

            if (FileName3.find(".fpf") != string::npos)
            {
                FileFound = 1;
                break;
            }

            vectloc++;
            it++;
        }

        FileFoundVect.push_back(FileFound);
        PostCompressionFileListVect.push_back(FileName3);

        hFind3 = readdir(FindFileData3);

        if (hFind3 == NULL)
        {
            break;
        }
    }

    vector<int>::iterator it2;
    it2 = FileFoundVect.begin();
    int vectorposition = 0;

    while (it2 < FileFoundVect.end())
    {
        if (FileFoundVect[vectorposition] == 0)
        {
            cout << "Extra File Found: " << PostCompressionFileListVect[vectorposition];
            cerr << "Extra File Found: " << PostCompressionFileListVect[vectorposition];

            if (Fail != 2)
            {
                Fail = 1;
            }
        }

        it2++;
        vectorposition++;
    }

    closedir(FindFileData3);

    //Check through all files in the current directory.
    DIR *FindFileData4;
    struct dirent *hFind4;
    string FileName4;

    int filenumB = 0;
    vector<int> FileFoundVectB;
    vector<string> PostCompressionFileListVectB;

    FindFileData4 = opendir(EF_SourString2.c_str());
    hFind4 = readdir(FindFileData4);

    while (FileName4.compare(hFind4->d_name) != 0)
    {
        FileName4 = hFind4->d_name;
        //puts(hFind4->d_name);

        int vectloc = 0;
        int FileFound = 0;
        vector<string>::iterator it;
        it = SourceFileVectorB.begin();

        while (it < SourceFileVectorB.end())
        {
            //checks current directory for opsnr.stt file
            if (FileName4.compare("opsnr.stt") == 0)
            {
                FileFound = 0;
                break;
            }

            if (FileName4.compare(SourceFileVectorB[vectloc]) == 0)
            {
                FileFound = 1;
                break;
            }

            if (FileName4.find(".fpf") != string::npos)
            {
                FileFound = 1;
                break;
            }

            vectloc++;
            it++;
        }

        FileFoundVectB.push_back(FileFound);
        PostCompressionFileListVectB.push_back(FileName4);

        hFind4 = readdir(FindFileData4);

        if (hFind4 == NULL)
        {
            break;
        }
    }

    vector<int>::iterator it2B;
    it2B = FileFoundVectB.begin();
    int vectorpositionB = 0;


    cout << "\n";
    cerr << "\n";

    while (it2B < FileFoundVectB.end())
    {
        if (FileFoundVectB[vectorpositionB] == 0)
        {
            cout << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];
            cerr << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];

            if (Fail != 2)
            {
                Fail = 1;
            }
        }

        it2B++;
        vectorpositionB++;
    }

    closedir(FindFileData4);
    //////////////////////////////////////////////////////////////

    cout << "\n";
    cerr << "\n";

    if (Fail == 2)
    {
        vector<string>::iterator it3;
        it3 = DestFileExtraVector.begin();
        int Varloc = 0;

        while (it3 < DestFileExtraVector.end())
        {
            cout << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
            cerr << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
            Varloc++;
            it3++;
        }

        Fail = 1;
    }

    if (Fail == 1)
    {
        tprintf("\n------------------------------------------------------------------------------\n");

        if (TestType == 2)
        {
            cout << "\nExtra files found\n";
            cerr << "\nExtra files found\n";

            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);

            ofstream outfile(ExtraFileCheckResult);
            outfile << 1;
            outfile.close();

            return 10;
        }

        tprintf("\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "Extra Files Found - Failed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        tprintf("\nFailed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (Fail == 0)
    {
        tprintf("------------------------------------------------------------------------------\n");

        if (TestType == 2)
        {
            cout << "\nNo extra files found\n";
            cerr << "\nNo extra files found\n";

            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);

            ofstream outfile(ExtraFileCheckResult);
            outfile << 0;
            outfile.close();

            return 10;
        }

        tprintf("\n\nResults:\n\n");

        char OutputChar1[255];
        snprintf(OutputChar1, 255, "No extra files found - Passed");
        string OutputChar1str = OutputChar1;
        formated_print(OutputChar1str, 5);
        tprintf("\n");

        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }

#endif
#if defined(__POWERPC__)
    {
        char *CompressString = "opt.DeleteFirstPassFile";

        if (!(argc == 3 || argc == 4))
        {
            printf(
                "  ExtraFileCheck \n\n"
                "    <inputfile>\n"
                "    <Optional Settings File>\n"
                "\n"
            );
            return 0;
        }

        ////////////Formatting Test Specific Directory////////////
        string WorkingDirString = "";

        char WorkingDir3[255] = "";
        char *MyDir = "ExtraFileCheckTest";
        string MainDirString = "";
        char File1[255] = "";

        if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
            return 11;

        string ExtraFileDir = WorkingDirString;
        ////////////////////OutPut Directory location////////////////////
        char EF_DestChar[255];
        snprintf(EF_DestChar, 255, "%s", ExtraFileDir.c_str());
        string EF_DestString = EF_DestChar;
        //////////////////////////////////////////////////////////////////
        //////////////////Program Location///////////////////////////////
        char EF_SourChar[255];
        char EF_SourChar2[255];
        vpxt_folder_name(argv[0], EF_SourChar2);
        snprintf(EF_SourChar, 255, "%s", EF_SourChar2);
        string EF_SourString1 = EF_SourChar;
        EF_SourString1.erase(EF_SourString1.length() - 1, 1);
        ////////////////////////////////////////////////////////////////
        //////////////////Current Dir Locatoin//////////////////////////
        char EF_SourChar3[256];
        getcwd(EF_SourChar3, 256);

        string EF_SourString2 = EF_SourChar3;
        ////////////////////////////////////////////////////////////////

        string WorkingDir4 = WorkingDirString;
        string WorkingDir5 = WorkingDirString;

        WorkingDir4.append(slashCharStr());
        WorkingDir4.append("ExtraFileCheck.ivf");

        WorkingDir5.append(slashCharStr());
        WorkingDir5.append("ExtraFileCheckResult.txt");

        char ExtraFileCheck[255];
        char ExtraFileCheckResult[255];

        snprintf(ExtraFileCheck, 255, "%s", WorkingDir4.c_str());
        snprintf(ExtraFileCheckResult, 255, "%s", WorkingDir5.c_str());

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

        tprintf("Extra File Check\n");

        tprintf("---------------------------Checking for OPSNR Files----------------------------\n");

        /////////////////////////////////////////////////Delete opsnr.stt////////////////////////////////////////////
        char CurrentPath[260];
        getcwd(CurrentPath, 260);

        string CurrentPathStr = CurrentPath;

        cout << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";
        cerr << "\nChecking: " << CurrentPathStr.c_str() << " For opsnr.stt \n";

        CurrentPathStr.append(slashCharStr());
        CurrentPathStr.append("opsnr.stt");

        FILE *opsnrExists = fopen(CurrentPathStr.c_str(), "rb");

        if (opsnrExists == NULL)
        {
            cout << "opsnr.stt File not found. \n";
            cerr << "opsnr.stt File not found. \n";

        }
        else
        {
            fclose(opsnrExists);
            remove(CurrentPathStr.c_str());
            cout << "opsnr.stt File found and deleted. \n";
            cerr << "opsnr.stt File found and deleted. \n";
        }

        string EF_DestString2 = EF_DestChar;

        cout << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";
        cerr << "\nChecking: " << EF_DestString2.c_str() << " For opsnr.stt \n";

        EF_DestString2.append(slashCharStr());
        EF_DestString2.append("opsnr.stt");

        FILE *opsnrExists2 = fopen(EF_DestString2.c_str(), "rb");

        if (opsnrExists2 == NULL)
        {
            cout << "opsnr.stt File not found. \n";
            cerr << "opsnr.stt File not found. \n";
        }
        else
        {
            fclose(opsnrExists2);
            remove(EF_DestString2.c_str());
            cout << "opsnr.stt File found and deleted. \n";
            cerr << "opsnr.stt File found and deleted. \n";
        }

        tprintf("------------------------------------------------------------------------------\n");
        ////////////////////////////////////////////////////////////////

        char *input = argv[2];

        VP8_CONFIG opt;
        vpxt_default_parameters(opt);

        ///////////////////Use Custom Settings///////////////////
        if (argc == 4)
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
        }

        /////////////////////////////////////////////////////////

        int CompressInt = opt.allow_df;
        int BitRate = opt.target_bandwidth;
        int Fail = 0;

        ///////////////////////////Record all files in the executable + Current directory///////////////////////////
        //Record all files in the executable directory.
        DIR *FindFileDataA;
        struct dirent *hFindA;
        string FileNameA;
        vector<string> SourceFileVectorA;

        FindFileDataA = opendir(EF_SourString1.c_str());
        hFindA = readdir(FindFileDataA);

        while (FileNameA.compare(hFindA->d_name) != 0)
        {
            FileNameA = hFindA->d_name;
            SourceFileVectorA.push_back(FileNameA);
            hFindA = readdir(FindFileDataA);

            if (hFindA == NULL)
            {
                break;
            }
        }

        closedir(FindFileDataA);

        //Record all files in the current directory.
        DIR *FindFileDataB;
        struct dirent *hFindB;
        string FileNameB;
        vector<string> SourceFileVectorB;

        FindFileDataB = opendir(EF_SourString2.c_str());
        hFindB = readdir(FindFileDataB);

        while (FileNameB.compare(hFindB->d_name) != 0)
        {
            FileNameB = hFindB->d_name;
            SourceFileVectorB.push_back(FileNameB);
            hFindB = readdir(FindFileDataB);

            if (hFindB == NULL)
            {
                break;
            }
        }

        closedir(FindFileDataB);

        //////////////////////////////////////////////////////////////
        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            tprintf("\nExtraFileCheck Test Run Previously Retreiveing Result\n");

            ifstream infile(ExtraFileCheckResult);

            if (!infile.is_open())
            {
                tprintf("File: %s not opened", ExtraFileCheckResult);
                return 0;
            }

            infile >> Fail;
            infile.close();

            if (Fail == 1)
            {
                cout << "\n\nExtra files found - Test Failed\n";
                cerr << "\n\nExtra files found - Test Failed\n";
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 0;
            }

            if (Fail == 0)
            {
                cout << "\n\n No extra files found - Test Passed\n";
                cerr << "\n\n No extra files found - Test Passed\n";
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 1;
            }
        }
        else
        {
            opt.Mode = MODE_SECONDPASS_BEST;

            if (vpxt_compress_ivf_to_ivf(input, ExtraFileCheck, opt.multi_threaded, BitRate, opt, CompressString, 0, 0, 0/*opt.DeleteFirstPassFile*/) == -1)
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

        tprintf("\n\n---------------------------Checking for Extra Files----------------------------\n\n");

        cout << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";
        cerr << "\n\nChecking:\n\n " << EF_DestString << "\n\n" << EF_SourString1 << " for extra files.\n";

        cout << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";
        cerr << "\nAnd\n\n" << EF_SourString2.c_str() << " for extra files.\n\n";

        ///////////////////////////Check all files in the executable + Current directory///////////////////////////
        //Check all files in the output directory.
        DIR *FindFileData2;
        struct dirent *hFind2;

        FindFileData2 = opendir(EF_DestString.c_str());
        hFind2 = readdir(FindFileData2);

        string FileName2;
        vector<string> DestFileExtraVector;

        while (FileName2.compare(hFind2->d_name) != 0)
        {
            FileName2 = hFind2->d_name;

            if (FileName2.find(".fpf") == string::npos && FileName2.compare("ExtraFileCheck.ivf") != 0 &&
                FileName2.compare(".") != 0 && FileName2.compare("..") != 0 && FileName2.compare("ExtraFileCheckTest.txt")
                && FileName2.compare("Extra File Check Test_TestOnly.txt") && FileName2.compare("ExtraFileCheck_Paramaters.txt")
                && FileName2.compare("ExtraFileCheck_APIParamaters.txt"))
            {
                DestFileExtraVector.push_back(FileName2);
                Fail = 2;
            }

            if (FileName2.compare("opsnr.stt") == 0)
            {
                DestFileExtraVector.push_back(FileName2);
                Fail = 2;
            }

            hFind2 = readdir(FindFileData2);

            if (hFind2 == NULL)
            {
                break;
            }
        }

        closedir(FindFileData2);

        //Check through all files in the executable directory.
        DIR *FindFileData3;
        struct dirent *hFind3;
        string FileName3;

        int filenum = 0;
        vector<int> FileFoundVect;
        vector<string> PostCompressionFileListVect;

        FindFileData3 = opendir(EF_SourString1.c_str());
        hFind3 = readdir(FindFileData3);

        while (FileName3.compare(hFind3->d_name) != 0)
        {
            FileName3 = hFind3->d_name;

            int vectloc = 0;
            int FileFound = 0;
            vector<string>::iterator it;
            it = SourceFileVectorA.begin();

            while (it < SourceFileVectorA.end())
            {
                //checks Exe directory for opsnr.stt file
                if (FileName3.compare("opsnr.stt") == 0)
                {
                    FileFound = 0;
                    break;
                }

                if (FileName3.compare(SourceFileVectorA[vectloc]) == 0)
                {
                    FileFound = 1;
                    break;
                }

                if (FileName3.find(".fpf") != string::npos)
                {
                    FileFound = 1;
                    break;
                }

                vectloc++;
                it++;
            }

            FileFoundVect.push_back(FileFound);
            PostCompressionFileListVect.push_back(FileName3);

            hFind3 = readdir(FindFileData3);

            if (hFind3 == NULL)
            {
                break;
            }
        }

        vector<int>::iterator it2;
        it2 = FileFoundVect.begin();
        int vectorposition = 0;

        while (it2 < FileFoundVect.end())
        {
            if (FileFoundVect[vectorposition] == 0)
            {
                cout << "Extra File Found: " << PostCompressionFileListVect[vectorposition];
                cerr << "Extra File Found: " << PostCompressionFileListVect[vectorposition];

                if (Fail != 2)
                {
                    Fail = 1;
                }
            }

            it2++;
            vectorposition++;
        }

        closedir(FindFileData3);

        //Check through all files in the current directory.
        DIR *FindFileData4;
        struct dirent *hFind4;
        string FileName4;

        int filenumB = 0;
        vector<int> FileFoundVectB;
        vector<string> PostCompressionFileListVectB;

        FindFileData4 = opendir(EF_SourString2.c_str());
        hFind4 = readdir(FindFileData4);

        while (FileName4.compare(hFind4->d_name) != 0)
        {
            FileName4 = hFind4->d_name;
            //puts(hFind4->d_name);

            int vectloc = 0;
            int FileFound = 0;
            vector<string>::iterator it;
            it = SourceFileVectorB.begin();

            while (it < SourceFileVectorB.end())
            {
                //checks current directory for opsnr.stt file
                if (FileName4.compare("opsnr.stt") == 0)
                {
                    FileFound = 0;
                    break;
                }

                if (FileName4.compare(SourceFileVectorB[vectloc]) == 0)
                {
                    FileFound = 1;
                    break;
                }

                if (FileName4.find(".fpf") != string::npos)
                {
                    FileFound = 1;
                    break;
                }

                vectloc++;
                it++;
            }

            FileFoundVectB.push_back(FileFound);
            PostCompressionFileListVectB.push_back(FileName4);

            hFind4 = readdir(FindFileData4);

            if (hFind4 == NULL)
            {
                break;
            }
        }

        vector<int>::iterator it2B;
        it2B = FileFoundVectB.begin();
        int vectorpositionB = 0;


        cout << "\n";
        cerr << "\n";

        while (it2B < FileFoundVectB.end())
        {
            if (FileFoundVectB[vectorpositionB] == 0)
            {
                cout << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];
                cerr << "\nExtra File Found: " << PostCompressionFileListVectB[vectorpositionB];

                if (Fail != 2)
                {
                    Fail = 1;
                }
            }

            it2B++;
            vectorpositionB++;
        }

        closedir(FindFileData4);
        //////////////////////////////////////////////////////////////

        cout << "\n";
        cerr << "\n";

        if (Fail == 2)
        {
            vector<string>::iterator it3;
            it3 = DestFileExtraVector.begin();
            int Varloc = 0;

            while (it3 < DestFileExtraVector.end())
            {
                cout << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
                cerr << "Extra File Found: " << DestFileExtraVector[Varloc] << "\n";
                Varloc++;
                it3++;
            }

            Fail = 1;
        }

        if (Fail == 1)
        {
            tprintf("\n------------------------------------------------------------------------------\n");

            if (TestType == 2)
            {
                cout << "\nExtra files found\n";
                cerr << "\nExtra files found\n";

                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 1;
                outfile.close();

                return 10;
            }

            tprintf("\n\nResults:\n\n");

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Extra Files Found - Failed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");

            tprintf("\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Fail == 0)
        {
            tprintf("------------------------------------------------------------------------------\n");

            if (TestType == 2)
            {
                cout << "\nNo extra files found\n";
                cerr << "\nNo extra files found\n";

                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 0;
                outfile.close();

                return 10;
            }

            tprintf("\n\nResults:\n\n");

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "No extra files found - Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");

            tprintf("\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 1;
        }

#endif
#if defined(_WIN32)
        char *CompressString = "opt.DeleteFirstPassFile";

        if (!(argc == 3 || argc == 4))
        {
            printf(
                "  ExtraFileCheck \n\n"
                "    <inputfile>\n"
                "    <Optional Settings File>\n"
                "\n"
            );
            return 0;
        }

        ////////////Formatting Test Specific Directory////////////
        string WorkingDirString = "";

        char WorkingDir3[255] = "";
        char *MyDir = "ExtraFileCheckTest";
        string MainDirString = "";
        char File1[255] = "";

        if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
            return 11;

        string ExtraFileDir = WorkingDirString;
        ExtraFileDir.push_back(slashChar());
        ////////////////////OutPut Directory location////////////////////
        char EF_DestChar[255];
        snprintf(EF_DestChar, 255, "%s", ExtraFileDir.c_str());
        string EF_DestString = EF_DestChar;
        EF_DestString.append("*");
        //////////////////////////////////////////////////////////////////
        //////////////////Program Location///////////////////////////////
        char EF_SourChar[255];
        char EF_SourChar2[255];
        vpxt_folder_name(argv[0], EF_SourChar2);
        snprintf(EF_SourChar, 255, "%s", EF_SourChar2);
        string EF_SourString1 = EF_SourChar;
        EF_SourString1.append("*");
        ////////////////////////////////////////////////////////////////
        //////////////////Current Dir Locatoin//////////////////////////
        TCHAR szDirectory[MAX_PATH] = "";

        if (!::GetCurrentDirectory(sizeof(szDirectory) - 1, szDirectory))
        {
            cout << "Could not get current directory";
            cerr << "Could not get current directory";
            return 0;
        }

        string EF_SourString2 = szDirectory;
        EF_SourString2.append("\\");
        EF_SourString2.append("*");
        ////////////////////////////////////////////////////////////////

        string WorkingDir4 = WorkingDirString;
        string WorkingDir5 = WorkingDirString;

        WorkingDir4.append("\\ExtraFileCheck.ivf");
        WorkingDir5.append("\\ExtraFileCheckResult.txt");

        char ExtraFileCheck[255];
        char ExtraFileCheckResult[255];

        snprintf(ExtraFileCheck, 255, "%s", WorkingDir4.c_str());
        snprintf(ExtraFileCheckResult, 255, "%s", WorkingDir5.c_str());

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

        tprintf("Extra File Check\n\n");
        tprintf("---------------------------Checking for OPSNR Files----------------------------\n");

        /////////////////////////////////////////////////Delete opsnr.stt////////////////////////////////////////////
        char CurrentPath[260];
        _getcwd(CurrentPath, 260);

        string CurrentPathStr = CurrentPath;

        cout << "Checking: " << CurrentPathStr.c_str() << " For opsnr.stt - ";
        cerr << "Checking: " << CurrentPathStr.c_str() << " For opsnr.stt - ";

        CurrentPathStr.append(slashCharStr());
        CurrentPathStr.append("opsnr.stt");

        FILE *opsnrExists = fopen(CurrentPathStr.c_str(), "rb");

        if (opsnrExists == NULL)
        {
            cout << "opsnr.stt File not found. \n\n";
            cerr << "opsnr.stt File not found. \n\n";

        }
        else
        {
            fclose(opsnrExists);
            remove(CurrentPathStr.c_str());
            cout << "opsnr.stt File found and deleted. \n";
            cerr << "opsnr.stt File found and deleted. \n";
        }

        string EF_DestString2 = EF_DestChar;

        cout << "Checking: " << EF_DestString2.c_str() << " For opsnr.stt - ";
        cerr << "Checking: " << EF_DestString2.c_str() << " For opsnr.stt - ";

        EF_DestString2.append(slashCharStr());
        EF_DestString2.append("opsnr.stt");

        FILE *opsnrExists2 = fopen(EF_DestString2.c_str(), "rb");

        if (opsnrExists2 == NULL)
        {
            cout << "opsnr.stt File not found. \n";
            cerr << "opsnr.stt File not found. \n";
        }
        else
        {
            fclose(opsnrExists2);
            remove(EF_DestString2.c_str());
            cout << "opsnr.stt File found and deleted. \n";
            cerr << "opsnr.stt File found and deleted. \n";
        }

        tprintf("------------------------------------------------------------------------------\n");
        ////////////////////////////////////////////////////////////////


        char *input = argv[2];

        VP8_CONFIG opt;
        vpxt_default_parameters(opt);

        ///////////////////Use Custom Settings///////////////////
        if (argc == 4)
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
        }

        /////////////////////////////////////////////////////////

        int CompressInt = opt.allow_df;
        int BitRate = opt.target_bandwidth;
        int Fail = 0;

        //Record all files in the executable directory
        WIN32_FIND_DATA FindFileDataA;
        HANDLE hFindA;
        string FileNameA;
        vector<string> SourceFileVectorA;

        hFindA = FindFirstFile(EF_SourString1.c_str(), &FindFileDataA);

        while (FileNameA.compare(FindFileDataA.cFileName) != 0)
        {
            FileNameA = FindFileDataA.cFileName;
            SourceFileVectorA.push_back(FileNameA);
            FindNextFile(hFindA, &FindFileDataA);
        }

        //Record all files in the current directory.
        WIN32_FIND_DATA FindFileDataB;
        HANDLE hFindB;
        string FileNameB;
        vector<string> SourceFileVectorB;

        hFindB = FindFirstFile(EF_SourString2.c_str(), &FindFileDataB);

        while (FileNameB.compare(FindFileDataB.cFileName) != 0)
        {
            FileNameB = FindFileDataB.cFileName;
            SourceFileVectorB.push_back(FileNameB);
            FindNextFile(hFindB, &FindFileDataB);
        }

        //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
        if (TestType == 3)
        {
            tprintf("\nExtraFileCheck Test Run Previously Retreiveing Result\n");

            ifstream infile(ExtraFileCheckResult);

            if (!infile.is_open())
            {
                tprintf("File: %s not opened", ExtraFileCheckResult);
                return 0;
            }

            infile >> Fail;
            infile.close();

            if (Fail == 1)
            {
                cout << "\n\nExtra files found - Test Failed\n";
                cerr << "\n\nExtra files found - Test Failed\n";
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 0;
            }

            if (Fail == 0)
            {
                cout << "\n\n No extra files found - Test Passed\n";
                cerr << "\n\n No extra files found - Test Passed\n";
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 1;
            }
        }
        else
        {
            opt.Mode = MODE_SECONDPASS_BEST;

            if (vpxt_compress_ivf_to_ivf(input, ExtraFileCheck, opt.multi_threaded, BitRate, opt, CompressString, 1, 0/*opt.DeleteFirstPassFile*/) == -1)
            {
                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);
                return 2;
            }
        }

        tprintf("\n\n---------------------------Checking for Extra Files----------------------------\n\n");

        cout << "Checking: " << EF_DestString << "\n\nChecking: " << EF_SourString1;
        cerr << "Checking: " << EF_DestString << "\n\nChecking: " << EF_SourString1;

        cout << "\n\nChecking: " << EF_SourString2.c_str() << "\n\n";
        cerr << "\n\nChecking: " << EF_SourString2.c_str() << "\n\n";

        ////////////////////////////////////////////////Windows Specific///////////////////////////////////
        WIN32_FIND_DATA FindFileData2;
        HANDLE hFind2;

        hFind2 = FindFirstFile(EF_DestString.c_str(), &FindFileData2);

        string FileName2;
        vector<string> DestFileExtraVector;

        while (FileName2.compare(FindFileData2.cFileName) != 0)
        {
            FileName2 = FindFileData2.cFileName;

            //Add File names here that you want the file checker to ignore follow format in if
            if (FileName2.find(".fpf") == string::npos && FileName2.compare("ExtraFileCheck.ivf") != 0 &&
                FileName2.compare(".") != 0 && FileName2.compare("..") != 0 && FileName2.compare("ExtraFileCheckTest.txt")
                && FileName2.compare("Extra File Check Test_TestOnly.txt") && FileName2.compare("ExtraFileCheck_Paramaters.txt")
                && FileName2.compare("ExtraFileCheck_APIParamaters.txt"))
            {
                DestFileExtraVector.push_back(FileName2);
                Fail = 2;
            }

            if (FileName2.compare("opsnr.stt") == 0)
            {
                DestFileExtraVector.push_back(FileName2);
                Fail = 2;
            }

            FindNextFile(hFind2, &FindFileData2);
        }

        //Check through source for files other than the originals or a .fpf file.
        WIN32_FIND_DATA FindFileData3;
        HANDLE hFind3;
        string FileName3;

        int filenum = 0;
        vector<int> FileFoundVect;
        vector<string> PostCompressionFileListVect;

        hFind3 = FindFirstFile(EF_SourString1.c_str(), &FindFileData3);

        while (FileName3.compare(FindFileData3.cFileName) != 0)
        {
            FileName3 = FindFileData3.cFileName;

            int vectloc = 0;
            int FileFound = 0;
            vector<string>::iterator it;
            it = SourceFileVectorA.begin();

            while (it < SourceFileVectorA.end())
            {
                //checks Exe directory for opsnr.stt file
                if (FileName3.compare("opsnr.stt") == 0)
                {
                    FileFound = 0;
                    break;
                }

                if (FileName3.compare(SourceFileVectorA[vectloc]) == 0)
                {
                    FileFound = 1;
                    break;
                }

                if (FileName3.find(".fpf") != string::npos)
                {
                    FileFound = 1;
                    break;
                }

                vectloc++;
                it++;
            }

            FileFoundVect.push_back(FileFound);
            PostCompressionFileListVect.push_back(FileName3);

            FindNextFile(hFind3, &FindFileData3);
        }

        vector<int>::iterator it2;
        it2 = FileFoundVect.begin();
        int vectorposition = 0;

        while (it2 < FileFoundVect.end())
        {
            if (FileFoundVect[vectorposition] == 0)
            {
                cout << "Extra File Found: " << PostCompressionFileListVect[vectorposition];
                cerr << "Extra File Found: " << PostCompressionFileListVect[vectorposition];

                if (Fail != 2)
                {
                    Fail = 1;
                }
            }

            it2++;
            vectorposition++;
        }

        //Check through current directory a .fpf file.
        WIN32_FIND_DATA FindFileData4;
        HANDLE hFind4;
        string FileName4;

        int filenumB = 0;
        vector<int> FileFoundVectB;
        vector<string> PostCompressionFileListVectB;

        hFind4 = FindFirstFile(EF_SourString2.c_str(), &FindFileData4);

        while (FileName4.compare(FindFileData4.cFileName) != 0)
        {
            FileName4 = FindFileData4.cFileName;

            int vectloc = 0;
            int FileFound = 0;
            vector<string>::iterator it;
            it = SourceFileVectorB.begin();

            while (it < SourceFileVectorB.end())
            {
                //checks current directory for opsnr.stt file
                if (FileName4.compare("opsnr.stt") == 0)
                {
                    FileFound = 0;
                    break;
                }

                if (FileName4.compare(SourceFileVectorB[vectloc]) == 0)
                {
                    FileFound = 1;
                    break;
                }

                if (FileName4.find(".fpf") != string::npos)
                {
                    FileFound = 1;
                    break;
                }

                vectloc++;
                it++;
            }

            FileFoundVectB.push_back(FileFound);
            PostCompressionFileListVectB.push_back(FileName4);

            FindNextFile(hFind4, &FindFileData4);
        }

        vector<int>::iterator it2B;
        it2B = FileFoundVectB.begin();
        int vectorpositionB = 0;

        while (it2B < FileFoundVectB.end())
        {
            if (FileFoundVectB[vectorpositionB] == 0)
            {
                cout << "Extra File Found: " << PostCompressionFileListVectB[vectorpositionB] << "\n";
                cerr << "Extra File Found: " << PostCompressionFileListVectB[vectorpositionB] << "\n";

                if (Fail != 2)
                {
                    Fail = 1;
                }
            }

            it2B++;
            vectorpositionB++;
        }

        if (Fail == 2)
        {
            vector<string>::iterator it3;
            it3 = DestFileExtraVector.begin();
            int Varloc = 0;

            while (it3 < DestFileExtraVector.end())
            {
                cout << "\n" << "Extra File Found: " << DestFileExtraVector[Varloc];
                cerr << "\n" << "Extra File Found: " << DestFileExtraVector[Varloc];
                Varloc++;
                it3++;
            }

            Fail = 1;
        }

        if (Fail == 1)
        {
            tprintf("\n------------------------------------------------------------------------------\n");

            if (TestType == 2)
            {
                cout << "\nExtra files found\n";
                cerr << "\nExtra files found\n";

                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 1;
                outfile.close();

                return 10;
            }

            tprintf("\n\nResults:\n\n");

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Extra Files Found - Failed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");

            tprintf("\nFailed\n");

            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 0;
        }

        if (Fail == 0)
        {
            tprintf("------------------------------------------------------------------------------\n");

            if (TestType == 2)
            {
                cout << "\nNo extra files found\n";
                cerr << "\nNo extra files found\n";

                fclose(fp);
                string File1Str = File1;
                record_test_complete(MainDirString, File1Str, TestType);

                ofstream outfile(ExtraFileCheckResult);
                outfile << 0;
                outfile.close();

                return 10;
            }

            tprintf("\n\nResults:\n\n");

            char OutputChar1[255];
            snprintf(OutputChar1, 255, "No extra files found - Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");

            tprintf("\nPassed\n");

            fclose(fp);
            string File1Str = File1;
            record_test_complete(MainDirString, File1Str, TestType);
            return 1;
        }

#endif

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 6;

    }