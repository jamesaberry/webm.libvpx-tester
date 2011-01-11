#include "vpxt_test_declarations.h"

///////////////////////////////External Arrays to Test/////////////////////////////
//#define DATA_COV /* If DataCov Definition is commented out Array Code Coverage Tests are not inclued if it is not they are */
#if defined(DATA_COV)

extern "C" unsigned int vp8dx_bitreader_norm[256];
extern "C" unsigned int vp8_mv_cont_count[5][4];
extern "C" int vp8_default_zig_zag1d[16];
extern "C" UINT16 vp8_coef_bands_x[16];
//extern "C" UINT16 vp8_block2context[25*3];
extern "C" int vp8_block2left[25];
extern "C" int vp8_block2above[25];
extern "C" int vp8_block2type[25];
extern "C" int vp8_BLOCK2CONTEXT[25];

#define uchar unsigned char
typedef uchar cuchar;

extern "C" cuchar vp8_coef_bands[16];
extern "C" cuchar vp8_prev_token_class[12];
extern "C" short vp8_default_zig_zag_mask[16];
extern "C" int vp8_mb_feature_data_bits[2];

typedef signed char VP8_TreeIndex;

extern "C" VP8_TreeIndex vp8_coef_tree[22];

typedef unsigned char VP8_Prob;
typedef VP8_Prob Prob;

extern "C" Prob Pcat1[1];
extern "C" Prob Pcat2[2];
extern "C" Prob Pcat3[3];
extern "C" Prob Pcat4[4];
extern "C" Prob Pcat5[5];
extern "C" Prob Pcat6[11];

extern "C" VP8_TreeIndex cat1[2];
extern "C" VP8_TreeIndex cat2[4];
extern "C" VP8_TreeIndex cat3[6];
extern "C" VP8_TreeIndex cat4[8];
extern "C" VP8_TreeIndex cat5[10];
extern "C" VP8_TreeIndex cat6[22];

extern "C" int kf_y_mode_cts[5];
extern "C" int y_mode_cts[5];
extern "C" int uv_mode_cts[4];
extern "C" int kf_uv_mode_cts[4];
extern "C" int bmode_cts[10];

extern "C" VP8_Prob sub_mv_ref_prob [3];
extern "C" VP8_Prob vp8_sub_mv_ref_prob2[5][3];

typedef int VP8_MBsplit[16];

extern "C" VP8_MBsplit vp8_mbsplits [4];
extern "C" int vp8_mbsplit_count[4];
extern "C" VP8_Prob vp8_mbsplit_probs[3];

extern "C" VP8_TreeIndex vp8_bmode_tree[18];
extern "C" VP8_TreeIndex vp8_ymode_tree[8];
extern "C" VP8_TreeIndex vp8_kf_ymode_tree[8];
extern "C" VP8_TreeIndex vp8_uv_mode_tree[6];
extern "C" VP8_TreeIndex vp8_mbsplit_tree[6];
extern "C" VP8_TreeIndex vp8_mv_ref_tree[8];
extern "C" VP8_TreeIndex vp8_sub_mv_ref_tree[6];

extern "C" VP8_TreeIndex vp8_small_mvtree [14];
//extern "C" VP8_TreeIndex vp8_LowMVtree [6];
//extern "C" VP8_TreeIndex vp8_HighMVtree [8];

//extern "C" int vp8_Reverse3bits[8];
//extern "C" int vp8_Reverse4bits[16];

typedef VP8_Prob MV_CONTEXT [19];

extern "C" MV_CONTEXT vp8_mv_update_probs[2];
extern "C" MV_CONTEXT vp8_default_mv_context[2];

extern "C" int bilinear_filters[8][2];
extern "C" short sub_pel_filters[8][6];

//extern "C" short idctMatrix1[4][4];
//extern "C" short idctMatrix2[4][4];

extern "C" int vp8_mode_contexts[6][4];

extern "C" unsigned int vp8_kf_default_bmode_counts [10] [10] [10];

extern "C" short kernel5[5];
extern "C" short vp8_rv[440];

extern "C" int dc_qlookup[128];
extern "C" int ac_qlookup[128];
extern "C" int bbb[4];

//////////////////////////Testing//////////////////////////

extern struct VP8_TokenStruct
{
    int value;
    int Len;
};

extern "C" struct VP8_TokenStruct vp8_CoefEncodings[12];

extern "C" struct VP8_TokenStruct vp8_BmodeEncodings[10];
extern "C" struct VP8_TokenStruct VP8_YmodeEncodings[5];
extern "C" struct VP8_TokenStruct vp8_MBsplitEncodings[4];
extern "C" struct VP8_TokenStruct vp8_SmallMVencodings[8];

extern "C" struct VP8_TokenStruct vp8_MVrefEncodingArray[5];
extern "C" struct VP8_TokenStruct vp8_subMVrefEncodingArray[4];
extern "C" struct VP8_TokenStruct vp8_kfYmodeEncodings[5];
extern "C" struct VP8_TokenStruct vp8_UVmodeEncodings[4];

////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    INT16         MinVal;
    INT16         Length;
    UINT8 Probs[12];
} TOKENEXTRABITS;

extern "C" TOKENEXTRABITS vp8d_token_extra_bits2[12];

