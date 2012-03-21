#include "vpxt_test_declarations.h"
#include "vpxt_test_list.h"
#include "vpxt_driver.h"

int run_multiple_tests(int argc,
                       const char *argv[],
                       std::string WorkingDir,
                       int NumberofTests,
                       int DeleteIVFFiles)
{
    if (argc < 5)
    {
        if (argc < 4)
        {
            tprintf(PRINT_STD,
                    "  run_multipule_tests \n\n"
                    "    <Test Type>\n"
                    "      <1 - Create Files and Run Tests>\n"
                    "        <Input Text File>\n"
                    "      <2 - Create Files only>\n"
                    "        <Input Text File>\n"
                    "      <3 - Run Tests For Existing Files>\n"
                    "        <Input Test Directory>\n"
                    "      <4 - Resume Test>\n"
                    "        <Input Test Directory>\n"
                    "\n");

            return 0;
        }
    }

    if (WorkingDir.length() > 175)
    {
        tprintf(PRINT_BTH, "\nError: Folder Path Length too long.\n");
        return 0;
    }

    char buffer[512] = "";
    char buffer2[512] = "";
    char identicalFileBuffer[3] = "";

    int *PassFail = new int[NumberofTests+2];

    std::string SummCompAndTest = "";
    std::string SummComp = "";
    std::string SummTest = "";

    char input[256] = "";
    strncpy(input, argv[3], 256);

    std::string StringAr[20];
    std::string TimeStampAr2[2];
    std::string SelectorAr[999];
    std::string SelectorAr2[999];

    unsigned long RunTimeRecAr[999];
    ///////////////////////Record Time it took to run test//////////////////////
    int RecordRunTimes = 0; //If set to one will record run times of tests
    unsigned int RunTime1 = 0;
    unsigned int RunTime2 = 0;
    ////////////////////////////////////////////////////////////////////////////

    int y = 0;
    int Buf1Var = 0;
    int TestsRun = 0;
    int PrintMe = 1;
    int DummyArgvVar = 1;
    int trackthis1 = 0;
    int trackthis2 = 0;
    int CommentBool = 0;
    int PassFailInt = 0;
    int SelectorArInt = 0;
    int TestsRunCatch = 0;
    int MakeNewTestrun = 1;
    int identicalFileVar = 1;
    int NumberOfTestsRun = 1;
    int TestType = atoi(argv[2]);
    int linesskippedtracker = 0;

    long FilePositionTracker = 0;
    long FilePositionTracker2 = 0;

    std::string TimeStampPrevious = "";

    std::string WorkDirFileStr = "";
    std::string WorkingTextFilestr = "";
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "Summary";

    int (*vpxt_test_funct_ptr[MAXTENUM+1])(int, const char *const*,
        const std::string &, std::string[], int, int) = {NULL};
    vpxt_test_funct_ptr[AlWDFNUM] = &test_allow_drop_frames;
    vpxt_test_funct_ptr[ALWLGNUM] = &test_allow_lag;
    vpxt_test_funct_ptr[ALWSRNUM] = &test_allow_spatial_resampling;
    vpxt_test_funct_ptr[ARNRTNUM] = &test_arnr;
    vpxt_test_funct_ptr[AUTKFNUM] = &test_auto_key_frame;
    vpxt_test_funct_ptr[BUFLVNUM] = &test_buffer_level;
    vpxt_test_funct_ptr[CPUDENUM] = &test_change_cpu_dec;
    vpxt_test_funct_ptr[CPUENNUM] = &test_change_cpu_enc;
    vpxt_test_funct_ptr[CONQUNUM] = &test_constrained_quality;
    vpxt_test_funct_ptr[COPSRNUM] = &test_copy_set_reference;
    vpxt_test_funct_ptr[DTARTNUM] = &test_data_rate;
    vpxt_test_funct_ptr[DBMRLNUM] = &test_debug_matches_release;
    vpxt_test_funct_ptr[DFWMWNUM] = &test_drop_frame_watermark;
    vpxt_test_funct_ptr[ENCBONUM] = &test_encoder_break_out;
    vpxt_test_funct_ptr[ERRCONUM] = &test_error_concealment;
    vpxt_test_funct_ptr[ERRMWNUM] = &test_error_resolution;
    vpxt_test_funct_ptr[EXTFINUM] = &test_extra_file;
    vpxt_test_funct_ptr[FIXDQNUM] = &test_fixed_quantizer;
    vpxt_test_funct_ptr[FKEFRNUM] = &test_force_key_frame;
    vpxt_test_funct_ptr[FRSZTNUM] = &test_frame_size;
    vpxt_test_funct_ptr[GQVBQNUM] = &test_good_vs_best;
    vpxt_test_funct_ptr[LGIFRNUM] = &test_lag_in_frames;
    vpxt_test_funct_ptr[MAXQUNUM] = &test_max_quantizer;
    vpxt_test_funct_ptr[MEML1NUM] = &test_mem_leak;
    vpxt_test_funct_ptr[MEML2NUM] = &test_mem_leak2;
    vpxt_test_funct_ptr[MINQUNUM] = &test_min_quantizer;
    vpxt_test_funct_ptr[MULRENUM] = &test_multiple_resolution_encode;
    vpxt_test_funct_ptr[MULTDNUM] = &test_multithreaded_dec;
    vpxt_test_funct_ptr[MULTENUM] = &test_multithreaded_enc;
    vpxt_test_funct_ptr[NVOECPTK] = &test_new_vs_old_enc_cpu_tick;
    vpxt_test_funct_ptr[NVOPSNUM] = &test_new_vs_old_psnr;
    vpxt_test_funct_ptr[NVOTSNUM] = &test_new_vs_old_temp_scale;
    vpxt_test_funct_ptr[NOISENUM] = &test_noise_sensitivity;
    vpxt_test_funct_ptr[OV2PSNUM] = &test_one_pass_vs_two_pass;
    vpxt_test_funct_ptr[PLYALNUM] = &test_play_alternate;
    vpxt_test_funct_ptr[POSTPNUM] = &test_post_processor;
    vpxt_test_funct_ptr[RECBFNUM] = &test_reconstruct_buffer;
    vpxt_test_funct_ptr[RSDWMNUM] = &test_resample_down_watermark;
    vpxt_test_funct_ptr[SPEEDNUM] = &test_speed;
    vpxt_test_funct_ptr[TMPSCNUM] = &test_temporal_scalability;
    vpxt_test_funct_ptr[TVECTNUM] = &test_test_vector;
    vpxt_test_funct_ptr[TTVSFNUM] = &test_thirtytwo_vs_sixtyfour;
    vpxt_test_funct_ptr[TV2BTNUM] = &test_two_pass_vs_two_pass_best;
    vpxt_test_funct_ptr[UNDSHNUM] = &test_undershoot;
    vpxt_test_funct_ptr[VERSINUM] = &test_version;
    vpxt_test_funct_ptr[VPXMINUM] = &test_vpx_matches_int;
    vpxt_test_funct_ptr[WMLMMNUM] = &test_win_lin_mac_match;

    char* vpxt_test_name_list[MAXTENUM+1]={
        "RunTestsFromFile",
        "test_allow_drop_frames",
        "test_allow_lag",
        "test_allow_spatial_resampling",
        "test_arnr",
        "test_auto_key_frame",
        "test_buffer_level",
        "test_change_cpu_dec",
        "test_change_cpu_enc",
        "test_constrained_quality",
        "test_copy_set_reference",
        "test_data_rate",
        "test_debug_matches_release",
        "test_drop_frame_watermark",
        "test_encoder_break_out",
        "test_error_concealment",
        "test_error_resolution",
        "test_extra_file",
        "test_fixed_quantizer",
        "test_force_key_frame",
        "test_frame_size",
        "test_good_vs_best",
        "test_lag_in_frames",
        "test_max_quantizer",
        "test_mem_leak",
        "test_mem_leak2",
        "test_min_quantizer",
        "test_multiple_resolution_encode",
        "test_multithreaded_dec",
        "test_multithreaded_enc",
        "test_new_vs_old_enc_cpu_tick",
        "test_new_vs_old_psnr",
        "test_new_vs_old_temp_scale",
        "test_noise_sensitivity",
        "test_one_pass_vs_two_pass",
        "test_play_alternate",
        "test_post_processor",
        "test_reconstruct_buffer",
        "test_resample_down_watermark",
        "test_speed",
        "test_temporal_scalability",
        "test_test_vector",
        "test_thirtytwo_vs_sixtyfour",
        "test_two_pass_vs_two_pass_best",
        "test_undershoot",
        "test_version",
        "test_vpx_matches_int",
        "test_win_lin_mac_match"};

    ////////////////////Sets Stage for Resume Mode//////////////////////
    if (TestType == 4)
    {
        // Read in whats been done already from the summary updating all
        // variables needed start running the test from TestRuns.txt file since
        // It is auto commented it will resume with the last test not completed.
        // So long as all arrays are updated properly and variables are as well
        // It should work seemlessly.

        //location of main dir in as workingdir
        WorkingDir = argv[3];

        WorkingTextFilestr = WorkingDir;
        WorkingTextFilestr.append(slashCharStr());
        //TestsRun.txt original file ided
        WorkingTextFilestr.append("tests_run.txt");
        //Determines which test is in the process of being run and sets the
        //correct text file as input file
        SummCompAndTest = WorkingDir;
        SummCompAndTest.append(slashCharStr());
        SummCompAndTest.append("test_results.txt");         // Mode 1
        SummComp = WorkingDir;
        SummComp.append(slashCharStr());
        SummComp.append("compression_results.txt");         // Mode 2
        SummTest = WorkingDir;
        SummTest.append(slashCharStr());
        SummTest.append("compression_test_results.txt");    // Mode 3

        int TestRunning = 0;

        std::fstream SummCompAndTestFile;
        SummCompAndTestFile.open(SummCompAndTest.c_str(), std::fstream::in);

        if (SummCompAndTestFile.good())
        {
            TestRunning = 1;
        }

        std::fstream SummTestFile;
        SummTestFile.open(SummTest.c_str());

        if (SummTestFile.good())
        {
            TestRunning = 3;
        }

        std::fstream SummCompFile;
        SummCompFile.open(SummComp.c_str());

        if (SummCompFile.good())
        {
            if (TestRunning != 3)
            {
                TestRunning = 2;
            }
        }

        SummCompAndTestFile.close();
        SummTestFile.close();
        SummCompFile.close();

        //exits with error if it cant find a summary file
        if (TestRunning == 0)
        {
            tprintf(PRINT_STD, "\nError: No Summary File found\n");
            delete [] PassFail;
            return 0;
        }

        ///Sets Correct input file
        std::string PriorResultInput;

        if (TestRunning == 1)
        {
            PriorResultInput = SummCompAndTest;
        }

        if (TestRunning == 2)
        {
            PriorResultInput =  SummComp;
        }

        if (TestRunning == 3)
        {
            PriorResultInput = SummTest;
        }

        std::fstream PriorResultInputFile;
        PriorResultInputFile.open(PriorResultInput.c_str());

        int TestsrunCount = 0;
        int VarToInput = 0;
        int p = 0;

        //read in and throw away header
        while (p < 7)
        {

            PriorResultInputFile.getline(buffer, 1024);
            p++;
        }

        //read in data
        while (!PriorResultInputFile.eof())
        {
            PriorResultInputFile.getline(buffer, 1024);
            std::string bufferStr = buffer;
            int u = 0;

            //0 number not read yet|1 TestType not read yet|2 TestFolder not
            //read yet|3 Status not read yet
            while (buffer[u] != '\0' && buffer[u] != '\r' && u
                <= bufferStr.length())
            {
                if (buffer[u] != ' ')
                {
                    if (VarToInput == 5)
                    {
                        VarToInput = 0;
                    }

                    if (VarToInput == 3)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (buffer[u] != '\0' && buffer[u] != '\r')
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;

                        }

                        buffer[u] = '\0';
                        std::string PassFailStr = buffer3;

                        if (PassFailStr.compare("Passed") == 0)
                        {
                            PassFail[TestsrunCount] = 1;
                        }

                        if (PassFailStr.compare("Failed") == 0)
                        {
                            PassFail[TestsrunCount] = 0;
                        }

                        if (PassFailStr.compare("Indeterminate") == 0)
                        {
                            PassFail[TestsrunCount] = 2;
                        }

                        if (PassFailStr.compare("SeeComboRunLog") == 0)
                        {
                            PassFail[TestsrunCount] = 3;
                        }

                        if (PassFailStr.compare("SeePSNRLog") == 0)
                        {
                            PassFail[TestsrunCount] = 4;
                        }

                        if (PassFailStr.compare("RandomTestCompleted") == 0)
                        {
                            PassFail[TestsrunCount] = 5;
                        }

                        if (PassFailStr.compare("MinTestPassed") == 0)
                        {
                            PassFail[TestsrunCount] = 8;
                        }

                        if (PassFailStr.compare("CompressionMade") == 0)
                        {
                            PassFail[TestsrunCount] = 10;
                        }

                        if (PassFailStr.compare("ErrorFileMismatch") == 0)
                        {
                            PassFail[TestsrunCount] = 11;
                        }

                        if (PassFailStr.compare("TestNotSupported") == 0)
                        {
                            PassFail[TestsrunCount] = 12;
                        }

                        VarToInput = 4;

                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    if (VarToInput == 2)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (a < 24)
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;

                        }

                        if (buffer[u] == '_')
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;
                            buffer3[a] = buffer[u];
                        }

                        SelectorAr2[TestsrunCount] = buffer3;
                        SelectorAr2[TestsrunCount].append("\"");

                        VarToInput = 3;
                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    if (VarToInput == 1)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (buffer[u] != ' ')
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;

                        }

                        SelectorAr[TestsrunCount] = buffer3;
                        VarToInput = 2;
                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    if (VarToInput == 0)
                    {
                        char buffer3[1024];

                        int a = 0;

                        while (buffer[u] != ' ')
                        {
                            buffer3[a] = buffer[u];
                            u++;
                            a++;
                        }

                        VarToInput = 1;
                        memset(buffer3, 0, sizeof(buffer3));
                    }

                    //resets var at end
                    if (VarToInput == 4)
                    {
                        int a = 0;

                        while (buffer[u] != '\0' && buffer[u] != '\r')
                        {
                            u++;
                            a++;
                        }

                        VarToInput = 0;
                    }
                }

                u++;
            }


            if (buffer[0] != '\0' && buffer[u] != '\r')
            {
                // if the first character of a line after the initial input is
                // read happens to be a dash it signifies the end of Tests being
                // run and the start of the summary, meaning that all operations
                // have already been run and the tests should teminate.
                if (buffer[0] == '-')
                {
                    if (TestRunning == 1)
                    {
                        tprintf(PRINT_STD, "\nAll tests and compressions have "
                            "already been created and run.\n");
                        PriorResultInputFile.close();
                        exit(0);
                    }

                    if (TestRunning == 2)
                    {
                        std::cout << "\nAll compressions have already been "
                            "created.\n";
                        PriorResultInputFile.close();
                        exit(0);
                    }

                    if (TestRunning == 3)
                    {
                        std::cout << "\nAll tests on compressions have already "
                            "been run.\n";
                        PriorResultInputFile.close();
                        exit(0);
                    }
                }

                TestsrunCount++;
            }

        }

        TestType = TestRunning;
        SelectorArInt = TestsrunCount;
        NumberOfTestsRun = TestsrunCount;
        PassFailInt = TestsrunCount;
        TestsRun = TestsrunCount;
        TestsRunCatch = TestsrunCount;

        if (TestType == 3)
        {
            NumberOfTestsRun++;
        }

        PriorResultInputFile.close();

        WorkingDir.append("\"");

        PrintMe = 0;
        MakeNewTestrun = 0;
    }

    ////////////////////////////////////////////////////////////////////
    ////////////////////////////Format Output///////////////////////////


    if (TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());
        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar();
        WorkingDir3[v+1] = '\0';

        WorkDirFileStr = WorkingDir3;
        WorkDirFileStr.append("test_results.txt");

    }

    if (TestType == 2)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());
        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar();
        WorkingDir3[v+1] = '\0';

        WorkDirFileStr = WorkingDir3;
        WorkDirFileStr.append("compression_results.txt");
    }

    if (TestType == 3)
    {
        WorkDirFileStr = argv[3];
        WorkDirFileStr.append(slashCharStr());
        WorkDirFileStr.append("compression_test_results.txt");
    }


    if (TestType != 4)
    {
        ////////////Formatting New Input TextFile///////////
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());
        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar();
        WorkingDir3[v+1] = '\0';

        WorkingTextFilestr = WorkingDir3;
        WorkingTextFilestr.append("tests_run.txt");
        ///////////////////////////////////////////////////

    }

    ////////////////////////////////////////////////////////////////////
    /////////////////Makes New Working Text File From Input/////////////
    if (TestType != 3 && MakeNewTestrun == 1)
    {
        //This copies original driving text file over to a new formated one not
        //used for Mode 4 or 3
        std::fstream infile;
        infile.open(input);

        if (!infile.good())
        {
            tprintf(PRINT_STD, "\nInput File does not exist\n");
            delete [] PassFail;
            return 0;
        }

        std::string CreateDir2(WorkingDir);
        CreateDir2.insert(0, "md \"");
        CreateDir2.erase(CreateDir2.end() - 1);
        vpxt_make_dir_vpx(CreateDir2);

        std::ofstream outfile(WorkingTextFilestr.c_str());

        while (!infile.eof())
        {
            infile.getline(buffer, 512);

            if (buffer[0] == '%' || buffer[0] == '\0' || buffer[0] == '+' ||
                buffer[0] == '\r')
            {
                outfile << buffer << "\n";
            }
            else
            {
                if (buffer[0] != ' ')
                {
                    outfile << " " << buffer << "\n";
                }
                else
                {
                    outfile << buffer << "\n";
                }
            }
        }

        infile.close();
        outfile.close();
    }

    if (TestType == 3)
    {

        //location of main dir in as workingdir
        WorkingDir = argv[3];

        WorkingTextFilestr = WorkingDir;
        WorkingTextFilestr.append(slashCharStr());
        //TestsRun.txt original file ided
        WorkingTextFilestr.append("tests_run.txt");

        WorkingDir.append("\"");

        //Need to go through and replace all '+' from input file with '-' so
        //that test can keep track of "Tests Only" run
        std::fstream WorkingTextFilePrep;
        WorkingTextFilePrep.open(WorkingTextFilestr.c_str());

        while (!WorkingTextFilePrep.eof())
        {
            FilePositionTracker = WorkingTextFilePrep.tellg();
            WorkingTextFilePrep.getline(buffer, 1024);
            FilePositionTracker2 = WorkingTextFilePrep.tellg();

            std::string bufferstring = buffer;
            bufferstring.erase(0);

            if (buffer[0] == '+')
            {
                bufferstring.insert(0, " ");
                WorkingTextFilePrep.seekg(FilePositionTracker);
                WorkingTextFilePrep << bufferstring.c_str() ;
                WorkingTextFilePrep.seekg(FilePositionTracker2);
            }
        }


        WorkingTextFilePrep.close();

        std::string SummComp = argv[3];
        SummComp.append(slashCharStr());
        SummComp.append("compression_results.txt");

        std::fstream PriorResultInputFile2;
        PriorResultInputFile2.open(SummComp.c_str());

        if (!PriorResultInputFile2.good())
        {
            tprintf(PRINT_STD, "\nError opening file: %s\n", SummComp.c_str());
            delete [] PassFail;
            return 0;
        }

        int TestsrunCount = 0;
        int VarToInput = 0;
        int p = 0;

        //read in and throw away header
        while (p < 7)
        {
            PriorResultInputFile2.getline(buffer, 1024);
            p++;
        }

        //read in data
        int runonce = 1;
        char buffer3[255];

        while (!PriorResultInputFile2.eof())
        {
            PriorResultInputFile2.getline(buffer, 1024);
            runonce = 1;
            int u = 0;
            //0 number not read yet|1 TestType not read yet|2 TestFolder not
            //read yet|3 Status not read yet

            int Q = 35;

            while (Q < 35 + 24)
            {
                buffer3[u] = buffer[Q];
                Q++;
                u++;
            }

            SelectorAr2[TestsrunCount] = buffer3;
            SelectorAr2[TestsrunCount].append("\"");

            if (buffer[0] != '\0' && buffer[u] != '\r')
            {
                TestsrunCount++;
            }

            memset(buffer3, 0, sizeof(buffer3));
        }

        PriorResultInputFile2.close();
    }

    //////////////////////////////////////////////////////////////////////
    std::fstream WorkingTextFile;
    WorkingTextFile.open(WorkingTextFilestr.c_str());

    if (!WorkingTextFile.good())
    {
        tprintf(PRINT_STD, "\nInput File does not exist\n");
        delete [] PassFail;
        return 0;
    }

    //////////////////Files are actualy processed and run here//////////////////
    while (!WorkingTextFile.eof())
    {
        memset(buffer, 0, sizeof(buffer));

        const char *DummyArgv[999];
        DummyArgv[0] = argv[0];
        DummyArgvVar = 1;
        FilePositionTracker = WorkingTextFile.tellg();
        WorkingTextFile.getline(buffer, 1024);
        FilePositionTracker2 = WorkingTextFile.tellg();

        std::string bufferString = buffer;

        trackthis1++;

        //skips over any line starting with a % in the input file to allow for
        //comenting
        if (buffer[0] == '%' || buffer[0] == '\0' || buffer[0] == '+' ||
            buffer[0] == '-' || buffer[0] == '\r')
        {
            linesskippedtracker++;
        }
        else
        {
            Buf1Var = 0;

            //parses through gotline and seperates commands out
            while (buffer[Buf1Var] != '\0' && buffer[Buf1Var] != '\r')
            {
                int Buf2Var = 0;

                while (buffer[Buf1Var] != 64 && buffer[Buf1Var] != '\0'
                    && buffer[Buf1Var] != '\r')
                {
                    buffer2[Buf2Var] = buffer[Buf1Var];
                    Buf1Var++;
                    Buf2Var++;
                }

                buffer2[Buf2Var] = '\0';

                if (buffer[Buf1Var] != '\0' && buffer[Buf1Var] != '\r')
                {
                    Buf1Var++;
                }

                if (CommentBool == 0)
                {
                    StringAr[DummyArgvVar] = buffer2;
                    DummyArgvVar++;
                }
            }

            y = 1;

            while (y < DummyArgvVar)
            {
                DummyArgv[y] = StringAr[y].c_str();
                y++;
            }

            char NumberOfTestsRunChar[4];
            DummyArgv[y] = vpxt_itoa_custom(NumberOfTestsRun,
                NumberOfTestsRunChar, 10);

            if (CommentBool == 0)
            {
                tprintf(PRINT_STD, "\n");

                int selector = vpxt_identify_test(DummyArgv[1]);

                if (selector >= 0 && selector < MAXTENUM)
                {
                    NumberOfTestsRun++;
                }

                TimeStampPrevious = TimeStampAr2[0];
                TimeStampAr2[0] = date_string();

                //set the correct tester type dir string input
                std::string TestDir = WorkingDir;

                if (TestType == 3)
                {
                    TestDir = argv[3];
                }

                if (selector == 0)
                {
                    if (TestType == 1)
                    {
                        tprintf(PRINT_STD, "Not Implemented Yet.\n");
                    }
                }

                    SelectorAr[SelectorArInt] = vpxt_test_name_list[selector];
                    check_time_stamp(SelectorArInt, SelectorAr, SelectorAr2,
                        TimeStampPrevious, identicalFileVar, TimeStampAr2);
                    SelectorAr2[SelectorArInt] = TimeStampAr2[0];

                    if (RecordRunTimes == 1)
                        RunTime1 = vpxt_get_time();

                    PassFail[PassFailInt] = vpxt_test_funct_ptr[selector]
                    (DummyArgvVar, (char **)DummyArgv, TestDir, TimeStampAr2,
                        TestType, DeleteIVFFiles);

                    if (RecordRunTimes == 1){
                        RunTime2 = vpxt_get_time();
                        RunTimeRecAr[SelectorArInt] =
                            vpxt_get_time_in_micro_sec(RunTime1, RunTime2);
                    }

                    TestsRun++;

                /////////////////////////////On Fly out put/////////////////////
                FILE *fp;
                FILE *fp_html;

                if ((fp = freopen(WorkDirFileStr.c_str(), "a", stderr)) == NULL)
                {
                    tprintf(PRINT_STD, "Cannot open summary out put file.\n");
                    delete [] PassFail;
                    return 0;
                }

                std::string Test_Results_Html;
                vpxt_remove_file_extension(WorkDirFileStr.c_str(),
                    Test_Results_Html);
                Test_Results_Html.erase(Test_Results_Html.length() - 1, 1);
                Test_Results_Html.append(".html");

                fp_html = fopen(Test_Results_Html.c_str() , "a");

                if (PrintMe == 1)
                {

                    fprintf(fp_html, "\n<html>\n<body\n><p>\n<PRE><META "
                        "HTTP-EQUIV=\"REFRESH\" CONTENT=\"60\">");
                    fclose(fp_html);
                    print_header_info_to_file(Test_Results_Html.c_str());
                    fp_html = fopen(Test_Results_Html.c_str() , "a");

                    if (TestType == 1)
                    {
                        fprintf(stderr, "\n------------------------------------"
                            "-------------------------------------------\n\n"
                                "                                  Test Results"
                                " \n");
                        fprintf(fp_html, "\n-----------------------------------"
                            "--------------------------------------------\n\n"
                                "                                  Test Results"
                                " \n");
                    }

                    if (TestType == 2)
                    {
                        fprintf(stderr, "\n------------------------------------"
                            "-------------------------------------------\n\n"
                                "                                Test "
                                "Compressions \n");
                        fprintf(fp_html, "\n<html>\n<body\n><p>\n<PRE>\n-------"
                            "--------------------------------------------------"
                            "----------------------\n\n"
                                "                                Test "
                                "Compressions \n");
                    }

                    if (TestType == 3)
                    {

                        fprintf(stderr, "\n------------------------------------"
                            "-------------------------------------------\n\n"
                                "                                Test Only "
                                "Results \n");
                        fprintf(fp_html, "\n<html>\n<body\n><p>\n<PRE>\n-------"
                            "--------------------------------------------------"
                            "----------------------\n\n"
                                "                                Test Only "
                                "Results \n");
                    }

                    fprintf(stderr, "\n\n%4s %-32s%-25s%s\n\n", "#", "Test Name"
                        , "Date and Time", "Status");
                    fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#", "Test "
                        "Name" , "Date and Time", "Status");

                    PrintMe = 0;
                }

                SelectorAr2[SelectorArInt].erase(
                    SelectorAr2[SelectorArInt].end() - 1);
                fprintf(stderr, "%4i %-32s%-25s", SelectorArInt,
                    SelectorAr[SelectorArInt].c_str(),
                    SelectorAr2[SelectorArInt].c_str());

                ////////////////////////////////////////////////////////////////
                std::string LowerCaseName = SelectorAr[SelectorArInt].c_str();
                vpxt_lower_case_string(LowerCaseName);

                std::string HtmlName =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlName.append(LowerCaseName);
                HtmlName.append("/");
                HtmlName.append(SelectorAr2[SelectorArInt].c_str());
                HtmlName.append("/");
                HtmlName.append(LowerCaseName);
                HtmlName.append(".txt\" color=\"green\">");
                HtmlName.append(SelectorAr[SelectorArInt].c_str());
                HtmlName.append("</a>");

                std::string HtmlStatus =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlStatus.append(LowerCaseName);
                HtmlStatus.append("/");
                HtmlStatus.append(SelectorAr2[SelectorArInt].c_str());
                HtmlStatus.append("/");
                HtmlStatus.append(LowerCaseName);

                std::string HtmlFolder =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlFolder.append(LowerCaseName);
                HtmlFolder.append("/");
                HtmlFolder.append(SelectorAr2[SelectorArInt].c_str());
                HtmlFolder.append("\" color=\"green\">");
                HtmlFolder.append(SelectorAr2[SelectorArInt].c_str());
                HtmlFolder.append("</a>");

                fprintf(fp_html, "%4i", SelectorArInt);
                fprintf(fp_html, " %s", HtmlName.c_str());

                int numberofspaces = 0;

                while (SelectorAr[SelectorArInt].size() + numberofspaces < 31)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                fprintf(fp_html, " %s", HtmlFolder.c_str());
                numberofspaces = 0;

                while (SelectorAr2[SelectorArInt].size() + numberofspaces < 25)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                ////////////////////////////////////////////////////////////////


                SelectorAr2[SelectorArInt].append("\"");

                if (PassFail[SelectorArInt] == 1)
                {
                    tprintf(PRINT_ERR, "Passed\n");

                    HtmlStatus.append(".txt\" color=\"green\">");
                    HtmlStatus.append("Passed");
                    HtmlStatus.append("</a>");
                    fprintf(fp_html, "%s \n", HtmlStatus.c_str());
                }

                if (PassFail[SelectorArInt] == 0)
                {
                    tprintf(PRINT_ERR, "Failed\n");

                    HtmlStatus.append(".txt\" color=\"red\">");
                    HtmlStatus.append("Failed");
                    HtmlStatus.append("</a>");
                    fprintf(fp_html, "%s \n", HtmlStatus.c_str());
                }

                if (PassFail[SelectorArInt] == 2)
                {
                    tprintf(PRINT_ERR, "Indeterminate\n");

                    HtmlStatus.append(".txt\" color=\"yellow\">");
                    HtmlStatus.append("Indeterminate");
                    HtmlStatus.append("</a>");
                    fprintf(fp_html, "%s \n", HtmlStatus.c_str());
                }

                if (PassFail[SelectorArInt] == 3)
                {
                    tprintf(PRINT_ERR, "SeeComboRunLog\n");
                    fprintf(fp_html, "SeeComboRunLog\n");
                }

                if (PassFail[SelectorArInt] == 4)
                {
                    tprintf(PRINT_ERR, "SeePSNRLog\n");
                    fprintf(fp_html, "SeePSNRLog\n");
                }

                if (PassFail[SelectorArInt] == 5)
                {
                    tprintf(PRINT_ERR, "RandomTestCompleted\n");
                    fprintf(fp_html, "RandomTestCompleted\n");
                }

                if (PassFail[SelectorArInt] == 8)
                {
                    tprintf(PRINT_ERR, "MinTestPassed\n");

                    HtmlStatus.append(".txt\" color=\"yellow\">");
                    HtmlStatus.append("MinTestPassed");
                    HtmlStatus.append("</a>");
                    fprintf(fp_html, "%s \n", HtmlStatus.c_str());
                }

                if (PassFail[SelectorArInt] == 10)
                {
                    tprintf(PRINT_ERR, "CompressionMade\n");
                    fprintf(fp_html, "CompressionMade\n");
                }

                if (PassFail[SelectorArInt] == 11)
                {
                    tprintf(PRINT_ERR, "ErrorFileMismatch\n");
                    fprintf(fp_html, "ErrorFileMismatch\n");
                }

                if (PassFail[SelectorArInt] == 12)
                {
                    tprintf(PRINT_ERR, "TestNotSupported\n");
                    fprintf(fp_html, "TestNotSupported\n");
                }

                fclose(fp);
                fclose(fp_html);

                WorkingTextFile.seekg(FilePositionTracker);
                std::string bufferstring = buffer;
                bufferstring.erase(0);

                if (TestType == 3)
                {
                    bufferstring.insert(0, "-");
                }
                else
                {
                    bufferstring.insert(0, "+");
                }

                WorkingTextFile << bufferstring.c_str() ;

                WorkingTextFile.seekg(FilePositionTracker2);
                ////////////////////////////////////////////////////////////////


                PassFailInt++;
                SelectorArInt++;
            }

            ////////////////////////////////////////////////////////////////////
            CommentBool = 0;
        }
    }

    WorkingTextFile.close();
    //outputs a summary of the test results

    if (TestsRun == 0)
    {
        std::cout << "Test file specified contains no tests.\n";
        delete [] PassFail;
        return 0;
    }

    if (TestsRun == TestsRunCatch)
    {
        if (TestType == 1)
        {
            tprintf(PRINT_STD, "\nAll tests and compressions have already been "
                "created and run.\n");
            delete [] PassFail;
            return 0;
        }

        if (TestType == 2)
        {
            tprintf(PRINT_STD, "\nAll compressions have already been created."
                "\n");
            delete [] PassFail;
            return 0;
        }

        if (TestType == 3)
        {
            tprintf(PRINT_STD, "\nAll tests on compressions have already been "
                "run.\n");
            delete [] PassFail;
            return 0;
        }
    }

    FILE *fp;
    FILE *fp_html;

    if ((fp = freopen(WorkDirFileStr.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open summary out put file.\n");
        delete [] PassFail;
        return 0;
    }

    std::string Test_Results_Html;
    vpxt_remove_file_extension(WorkDirFileStr.c_str(), Test_Results_Html);
    Test_Results_Html.erase(Test_Results_Html.length() - 1, 1);
    Test_Results_Html.append(".html");

    fp_html = fopen(Test_Results_Html.c_str() , "w");
    fprintf(fp_html, "\n<html>\n<body\n><p>\n<PRE>\n");
    fclose(fp_html);

    tprintf(PRINT_STD, "\n\n\n");
    print_header_info();
    print_header_info_to_file(Test_Results_Html.c_str());

    fp_html = fopen(Test_Results_Html.c_str() , "a");

    y = 0;

    while (y < TestsRun)
    {
        SelectorAr2[y].erase(SelectorAr2[y].end() - 1);
        y++;
    }

    int TestIndicator = 0;
    y = 0;

    if (TestType == 3 || TestType == 1)
    {

        if (TestType == 1)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                    "Test Results - Failed \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                    "Test Results - Failed \n");
        }

        if (TestType == 3)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                    "Test Results - Failed \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                    "Test Results - Failed \n");
        }

        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] == 0)
            {
                if (TestIndicator == 0)
                {
                    tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
                        "Test Name" , "Date and Time", "Status");
                    fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
                        "Test Name" , "Date and Time", "Status");
                }

                TestIndicator = 1;
                tprintf(PRINT_BTH, "%4i %-32s%-25s", y, SelectorAr[y].c_str(),
                    SelectorAr2[y].c_str());
                //////////////////////////////////////////////////////////////////////////////////
                std::string LowerCaseName = SelectorAr[y].c_str();
                vpxt_lower_case_string(LowerCaseName);

                std::string HtmlName =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlName.append(LowerCaseName);
                HtmlName.append("/");
                HtmlName.append(SelectorAr2[y].c_str());
                HtmlName.append("/");
                HtmlName.append(LowerCaseName);
                HtmlName.append(".txt\" color=\"green\">");
                HtmlName.append(SelectorAr[y].c_str());
                HtmlName.append("</a>");

                std::string HtmlFolder =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlFolder.append(LowerCaseName);
                HtmlFolder.append("/");
                HtmlFolder.append(SelectorAr2[y].c_str());
                HtmlFolder.append("\" color=\"green\">");
                HtmlFolder.append(SelectorAr2[y].c_str());
                HtmlFolder.append("</a>");

                fprintf(fp_html, "%4i", y);
                fprintf(fp_html, " %s", HtmlName.c_str());

                int numberofspaces = 0;

                while (SelectorAr[y].size() + numberofspaces < 31)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                fprintf(fp_html, " %s", HtmlFolder.c_str());
                numberofspaces = 0;

                while (SelectorAr2[y].size() + numberofspaces < 25)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                ////////////////////////////////////////////////////////////////
                tprintf(PRINT_BTH, "Failed\n");

                std::string HtmlStatus =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlStatus.append(LowerCaseName);
                HtmlStatus.append("/");
                HtmlStatus.append(SelectorAr2[y].c_str());
                HtmlStatus.append("/");
                HtmlStatus.append(LowerCaseName);
                HtmlStatus.append(".txt\" color=\"green\">");
                HtmlStatus.append("Failed");
                HtmlStatus.append("</a>");
                fprintf(fp_html, "%s \n", HtmlStatus.c_str());
            }

            y++;
        }

        if (TestIndicator == 0)
        {
            tprintf(PRINT_BTH, "\n    NONE\n\n");
            fprintf(fp_html, "\n    NONE\n\n");
        }


        if (TestType == 1)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                    "Test Results - Indeterminate \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                    "Test Results - Indeterminate \n");
        }

        if (TestType == 3)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                    "Test Results - Indeterminate \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                    "Test Results - Indeterminate \n");
        }

        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] == 2)
            {
                if (TestIndicator == 0)
                {
                    tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
                        "Test Name" , "Date and Time", "Status");
                    fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
                        "Test Name" , "Date and Time", "Status");
                }

                TestIndicator = 1;
                tprintf(PRINT_BTH, "%4i %-32s%-25s", y, SelectorAr[y].c_str(),
                    SelectorAr2[y].c_str());
                ////////////////////////////////////////////////////////////////
                std::string LowerCaseName = SelectorAr[y].c_str();
                vpxt_lower_case_string(LowerCaseName);

                std::string HtmlName =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlName.append(LowerCaseName);
                HtmlName.append("/");
                HtmlName.append(SelectorAr2[y].c_str());
                HtmlName.append("/");
                HtmlName.append(LowerCaseName);
                HtmlName.append(".txt\" color=\"green\">");
                HtmlName.append(SelectorAr[y].c_str());
                HtmlName.append("</a>");

                std::string HtmlFolder =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlFolder.append(LowerCaseName);
                HtmlFolder.append("/");
                HtmlFolder.append(SelectorAr2[y].c_str());
                HtmlFolder.append("\" color=\"green\">");
                HtmlFolder.append(SelectorAr2[y].c_str());
                HtmlFolder.append("</a>");

                fprintf(fp_html, "%4i", y);
                fprintf(fp_html, " %s", HtmlName.c_str());

                int numberofspaces = 0;

                while (SelectorAr[y].size() + numberofspaces < 31)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                fprintf(fp_html, " %s", HtmlFolder.c_str());
                numberofspaces = 0;

                while (SelectorAr2[y].size() + numberofspaces < 25)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                ////////////////////////////////////////////////////////////////
                tprintf(PRINT_BTH, "Indeterminate\n");

                std::string HtmlStatus =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlStatus.append(LowerCaseName);
                HtmlStatus.append("/");
                HtmlStatus.append(SelectorAr2[y].c_str());
                HtmlStatus.append("/");
                HtmlStatus.append(LowerCaseName);
                HtmlStatus.append(".txt\" color=\"green\">");
                HtmlStatus.append("Indeterminate");
                HtmlStatus.append("</a>");
                fprintf(fp_html, "%s \n", HtmlStatus.c_str());
            }

            y++;
        }

        if (TestIndicator == 0)
        {
            tprintf(PRINT_BTH, "\n    NONE\n\n");
            fprintf(fp_html, "\n    NONE\n\n");
        }

        if (TestType == 1)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                    "Test Results - MinPassed \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                    "Test Results - MinPassed \n");
        }

        if (TestType == 3)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                    "Test Results - MinPassed \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                    "Test Results - MinPassed \n");
        }

        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] == 8)
            {
                if (TestIndicator == 0)
                {
                    tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
                        "Test Name" , "Date and Time", "Status");
                    fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
                        "Test Name" , "Date and Time", "Status");
                }

                TestIndicator = 1;
                tprintf(PRINT_BTH, "%4i %-32s%-25s", y, SelectorAr[y].c_str(),
                    SelectorAr2[y].c_str());
                ////////////////////////////////////////////////////////////////
                std::string LowerCaseName = SelectorAr[y].c_str();
                vpxt_lower_case_string(LowerCaseName);

                std::string HtmlName =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlName.append(LowerCaseName);
                HtmlName.append("/");
                HtmlName.append(SelectorAr2[y].c_str());
                HtmlName.append("/");
                HtmlName.append(LowerCaseName);
                HtmlName.append(".txt\" color=\"green\">");
                HtmlName.append(SelectorAr[y].c_str());
                HtmlName.append("</a>");

                std::string HtmlFolder =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlFolder.append(LowerCaseName);
                HtmlFolder.append("/");
                HtmlFolder.append(SelectorAr2[y].c_str());
                HtmlFolder.append("\" color=\"green\">");
                HtmlFolder.append(SelectorAr2[y].c_str());
                HtmlFolder.append("</a>");

                fprintf(fp_html, "%4i", y);
                fprintf(fp_html, " %s", HtmlName.c_str());

                int numberofspaces = 0;

                while (SelectorAr[y].size() + numberofspaces < 31)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                fprintf(fp_html, " %s", HtmlFolder.c_str());
                numberofspaces = 0;

                while (SelectorAr2[y].size() + numberofspaces < 25)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                ////////////////////////////////////////////////////////////////
                tprintf(PRINT_BTH, "MinTestPassed\n");

                std::string HtmlStatus =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlStatus.append(LowerCaseName);
                HtmlStatus.append("/");
                HtmlStatus.append(SelectorAr2[y].c_str());
                HtmlStatus.append("/");
                HtmlStatus.append(LowerCaseName);
                HtmlStatus.append(".txt\" color=\"green\">");
                HtmlStatus.append("MinTestPassed");
                HtmlStatus.append("</a>");
                fprintf(fp_html, "%s \n", HtmlStatus.c_str());
            }

            y++;
        }

        if (TestIndicator == 0)
        {
            tprintf(PRINT_BTH, "\n    NONE\n\n");
            fprintf(fp_html, "\n    NONE\n\n");
        }

        if (TestType == 1)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                    "Test Results - Passed \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                    "Test Results - Passed \n");
        }

        if (TestType == 3)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                    "Test Results - Passed \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                    "Test Results - Passed \n");
        }

        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] == 1)
            {
                if (TestIndicator == 0)
                {
                    tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
                        "Test Name" , "Date and Time", "Status");
                    fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
                        "Test Name" , "Date and Time", "Status");
                }

                TestIndicator = 1;
                tprintf(PRINT_BTH, "%4i %-32s%-25s", y, SelectorAr[y].c_str(),
                    SelectorAr2[y].c_str());

                ////////////////////////////////////////////////////////////////
                std::string LowerCaseName = SelectorAr[y].c_str();
                vpxt_lower_case_string(LowerCaseName);

                std::string HtmlName =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlName.append(LowerCaseName);
                HtmlName.append("/");
                HtmlName.append(SelectorAr2[y].c_str());
                HtmlName.append("/");
                HtmlName.append(LowerCaseName);
                HtmlName.append(".txt\" color=\"green\">");
                HtmlName.append(SelectorAr[y].c_str());
                HtmlName.append("</a>");

                std::string HtmlFolder =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlFolder.append(LowerCaseName);
                HtmlFolder.append("/");
                HtmlFolder.append(SelectorAr2[y].c_str());
                HtmlFolder.append("\" color=\"green\">");
                HtmlFolder.append(SelectorAr2[y].c_str());
                HtmlFolder.append("</a>");

                fprintf(fp_html, "%4i", y);
                fprintf(fp_html, " %s", HtmlName.c_str());

                int numberofspaces = 0;

                while (SelectorAr[y].size() + numberofspaces < 31)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                fprintf(fp_html, " %s", HtmlFolder.c_str());
                numberofspaces = 0;

                while (SelectorAr2[y].size() + numberofspaces < 25)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                ////////////////////////////////////////////////////////////////

                tprintf(PRINT_BTH, "Passed\n");

                std::string HtmlStatus =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlStatus.append(LowerCaseName);
                HtmlStatus.append("/");
                HtmlStatus.append(SelectorAr2[y].c_str());
                HtmlStatus.append("/");
                HtmlStatus.append(LowerCaseName);
                HtmlStatus.append(".txt\" color=\"green\">");
                HtmlStatus.append("Passed");
                HtmlStatus.append("</a>");
                fprintf(fp_html, "%s \n", HtmlStatus.c_str());
            }

            y++;
        }

        if (TestIndicator == 0)
        {
            tprintf(PRINT_BTH, "\n    NONE\n\n");
            fprintf(fp_html, "\n    NONE\n\n");
        }


        if (TestType == 1)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                    "Test Results - Other \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                    "Test Results - Other \n");
        }

        if (TestType == 3)
        {
            tprintf(PRINT_BTH, "\n---------------------------------------------"
                "----------------------------------\n\n"
                    "Test Results - Other \n");
            fprintf(fp_html, "\n-----------------------------------------------"
                "--------------------------------\n\n"
                    "Test Results - Other \n");
        }

        TestIndicator = 0;
        y = 0;

        while (y < TestsRun)
        {
            if (PassFail[y] != 0 && PassFail[y] != 1 && PassFail[y] != 8 &&
                PassFail[y] != 2)
            {
                if (TestIndicator == 0)
                {
                    tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
                        "Test Name" , "Date and Time", "Status");
                    fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
                        "Test Name" , "Date and Time", "Status");
                }

                TestIndicator = 1;
                tprintf(PRINT_BTH, "%4i %-32s%-25s", y, SelectorAr[y].c_str(),
                    SelectorAr2[y].c_str());
                ////////////////////////////////////////////////////////////////
                std::string LowerCaseName = SelectorAr[y].c_str();
                vpxt_lower_case_string(LowerCaseName);

                std::string HtmlName =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlName.append(LowerCaseName);
                HtmlName.append("/");
                HtmlName.append(SelectorAr2[y].c_str());
                HtmlName.append("/");
                HtmlName.append(LowerCaseName);
                HtmlName.append(".txt\" color=\"green\">");
                HtmlName.append(SelectorAr[y].c_str());
                HtmlName.append("</a>");

                std::string HtmlFolder =
                  "<a style=\"color: #800000; text-decoration: none;\" href=\"";
                HtmlFolder.append(LowerCaseName);
                HtmlFolder.append("/");
                HtmlFolder.append(SelectorAr2[y].c_str());
                HtmlFolder.append("\" color=\"green\">");
                HtmlFolder.append(SelectorAr2[y].c_str());
                HtmlFolder.append("</a>");

                fprintf(fp_html, "%4i", y);
                fprintf(fp_html, " %s", HtmlName.c_str());

                int numberofspaces = 0;

                while (SelectorAr[y].size() + numberofspaces < 31)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                fprintf(fp_html, " %s", HtmlFolder.c_str());
                numberofspaces = 0;

                while (SelectorAr2[y].size() + numberofspaces < 25)
                {
                    fprintf(fp_html, " ");
                    numberofspaces = numberofspaces + 1;
                }

                ////////////////////////////////////////////////////////////////

                if (PassFail[y] == 3)
                {
                    tprintf(PRINT_BTH, "SeeComboRunLog\n");
                    fprintf(fp_html, "SeeComboRunLog\n");
                }

                if (PassFail[y] == 4)
                {
                    tprintf(PRINT_BTH, "SeePSNRLog\n");
                    fprintf(fp_html, "SeePSNRLog\n");
                }

                if (PassFail[y] == 5)
                {
                    tprintf(PRINT_BTH, "RandomTestCompleted\n");
                    fprintf(fp_html, "RandomTestCompleted\n");
                }

                if (PassFail[y] == 10)
                {
                    tprintf(PRINT_BTH, "CompressionMade.\n");
                    fprintf(fp_html, "CompressionMade.\n");
                }

                if (PassFail[y] == 11)
                {
                    tprintf(PRINT_BTH, "ErrorFileMismatch\n");
                    fprintf(fp_html, "ErrorFileMismatch\n");
                }

                if (PassFail[y] == 12)
                {
                    tprintf(PRINT_BTH, "TestNotSupported\n");
                    fprintf(fp_html, "TestNotSupported\n");
                }

            }

            y++;
        }

        if (TestIndicator == 0)
        {
            tprintf(PRINT_BTH, "\n    NONE\n\n");
            fprintf(fp_html, "\n    NONE\n\n");
        }

        /////////////////////////////Output Time it took to run test////////////
        if (RecordRunTimes == 1)
        {
            std::string TimeoutputStr;
            vpxt_remove_file_extension(WorkDirFileStr.c_str(), TimeoutputStr);
            TimeoutputStr.append("Times.txt");

            FILE *outputTime;
            outputTime = fopen(TimeoutputStr.c_str() , "w");

            tprintf(PRINT_STD, "\n---------------------------------------------"
                "----------------------------------\n\n"
                    "                       Time to Run Tests\n");
            fprintf(outputTime, "\n--------------------------------------------"
                "-----------------------------------\n\n"
                    "                       Time to Run Tests\n");

            tprintf(PRINT_STD, "\n\n%4s %-28s%s\n\n", "",
                " Test Name", "            Time");
            fprintf(outputTime, "\n\n%4s %-28s%s\n\n", "",
                " Test Name", "            Time");

            y = 0;

            while (y < TestsRun)
            {
                tprintf(PRINT_STD, "   %s", SelectorAr[y].c_str());
                fprintf(outputTime, "   %s", SelectorAr[y].c_str());

                int z = SelectorAr[y].size();

                while (z <= 25)
                {
                    tprintf(PRINT_STD, " ");
                    fprintf(outputTime, " ");
                    z++;
                }

                tprintf(PRINT_STD, "%20u ms\n", RunTimeRecAr[y]);
                fprintf(outputTime, "%20u ms\n", RunTimeRecAr[y]);
                y++;
            }
        }

        ////////////////////////////////////////////////////////////////////////
    }

    if (TestType == 1)
    {
        tprintf(PRINT_BTH, "\n-------------------------------------------------"
            "------------------------------\n\n"
                "Chronological Test Results \n");
        fprintf(fp_html, "\n---------------------------------------------------"
            "----------------------------\n\n"
                "Chronological Test Results \n");
    }

    if (TestType == 2)
    {
        tprintf(PRINT_BTH, "\n-------------------------------------------------"
            "------------------------------\n\n"
                "Chronological Test Compressions \n");
        fprintf(fp_html, "\n---------------------------------------------------"
            "----------------------------\n\n"
                "Chronological Test Compressions \n");
    }

    if (TestType == 3)
    {
        tprintf(PRINT_BTH, "\n-------------------------------------------------"
            "------------------------------\n\n"
                "Chronological Test Only Results \n");
        fprintf(fp_html, "\n---------------------------------------------------"
            "----------------------------\n\n"
                "Chronological Test Only Results \n");
    }

    y = 0;

    tprintf(PRINT_BTH, "\n\n%4s %-32s%-25s%s\n\n", "#",
        "Test Name" , "Date and Time", "Status");
    fprintf(fp_html, "\n\n%4s %-32s%-25s%s\n\n", "#",
        "Test Name" , "Date and Time", "Status");

    while (y < TestsRun)
    {
        tprintf(PRINT_BTH, "%4i %-32s%-25s", y, SelectorAr[y].c_str(),
            SelectorAr2[y].c_str());

        ////////////////////////////////////////////////////////////////////////
        std::string LowerCaseName = SelectorAr[y].c_str();
        vpxt_lower_case_string(LowerCaseName);

        std::string HtmlName =
            "<a style=\"color: #800000; text-decoration: none;\" href=\"";
        HtmlName.append(LowerCaseName);
        HtmlName.append("/");
        HtmlName.append(SelectorAr2[y].c_str());
        HtmlName.append("/");
        HtmlName.append(LowerCaseName);
        HtmlName.append(".txt\" color=\"green\">");
        HtmlName.append(SelectorAr[y].c_str());
        HtmlName.append("</a>");

        std::string HtmlStatus =
            "<a style=\"color: #800000; text-decoration: none;\" href=\"";
        HtmlStatus.append(LowerCaseName);
        HtmlStatus.append("/");
        HtmlStatus.append(SelectorAr2[y].c_str());
        HtmlStatus.append("/");
        HtmlStatus.append(LowerCaseName);

        std::string HtmlFolder =
            "<a style=\"color: #800000; text-decoration: none;\" href=\"";
        HtmlFolder.append(LowerCaseName);
        HtmlFolder.append("/");
        HtmlFolder.append(SelectorAr2[y].c_str());
        HtmlFolder.append("\" color=\"green\">");
        HtmlFolder.append(SelectorAr2[y].c_str());
        HtmlFolder.append("</a>");

        fprintf(fp_html, "%4i", y);
        fprintf(fp_html, " %s", HtmlName.c_str());

        int numberofspaces = 0;

        while (SelectorAr[y].size() + numberofspaces < 31)
        {
            fprintf(fp_html, " ");
            numberofspaces = numberofspaces + 1;
        }

        fprintf(fp_html, " %s", HtmlFolder.c_str());
        numberofspaces = 0;

        while (SelectorAr2[y].size() + numberofspaces < 25)
        {
            fprintf(fp_html, " ");
            numberofspaces = numberofspaces + 1;
        }

        ////////////////////////////////////////////////////////////////////////


        if (PassFail[y] == 1)
        {
            tprintf(PRINT_BTH, "Passed\n");

            HtmlStatus.append(".txt\" color=\"green\">");
            HtmlStatus.append("Passed");
            HtmlStatus.append("</a>");
            fprintf(fp_html, "%s \n", HtmlStatus.c_str());
        }

        if (PassFail[y] == 0)
        {
            tprintf(PRINT_BTH, "Failed\n");

            HtmlStatus.append(".txt\" color=\"red\">");
            HtmlStatus.append("Failed");
            HtmlStatus.append("</a>");
            fprintf(fp_html, "%s \n", HtmlStatus.c_str());
        }

        if (PassFail[y] == 2)
        {
            tprintf(PRINT_BTH, "Indeterminate\n");

            HtmlStatus.append(".txt\" color=\"yellow\">");
            HtmlStatus.append("Indeterminate");
            HtmlStatus.append("</a>");
            fprintf(fp_html, "%s \n", HtmlStatus.c_str());
        }

        if (PassFail[y] == 3)
        {
            tprintf(PRINT_BTH, "SeeComboRunLog\n");
            fprintf(fp_html, "SeeComboRunLog\n");
        }

        if (PassFail[y] == 4)
        {
            tprintf(PRINT_BTH, "SeePSNRLog\n");
            fprintf(fp_html, "SeePSNRLog\n");
        }

        if (PassFail[y] == 5)
        {
            tprintf(PRINT_BTH, "RandomTestCompleted\n");
            fprintf(fp_html, "RandomTestCompleted\n");
        }

        if (PassFail[y] == 8)
        {
            tprintf(PRINT_BTH, "MinTestPassed\n");

            HtmlStatus.append(".txt\" color=\"yellow\">");
            HtmlStatus.append("MinTestPassed");
            HtmlStatus.append("</a>");
            fprintf(fp_html, "%s \n", HtmlStatus.c_str());
        }

        if (PassFail[y] == 10)
        {
            tprintf(PRINT_BTH, "CompressionMade\n");
            fprintf(fp_html, "CompressionMade\n");
        }

        if (PassFail[y] == 11)
        {
            tprintf(PRINT_BTH, "ErrorFileMismatch\n");
            fprintf(fp_html, "ErrorFileMismatch\n");
        }

        if (PassFail[y] == 12)
        {
            tprintf(PRINT_BTH, "TestNotSupported\n");
            fprintf(fp_html, "TestNotSupported\n");
        }

        y++;
    }

    fclose(fp);
    fclose(fp_html);
    delete [] PassFail;
    return 0;
}