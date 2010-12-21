#include "vpxt_test_declarations.h"

int tool_rand_comp(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{

    char *CompressString = "RandomInput";

    if (argc != 3)
    {
        tprintf(PRINT_STD, "\n"
                "  RandComp\n\n"
                "    <Input File>\n\n"
               );
        return 0;
    }

    char *input = argv[2];
    ////////////Formatting Test Specific Directory////////////
    char WorkingDir2[255] = "";
    char WorkingDir3[255] = "";
    char *MyDir = "RandComp";
    string MainDirString = "";
    char File1[255] = "";

    snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());

    int v = 0;

    while (WorkingDir2[v] != '\"')
    {
        WorkingDir3[v] = WorkingDir2[v];
        v++;
    }

    WorkingDir3[v] = slashChar();
    WorkingDir3[v+1] = '\0';
    string WorkingDirString = WorkingDir3;
    WorkingDirString.append(MyDir);
    WorkingDirString.append(slashCharStr());
    WorkingDirString.append(FilesAr[0]);
    WorkingDirString.erase(WorkingDirString.length() - 1, 1);

    string CreateDir2 = WorkingDirString;
    CreateDir2.insert(0, "md \"");
    vpxt_make_dir_vpx(CreateDir2.c_str());

    string WorkingDir4 = WorkingDirString;
    string WorkingDir5 = WorkingDirString;

    WorkingDir4.append(slashCharStr());

    char InputFileName[255];
    vpxt_file_name(input, InputFileName, 0);
    string InputFileNameStr = InputFileName;

    if (InputFileNameStr.substr(InputFileNameStr.length() - 4, 4).compare(".ivf") == 0) //if file extension is present remove it
        WorkingDir4.append(InputFileNameStr.substr(0, InputFileNameStr.length() - 4));
    else
        WorkingDir4.append(InputFileName);

    WorkingDir4.append("-RandComp.ivf");
    WorkingDir5.append(slashCharStr());
    WorkingDir5.append("RandCompOutPut.txt");

    char RandComp[255];
    char RandCompParOut[255];

    snprintf(RandComp, 255, "%s", WorkingDir4.c_str());
    snprintf(RandCompParOut, 255, "%s", WorkingDir5.c_str());

    /////////////OutPutfile////////////
    string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);
    TextfileString.append(".txt");

    FILE *fp;

    if ((fp = freopen(TextfileString.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n", TextfileString.c_str());
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



    VP8_CONFIG opt = vpxt_random_parameters(opt, input, 1);
    int Mode = opt.Mode;

    if (Mode == 0)
    {
        opt.Mode = MODE_REALTIME;
        //vpxt_output_settings(RandCompParOut,opt);
        int speed = opt.multi_threaded;
        int BitRate = opt.target_bandwidth;

        if (vpxt_compress_ivf_to_ivf(input, RandComp, speed, BitRate, opt, CompressString, 1, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, TestType);
            return 2;
        }
    }

    if (Mode == 1)
    {
        opt.Mode = MODE_GOODQUALITY;
        //vpxt_output_settings(RandCompParOut,opt);
        int speed = opt.multi_threaded;
        int BitRate = opt.target_bandwidth;

        if (vpxt_compress_ivf_to_ivf(input, RandComp, speed, BitRate, opt, CompressString, 1, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, TestType);
            return 2;
        }
    }

    if (Mode == 2)
    {
        opt.Mode = MODE_BESTQUALITY;
        //vpxt_output_settings(RandCompParOut,opt);
        int speed = opt.multi_threaded;
        int BitRate = opt.target_bandwidth;

        if (vpxt_compress_ivf_to_ivf(input, RandComp, speed, BitRate, opt, CompressString, 1, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, TestType);
            return 2;
        }
    }

    if (Mode == 3)
    {
    }

    if (Mode == 4)
    {
        int speed = opt.multi_threaded;
        int BitRate = opt.target_bandwidth;
        opt.Mode = MODE_SECONDPASS_BEST;

        //vpxt_output_settings(RandCompParOut,opt);
        if (vpxt_compress_ivf_to_ivf(input, RandComp, speed, BitRate, opt, CompressString, 1, 0, 0, 3, 3) == -1)
        {
            fclose(fp);
            record_test_complete(MainDirString, File1, TestType);
            return 2;
        }
    }

    return 5;
}