//extern "C" struct
//{
//  int row;
//  int col;
//  int weight;  /*  was 5 - (abs(nearB[i].row) + abs(nearB[i].col)) */
//  int block;
//} nearB[3];
/////////////////////////////////////////////////////////////////////////////////
#endif
int tool_array_coverage(int argc, const char *const *argv, const std::string &WorkingDir, std::string FilesAr[])
{

    //DATA COVERED AS OF 05-23-2010
    //vp8dx_bitreader_norm
    //vp8_default_zig_zag1d
    //vp8_coef_bands_x
    //vp8_block2left
    //vp8_block2above
    //vp8_block2type
    //vp8_BLOCK2CONTEXT
    //vp8_coef_bands
    //vp8_prev_token_class
    //vp8_default_zig_zag_mask
    //vp8_mb_feature_data_bits
    //vp8_coef_tree
    //Pcat1
    //Pcat2
    //Pcat3
    //Pcat4
    //Pcat5
    //Pcat6
    //cat1
    //cat2
    //cat3
    //cat4
    //cat5
    //cat6
    //kf_y_mode_cts
    //y_mode_cts
    //uv_mode_cts
    //kf_uv_mode_cts
    //bmode_cts
    //sub_mv_ref_prob
    //vp8_mbsplits
    //vp8_mbsplit_count
    //vp8_mbsplit_probs
    //vp8_bmode_tree
    //vp8_ymode_tree
    //vp8_kf_ymode_tree
    //vp8_uv_mode_tree
    //vp8_mbsplit_tree
    //vp8_mv_ref_tree
    //vp8_sub_mv_ref_tree
    //vp8_small_mvtree
    //vp8_mv_update_probs
    //vp8_default_mv_context
    //bilinear_filters
    //sub_pel_filters
    //vp8_mode_contexts
    //kernel5
    //vp8_rv
    //dc_qlookup
    //ac_qlookup
    //bbb
    //vp8_kf_default_bmode_counts
    //vp8d_token_extra_bits2
    //vp8_CoefEncodings
    //vp8_BmodeEncodings
    //vp8_MBsplitEncodings
    //vp8_SmallMVencodings
    //vp8_MVrefEncodingArray
    //vp8_subMVrefEncodingArray
    //vp8_kfYmodeEncodings
    //vp8_UVmodeEncodings
#if defined(DATA_COV)
    char *CompressString = "Code Coverage";

    char *input = argv[2];

    if (argc < 7)
    {
        tprintf(PRINT_STD,
                "  CodeCoverage \n\n"
                "    <Input File>\n"
                "    <Mode>\n"
                "          (0)Realtime/Live Encoding\n"
                "          (1)Good Quality Fast Encoding\n"
                "          (2)One Pass Best Quality\n"
                "          (3)Two Pass - First Pass\n"
                "          (4)Two Pass\n"
                "          (5)Two Pass Best Quality\n"
                "    <Target Bit Rate>\n "
                "    <Compress Input File or Decode>\n "
                "          (NoCompress)Decode Input File\n"
                "          (MakeCompression)Compress Input File\n"
                "    <Array To Test 1 - 3 >\n "
                "    <First Varriable in Array To Test>\n "
                "    <Second Varriable in Array To Test - For 2 Dimensional Arrays>\n "

               );

        return 0;

    }



    ////////////Formatting Test Specific Directory////////////

    std::string WorkingDirString; // <- All Options need to set a value for this
    std::string Mode3TestMatch;
    std::string MainDirString;
    char *MyDir = "CodeCoverage";
    char WorkingDir2[255];
    char WorkingDir3[255];
    char File1[255];

    int TestType = 1;

    if (TestType == 2 || TestType == 1)
    {
        snprintf(WorkingDir2, 255, "%s", WorkingDir.c_str());
        //strcpy(WorkingDir2,WorkingDir.c_str());
        int v = 0;

        while (WorkingDir2[v] != '\"')
        {
            WorkingDir3[v] = WorkingDir2[v];
            v++;
        }

        WorkingDir3[v] = slashChar;
        WorkingDir3[v+1] = '\0';
        WorkingDirString = WorkingDir3;
        /////////////////////////////////////////////////////////////////////////////////
        MainDirString = WorkingDir3;
        MainDirString.append("FileIndex.txt");
        /////////////////////////////////////////////////////////////////////////////////
        WorkingDirString.append(MyDir);
        WorkingDirString.append(slashCharStr());
        WorkingDirString.append(FilesAr[0]);
        WorkingDirString.erase(WorkingDirString.length() - 1, 1);

        std::string CreateDir2 = WorkingDirString;
        CreateDir2.insert(0, "md \"");

        vpxt_make_dir_vpx(CreateDir2.c_str());

        ///////////////////////Records FileLocations for MultiPlat Test/////////////////
        if (TestType == 2)
        {
            char WorkingDirString2[255];
            snprintf(WorkingDirString2, 255, "%s", WorkingDirString.c_str());
            vpxt_subfolder_name(WorkingDirString2, File1);
        }

        ////////////////////////////////////////////////////////////////////////////////
    }
    else
    {
        //Use WorkingDir to get the main folder
        //Use Index File to get the rest of the string
        //Put it all together Setting WorkingDirString to the location of the files we want to examine.
        char buffer[255];

        std::string WorkingDir2 = WorkingDir;

        WorkingDir2.append(slashCharStr());
        MainDirString = WorkingDir2;
        MainDirString.append("FileIndex.txt");

        std::fstream FileStream;
        FileStream.open(MainDirString.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);

        int n = 0;

        while (n < atoi(argv[5]))
        {
            FileStream.getline(buffer, 255);
            n++;
        }

        FileStream.close();

        char Mode3TestMatchChar[255];
        vpxt_test_name(buffer, Mode3TestMatchChar);
        Mode3TestMatch = Mode3TestMatchChar;

        if (Mode3TestMatch.compare(MyDir) != 0)
        {
            tprintf(PRINT_STD, "ErrorFileMisMatch ");
            tprintf(PRINT_STD, "Mode3TestMatch: %s MyDir: %s", Mode3TestMatch, MyDir);
            return 11;
        }

        WorkingDir2.append(buffer);
        WorkingDirString = WorkingDir2;
    }

    std::string WorkingDir4 = WorkingDirString;
    std::string WorkingDir5 = WorkingDirString;
    std::string WorkingDir5b = WorkingDirString;

    WorkingDir4.append(slashCharStr());
    WorkingDir4.append("CodeCoverageDecCorrectMD5.txt");
    WorkingDir5.append(slashCharStr());
    WorkingDir5.append("CodeCoverageDecModifiedMD5.txt");
    WorkingDir5b.append(slashCharStr());
    WorkingDir5b.append("CodeCoverageCompression.ivf");

    char CodeCoverageDecCorrect[255];
    char CodeCoverageDecModified[255];
    char CodeCoverageCompression[255];

    snprintf(CodeCoverageDecCorrect, 255, "%s", WorkingDir4.c_str());
    snprintf(CodeCoverageDecModified, 255, "%s", WorkingDir5.c_str());
    snprintf(CodeCoverageCompression, 255, "%s", WorkingDir5b.c_str());

    /////////////OutPutfile////////////
    std::string TextfileString = WorkingDirString;
    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);

    if (TestType == 2 || TestType == 1)
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

    char *input2 = argv[5];

    tprintf(PRINT_STD, "Code Coverage Test\n");
    fprintf(stderr, "Code Coverage Test\n");

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    opt.target_bandwidth = BitRate;
    std::string CompressControl = argv[5];

    if (CompressControl.compare("MakeCompression") == 0)
    {
        if (Mode == 0)
        {
            opt.Mode = MODE_REALTIME;
            vpxt_time_compress_ivf_to_ivf(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0, 0, 0);
        }

        if (Mode == 1)
        {
            opt.Mode = MODE_GOODQUALITY;
            vpxt_time_compress_ivf_to_ivf(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0, 0, 0);
        }

        if (Mode == 2)
        {
            opt.Mode = MODE_BESTQUALITY;

            vpxt_time_compress_ivf_to_ivf(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0, 0, 0);
        }

        if (Mode == 3)
        {
        }

        if (Mode == 4)
        {
            opt.Mode = MODE_SECONDPASS;
            vpxt_time_compress_ivf_to_ivf(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0, 0, 0);
        }

        if (Mode == 5)
        {
            opt.Mode = MODE_SECONDPASS_BEST;
            vpxt_time_compress_ivf_to_ivf(input, CodeCoverageCompression, speed, BitRate, opt, CompressString, 0, 0, 0);
        }
    }

    else
    {
        snprintf(CodeCoverageCompression, 255, "%s", argv[2]);
    }

    int x = 0;
    int y = 0;
    int z = 0;

    if (argc < 8)
    {
        std::cout << "\nMust Input At least one array variable to check\n";
        return 0;
    }

    x = atoi(argv[7]);

    if (argc > 8)
    {
        y = atoi(argv[8]);
    }

    if (argc > 9)
    {
        z = atoi(argv[9]);
    }

    int StorageInt;
    int StorageInt2;
    char StorageChar;
    short StorageShort;

    std::cout << "\n";
    std::cerr << "\n";
    std::string arrayname = argv[6];
    int StringFound = 0;


    if (arrayname.compare("vp8dx_bitreader_norm") == 0)
    {
        if (x > 255)
        {
            std::cout << "\nMax Array Value = 255\n";
            return 0;
        }

        std::cout << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        std::cerr << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        StringFound = 1;
    }

    /*if(arrayname.compare("vp8_mv_cont_count")==0)
    {
    if(x > 4)
    {
    std::cout << "\nMax x Array Value = 4\n";
    return 0;
    }
    if(y > 3)
    {
    std::cout << "\nMax y Array Value = 3\n";
    return 0;
    }
    std::cout << "vp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    std::cerr << "vp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    StringFound = 1;
    }*/
    if (arrayname.compare("vp8_default_zig_zag1d") == 0)
    {
        if (x > 15)
        {
            std::cout << "\nMax Array Value = 15\n";
            return 0;
        }

        std::cout << "vp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        std::cerr << "vp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_coef_bands_x") == 0)
    {
        if (x > 15)
        {
            std::cout << "\nMax Array Value = 15\n";
            return 0;
        }

        std::cout << "vp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        std::cerr << "vp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        StringFound = 1;
    }

    //if(arrayname.compare("vp8_block2context")==0)
    //{
    //  if(x > 74)
    //  {
    //      std::cout << "\nMax Array Value = 74\n";
    //      return 0;
    //  }
    //  std::cout << "vp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //  std::cerr << "vp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //  StringFound = 1;
    //}

    if (arrayname.compare("vp8_block2left") == 0)
    {
        if (x > 24)
        {
            std::cout << "\nMax Array Value = 24\n";
            return 0;
        }

        std::cout << "vp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        std::cerr << "vp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_block2above") == 0)
    {
        if (x > 24)
        {
            std::cout << "\nMax Array Value = 24\n";
            return 0;
        }

        std::cout << "vp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        std::cerr << "vp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_block2type") == 0)
    {
        if (x > 24)
        {
            std::cout << "\nMax Array Value = 24\n";
            return 0;
        }

        std::cout << "vp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        std::cerr << "vp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_BLOCK2CONTEXT") == 0)
    {
        if (x > 24)
        {
            std::cout << "\nMax Array Value = 24\n";
            return 0;
        }

        std::cout << "vp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        std::cerr << "vp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_coef_bands") == 0) //Char
    {
        if (x > 15)
        {
            std::cout << "\nMax Array Value = 15\n";
            return 0;
        }

        std::cout << "vp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        std::cerr << "vp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_prev_token_class") == 0) //Char
    {
        if (x > 11)
        {
            std::cout << "\nMax Array Value = 11\n";
            return 0;
        }

        std::cout << "vp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        std::cerr << "vp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_default_zig_zag_mask") == 0) //short
    {
        if (x > 15)
        {
            std::cout << "\nMax Array Value = 15\n";
            return 0;
        }

        std::cout << "vp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        std::cerr << "vp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_mb_feature_data_bits") == 0)
    {
        if (x > 24)
        {
            std::cout << "\nMax Array Value = 24\n";
            return 0;
        }

        std::cout << "vp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        std::cerr << "vp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_coef_tree") == 0) //Char
    {
        if (x > 21)
        {
            std::cout << "\nMax Array Value = 21\n";
            return 0;
        }

        std::cout << "vp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        std::cerr << "vp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat1") == 0) //Char
    {
        if (x > 0)
        {
            std::cout << "\nMax Array Value = 0\n";
            return 0;
        }

        std::cout << "Pcat1[" << x << "] = " << Pcat1[x] << "\n";
        std::cerr << "Pcat1[" << x << "] = " << Pcat1[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat2") == 0) //Char
    {
        if (x > 1)
        {
            std::cout << "\nMax Array Value = 1\n";
            return 0;
        }

        std::cout << "Pcat2[" << x << "] = " << Pcat2[x] << "\n";
        std::cerr << "Pcat2[" << x << "] = " << Pcat2[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat3") == 0) //Char
    {
        if (x > 2)
        {
            std::cout << "\nMax Array Value = 2\n";
            return 0;
        }

        std::cout << "Pcat3[" << x << "] = " << Pcat3[x] << "\n";
        std::cerr << "Pcat3[" << x << "] = " << Pcat3[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat4") == 0) //Char
    {
        if (x > 3)
        {
            std::cout << "\nMax Array Value = 3\n";
            return 0;
        }

        std::cout << "Pcat4[" << x << "] = " << Pcat4[x] << "\n";
        std::cerr << "Pcat4[" << x << "] = " << Pcat4[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat5") == 0) //Char
    {
        if (x > 4)
        {
            std::cout << "\nMax Array Value = 4\n";
            return 0;
        }

        std::cout << "Pcat5[" << x << "] = " << Pcat5[x] << "\n";
        std::cerr << "Pcat5[" << x << "] = " << Pcat5[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("Pcat6") == 0) //Char
    {
        if (x > 10)
        {
            std::cout << "\nMax Array Value = 10\n";
            return 0;
        }

        std::cout << "Pcat6[" << x << "] = " << Pcat6[x] << "\n";
        std::cerr << "Pcat6[" << x << "] = " << Pcat6[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat1") == 0) //Char
    {
        if (x > 1)
        {
            std::cout << "\nMax Array Value = 1\n";
            return 0;
        }

        std::cout << "cat1[" << x << "] = " << cat1[x] << "\n";
        std::cerr << "cat1[" << x << "] = " << cat1[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat2") == 0) //Char
    {
        if (x > 3)
        {
            std::cout << "\nMax Array Value = 3\n";
            return 0;
        }

        std::cout << "cat2[" << x << "] = " << cat2[x] << "\n";
        std::cerr << "cat2[" << x << "] = " << cat2[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat3") == 0) //Char
    {
        if (x > 5)
        {
            std::cout << "\nMax Array Value = 5\n";
            return 0;
        }

        std::cout << "cat3[" << x << "] = " << cat3[x] << "\n";
        std::cerr << "cat3[" << x << "] = " << cat3[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat4") == 0) //Char
    {
        if (x > 7)
        {
            std::cout << "\nMax Array Value = 7\n";
            return 0;
        }

        std::cout << "cat4[" << x << "] = " << cat4[x] << "\n";
        std::cerr << "cat4[" << x << "] = " << cat4[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat5") == 0) //Char
    {
        if (x > 9)
        {
            std::cout << "\nMax Array Value = 9\n";
            return 0;
        }

        std::cout << "cat5[" << x << "] = " << cat5[x] << "\n";
        std::cerr << "cat5[" << x << "] = " << cat5[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("cat6") == 0) //Char
    {
        if (x > 21)
        {
            std::cout << "\nMax Array Value = 21\n";
            return 0;
        }

        std::cout << "cat6[" << x << "] = " << cat6[x] << "\n";
        std::cerr << "cat6[" << x << "] = " << cat6[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("kf_y_mode_cts") == 0)
    {
        if (x > 4)
        {
            std::cout << "\nMax Array Value = 4\n";
            return 0;
        }

        std::cout << "kf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        std::cerr << "kf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("y_mode_cts") == 0)
    {
        if (x > 4)
        {
            std::cout << "\nMax Array Value = 4\n";
            return 0;
        }

        std::cout << "y_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        std::cerr << "y_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("uv_mode_cts") == 0)
    {
        if (x > 3)
        {
            std::cout << "\nMax Array Value = 3\n";
            return 0;
        }

        std::cout << "uv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        std::cerr << "uv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("kf_uv_mode_cts") == 0)
    {
        if (x > 3)
        {
            std::cout << "\nMax Array Value = 3\n";
            return 0;
        }

        std::cout << "kf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        std::cerr << "kf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("bmode_cts") == 0)
    {
        if (x > 9)
        {
            std::cout << "\nMax Array Value = 9\n";
            return 0;
        }

        std::cout << "bmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        std::cerr << "bmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("sub_mv_ref_prob") == 0)
    {
        if (x > 2)
        {
            std::cout << "\nMax Array Value = 2\n";
            return 0;
        }

        std::cout << "sub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        std::cerr << "sub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        StringFound = 1;
    }

    /*if(arrayname.compare("vp8_sub_mv_ref_prob2")==0)
    {
    if(x > 4)
    {
    std::cout << "\nMax x Array Value = 4\n";
    return 0;
    }
    if(y > 3)
    {
    std::cout << "\nMax y Array Value = 3\n";
    return 0;
    }
    std::cout << "vp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    std::cerr << "vp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    StringFound = 1;
    }*/
    if (arrayname.compare("vp8_mbsplits") == 0)
    {
        if (x > 3)
        {
            std::cout << "\nMax x Array Value = 3\n";
            return 0;
        }

        if (y > 15)
        {
            std::cout << "\nMax y Array Value = 15\n";
            return 0;
        }

        std::cout << "vp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        std::cerr << "vp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_mbsplit_count") == 0)
    {
        if (x > 3)
        {
            std::cout << "\nMax Array Value = 3\n";
            return 0;
        }

        std::cout << "vp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        std::cerr << "vp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_mbsplit_probs") == 0) //Char
    {
        if (x > 2)
        {
            std::cout << "\nMax Array Value = 2\n";
            return 0;
        }

        std::cout << "vp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        std::cerr << "vp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_bmode_tree") == 0) //Char
    {
        if (x > 17)
        {
            std::cout << "\nMax Array Value = 17\n";
            return 0;
        }

        std::cout << "vp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        std::cerr << "vp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_ymode_tree") == 0) //Char
    {
        if (x > 7)
        {
            std::cout << "\nMax Array Value = 7\n";
            return 0;
        }

        std::cout << "vp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        std::cerr << "vp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_kf_ymode_tree") == 0) //Char
    {
        if (x > 7)
        {
            std::cout << "\nMax Array Value = 7\n";
            return 0;
        }

        std::cout << "vp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        std::cerr << "vp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_uv_mode_tree") == 0) //Char
    {
        if (x > 5)
        {
            std::cout << "\nMax Array Value = 5\n";
            return 0;
        }

        std::cout << "vp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        std::cerr << "vp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_mbsplit_tree") == 0) //Char
    {
        if (x > 5)
        {
            std::cout << "\nMax Array Value = 5\n";
            return 0;
        }

        std::cout << "vp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        std::cerr << "vp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_mv_ref_tree") == 0) //Char
    {
        if (x > 7)
        {
            std::cout << "\nMax Array Value = 7\n";
            return 0;
        }

        std::cout << "vp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        std::cerr << "vp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_sub_mv_ref_tree") == 0) //Char
    {
        if (x > 5)
        {
            std::cout << "\nMax Array Value = 5\n";
            return 0;
        }

        std::cout << "vp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        std::cerr << "vp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_small_mvtree") == 0) //Char
    {
        if (x > 13)
        {
            std::cout << "\nMax Array Value = 13\n";
            return 0;
        }

        std::cout << "vp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        std::cerr << "vp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        StringFound = 1;
    }

    //if(arrayname.compare("vp8_LowMVtree")==0)//Char
    //{
    //  if(x > 5)
    //  {
    //      std::cout << "\nMax Array Value = 5\n";
    //      return 0;
    //  }
    //  std::cout << "vp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //  std::cerr << "vp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //  StringFound = 1;
    //}
    //if(arrayname.compare("vp8_HighMVtree")==0)//Char
    //{
    //  if(x > 7)
    //  {
    //      std::cout << "\nMax Array Value = 7\n";
    //      return 0;
    //  }
    //  std::cout << "vp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //  std::cerr << "vp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //  StringFound = 1;
    //}
    //if(arrayname.compare("vp8_Reverse3bits")==0)
    //{
    //  if(x > 7)
    //  {
    //      std::cout << "\nMax Array Value = 7\n";
    //      return 0;
    //  }
    //  std::cout << "vp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //  std::cerr << "vp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //  StringFound = 1;
    //}
    //if(arrayname.compare("vp8_Reverse4bits")==0)
    //{
    //  if(x > 15)
    //  {
    //      std::cout << "\nMax Array Value = 15\n";
    //      return 0;
    //  }
    //  std::cout << "vp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //  std::cerr << "vp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //  StringFound = 1;
    //}
    if (arrayname.compare("vp8_mv_update_probs") == 0)
    {
        if (x > 1)
        {
            std::cout << "\nMax x Array Value = 1\n";
            return 0;
        }

        if (y > 18)
        {
            std::cout << "\nMax y Array Value = 18\n";
            return 0;
        }

        std::cout << "vp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        std::cerr << "vp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_default_mv_context") == 0)
    {
        if (x > 1)
        {
            std::cout << "\nMax x Array Value = 1\n";
            return 0;
        }

        if (y > 18)
        {
            std::cout << "\nMax y Array Value = 18\n";
            return 0;
        }

        std::cout << "vp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        std::cerr << "vp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("bilinear_filters") == 0)
    {
        if (x > 7)
        {
            std::cout << "\nMax x Array Value = 7\n";
            return 0;
        }

        if (y > 1)
        {
            std::cout << "\nMax y Array Value = 1\n";
            return 0;
        }

        std::cout << "bilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        std::cerr << "bilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("sub_pel_filters") == 0)
    {
        if (x > 7)
        {
            std::cout << "\nMax x Array Value = 7\n";
            return 0;
        }

        if (y > 5)
        {
            std::cout << "\nMax y Array Value = 5\n";
            return 0;
        }

        std::cout << "sub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        std::cerr << "sub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        StringFound = 1;
    }

    //if(arrayname.compare("idctMatrix1")==0)
    //{
    //  if(x > 3)
    //  {
    //      std::cout << "\nMax x Array Value = 3\n";
    //      return 0;
    //  }
    //  if(y > 3)
    //  {
    //      std::cout << "\nMax y Array Value = 3\n";
    //      return 0;
    //  }
    //  std::cout << "idctMatrix1[" << x << "][" << y << "] = " << idctMatrix1[x][y] << "\n";
    //  std::cerr << "idctMatrix1[" << x << "][" << y << "] = " << idctMatrix1[x][y] << "\n";
    //  StringFound = 1;
    //}
    //if(arrayname.compare("idctMatrix2")==0)
    //{
    //  if(x > 3)
    //  {
    //      std::cout << "\nMax x Array Value = 3\n";
    //      return 0;
    //  }
    //  if(y > 3)
    //  {
    //      std::cout << "\nMax y Array Value = 3\n";
    //      return 0;
    //  }
    //  std::cout << "idctMatrix2[" << x << "][" << y << "] = " << idctMatrix2[x][y] << "\n";
    //  std::cerr << "idctMatrix2[" << x << "][" << y << "] = " << idctMatrix2[x][y] << "\n";
    //  StringFound = 1;
    //}
    if (arrayname.compare("vp8_mode_contexts") == 0)
    {
        if (x > 5)
        {
            std::cout << "\nMax x Array Value = 5\n";
            return 0;
        }

        if (y > 3)
        {
            std::cout << "\nMax y Array Value = 3\n";
            return 0;
        }

        std::cout << "vp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        std::cerr << "vp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("kernel5") == 0) //short
    {
        if (x > 4)
        {
            std::cout << "\nMax Array Value = 4\n";
            return 0;
        }

        std::cout << "kernel5[" << x << "] = " << kernel5[x] << "\n";
        std::cerr << "kernel5[" << x << "] = " << kernel5[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_rv") == 0) //short
    {
        if (x > 439)
        {
            std::cout << "\nMax Array Value = 439\n";
            return 0;
        }

        std::cout << "vp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        std::cerr << "vp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("dc_qlookup") == 0)
    {
        if (x > 127)
        {
            std::cout << "\nMax Array Value = 127\n";
            return 0;
        }

        std::cout << "dc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        std::cerr << "dc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("ac_qlookup") == 0)
    {
        if (x > 127)
        {
            std::cout << "\nMax Array Value = 127\n";
            return 0;
        }

        std::cout << "ac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        std::cerr << "ac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("bbb") == 0)
    {
        if (x > 3)
        {
            std::cout << "\nMax Array Value = 3\n";
            return 0;
        }

        std::cout << "bbb[" << x << "] = " << bbb[x] << "\n";
        std::cerr << "bbb[" << x << "] = " << bbb[x] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_kf_default_bmode_counts") == 0)
    {
        if (x > 9)
        {
            std::cout << "\nMax x Array Value = 9\n";
            return 0;
        }

        if (y > 9)
        {
            std::cout << "\nMax y Array Value = 9\n";
            return 0;
        }

        if (z > 9)
        {
            std::cout << "\nMax z Array Value = 9\n";
            return 0;
        }

        std::cout << "vp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        std::cerr << "vp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8d_token_extra_bits2") == 0)
    {
        if (x > 12)
        {
            std::cout << "\nMax x Array Value = 2\n";
            return 0;
        }

        if (y > 2)
        {
            std::cout << "\nMax y Array Value = 9\n";
            return 0;
        }

        if (z > 12)
        {
            std::cout << "\nMax z Array Value = 12\n";
            return 0;
        }

        std::cout << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            std::cout << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            std::cout << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            std::cout << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        std::cerr << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            std::cerr << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            std::cerr << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            std::cerr << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        //              vp8d_token_extra_bits2[0].Probs[1];
        //              vp8d_token_extra_bits2[0].Probs[1];

        StringFound = 1;
    }

    //if(arrayname.compare("nearB")==0)
    //{
    //  if(x > 2)
    //  {
    //      std::cout << "\nMax x Array Value = 2\n";
    //      return 0;
    //  }
    //  if(y > 3)
    //  {
    //      std::cout << "\nMax y Array Value = 3\n";
    //      return 0;
    //  }

    //  std::cout << "nearB[" << x << "].";

    //  if(y == 0)
    //  {
    //      std::cout << "row = " << nearB[x].row << "\n";
    //  }
    //  if(y == 1)
    //  {
    //      std::cout << "col = " << nearB[x].col << "\n";
    //  }
    //  if(y == 2)
    //  {
    //      std::cout << "weight = " << nearB[x].weight << "\n";
    //  }
    //  if(y == 3)
    //  {
    //      std::cout << "block = " << nearB[x].block << "\n";
    //  }

    //  std::cerr << "nearB[" << x << "].";

    //  if(y == 0)
    //  {
    //      std::cerr << "row = " << nearB[x].row << "\n";
    //  }
    //  if(y == 1)
    //  {
    //      std::cerr << "col = " << nearB[x].col << "\n";
    //  }
    //  if(y == 2)
    //  {
    //      std::cerr << "weight = " << nearB[x].weight << "\n";
    //  }
    //  if(y == 3)
    //  {
    //      std::cerr << "block = " << nearB[x].block << "\n";
    //  }

    //  StringFound = 1;
    //}
    if (arrayname.compare("vp8_CoefEncodings") == 0)
    {
        if (x > 11)
        {
            std::cout << "\nMax Array Value = 11\n";
            return 0;
        }

        std::cout << "vp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        std::cout << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        std::cerr << "vp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        std::cerr << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_BmodeEncodings") == 0)
    {
        if (x > 10)
        {
            std::cout << "\nMax Array Value = 10\n";
            return 0;
        }

        std::cout << "vp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        std::cout << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        std::cerr << "vp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        std::cerr << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_MBsplitEncodings") == 0)
    {
        if (x > 4)
        {
            std::cout << "\nMax Array Value = 4\n";
            return 0;
        }

        std::cout << "vp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        std::cout << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        std::cerr << "vp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        std::cerr << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_SmallMVencodings") == 0)
    {
        if (x > 8)
        {
            std::cout << "\nMax Array Value = 8\n";
            return 0;
        }

        std::cout << "vp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        std::cout << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        std::cerr << "vp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        std::cerr << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_MVrefEncodingArray") == 0)
    {
        if (x > 5)
        {
            std::cout << "\nMax Array Value = 5\n";
            return 0;
        }

        std::cout << "vp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        std::cout << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        std::cerr << "vp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        std::cerr << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_subMVrefEncodingArray") == 0)
    {
        if (x > 4)
        {
            std::cout << "\nMax Array Value = 4\n";
            return 0;
        }

        std::cout << "vp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        std::cout << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        std::cerr << "vp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        std::cerr << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_kfYmodeEncodings") == 0)
    {
        if (x > 5)
        {
            std::cout << "\nMax Array Value = 5\n";
            return 0;
        }

        std::cout << "vp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        std::cout << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        std::cerr << "vp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        std::cerr << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        StringFound = 1;
    }

    if (arrayname.compare("vp8_UVmodeEncodings") == 0)
    {
        if (x > 4)
        {
            std::cout << "\nMax Array Value = 4\n";
            return 0;
        }

        std::cout << "vp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        std::cout << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        std::cerr << "vp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        std::cerr << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        StringFound = 1;
    }





    if (StringFound == 0)
    {
        std::cout << "Array: " << arrayname << " Not Found - Valid Selections Include:\n";
        std::cout << " vp8dx_bitreader_norm[128]" << "\n";
        //std::cout << " vp8_mv_cont_count[5][4]" << "\n";
        std::cout << " vp8_default_zig_zag1d[16]" << "\n";
        std::cout << " vp8_coef_bands_x[16]" << "\n";
        //std::cout << " vp8_block2context[25*3]" << "\n";
        std::cout << " vp8_block2left[25]" << "\n";
        std::cout << " vp8_block2above[25]" << "\n";
        std::cout << " vp8_block2type[25]" << "\n";
        std::cout << " vp8_BLOCK2CONTEXT[25]" << "\n";
        std::cout << " vp8_coef_bands[16]" << "\n";
        std::cout << " vp8_prev_token_class[12]" << "\n";
        std::cout << " vp8_default_zig_zag_mask[16]" << "\n";
        std::cout << " vp8_mb_feature_data_bits[2]" << "\n";
        std::cout << " vp8_coef_tree[22]" << "\n";
        std::cout << " Pcat1[1]" << "\n";
        std::cout << " Pcat2[2]" << "\n";
        std::cout << " Pcat3[3]" << "\n";
        std::cout << " Pcat4[4]" << "\n";
        std::cout << " Pcat5[5]" << "\n";
        std::cout << " Pcat6[11]" << "\n";
        std::cout << " cat1[2]" << "\n";
        std::cout << " cat2[4]" << "\n";
        std::cout << " cat3[6]" << "\n";
        std::cout << " cat4[8]" << "\n";
        std::cout << " cat5[10]" << "\n";
        std::cout << " cat6[22]" << "\n";
        std::cout << " kf_y_mode_cts[5]" << "\n";
        std::cout << " y_mode_cts[5]" << "\n";
        std::cout << " uv_mode_cts[4]" << "\n";
        std::cout << " kf_uv_mode_cts[4]" << "\n";
        std::cout << " bmode_cts[10]" << "\n";
        std::cout << " sub_mv_ref_prob [3]" << "\n";
        //std::cout << " vp8_sub_mv_ref_prob2[5][3]" << "\n";
        std::cout << " vp8_mbsplits [4]" << "\n";
        std::cout << " int vp8_mbsplit_count[4]" << "\n";
        std::cout << " VP8_Prob vp8_mbsplit_probs[3]" << "\n";
        std::cout << " vp8_bmode_tree[18]" << "\n";
        std::cout << " vp8_ymode_tree[8]" << "\n";
        std::cout << " vp8_kf_ymode_tree[8]" << "\n";
        std::cout << " vp8_uv_mode_tree[6]" << "\n";
        std::cout << " vp8_mbsplit_tree[6]" << "\n";
        std::cout << " vp8_mv_ref_tree[8]" << "\n";
        std::cout << " vp8_sub_mv_ref_tree[6]" << "\n";
        std::cout << " vp8_small_mvtree [14]" << "\n";
        //std::cout << " vp8_LowMVtree [6]" << "\n";
        //std::cout << " vp8_HighMVtree [8]" << "\n";
        //std::cout << " vp8_Reverse3bits[8]" << "\n";
        //std::cout << " vp8_Reverse4bits[16]" << "\n";
        std::cout << " vp8_mv_update_probs[2]" << "\n";
        std::cout << " vp8_default_mv_context[2]" << "\n";
        std::cout << " bilinear_filters[8][2]" << "\n";
        std::cout << " sub_pel_filters[8][6]" << "\n";
        std::cout << " idctMatrix1[4][4]" << "\n";
        std::cout << " idctMatrix2[4][4]" << "\n";
        std::cout << " vp8_mode_contexts[6][4]" << "\n";
        std::cout << " vp8_kf_default_bmode_counts [10] [10] [10]" << "\n";
        std::cout << " kernel5[5]" << "\n";
        std::cout << " vp8_rv[440]" << "\n";
        std::cout << " dc_qlookup[128]" << "\n";
        std::cout << " ac_qlookup[128]" << "\n";
        std::cout << " bbb[4]" << "\n";
        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////
    //vpxt_time_decompress_ivf_to_ivf(CodeCoverageCompression, CodeCoverageDecCorrect);
    vpxt_dec_compute_md5(CodeCoverageCompression, CodeCoverageDecCorrect);
    //////////////////////////////////////////////////////////////////////////////////////

    if (arrayname.compare("vp8dx_bitreader_norm") == 0)
    {
        std::cout << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        std::cerr << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8dx_bitreader_norm[x];
        vp8dx_bitreader_norm[x] = vp8dx_bitreader_norm[x] + 1;
        std::cout << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        std::cerr << "vp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
    }

    //if(arrayname.compare("vp8_mv_cont_count")==0)
    //{
    //  std::cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //  std::cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //  std::cout << "\n\n";
    //  std::cerr << "\n\n";
    //  StorageInt = vp8_mv_cont_count[x][y];
    //  vp8_mv_cont_count[x][y] = vp8_mv_cont_count[x][y]+1;
    //  std::cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //  std::cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //}
    if (arrayname.compare("vp8_default_zig_zag1d") == 0)
    {
        std::cout << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        std::cerr << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_default_zig_zag1d[x];
        vp8_default_zig_zag1d[x] = vp8_default_zig_zag1d[x]  + 20;
        std::cout << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        std::cerr << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_bands_x") == 0)
    {
        std::cout << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        std::cerr << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_coef_bands_x[x];
        vp8_coef_bands_x[x] = -1;
        //vp8_coef_bands_x[x] = vp8_coef_bands_x[x]  + 20;
        std::cout << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        std::cerr << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
    }

    //if(arrayname.compare("vp8_block2context")==0)
    //{
    //  std::cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //  std::cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //  std::cout << "\n\n";
    //  std::cerr << "\n\n";
    //  StorageInt = vp8_block2context[x];
    //  vp8_block2context[x] = vp8_block2context[x]  + 20;
    //  std::cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //  std::cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //}
    if (arrayname.compare("vp8_block2left") == 0)
    {
        std::cout << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        std::cerr << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        StorageInt = vp8_block2left[x];
        vp8_block2left[x] = vp8_block2left[x]  + 20;
        std::cout << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        std::cerr << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
    }

    if (arrayname.compare("vp8_block2above") == 0)
    {
        std::cout << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        std::cerr << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_block2above[x];
        vp8_block2above[x] = vp8_block2above[x]  + 20;
        std::cout << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        std::cerr << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
    }

    if (arrayname.compare("vp8_block2type") == 0)
    {
        std::cout << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        std::cerr << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_block2type[x];
        vp8_block2type[x] = vp8_block2type[x]  + 20;
        std::cout << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        std::cerr << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
    }

    if (arrayname.compare("vp8_BLOCK2CONTEXT") == 0)
    {
        std::cout << "\nvp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        std::cerr << "\nvp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_BLOCK2CONTEXT[x];
        vp8_BLOCK2CONTEXT[x] = vp8_BLOCK2CONTEXT[x]  + 20;
        std::cout << "\nvp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        std::cerr << "\nvp8_BLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_bands") == 0) //Char
    {
        std::cout << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        std::cerr << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_coef_bands[x];
        vp8_coef_bands[x] = vp8_coef_bands[x]  + 20;
        //unsigned char nine = '9';
        //std::cout << nine << "\n";
        //vp8_coef_bands[x] = nine;
        std::cout << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        std::cerr << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
    }

    if (arrayname.compare("vp8_prev_token_class") == 0) //Char
    {
        std::cout << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        std::cerr << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_prev_token_class[x];
        vp8_prev_token_class[x] = vp8_prev_token_class[x]  + 20;
        //unsigned char nine = '9';
        //std::cout << nine << "\n";
        //vp8_coef_bands[x] = nine;
        std::cout << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        std::cerr << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
    }

    if (arrayname.compare("vp8_default_zig_zag_mask") == 0) //short
    {
        std::cout << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        std::cerr << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageShort = vp8_default_zig_zag_mask[x];
        vp8_default_zig_zag_mask[x] = vp8_default_zig_zag_mask[x]  + 20;
        std::cout << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        std::cerr << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
    }

    if (arrayname.compare("vp8_mb_feature_data_bits") == 0)
    {
        std::cout << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        std::cerr << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_mb_feature_data_bits[x];
        vp8_mb_feature_data_bits[x] = vp8_mb_feature_data_bits[x]  + 20;
        std::cout << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        std::cerr << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_tree") == 0) //Char
    {
        std::cout << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        std::cerr << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_coef_tree[x];
        vp8_coef_tree[x] = vp8_coef_tree[x]  + 20;
        std::cout << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        std::cerr << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
    }

    if (arrayname.compare("Pcat1") == 0) //Char
    {
        std::cout << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        std::cerr << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = Pcat1[x];
        //Pcat1[x] = Pcat1[x]  + 20;
        Pcat1[x] = 2000;
        std::cout << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        std::cerr << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
    }

    if (arrayname.compare("Pcat2") == 0) //Char
    {
        std::cout << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        std::cerr << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = Pcat2[x];
        //Pcat2[x] = Pcat2[x]  + 20;
        Pcat2[x] = -1;
        std::cout << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        std::cerr << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
    }

    if (arrayname.compare("Pcat3") == 0) //Char
    {
        std::cout << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        std::cerr << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = Pcat3[x];
        //Pcat3[x] = Pcat3[x]  + 20;
        Pcat3[x] = -1;
        std::cout << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        std::cerr << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
    }

    if (arrayname.compare("Pcat4") == 0) //Char
    {
        std::cout << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        std::cerr << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = Pcat4[x];
        //Pcat1[x] = Pcat4[x]  + 20;
        Pcat4[x] = -1;
        std::cout << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        std::cerr << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
    }

    if (arrayname.compare("Pcat5") == 0) //Char
    {
        std::cout << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        std::cerr << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = Pcat5[x];
        //Pcat5[x] = Pcat5[x]  + 20;
        Pcat5[x] = -1;
        std::cout << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        std::cerr << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
    }

    if (arrayname.compare("Pcat6") == 0) //Char
    {
        std::cout << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        std::cerr << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = Pcat6[x];
        //Pcat6[x] = Pcat6[x]  + 20;
        Pcat6[x] = -1;
        std::cout << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        std::cerr << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
    }

    if (arrayname.compare("cat1") == 0) //Char
    {
        std::cout << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        std::cerr << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = cat1[x];
        //cat1[x] = cat1[x]  + 20;
        cat1[x] = -1;
        std::cout << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        std::cerr << "\ncat1[" << x << "] = " << cat1[x] << "\n";
    }

    if (arrayname.compare("cat2") == 0) //Char
    {
        std::cout << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        std::cerr << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = cat2[x];
        //cat2[x] = cat2[x]  + 20;
        cat2[x] = -1;
        std::cout << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        std::cerr << "\ncat2[" << x << "] = " << cat2[x] << "\n";
    }

    if (arrayname.compare("cat3") == 0) //Char
    {
        std::cout << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        std::cerr << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = cat3[x];
        //cat3[x] = cat3[x]  + 20;
        cat3[x] = -1;
        std::cout << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        std::cerr << "\ncat3[" << x << "] = " << cat3[x] << "\n";
    }

    if (arrayname.compare("cat4") == 0) //Char
    {
        std::cout << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        std::cerr << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = cat4[x];
        //cat4[x] = cat4[x]  + 20;
        cat4[x] = -1;
        std::cout << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        std::cerr << "\ncat4[" << x << "] = " << cat4[x] << "\n";
    }

    if (arrayname.compare("cat5") == 0) //Char
    {
        std::cout << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        std::cerr << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = cat5[x];
        //cat5[x] = cat5[x]  + 20;
        cat5[x] = -1;
        std::cout << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        std::cerr << "\ncat5[" << x << "] = " << cat5[x] << "\n";
    }

    if (arrayname.compare("cat6") == 0) //Char
    {
        std::cout << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        std::cerr << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = cat6[x];
        //cat6[x] = cat6[x]  + 20;
        cat6[x] = -1;
        std::cout << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        std::cerr << "\ncat6[" << x << "] = " << cat6[x] << "\n";
    }

    if (arrayname.compare("kf_y_mode_cts") == 0)
    {
        std::cout << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        std::cerr << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = kf_y_mode_cts[x];
        kf_y_mode_cts[x] = kf_y_mode_cts[x]  + 20;
        std::cout << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        std::cerr << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
    }

    if (arrayname.compare("y_mode_cts") == 0)
    {
        std::cout << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        std::cerr << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = y_mode_cts[x];
        y_mode_cts[x] = y_mode_cts[x]  + 20;
        std::cout << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        std::cerr << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
    }

    if (arrayname.compare("uv_mode_cts") == 0)
    {
        std::cout << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        std::cerr << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = uv_mode_cts[x];
        uv_mode_cts[x] = uv_mode_cts[x]  + 20;
        std::cout << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        std::cerr << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
    }

    if (arrayname.compare("kf_uv_mode_cts") == 0)
    {
        std::cout << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        std::cerr << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = kf_uv_mode_cts[x];
        kf_uv_mode_cts[x] = kf_uv_mode_cts[x]  + 20;
        std::cout << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        std::cerr << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
    }

    if (arrayname.compare("bmode_cts") == 0)
    {
        std::cout << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        std::cerr << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = bmode_cts[x];
        //bmode_cts[x] = bmode_cts[x]  + 2000;
        bmode_cts[x] = 0;
        std::cout << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        std::cerr << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
    }

    if (arrayname.compare("sub_mv_ref_prob") == 0)
    {
        std::cout << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        std::cerr << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = sub_mv_ref_prob[x];
        sub_mv_ref_prob[x] = sub_mv_ref_prob[x]  + 20;
        std::cout << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        std::cerr << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
    }

    /*if(arrayname.compare("vp8_sub_mv_ref_prob2")==0)
    {
    std::cout << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    std::cerr << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    std::cout << "\n\n";
    std::cerr << "\n\n";
    StorageInt = vp8_sub_mv_ref_prob2[x][y];
    vp8_sub_mv_ref_prob2[x][y] = vp8_sub_mv_ref_prob2[x][y]+1;
    std::cout << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    std::cerr << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    }*/
    if (arrayname.compare("vp8_mbsplits") == 0)
    {
        std::cout << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        std::cerr << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_mbsplits[x][y];
        vp8_mbsplits[x][y] = vp8_mbsplits[x][y] + 1;
        std::cout << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        std::cerr << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_count") == 0)
    {
        std::cout << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        std::cerr << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_mbsplit_count[x];
        vp8_mbsplit_count[x] = vp8_mbsplit_count[x]  + 20;
        std::cout << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        std::cerr << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_probs") == 0) //Char
    {
        std::cout << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        std::cerr << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_mbsplit_probs[x];
        vp8_mbsplit_probs[x] = vp8_mbsplit_probs[x]  + 20;
        std::cout << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        std::cerr << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
    }

    if (arrayname.compare("vp8_bmode_tree") == 0) //Char
    {
        std::cout << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        std::cerr << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_mbsplit_probs[x];
        vp8_mbsplit_probs[x] = vp8_mbsplit_probs[x]  + 20;
        std::cout << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        std::cerr << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_ymode_tree") == 0) //Char
    {
        std::cout << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        std::cerr << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_ymode_tree[x];
        vp8_ymode_tree[x] = vp8_ymode_tree[x]  + 20;
        std::cout << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        std::cerr << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_kf_ymode_tree") == 0) //Char
    {
        std::cout << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        std::cerr << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_kf_ymode_tree[x];
        vp8_kf_ymode_tree[x] = vp8_kf_ymode_tree[x]  + 20;
        std::cout << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        std::cerr << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_uv_mode_tree") == 0) //Char
    {
        std::cout << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        std::cerr << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_uv_mode_tree[x];
        vp8_uv_mode_tree[x] = vp8_uv_mode_tree[x]  + 20;
        std::cout << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        std::cerr << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_tree") == 0) //Char
    {
        std::cout << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        std::cerr << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_mbsplit_tree[x];
        vp8_mbsplit_tree[x] = vp8_mbsplit_tree[x]  + 20;
        std::cout << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        std::cerr << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_mv_ref_tree") == 0) //Char
    {
        std::cout << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        std::cerr << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_mv_ref_tree[x];
        vp8_mv_ref_tree[x] = vp8_mv_ref_tree[x]  + 20;
        std::cout << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        std::cerr << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_sub_mv_ref_tree") == 0) //Char
    {
        std::cout << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        std::cerr << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_sub_mv_ref_tree[x];
        vp8_sub_mv_ref_tree[x] = vp8_sub_mv_ref_tree[x]  + 20;
        std::cout << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        std::cerr << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_small_mvtree") == 0) //Char
    {
        std::cout << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        std::cerr << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_small_mvtree[x];
        vp8_small_mvtree[x] = vp8_small_mvtree[x]  + 20;
        std::cout << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        std::cerr << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
    }

    //if(arrayname.compare("vp8_LowMVtree")==0)//Char
    //{
    //  std::cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //  std::cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //  std::cout << "\n\n";
    //  std::cerr << "\n\n";
    //  StorageChar = vp8_LowMVtree[x];
    //  vp8_LowMVtree[x] = vp8_LowMVtree[x]  + 20;
    //  std::cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //  std::cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //}
    //if(arrayname.compare("vp8_HighMVtree")==0)//Char
    //{
    //  std::cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //  std::cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //  std::cout << "\n\n";
    //  std::cerr << "\n\n";
    //  StorageChar = vp8_HighMVtree[x];
    //  vp8_HighMVtree[x] = vp8_HighMVtree[x]  + 20;
    //  std::cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //  std::cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //}
    //if(arrayname.compare("vp8_Reverse3bits")==0)
    //{
    //  std::cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //  std::cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //  std::cout << "\n\n";
    //  std::cerr << "\n\n";
    //  StorageInt = vp8_Reverse3bits[x];
    //  vp8_Reverse3bits[x] = vp8_Reverse3bits[x]  + 20;
    //  std::cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //  std::cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //}
    //if(arrayname.compare("vp8_Reverse4bits")==0)
    //{
    //  std::cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //  std::cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //  std::cout << "\n\n";
    //  std::cerr << "\n\n";
    //  StorageInt = vp8_Reverse4bits[x];
    //  vp8_Reverse4bits[x] = vp8_Reverse4bits[x]  + 20;
    //  std::cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //  std::cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //}
    if (arrayname.compare("vp8_mv_update_probs") == 0)
    {
        std::cout << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        std::cerr << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_mv_update_probs[x][y];
        vp8_mv_update_probs[x][y] = vp8_mv_update_probs[x][y] + 1;
        std::cout << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        std::cerr << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
    }

    if (arrayname.compare("vp8_default_mv_context") == 0)
    {
        std::cout << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        std::cerr << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageChar = vp8_default_mv_context[x][y];
        vp8_default_mv_context[x][y] = vp8_default_mv_context[x][y] + 1;
        std::cout << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        std::cerr << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
    }

    if (arrayname.compare("bilinear_filters") == 0)
    {
        std::cout << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        std::cerr << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = bilinear_filters[x][y];
        bilinear_filters[x][y] = bilinear_filters[x][y] + 1;
        std::cout << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        std::cerr << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
    }

    if (arrayname.compare("sub_pel_filters") == 0)
    {
        std::cout << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        std::cerr << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageShort = sub_pel_filters[x][y];
        sub_pel_filters[x][y] = sub_pel_filters[x][y] + 1;
        std::cout << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        std::cerr << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
    }

    if (arrayname.compare("vp8_mode_contexts") == 0)
    {
        std::cout << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        std::cerr << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_mode_contexts[x][y];
        vp8_mode_contexts[x][y] = vp8_mode_contexts[x][y] + 1;
        std::cout << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        std::cerr << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
    }

    if (arrayname.compare("kernel5") == 0) //short
    {
        std::cout << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        std::cerr << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageShort = kernel5[x];
        kernel5[x] = kernel5[x]  + 20;
        std::cout << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        std::cerr << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
    }

    if (arrayname.compare("vp8_rv") == 0) //short
    {
        std::cout << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        std::cerr << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageShort = vp8_rv[x];
        vp8_rv[x] = vp8_rv[x]  + 20;
        std::cout << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        std::cerr << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
    }

    if (arrayname.compare("dc_qlookup") == 0)
    {
        std::cout << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        std::cerr << "\nrdc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = dc_qlookup[x];
        dc_qlookup[x] = dc_qlookup[x]  + 2000;
        //dc_qlookup[x] = dc_qlookup[x]  + 20;
        //dc_qlookup[x] = - 100;
        std::cout << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        std::cerr << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
    }

    if (arrayname.compare("ac_qlookup") == 0)
    {
        std::cout << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        std::cerr << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = ac_qlookup[x];
        //ac_qlookup[x] = ac_qlookup[x]  + 2000;
        //ac_qlookup[x] = ac_qlookup[x]  + 20;
        ac_qlookup[x] = -100;
        std::cout << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        std::cerr << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
    }

    if (arrayname.compare("bbb") == 0)
    {
        std::cout << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        std::cerr << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = bbb[x];
        bbb[x] = bbb[x]  + 20;
        std::cout << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        std::cerr << "\nbbb[" << x << "] = " << bbb[x] << "\n";
    }

    if (arrayname.compare("vp8_kf_default_bmode_counts") == 0)
    {
        std::cout << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        std::cerr << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_kf_default_bmode_counts[x][y][z];
        vp8_kf_default_bmode_counts[x][y][z] = vp8_kf_default_bmode_counts[x][y][z] + 20;
        std::cout << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        std::cerr << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
    }

    if (arrayname.compare("vp8d_token_extra_bits2") == 0)
    {
        std::cout << "\n";
        std::cerr << "\n";

        std::cout << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            std::cout << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            std::cout << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            std::cout << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        std::cerr << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            std::cerr << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            std::cerr << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            std::cerr << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        std::cout << "\n\n";
        std::cerr << "\n\n";

        if (y == 0)
        {
            StorageInt = vp8d_token_extra_bits2[x].MinVal;
            vp8d_token_extra_bits2[x].MinVal = vp8d_token_extra_bits2[x].MinVal + 20;
        }

        if (y == 1)
        {
            StorageInt = vp8d_token_extra_bits2[x].Length;
            vp8d_token_extra_bits2[x].Length = vp8d_token_extra_bits2[x].Length + 20;
        }

        if (y == 2)
        {
            StorageInt = vp8d_token_extra_bits2[x].Probs[z];
            vp8d_token_extra_bits2[x].Probs[z] = vp8d_token_extra_bits2[x].Probs[z] + 20;
        }


        std::cout << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            std::cout << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            std::cout << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            std::cout << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        std::cerr << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            std::cerr << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            std::cerr << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            std::cerr << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }
    }

    /*if(arrayname.compare("nearB")==0)
    {
    std::cout << "\n";
    std::cerr << "\n";

    std::cout << "nearB[" << x << "].";

    if(y == 0)
    {
    std::cout << "row = " << nearB[x].row << "\n";
    }
    if(y == 1)
    {
    std::cout << "col = " << nearB[x].col << "\n";
    }
    if(y == 2)
    {
    std::cout << "weight = " << nearB[x].weight << "\n";
    }
    if(y == 3)
    {
    std::cout << "block = " << nearB[x].block << "\n";
    }

    std::cerr << "nearB[" << x << "].";

    if(y == 0)
    {
    std::cerr << "row = " << nearB[x].row << "\n";
    }
    if(y == 1)
    {
    std::cerr << "col = " << nearB[x].col << "\n";
    }
    if(y == 2)
    {
    std::cerr << "weight = " << nearB[x].weight << "\n";
    }
    if(y == 3)
    {
    std::cerr << "block = " << nearB[x].block << "\n";
    }

    std::cout << "\n\n";
    std::cerr << "\n\n";

    if(y == 0)
    {
    StorageInt = nearB[x].row;
    nearB[x].row = nearB[x].row+20;
    }
    if(y == 1)
    {
    StorageInt = nearB[x].col;
    nearB[x].col = nearB[x].col+20;
    }
    if(y == 2)
    {
    StorageInt = nearB[x].weight;
    nearB[x].weight = nearB[x].weight+20;
    }
    if(y == 3)
    {
    StorageInt = nearB[x].block;
    nearB[x].block = nearB[x].block+20;
    }

    std::cout << "nearB[" << x << "].";

    if(y == 0)
    {
    std::cout << "row = " << nearB[x].row << "\n";
    }
    if(y == 1)
    {
    std::cout << "col = " << nearB[x].col << "\n";
    }
    if(y == 2)
    {
    std::cout << "weight = " << nearB[x].weight << "\n";
    }
    if(y == 3)
    {
    std::cout << "block = " << nearB[x].block << "\n";
    }

    std::cerr << "nearB[" << x << "].";

    if(y == 0)
    {
    std::cerr << "row = " << nearB[x].row << "\n";
    }
    if(y == 1)
    {
    std::cerr << "col = " << nearB[x].col << "\n";
    }
    if(y == 2)
    {
    std::cerr << "weight = " << nearB[x].weight << "\n";
    }
    if(y == 3)
    {
    std::cerr << "block = " << nearB[x].block << "\n";
    }
    }*/

    if (arrayname.compare("vp8_CoefEncodings") == 0)
    {
        std::cout << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        std::cout << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        std::cerr << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        std::cerr << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_CoefEncodings[x].value;
        StorageInt2 = vp8_CoefEncodings[x].Len;
        vp8_CoefEncodings[x].value = vp8_CoefEncodings[x].value  + 2000;
        vp8_CoefEncodings[x].Len = vp8_CoefEncodings[x].Len  + 2000;
        std::cout << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        std::cout << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        std::cerr << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        std::cerr << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_BmodeEncodings") == 0)
    {
        std::cout << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        std::cout << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        std::cerr << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_BmodeEncodings[x].value;
        StorageInt2 = vp8_BmodeEncodings[x].Len;
        vp8_BmodeEncodings[x].value = vp8_BmodeEncodings[x].value  + 2000;
        vp8_BmodeEncodings[x].Len = vp8_BmodeEncodings[x].Len  + 2000;
        std::cout << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        std::cout << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        std::cerr << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_MBsplitEncodings") == 0)
    {
        std::cout << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        std::cout << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        std::cerr << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        std::cerr << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_MBsplitEncodings[x].value;
        StorageInt2 = vp8_MBsplitEncodings[x].Len;
        vp8_MBsplitEncodings[x].value = vp8_MBsplitEncodings[x].value  + 2000;
        vp8_MBsplitEncodings[x].Len = vp8_MBsplitEncodings[x].Len  + 2000;
        std::cout << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        std::cout << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        std::cerr << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        std::cerr << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_SmallMVencodings") == 0)
    {
        std::cout << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        std::cout << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        std::cerr << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        std::cerr << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_SmallMVencodings[x].value;
        StorageInt2 = vp8_SmallMVencodings[x].Len;
        vp8_SmallMVencodings[x].value = vp8_SmallMVencodings[x].value  + 2000;
        vp8_SmallMVencodings[x].Len = vp8_SmallMVencodings[x].Len + 2000;
        std::cout << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        std::cout << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        std::cerr << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        std::cerr << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_MVrefEncodingArray") == 0)
    {
        std::cout << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        std::cout << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        std::cerr << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        std::cerr << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_MVrefEncodingArray[x].value;
        StorageInt2 = vp8_MVrefEncodingArray[x].Len;
        vp8_MVrefEncodingArray[x].value = vp8_MVrefEncodingArray[x].value  + 2000;
        vp8_MVrefEncodingArray[x].Len = vp8_MVrefEncodingArray[x].Len  + 2000;
        std::cout << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        std::cout << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        std::cerr << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        std::cerr << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
    }

    if (arrayname.compare("vp8_subMVrefEncodingArray") == 0)
    {
        std::cout << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        std::cout << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        std::cerr << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        std::cerr << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_subMVrefEncodingArray[x].value;
        StorageInt2 = vp8_subMVrefEncodingArray[x].Len;
        vp8_subMVrefEncodingArray[x].value = vp8_subMVrefEncodingArray[x].value  + 2000;
        vp8_subMVrefEncodingArray[x].Len = vp8_subMVrefEncodingArray[x].Len  + 2000;
        std::cout << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        std::cout << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        std::cerr << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        std::cerr << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
    }

    if (arrayname.compare("vp8_kfYmodeEncodings") == 0)
    {
        std::cout << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        std::cout << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        std::cerr << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_kfYmodeEncodings[x].value;
        StorageInt2 = vp8_kfYmodeEncodings[x].Len;
        vp8_kfYmodeEncodings[x].value = vp8_kfYmodeEncodings[x].value  + 2000;
        vp8_kfYmodeEncodings[x].Len = vp8_kfYmodeEncodings[x].Len  + 2000;
        std::cout << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        std::cout << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        std::cerr << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_UVmodeEncodings") == 0)
    {
        std::cout << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        std::cout << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        std::cerr << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        std::cout << "\n\n";
        std::cerr << "\n\n";
        StorageInt = vp8_UVmodeEncodings[x].value;
        StorageInt2 = vp8_UVmodeEncodings[x].Len;
        vp8_UVmodeEncodings[x].value = vp8_UVmodeEncodings[x].value  + 2000;
        vp8_UVmodeEncodings[x].Len = vp8_UVmodeEncodings[x].Len + 2000;
        std::cout << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        std::cout << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        std::cerr << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
    }



    //////////////////////////////////////////////////////////////////////////////////////
    //vpxt_time_decompress_ivf_to_ivf(CodeCoverageCompression, CodeCoverageDecModified);
    vpxt_dec_compute_md5(CodeCoverageCompression, CodeCoverageDecModified);
    //////////////////////////////////////////////////////////////////////////////////////

    if (arrayname.compare("vp8dx_bitreader_norm") == 0)
    {
        std::cout << "\nvp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        std::cerr << "\nvp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        vp8dx_bitreader_norm[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
        std::cerr << "\nvp8dx_bitreader_norm[" << x << "] = " << vp8dx_bitreader_norm[x] << "\n";
    }

    //if(arrayname.compare("vp8_mv_cont_count")==0)
    //{
    //  std::cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //  std::cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //  vp8_mv_cont_count[x][y]=StorageInt;
    //  std::cout << "\nArray Var Reset to:";
    //  std::cerr << "\nArray Var Reset to:";
    //  std::cout << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //  std::cerr << "\nvp8_mv_cont_count[" << x << "][" << y << "] = " << vp8_mv_cont_count[x][y] << "\n";
    //}
    if (arrayname.compare("vp8_default_zig_zag1d") == 0)
    {
        std::cout << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        std::cerr << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        vp8_default_zig_zag1d[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
        std::cerr << "\nvp8_default_zig_zag1d[" << x << "] = " << vp8_default_zig_zag1d[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_bands_x") == 0)
    {
        std::cout << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        std::cerr << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        vp8_coef_bands_x[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
        std::cerr << "\nvp8_coef_bands_x[" << x << "] = " << vp8_coef_bands_x[x] << "\n";
    }

    //if(arrayname.compare("vp8_block2context")==0)
    //{
    //  std::cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //  std::cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //  vp8_block2context[x] = StorageInt;
    //  std::cout << "\nArray Var Reset to:";
    //  std::cerr << "\nArray Var Reset to:";
    //  std::cout << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //  std::cerr << "\nvp8_block2context[" << x << "] = " << vp8_block2context[x] << "\n";
    //}
    if (arrayname.compare("vp8_block2left") == 0)
    {
        std::cout << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        std::cerr << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        vp8_block2left[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
        std::cerr << "\nvp8_block2left[" << x << "] = " << vp8_block2left[x] << "\n";
    }

    if (arrayname.compare("vp8_block2above") == 0)
    {
        std::cout << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        std::cerr << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        vp8_block2above[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
        std::cerr << "\nvp8_block2above[" << x << "] = " << vp8_block2above[x] << "\n";
    }

    if (arrayname.compare("vp8_block2type") == 0)
    {
        std::cout << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        std::cerr << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        vp8_block2type[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
        std::cerr << "\nvp8_block2type[" << x << "] = " << vp8_block2type[x] << "\n";
    }

    if (arrayname.compare("vp8_BLOCK2CONTEXT") == 0)
    {
        std::cout << "\nONYXBLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        std::cerr << "\nONYXBLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        vp8_BLOCK2CONTEXT[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nONYXBLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
        std::cerr << "\nONYXBLOCK2CONTEXT[" << x << "] = " << vp8_BLOCK2CONTEXT[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_bands") == 0) //Char
    {
        std::cout << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        std::cerr << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        vp8_coef_bands[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
        std::cerr << "\nvp8_coef_bands[" << x << "] = " << vp8_coef_bands[x] << "\n";
    }

    if (arrayname.compare("vp8_prev_token_class") == 0) //Char
    {
        std::cout << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        std::cerr << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        vp8_prev_token_class[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
        std::cerr << "\nvp8_prev_token_class[" << x << "] = " << vp8_prev_token_class[x] << "\n";
    }

    if (arrayname.compare("vp8_default_zig_zag_mask") == 0) //short
    {
        std::cout << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        std::cerr << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        vp8_default_zig_zag_mask[x] = StorageShort;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
        std::cerr << "\nvp8_default_zig_zag_mask[" << x << "] = " << vp8_default_zig_zag_mask[x] << "\n";
    }

    if (arrayname.compare("vp8_mb_feature_data_bits") == 0)
    {
        std::cout << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        std::cerr << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        vp8_mb_feature_data_bits[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
        std::cerr << "\nvp8_mb_feature_data_bits[" << x << "] = " << vp8_mb_feature_data_bits[x] << "\n";
    }

    if (arrayname.compare("vp8_coef_tree") == 0) //Char
    {
        std::cout << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        std::cerr << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        vp8_coef_tree[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
        std::cerr << "\nvp8_coef_tree[" << x << "] = " << vp8_coef_tree[x] << "\n";
    }

    if (arrayname.compare("Pcat1") == 0) //Char
    {
        std::cout << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        std::cerr << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        Pcat1[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
        std::cerr << "\nPcat1[" << x << "] = " << Pcat1[x] << "\n";
    }

    if (arrayname.compare("Pcat2") == 0) //Char
    {
        std::cout << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        std::cerr << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        Pcat2[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
        std::cerr << "\nPcat2[" << x << "] = " << Pcat2[x] << "\n";
    }

    if (arrayname.compare("Pcat3") == 0) //Char
    {
        std::cout << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        std::cerr << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        Pcat3[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
        std::cerr << "\nPcat3[" << x << "] = " << Pcat3[x] << "\n";
    }

    if (arrayname.compare("Pcat4") == 0) //Char
    {
        std::cout << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        std::cerr << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        Pcat4[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
        std::cerr << "\nPcat4[" << x << "] = " << Pcat4[x] << "\n";
    }

    if (arrayname.compare("Pcat5") == 0) //Char
    {
        std::cout << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        std::cerr << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        Pcat5[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
        std::cerr << "\nPcat5[" << x << "] = " << Pcat5[x] << "\n";
    }

    if (arrayname.compare("Pcat6") == 0) //Char
    {
        std::cout << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        std::cerr << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        Pcat6[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
        std::cerr << "\nPcat6[" << x << "] = " << Pcat6[x] << "\n";
    }

    if (arrayname.compare("cat1") == 0) //Char
    {
        std::cout << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        std::cerr << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        cat1[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\ncat1[" << x << "] = " << cat1[x] << "\n";
        std::cerr << "\ncat1[" << x << "] = " << cat1[x] << "\n";
    }

    if (arrayname.compare("cat2") == 0) //Char
    {
        std::cout << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        std::cerr << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        cat2[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\ncat2[" << x << "] = " << cat2[x] << "\n";
        std::cerr << "\ncat2[" << x << "] = " << cat2[x] << "\n";
    }

    if (arrayname.compare("cat3") == 0) //Char
    {
        std::cout << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        std::cerr << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        cat3[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\ncat3[" << x << "] = " << cat3[x] << "\n";
        std::cerr << "\ncat3[" << x << "] = " << cat3[x] << "\n";
    }

    if (arrayname.compare("cat4") == 0) //Char
    {
        std::cout << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        std::cerr << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        Pcat4[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\ncat4[" << x << "] = " << cat4[x] << "\n";
        std::cerr << "\ncat4[" << x << "] = " << cat4[x] << "\n";
    }

    if (arrayname.compare("cat5") == 0) //Char
    {
        std::cout << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        std::cerr << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        Pcat5[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\ncat5[" << x << "] = " << cat5[x] << "\n";
        std::cerr << "\ncat5[" << x << "] = " << cat5[x] << "\n";
    }

    if (arrayname.compare("cat6") == 0) //Char
    {
        std::cout << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        std::cerr << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        cat6[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\ncat6[" << x << "] = " << cat6[x] << "\n";
        std::cerr << "\ncat6[" << x << "] = " << cat6[x] << "\n";
    }

    if (arrayname.compare("kf_y_mode_cts") == 0)
    {
        std::cout << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        std::cerr << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        kf_y_mode_cts[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
        std::cerr << "\nkf_y_mode_cts[" << x << "] = " << kf_y_mode_cts[x] << "\n";
    }

    if (arrayname.compare("y_mode_cts") == 0)
    {
        std::cout << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        std::cerr << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        y_mode_cts[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
        std::cerr << "\ny_mode_cts[" << x << "] = " << y_mode_cts[x] << "\n";
    }

    if (arrayname.compare("uv_mode_cts") == 0)
    {
        std::cout << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        std::cerr << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        uv_mode_cts[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
        std::cerr << "\nuv_mode_cts[" << x << "] = " << uv_mode_cts[x] << "\n";
    }

    if (arrayname.compare("kf_uv_mode_cts") == 0)
    {
        std::cout << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        std::cerr << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        kf_uv_mode_cts[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
        std::cerr << "\nkf_uv_mode_cts[" << x << "] = " << kf_uv_mode_cts[x] << "\n";
    }

    if (arrayname.compare("bmode_cts") == 0)
    {
        std::cout << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        std::cerr << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        bmode_cts[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
        std::cerr << "\nbmode_cts[" << x << "] = " << bmode_cts[x] << "\n";
    }

    if (arrayname.compare("sub_mv_ref_prob") == 0)
    {
        std::cout << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        std::cerr << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        sub_mv_ref_prob[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
        std::cerr << "\nsub_mv_ref_prob[" << x << "] = " << sub_mv_ref_prob[x] << "\n";
    }

    //if(arrayname.compare("vp8_sub_mv_ref_prob2")==0)
    //{
    //  std::cout << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    //  std::cerr << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    //  vp8_sub_mv_ref_prob2[x][y]=StorageInt;
    //  std::cout << "\nArray Var Reset to:";
    //  std::cerr << "\nArray Var Reset to:";
    //  std::cout << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    //  std::cerr << "\nvp8_sub_mv_ref_prob2[" << x << "][" << y << "] = " << vp8_sub_mv_ref_prob2[x][y] << "\n";
    //}
    if (arrayname.compare("vp8_mbsplits") == 0)
    {
        std::cout << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        std::cerr << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        vp8_mbsplits[x][y] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
        std::cerr << "\nvp8_mbsplits[" << x << "][" << y << "] = " << vp8_mbsplits[x][y] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_count") == 0)
    {
        std::cout << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        std::cerr << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        vp8_mbsplit_count[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
        std::cerr << "\nvp8_mbsplit_count[" << x << "] = " << vp8_mbsplit_count[x] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_probs") == 0) //Char
    {
        std::cout << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        std::cerr << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        vp8_mbsplit_probs[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
        std::cerr << "\nvp8_mbsplit_probs[" << x << "] = " << vp8_mbsplit_probs[x] << "\n";
    }

    if (arrayname.compare("vp8_bmode_tree") == 0) //Char
    {
        std::cout << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        std::cerr << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        vp8_mbsplit_probs[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
        std::cerr << "\nvp8_bmode_tree[" << x << "] = " << vp8_bmode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_ymode_tree") == 0) //Char
    {
        std::cout << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        std::cerr << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        vp8_ymode_tree[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
        std::cerr << "\nvp8_ymode_tree[" << x << "] = " << vp8_ymode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_kf_ymode_tree") == 0) //Char
    {
        std::cout << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        std::cerr << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        vp8_kf_ymode_tree[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
        std::cerr << "\nvp8_kf_ymode_tree[" << x << "] = " << vp8_kf_ymode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_uv_mode_tree") == 0) //Char
    {
        std::cout << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        std::cerr << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        vp8_uv_mode_tree[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
        std::cerr << "\nvp8_uv_mode_tree[" << x << "] = " << vp8_uv_mode_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_mbsplit_tree") == 0) //Char
    {
        std::cout << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        std::cerr << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        vp8_mbsplit_tree[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
        std::cerr << "\nvp8_mbsplit_tree[" << x << "] = " << vp8_mbsplit_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_mv_ref_tree") == 0) //Char
    {
        std::cout << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        std::cerr << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        vp8_mv_ref_tree[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
        std::cerr << "\nvp8_mv_ref_tree[" << x << "] = " << vp8_mv_ref_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_sub_mv_ref_tree") == 0) //Char
    {
        std::cout << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        std::cerr << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        vp8_sub_mv_ref_tree[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
        std::cerr << "\nvp8_sub_mv_ref_tree[" << x << "] = " << vp8_sub_mv_ref_tree[x] << "\n";
    }

    if (arrayname.compare("vp8_small_mvtree") == 0) //Char
    {
        std::cout << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        std::cerr << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        vp8_small_mvtree[x] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
        std::cerr << "\nvp8_small_mvtree[" << x << "] = " << vp8_small_mvtree[x] << "\n";
    }

    //if(arrayname.compare("vp8_LowMVtree")==0)//Char
    //{
    //  std::cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //  std::cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //  vp8_LowMVtree[x] = StorageChar;
    //  std::cout << "\nArray Var Reset to:";
    //  std::cerr << "\nArray Var Reset to:";
    //  std::cout << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //  std::cerr << "\nvp8_LowMVtree[" << x << "] = " << vp8_LowMVtree[x] << "\n";
    //}
    //if(arrayname.compare("vp8_HighMVtree")==0)//Char
    //{
    //  std::cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //  std::cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //  vp8_HighMVtree[x] = StorageChar;
    //  std::cout << "\nArray Var Reset to:";
    //  std::cerr << "\nArray Var Reset to:";
    //  std::cout << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //  std::cerr << "\nvp8_HighMVtree[" << x << "] = " << vp8_HighMVtree[x] << "\n";
    //}
    //if(arrayname.compare("vp8_Reverse3bits")==0)
    //{
    //  std::cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //  std::cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //  vp8_Reverse3bits[x] = StorageInt;
    //  std::cout << "\nArray Var Reset to:";
    //  std::cerr << "\nArray Var Reset to:";
    //  std::cout << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //  std::cerr << "\nvp8_Reverse3bits[" << x << "] = " << vp8_Reverse3bits[x] << "\n";
    //}
    //if(arrayname.compare("vp8_Reverse4bits")==0)
    //{
    //  std::cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //  std::cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //  vp8_Reverse4bits[x] = StorageInt;
    //  std::cout << "\nArray Var Reset to:";
    //  std::cerr << "\nArray Var Reset to:";
    //  std::cout << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //  std::cerr << "\nvp8_Reverse4bits[" << x << "] = " << vp8_Reverse4bits[x] << "\n";
    //}
    if (arrayname.compare("vp8_mv_update_probs") == 0)
    {
        std::cout << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        std::cerr << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        vp8_mv_update_probs[x][y] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
        std::cerr << "\nvp8_mv_update_probs[" << x << "][" << y << "] = " << vp8_mv_update_probs[x][y] << "\n";
    }

    if (arrayname.compare("vp8_default_mv_context") == 0)
    {
        std::cout << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        std::cerr << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        vp8_default_mv_context[x][y] = StorageChar;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
        std::cerr << "\nvp8_default_mv_context[" << x << "][" << y << "] = " << vp8_default_mv_context[x][y] << "\n";
    }

    if (arrayname.compare("bilinear_filters") == 0)
    {
        std::cout << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        std::cerr << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        bilinear_filters[x][y] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
        std::cerr << "\nbilinear_filters[" << x << "][" << y << "] = " << bilinear_filters[x][y] << "\n";
    }

    if (arrayname.compare("sub_pel_filters") == 0)
    {
        std::cout << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        std::cerr << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        sub_pel_filters[x][y] = StorageShort;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
        std::cerr << "\nsub_pel_filters[" << x << "][" << y << "] = " << sub_pel_filters[x][y] << "\n";
    }

    if (arrayname.compare("vp8_mode_contexts") == 0)
    {
        std::cout << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        std::cerr << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        vp8_mode_contexts[x][y] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
        std::cerr << "\nvp8_mode_contexts[" << x << "][" << y << "] = " << vp8_mode_contexts[x][y] << "\n";
    }

    if (arrayname.compare("kernel5") == 0) //short
    {
        std::cout << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        std::cerr << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        kernel5[x] = StorageShort;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
        std::cerr << "\nkernel5[" << x << "] = " << kernel5[x] << "\n";
    }

    if (arrayname.compare("vp8_rv") == 0) //short
    {
        std::cout << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        std::cerr << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        vp8_rv[x] = StorageShort;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
        std::cerr << "\nvp8_rv[" << x << "] = " << vp8_rv[x] << "\n";
    }

    if (arrayname.compare("dc_qlookup") == 0) //short
    {
        std::cout << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        std::cerr << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        dc_qlookup[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
        std::cerr << "\ndc_qlookup[" << x << "] = " << dc_qlookup[x] << "\n";
    }

    if (arrayname.compare("ac_qlookup") == 0)
    {
        std::cout << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        std::cerr << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        ac_qlookup[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
        std::cerr << "\nac_qlookup[" << x << "] = " << ac_qlookup[x] << "\n";
    }

    if (arrayname.compare("bbb") == 0)
    {
        std::cout << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        std::cerr << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        bbb[x] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nbbb[" << x << "] = " << bbb[x] << "\n";
        std::cerr << "\nbbb[" << x << "] = " << bbb[x] << "\n";
    }

    if (arrayname.compare("vp8_kf_default_bmode_counts") == 0)
    {
        std::cout << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        std::cerr << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        vp8_kf_default_bmode_counts[x][y][z] = StorageInt;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
        std::cerr << "\nvp8_kf_default_bmode_counts[" << x << "][" << y << "][" << z << "] = " << vp8_kf_default_bmode_counts[x][y][z] << "\n";
    }

    if (arrayname.compare("vp8d_token_extra_bits2") == 0)
    {
        std::cout << "\n";
        std::cerr << "\n";

        std::cout << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            std::cout << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            std::cout << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            std::cout << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        std::cerr << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            std::cerr << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            std::cerr << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            std::cerr << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        if (y == 0)
        {
            vp8d_token_extra_bits2[x].MinVal = StorageInt;
        }

        if (y == 1)
        {
            vp8d_token_extra_bits2[x].Length = StorageInt;
        }

        if (y == 2)
        {
            vp8d_token_extra_bits2[x].Probs[z] = StorageInt;
        }

        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";

        std::cout << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            std::cout << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            std::cout << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            std::cout << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }

        std::cerr << "vp8d_token_extra_bits2[" << x << "].";

        if (y == 0)
        {
            std::cerr << "MinVal = " << vp8d_token_extra_bits2[x].MinVal << "\n";
        }

        if (y == 1)
        {
            std::cerr << "Length = " << vp8d_token_extra_bits2[x].Length << "\n";
        }

        if (y == 2)
        {
            std::cerr << "Probs[" << z << "] = " << vp8d_token_extra_bits2[x].Probs[z] << "\n";
        }
    }

    //if(arrayname.compare("nearB")==0)
    //{
    //  std::cout << "\n";
    //  std::cerr << "\n";

    //  std::cout << "nearB[" << x << "].";

    //  if(y == 0)
    //  {
    //      std::cout << "row = " << nearB[x].row << "\n";
    //  }
    //  if(y == 1)
    //  {
    //      std::cout << "col = " << nearB[x].col << "\n";
    //  }
    //  if(y == 2)
    //  {
    //      std::cout << "weight = " << nearB[x].weight << "\n";
    //  }
    //  if(y == 3)
    //  {
    //      std::cout << "block = " << nearB[x].block << "\n";
    //  }

    //  std::cerr << "nearB[" << x << "].";

    //  if(y == 0)
    //  {
    //      std::cerr << "row = " << nearB[x].row << "\n";
    //  }
    //  if(y == 1)
    //  {
    //      std::cerr << "col = " << nearB[x].col << "\n";
    //  }
    //  if(y == 2)
    //  {
    //      std::cerr << "weight = " << nearB[x].weight << "\n";
    //  }
    //  if(y == 3)
    //  {
    //      std::cerr << "block = " << nearB[x].block << "\n";
    //  }

    //  //vp8_kf_default_bmode_counts[x][y][z]=StorageInt;
    //  if(y == 0)
    //  {
    //      nearB[x].row = StorageInt;
    //  }
    //  if(y == 1)
    //  {
    //      nearB[x].col = StorageInt;
    //  }
    //  if(y == 2)
    //  {
    //      nearB[x].weight = StorageInt;
    //  }
    //  if(y == 3)
    //  {
    //      nearB[x].block = StorageInt;
    //  }

    //  std::cout << "\nArray Var Reset to:";
    //  std::cerr << "\nArray Var Reset to:";

    //  std::cout << "nearB[" << x << "].";

    //  if(y == 0)
    //  {
    //      std::cout << "row = " << nearB[x].row << "\n";
    //  }
    //  if(y == 1)
    //  {
    //      std::cout << "col = " << nearB[x].col << "\n";
    //  }
    //  if(y == 2)
    //  {
    //      std::cout << "weight = " << nearB[x].weight << "\n";
    //  }
    //  if(y == 3)
    //  {
    //      std::cout << "block = " << nearB[x].block << "\n";
    //  }

    //  std::cerr << "nearB[" << x << "].";

    //  if(y == 0)
    //  {
    //      std::cerr << "row = " << nearB[x].row << "\n";
    //  }
    //  if(y == 1)
    //  {
    //      std::cerr << "col = " << nearB[x].col << "\n";
    //  }
    //  if(y == 2)
    //  {
    //      std::cerr << "weight = " << nearB[x].weight << "\n";
    //  }
    //  if(y == 3)
    //  {
    //      std::cerr << "block = " << nearB[x].block << "\n";
    //  }
    //}
    if (arrayname.compare("vp8_CoefEncodings") == 0)
    {
        std::cout << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        std::cout << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        std::cerr << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        std::cerr << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        vp8_CoefEncodings[x].value = StorageInt;
        vp8_CoefEncodings[x].Len = StorageInt2;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        std::cout << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
        std::cerr << "\nvp8_CoefEncodings[" << x << "].value = " << vp8_CoefEncodings[x].value << "\n";
        std::cerr << "vp8_CoefEncodings[" << x << "].Len = " << vp8_CoefEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_BmodeEncodings") == 0)
    {
        std::cout << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        std::cout << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        std::cerr << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        vp8_BmodeEncodings[x].value = StorageInt;
        vp8_BmodeEncodings[x].Len = StorageInt2;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        std::cout << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_BmodeEncodings[" << x << "].value = " << vp8_BmodeEncodings[x].value << "\n";
        std::cerr << "vp8_BmodeEncodings[" << x << "].Len = " << vp8_BmodeEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_MBsplitEncodings") == 0)
    {
        std::cout << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        std::cout << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        std::cerr << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        std::cerr << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        vp8_MBsplitEncodings[x].value = StorageInt;
        vp8_MBsplitEncodings[x].Len = StorageInt2;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        std::cout << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
        std::cerr << "\nvp8_MBsplitEncodings[" << x << "].value = " << vp8_MBsplitEncodings[x].value << "\n";
        std::cerr << "vp8_MBsplitEncodings[" << x << "].Len = " << vp8_MBsplitEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_SmallMVencodings") == 0)
    {
        std::cout << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        std::cout << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        std::cerr << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        std::cerr << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        vp8_SmallMVencodings[x].value = StorageInt;
        vp8_SmallMVencodings[x].Len = StorageInt2;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        std::cout << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
        std::cerr << "\nvp8_SmallMVencodings[" << x << "].value = " << vp8_SmallMVencodings[x].value << "\n";
        std::cerr << "vp8_SmallMVencodings[" << x << "].Len = " << vp8_SmallMVencodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_MVrefEncodingArray") == 0)
    {
        std::cout << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        std::cout << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        std::cerr << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        std::cerr << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        vp8_MVrefEncodingArray[x].value = StorageInt;
        vp8_MVrefEncodingArray[x].Len = StorageInt2;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        std::cout << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
        std::cerr << "\nvp8_MVrefEncodingArray[" << x << "].value = " << vp8_MVrefEncodingArray[x].value << "\n";
        std::cerr << "vp8_MVrefEncodingArray[" << x << "].Len = " << vp8_MVrefEncodingArray[x].Len << "\n";
    }

    if (arrayname.compare("vp8_subMVrefEncodingArray") == 0)
    {
        std::cout << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        std::cout << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        std::cerr << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        std::cerr << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        vp8_subMVrefEncodingArray[x].value = StorageInt;
        vp8_subMVrefEncodingArray[x].Len = StorageInt2;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        std::cout << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
        std::cerr << "\nvp8_subMVrefEncodingArray[" << x << "].value = " << vp8_subMVrefEncodingArray[x].value << "\n";
        std::cerr << "vp8_subMVrefEncodingArray[" << x << "].Len = " << vp8_subMVrefEncodingArray[x].Len << "\n";
    }

    if (arrayname.compare("vp8_kfYmodeEncodings") == 0)
    {
        std::cout << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        std::cout << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        std::cerr << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        vp8_kfYmodeEncodings[x].value = StorageInt;
        vp8_kfYmodeEncodings[x].Len = StorageInt2;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        std::cout << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_kfYmodeEncodings[" << x << "].value = " << vp8_kfYmodeEncodings[x].value << "\n";
        std::cerr << "vp8_kfYmodeEncodings[" << x << "].Len = " << vp8_kfYmodeEncodings[x].Len << "\n";
    }

    if (arrayname.compare("vp8_UVmodeEncodings") == 0)
    {
        std::cout << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        std::cout << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        std::cerr << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        vp8_UVmodeEncodings[x].value = StorageInt;
        vp8_UVmodeEncodings[x].Len = StorageInt2;
        std::cout << "\nArray Var Reset to:";
        std::cerr << "\nArray Var Reset to:";
        std::cout << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        std::cout << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
        std::cerr << "\nvp8_UVmodeEncodings[" << x << "].value = " << vp8_UVmodeEncodings[x].value << "\n";
        std::cerr << "vp8_UVmodeEncodings[" << x << "].Len = " << vp8_UVmodeEncodings[x].Len << "\n";
    }


    std::cout << "\n";
    std::cerr << "\n";

    char bufferCorrect[1024];
    std::fstream infileCorrect;
    infileCorrect.open(CodeCoverageDecCorrect);
    infileCorrect.getline(bufferCorrect, 1024);
    infileCorrect.close();

    char bufferModified[1024];
    std::fstream infileModified;
    infileModified.open(CodeCoverageDecModified);
    infileModified.getline(bufferModified, 1024);
    infileModified.close();

    std::string bufferCorrectSTR = bufferCorrect;
    //snprintf(bufferCorrectSTR,1024,"%s",bufferCorrect);

    //if(vpxt_compare_ivf(CodeCoverageDecCorrect,CodeCoverageDecModified) == -1)
    if (bufferCorrectSTR.compare(bufferModified) == 0)
    {
        std::cout << "\nMD5 Checksums Identical for variable number: " << x << " - Fail\n";
        std::cerr << "\nMD5 Checksums Identical for variable number: " << x << " - Fail\n";

        char rootDir[255];
        vpxt_folder_name(argv[0], rootDir);
        std::string CoverageSummaryTextFile = rootDir;
        CoverageSummaryTextFile.append("CoverageSummary.txt");

        char FullName[255];

        snprintf(FullName, 255, "%s", CoverageSummaryTextFile.c_str());

        std::ofstream outfile2(FullName, ios_base::app);

        if (argc == 8)
        {
            outfile2 << "\nMD5 Checksums Identical for: " << arrayname << "[" << x << "] - Fail";
        }

        if (argc == 9)
        {
            outfile2 << "\nMD5 Checksums Identical for: " << arrayname << "[" << x << "][" << y << "] - Fail";
        }

        if (argc == 10)
        {
            outfile2 << "\nMD5 Checksums Identical for: " << arrayname << "[" << x << "][" << y << "][" << z << "] - Fail";
        }

        outfile2.close();
    }
    else
    {
        std::cout << "\nFiles Not Identical - Pass\n";
        std::cerr << "\nFiles Not Identical - Pass\n";
    }

#else

    std::cout << "\nDecoder Array Coverage Tests Are Not Available In This Build.\n";

#endif


    return 0;
}

