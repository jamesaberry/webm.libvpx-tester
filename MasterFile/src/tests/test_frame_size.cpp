#include "vpxt_test_declarations.h"

int test_frame_size(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[], int TestType, int DeleteIVF)
{
    char *CompressString = "Frame Size";
    char *MyDir = "test_frame_size";

    if (!(argc == 8 || argc == 7))
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
                "    <Starting Width-must be a mult of 16>\n"
                "    <Starting Height-must be a mult of 16>\n"
                "    <Optional Settings File>\n"
                "\n"
               );

        return 0;
    }

    std::string input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int StartingWidth = atoi(argv[5]);
    int StartingHeight = atoi(argv[6]);

    int speed = 0;

    ////////////Formatting Test Specific Directory////////////
    std::string CurTestDirStr = ""; // <- All Options need to set a value for this
    std::string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    char InputFileName[256] = "";
    vpxt_file_name(input.c_str(), InputFileName, 1);

    std::string FrameSizeBase = CurTestDirStr;
    FrameSizeBase.append(slashCharStr());
    FrameSizeBase.append(InputFileName);

    char NewWidth[20];
    char NewHeight[20];
    std::string RawCrop[47];

    //height
    int counter = 0;
    int FileNum = 1;

    while (counter < 16)
    {
        RawCrop[FileNum] = FrameSizeBase;
        RawCrop[FileNum].append("_");
        vpxt_itoa_custom(StartingWidth, NewWidth, 10); //width
        RawCrop[FileNum].append(NewWidth);
        RawCrop[FileNum].append("x");
        vpxt_itoa_custom(StartingHeight - (counter), NewHeight, 10); //height
        RawCrop[FileNum].append(NewHeight);
        RawCrop[FileNum].append("_raw.ivf");

        counter++;
        FileNum++;
    }

    //width
    counter = 1;

    while (counter < 16)
    {
        RawCrop[FileNum] = FrameSizeBase;
        RawCrop[FileNum].append("_");
        vpxt_itoa_custom(StartingWidth - (counter), NewWidth, 10); //width
        RawCrop[FileNum].append(NewWidth);
        RawCrop[FileNum].append("x");
        vpxt_itoa_custom(StartingHeight, NewHeight, 10); //height
        RawCrop[FileNum].append(NewHeight);
        RawCrop[FileNum].append("_raw.ivf");

        counter++;
        FileNum++;
    }

    //width and height
    counter = 1;

    while (counter < 16)
    {
        RawCrop[FileNum] = FrameSizeBase;
        RawCrop[FileNum].append("_");
        vpxt_itoa_custom(StartingWidth - (counter), NewWidth, 10); //width
        RawCrop[FileNum].append(NewWidth);
        RawCrop[FileNum].append("x");
        vpxt_itoa_custom(StartingHeight - (counter), NewHeight, 10); //height
        RawCrop[FileNum].append(NewHeight);
        RawCrop[FileNum].append("_raw.ivf");

        counter++;
        FileNum++;
    }

    //encoded file names
    FileNum = 1;
    std::string EncCrop[47];

    while (FileNum < 47)
    {
        EncCrop[FileNum] = RawCrop[FileNum];
        EncCrop[FileNum].erase(EncCrop[FileNum].end() - 7, EncCrop[FileNum].end());
        EncCrop[FileNum].append("enc.ivf");
        FileNum++;
    }

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

    //Make sure starting width and height are mults of 16
    if ((StartingWidth % 16 != 0) && (StartingHeight % 16 != 0))
    {
        tprintf(PRINT_BTH, "\nError: Starting width and height are not multiples of 16\n\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    if (StartingHeight % 16 != 0)
    {
        tprintf(PRINT_BTH, "\nError: Starting height is not a multiple of 16\n\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    if (StartingWidth % 16 != 0)
    {
        tprintf(PRINT_BTH, "\nError: Starting width is not a multiple of 16\n\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    vpxt_cap_string_print(PRINT_BTH, "%s", MyDir);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 8)
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
    double PSNRAr[46];
    int RawCropNum = 1;

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == TEST_ONLY)
    {
        //Get Prexisting psnr values
        while (RawCropNum < 47)
        {
            PSNRAr[RawCropNum-1] = vpxt_get_psnr(EncCrop[RawCropNum].c_str());
            RawCropNum++;
        }
    }
    else
    {

        //Create Raw Crops
        int x = 0;
        opt.Mode = Mode;

        while (x < 16)
        {
            //Crop
            tprintf(PRINT_BTH, "\nCroping to %i %i", StartingWidth, StartingHeight - x);
            vpxt_crop_raw_ivf(input.c_str(), RawCrop[RawCropNum].c_str(), 0, 0, StartingWidth, StartingHeight - x, 1, 1);

            //Comp
            char FileNameChar[256];
            char FileNameChar2[256];
            snprintf(FileNameChar, 256, RawCrop[RawCropNum].c_str());
            vpxt_file_name(FileNameChar, FileNameChar2, 1);

            tprintf(PRINT_BTH, "\nCompressing %s", FileNameChar2);

            if (vpxt_compress_ivf_to_ivf(RawCrop[RawCropNum].c_str(), EncCrop[RawCropNum].c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                return 2;
            }

            //PSNR
            PSNRAr[RawCropNum-1] = vpxt_ivf_psnr(RawCrop[RawCropNum].c_str(), EncCrop[RawCropNum].c_str(), 0, 0, 1, NULL);

            char PSNROutFile[255];
            vpxt_remove_file_extension(EncCrop[RawCropNum].c_str(), PSNROutFile);
            char *PSNROutFileFull = strcat(PSNROutFile, "psnr.txt");

            std::ofstream outfilePSNR(PSNROutFileFull);
            outfilePSNR << PSNRAr[RawCropNum-1];
            outfilePSNR.close();

            //Delete(file deletions are done here due to the number of files that need to be generated)
            if (DeleteIVF)
            {
                vpxt_delete_files(1, RawCrop[RawCropNum].c_str());
                vpxt_delete_files(1, EncCrop[RawCropNum].c_str());
            }

            x++;
            RawCropNum++;
        }

        x = 1;

        while (x < 16)
        {
            //Crop
            tprintf(PRINT_BTH, "\nCroping to %i %i", StartingWidth - x, StartingHeight);
            vpxt_crop_raw_ivf(input.c_str(), RawCrop[RawCropNum].c_str(), 0, 0, StartingWidth - x, StartingHeight, 1, 1);

            //Comp
            char FileNameChar[256];
            char FileNameChar2[256];
            snprintf(FileNameChar, 256, EncCrop[RawCropNum].c_str());
            vpxt_file_name(FileNameChar, FileNameChar2, 1);

            tprintf(PRINT_BTH, "\nCompressing %s", FileNameChar2);

            if (vpxt_compress_ivf_to_ivf(RawCrop[RawCropNum].c_str(), EncCrop[RawCropNum].c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                return 2;
            }

            //PSNR
            PSNRAr[RawCropNum-1] = vpxt_ivf_psnr(RawCrop[RawCropNum].c_str(), EncCrop[RawCropNum].c_str(), 0, 0, 1, NULL);

            char PSNROutFile[255];
            vpxt_remove_file_extension(EncCrop[RawCropNum].c_str(), PSNROutFile);
            char *PSNROutFileFull = strcat(PSNROutFile, "psnr.txt");

            std::ofstream outfilePSNR(PSNROutFileFull);
            outfilePSNR << PSNRAr[RawCropNum-1];
            outfilePSNR.close();

            //Delete
            if (DeleteIVF)
            {
                vpxt_delete_files(1, RawCrop[RawCropNum].c_str());
                vpxt_delete_files(1, EncCrop[RawCropNum].c_str());
            }

            x++;
            RawCropNum++;
        }

        x = 1;

        while (x < 16)
        {
            //Crop
            tprintf(PRINT_BTH, "\nCroping to %i %i", StartingWidth - x, StartingHeight - x);
            vpxt_crop_raw_ivf(input.c_str(), RawCrop[RawCropNum].c_str(), 0, 0, StartingWidth - x, StartingHeight - x, 1, 1);

            //Comp
            char FileNameChar[256];
            char FileNameChar2[256];
            snprintf(FileNameChar, 256, RawCrop[RawCropNum].c_str());
            vpxt_file_name(FileNameChar, FileNameChar2, 1);

            tprintf(PRINT_BTH, "\nCompressing %s", FileNameChar2);

            if (vpxt_compress_ivf_to_ivf(RawCrop[RawCropNum].c_str(), EncCrop[RawCropNum].c_str(), speed, BitRate, opt, CompressString, 0, 0) == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                return 2;
            }

            //PSNR
            PSNRAr[RawCropNum-1] = vpxt_ivf_psnr(RawCrop[RawCropNum].c_str(), EncCrop[RawCropNum].c_str(), 0, 0, 1, NULL);

            char PSNROutFile[255];
            vpxt_remove_file_extension(EncCrop[RawCropNum].c_str(), PSNROutFile);
            char *PSNROutFileFull = strcat(PSNROutFile, "psnr.txt");

            std::ofstream outfilePSNR(PSNROutFileFull);
            outfilePSNR << PSNRAr[RawCropNum-1];
            outfilePSNR.close();

            //Delete
            if (DeleteIVF)
            {
                vpxt_delete_files(1, RawCrop[RawCropNum].c_str());
                vpxt_delete_files(1, EncCrop[RawCropNum].c_str());
            }

            x++;
            RawCropNum++;
        }
    }

    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    int PercentFail = 0;
    int MinPSNRFail = 0;

    RawCropNum = 1;
    double FivePercentPSNR = (5 * PSNRAr[0]) / 100;

    while (RawCropNum < 47)
    {
        char FileNameChar[256];
        char FileNameChar2[256];
        snprintf(FileNameChar, 256, EncCrop[RawCropNum].c_str());
        vpxt_file_name(FileNameChar, FileNameChar2, 0);

        if (RawCropNum == 1)
        {
            tprintf(PRINT_STD, "\n\n PSNR %s: %.2f", FileNameChar2, PSNRAr[RawCropNum-1]);
        }
        else if (PSNRAr[RawCropNum-1] <  PSNRAr[0] + FivePercentPSNR && PSNRAr[RawCropNum-1] >  PSNRAr[0] - FivePercentPSNR)
        {
            tprintf(PRINT_BTH, "\n PSNR %s: %.2f within 5%% of %.2f - Passed", FileNameChar2, PSNRAr[RawCropNum-1], PSNRAr[0]);
        }
        else
        {
            tprintf(PRINT_BTH, "\n PSNR %s: %.2f not within 5%% of %.2f - Failed", FileNameChar2, PSNRAr[RawCropNum-1], PSNRAr[0]);
            PercentFail = 1;
        }

        RawCropNum++;
    }

    tprintf(PRINT_BTH, "\n\n");

    RawCropNum = 1;

    while (RawCropNum < 47)
    {
        char FileNameChar[256];
        char FileNameChar2[256];
        snprintf(FileNameChar, 256, EncCrop[RawCropNum].c_str());
        vpxt_file_name(FileNameChar, FileNameChar2, 0);

        if (PSNRAr[RawCropNum-1] > 25.0)
        {
            tprintf(PRINT_BTH, "\n PSNR %s: %.2f > %.2f - Passed", FileNameChar2, PSNRAr[RawCropNum-1], 25.0);
        }
        else
        {
            tprintf(PRINT_BTH, "\n PSNR %s: %.2f < %.2f - Failed", FileNameChar2, PSNRAr[RawCropNum-1], 25.0);
            MinPSNRFail = 1;
        }

        RawCropNum++;
    }


    tprintf(PRINT_STD, "\n");
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    int fail = 0;

    if (PercentFail == 0)
    {
        vpxt_formated_print(RESPRT, "All PSNRs are within 5%% of %.2f - Passed", PSNRAr[0]);
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Not all PSNRs are within 5%% of %.2f - Failed", PSNRAr[0]);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (MinPSNRFail == 0)
    {
        vpxt_formated_print(RESPRT, "All PSNRs are greater than 25.0 - Passed");
        tprintf(PRINT_BTH, "\n");
    }
    else
    {
        vpxt_formated_print(RESPRT, "Not all PSNRs are greater than 25.0 - Failed");
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;

}