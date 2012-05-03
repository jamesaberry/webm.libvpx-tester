#include "vpxt_test_declarations.h"

#define max_test_vector 102

int test_test_vector(int argc,
                     const char** argv,
                     const std::string &working_dir,
                     const std::string sub_folder_str,
                     int test_type,
                     int delete_ivf,
                     int artifact_detection)
{
    char *comp_out_str = "Test Vector Check";
    char *test_dir = "test_test_vector";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    int threads = atoi(argv[2]);
    std::string input = argv[3];

    int cur_test_vector = 1;

    //////////// Formatting Test Specific directory ////////////

    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    std::string test_vector_folder = input;
    std::string test_vector_out_folder = cur_test_dir_str.c_str();

    int test_vector_num = 0;
    std::string test_vector_str_arr[max_test_vector];

    while (test_vector_num < max_test_vector)
    {
        test_vector_str_arr[test_vector_num] = test_vector_folder +
            slashCharStr();
        test_vector_num++;
    }

    test_vector_num = 0;
    std::string test_vector_raw_str_arr[max_test_vector];

    while (test_vector_num < max_test_vector)
    {
        test_vector_raw_str_arr[test_vector_num] = test_vector_out_folder +
            slashCharStr();
        test_vector_num++;
    }

    test_vector_str_arr[1] += "vp80-00-comprehensive-001.ivf";
    test_vector_str_arr[2] += "vp80-00-comprehensive-002.ivf";
    test_vector_str_arr[3] += "vp80-00-comprehensive-003.ivf";
    test_vector_str_arr[4] += "vp80-00-comprehensive-004.ivf";
    test_vector_str_arr[5] += "vp80-00-comprehensive-005.ivf";
    test_vector_str_arr[6] += "vp80-00-comprehensive-006.ivf";
    test_vector_str_arr[7] += "vp80-00-comprehensive-007.ivf";
    test_vector_str_arr[8] += "vp80-00-comprehensive-008.ivf";
    test_vector_str_arr[9] += "vp80-00-comprehensive-009.ivf";
    test_vector_str_arr[10] += "vp80-00-comprehensive-010.ivf";
    test_vector_str_arr[11] += "vp80-00-comprehensive-011.ivf";
    test_vector_str_arr[12] += "vp80-00-comprehensive-012.ivf";
    test_vector_str_arr[13] += "vp80-00-comprehensive-013.ivf";
    test_vector_str_arr[14] += "vp80-00-comprehensive-014.ivf";
    test_vector_str_arr[15] += "vp80-00-comprehensive-015.ivf";
    test_vector_str_arr[16] += "vp80-00-comprehensive-016.ivf";
    test_vector_str_arr[17] += "vp80-00-comprehensive-017.ivf";
    test_vector_str_arr[18] += "vp80-01-intra-1400.ivf";
    test_vector_str_arr[19] += "vp80-01-intra-1411.ivf";
    test_vector_str_arr[20] += "vp80-01-intra-1416.ivf";
    test_vector_str_arr[21] += "vp80-01-intra-1417.ivf";
    test_vector_str_arr[22] += "vp80-02-inter-1402.ivf";
    test_vector_str_arr[23] += "vp80-02-inter-1412.ivf";
    test_vector_str_arr[24] += "vp80-02-inter-1418.ivf";
    test_vector_str_arr[25] += "vp80-02-inter-1424.ivf";
    test_vector_str_arr[26] += "vp80-03-segmentation-1401.ivf";
    test_vector_str_arr[27] += "vp80-03-segmentation-1403.ivf";
    test_vector_str_arr[28] += "vp80-03-segmentation-1407.ivf";
    test_vector_str_arr[29] += "vp80-03-segmentation-1408.ivf";
    test_vector_str_arr[30] += "vp80-03-segmentation-1409.ivf";
    test_vector_str_arr[31] += "vp80-03-segmentation-1410.ivf";
    test_vector_str_arr[32] += "vp80-03-segmentation-1413.ivf";
    test_vector_str_arr[33] += "vp80-03-segmentation-1414.ivf";
    test_vector_str_arr[34] += "vp80-03-segmentation-1415.ivf";
    test_vector_str_arr[35] += "vp80-03-segmentation-1425.ivf";
    test_vector_str_arr[36] += "vp80-03-segmentation-1426.ivf";
    test_vector_str_arr[37] += "vp80-03-segmentation-1427.ivf";
    test_vector_str_arr[38] += "vp80-03-segmentation-1432.ivf";
    test_vector_str_arr[39] += "vp80-03-segmentation-1435.ivf";
    test_vector_str_arr[40] += "vp80-03-segmentation-1436.ivf";
    test_vector_str_arr[41] += "vp80-03-segmentation-1437.ivf";
    test_vector_str_arr[42] += "vp80-03-segmentation-1441.ivf";
    test_vector_str_arr[43] += "vp80-03-segmentation-1442.ivf";
    test_vector_str_arr[44] += "vp80-04-partitions-1404.ivf";
    test_vector_str_arr[45] += "vp80-04-partitions-1405.ivf";
    test_vector_str_arr[46] += "vp80-04-partitions-1406.ivf";
    test_vector_str_arr[47] += "vp80-05-sharpness-1428.ivf";
    test_vector_str_arr[48] += "vp80-05-sharpness-1429.ivf";
    test_vector_str_arr[49] += "vp80-05-sharpness-1430.ivf";
    test_vector_str_arr[50] += "vp80-05-sharpness-1431.ivf";
    test_vector_str_arr[51] += "vp80-05-sharpness-1433.ivf";
    test_vector_str_arr[52] += "vp80-05-sharpness-1434.ivf";
    test_vector_str_arr[53] += "vp80-05-sharpness-1438.ivf";
    test_vector_str_arr[54] += "vp80-05-sharpness-1439.ivf";
    test_vector_str_arr[55] += "vp80-05-sharpness-1440.ivf";
    test_vector_str_arr[56] += "vp80-05-sharpness-1443.ivf";
    test_vector_str_arr[57] += "vp80-06-cropping-001.ivf";
    test_vector_str_arr[58] += "vp80-06-cropping-002.ivf";
    test_vector_str_arr[59] += "vp80-06-cropping-003.ivf";
    test_vector_str_arr[60] += "vp80-06-cropping-004.ivf";
    test_vector_str_arr[61] += "vp80-06-cropping-005.ivf";
    test_vector_str_arr[62] += "vp80-06-cropping-006.ivf";
    test_vector_str_arr[63] += "vp80-06-cropping-007.ivf";
    test_vector_str_arr[64] += "vp80-06-cropping-008.ivf";
    test_vector_str_arr[65] += "vp80-06-cropping-009.ivf";
    test_vector_str_arr[66] += "vp80-06-cropping-010.ivf";
    test_vector_str_arr[67] += "vp80-06-cropping-011.ivf";
    test_vector_str_arr[68] += "vp80-06-cropping-012.ivf";
    test_vector_str_arr[69] += "vp80-06-cropping-013.ivf";
    test_vector_str_arr[70] += "vp80-06-cropping-014.ivf";
    test_vector_str_arr[71] += "vp80-06-cropping-015.ivf";
    test_vector_str_arr[72] += "vp80-06-cropping-016.ivf";
    test_vector_str_arr[73] += "vp80-06-cropping-017.ivf";
    test_vector_str_arr[74] += "vp80-06-cropping-018.ivf";
    test_vector_str_arr[75] += "vp80-06-cropping-019.ivf";
    test_vector_str_arr[76] += "vp80-06-cropping-020.ivf";
    test_vector_str_arr[77] += "vp80-06-cropping-021.ivf";
    test_vector_str_arr[78] += "vp80-06-cropping-022.ivf";
    test_vector_str_arr[79] += "vp80-06-cropping-023.ivf";
    test_vector_str_arr[80] += "vp80-06-cropping-024.ivf";
    test_vector_str_arr[81] += "vp80-06-cropping-025.ivf";
    test_vector_str_arr[82] += "vp80-06-cropping-026.ivf";
    test_vector_str_arr[83] += "vp80-06-cropping-027.ivf";
    test_vector_str_arr[84] += "vp80-06-cropping-028.ivf";
    test_vector_str_arr[85] += "vp80-06-cropping-029.ivf";
    test_vector_str_arr[86] += "vp80-06-cropping-030.ivf";
    test_vector_str_arr[87] += "vp80-06-cropping-031.ivf";
    test_vector_str_arr[88] += "vp80-06-cropping-032.ivf";
    test_vector_str_arr[89] += "vp80-06-cropping-033.ivf";
    test_vector_str_arr[90] += "vp80-06-cropping-034.ivf";
    test_vector_str_arr[91] += "vp80-06-cropping-035.ivf";
    test_vector_str_arr[92] += "vp80-06-cropping-036.ivf";
    test_vector_str_arr[93] += "vp80-06-cropping-037.ivf";
    test_vector_str_arr[94] += "vp80-06-cropping-038.ivf";
    test_vector_str_arr[95] += "vp80-06-cropping-039.ivf";
    test_vector_str_arr[96] += "vp80-06-cropping-040.ivf";
    test_vector_str_arr[97] += "vp80-06-cropping-041.ivf";
    test_vector_str_arr[98] += "vp80-06-cropping-042.ivf";
    test_vector_str_arr[99] += "vp80-06-cropping-043.ivf";
    test_vector_str_arr[100] += "vp80-06-cropping-044.ivf";
    test_vector_str_arr[101] += "vp80-06-cropping-045.ivf";

    test_vector_raw_str_arr[1] += "vp80-00-comprehensive-001.raw";
    test_vector_raw_str_arr[2] += "vp80-00-comprehensive-002.raw";
    test_vector_raw_str_arr[3] += "vp80-00-comprehensive-003.raw";
    test_vector_raw_str_arr[4] += "vp80-00-comprehensive-004.raw";
    test_vector_raw_str_arr[5] += "vp80-00-comprehensive-005.raw";
    test_vector_raw_str_arr[6] += "vp80-00-comprehensive-006.raw";
    test_vector_raw_str_arr[7] += "vp80-00-comprehensive-007.raw";
    test_vector_raw_str_arr[8] += "vp80-00-comprehensive-008.raw";
    test_vector_raw_str_arr[9] += "vp80-00-comprehensive-009.raw";
    test_vector_raw_str_arr[10] += "vp80-00-comprehensive-010.raw";
    test_vector_raw_str_arr[11] += "vp80-00-comprehensive-011.raw";
    test_vector_raw_str_arr[12] += "vp80-00-comprehensive-012.raw";
    test_vector_raw_str_arr[13] += "vp80-00-comprehensive-013.raw";
    test_vector_raw_str_arr[14] += "vp80-00-comprehensive-014.raw";
    test_vector_raw_str_arr[15] += "vp80-00-comprehensive-015.raw";
    test_vector_raw_str_arr[16] += "vp80-00-comprehensive-016.raw";
    test_vector_raw_str_arr[17] += "vp80-00-comprehensive-017.raw";
    test_vector_raw_str_arr[18] += "vp80-01-intra-1400.raw";
    test_vector_raw_str_arr[19] += "vp80-01-intra-1411.raw";
    test_vector_raw_str_arr[20] += "vp80-01-intra-1416.raw";
    test_vector_raw_str_arr[21] += "vp80-01-intra-1417.raw";
    test_vector_raw_str_arr[22] += "vp80-02-inter-1402.raw";
    test_vector_raw_str_arr[23] += "vp80-02-inter-1412.raw";
    test_vector_raw_str_arr[24] += "vp80-02-inter-1418.raw";
    test_vector_raw_str_arr[25] += "vp80-02-inter-1424.raw";
    test_vector_raw_str_arr[26] += "vp80-03-segmentation-1401.raw";
    test_vector_raw_str_arr[27] += "vp80-03-segmentation-1403.raw";
    test_vector_raw_str_arr[28] += "vp80-03-segmentation-1407.raw";
    test_vector_raw_str_arr[29] += "vp80-03-segmentation-1408.raw";
    test_vector_raw_str_arr[30] += "vp80-03-segmentation-1409.raw";
    test_vector_raw_str_arr[31] += "vp80-03-segmentation-1410.raw";
    test_vector_raw_str_arr[32] += "vp80-03-segmentation-1413.raw";
    test_vector_raw_str_arr[33] += "vp80-03-segmentation-1414.raw";
    test_vector_raw_str_arr[34] += "vp80-03-segmentation-1415.raw";
    test_vector_raw_str_arr[35] += "vp80-03-segmentation-1425.raw";
    test_vector_raw_str_arr[36] += "vp80-03-segmentation-1426.raw";
    test_vector_raw_str_arr[37] += "vp80-03-segmentation-1427.raw";
    test_vector_raw_str_arr[38] += "vp80-03-segmentation-1432.raw";
    test_vector_raw_str_arr[39] += "vp80-03-segmentation-1435.raw";
    test_vector_raw_str_arr[40] += "vp80-03-segmentation-1436.raw";
    test_vector_raw_str_arr[41] += "vp80-03-segmentation-1437.raw";
    test_vector_raw_str_arr[42] += "vp80-03-segmentation-1441.raw";
    test_vector_raw_str_arr[43] += "vp80-03-segmentation-1442.raw";
    test_vector_raw_str_arr[44] += "vp80-04-partitions-1404.raw";
    test_vector_raw_str_arr[45] += "vp80-04-partitions-1405.raw";
    test_vector_raw_str_arr[46] += "vp80-04-partitions-1406.raw";
    test_vector_raw_str_arr[47] += "vp80-05-sharpness-1428.raw";
    test_vector_raw_str_arr[48] += "vp80-05-sharpness-1429.raw";
    test_vector_raw_str_arr[49] += "vp80-05-sharpness-1430.raw";
    test_vector_raw_str_arr[50] += "vp80-05-sharpness-1431.raw";
    test_vector_raw_str_arr[51] += "vp80-05-sharpness-1433.raw";
    test_vector_raw_str_arr[52] += "vp80-05-sharpness-1434.raw";
    test_vector_raw_str_arr[53] += "vp80-05-sharpness-1438.raw";
    test_vector_raw_str_arr[54] += "vp80-05-sharpness-1439.raw";
    test_vector_raw_str_arr[55] += "vp80-05-sharpness-1440.raw";
    test_vector_raw_str_arr[56] += "vp80-05-sharpness-1443.raw";
    test_vector_raw_str_arr[57] += "vp80-06-cropping-001.raw";
    test_vector_raw_str_arr[58] += "vp80-06-cropping-002.raw";
    test_vector_raw_str_arr[59] += "vp80-06-cropping-003.raw";
    test_vector_raw_str_arr[60] += "vp80-06-cropping-004.raw";
    test_vector_raw_str_arr[61] += "vp80-06-cropping-005.raw";
    test_vector_raw_str_arr[62] += "vp80-06-cropping-006.raw";
    test_vector_raw_str_arr[63] += "vp80-06-cropping-007.raw";
    test_vector_raw_str_arr[64] += "vp80-06-cropping-008.raw";
    test_vector_raw_str_arr[65] += "vp80-06-cropping-009.raw";
    test_vector_raw_str_arr[66] += "vp80-06-cropping-010.raw";
    test_vector_raw_str_arr[67] += "vp80-06-cropping-011.raw";
    test_vector_raw_str_arr[68] += "vp80-06-cropping-012.raw";
    test_vector_raw_str_arr[69] += "vp80-06-cropping-013.raw";
    test_vector_raw_str_arr[70] += "vp80-06-cropping-014.raw";
    test_vector_raw_str_arr[71] += "vp80-06-cropping-015.raw";
    test_vector_raw_str_arr[72] += "vp80-06-cropping-016.raw";
    test_vector_raw_str_arr[73] += "vp80-06-cropping-017.raw";
    test_vector_raw_str_arr[74] += "vp80-06-cropping-018.raw";
    test_vector_raw_str_arr[75] += "vp80-06-cropping-019.raw";
    test_vector_raw_str_arr[76] += "vp80-06-cropping-020.raw";
    test_vector_raw_str_arr[77] += "vp80-06-cropping-021.raw";
    test_vector_raw_str_arr[78] += "vp80-06-cropping-022.raw";
    test_vector_raw_str_arr[79] += "vp80-06-cropping-023.raw";
    test_vector_raw_str_arr[80] += "vp80-06-cropping-024.raw";
    test_vector_raw_str_arr[81] += "vp80-06-cropping-025.raw";
    test_vector_raw_str_arr[82] += "vp80-06-cropping-026.raw";
    test_vector_raw_str_arr[83] += "vp80-06-cropping-027.raw";
    test_vector_raw_str_arr[84] += "vp80-06-cropping-028.raw";
    test_vector_raw_str_arr[85] += "vp80-06-cropping-029.raw";
    test_vector_raw_str_arr[86] += "vp80-06-cropping-030.raw";
    test_vector_raw_str_arr[87] += "vp80-06-cropping-031.raw";
    test_vector_raw_str_arr[88] += "vp80-06-cropping-032.raw";
    test_vector_raw_str_arr[89] += "vp80-06-cropping-033.raw";
    test_vector_raw_str_arr[90] += "vp80-06-cropping-034.raw";
    test_vector_raw_str_arr[91] += "vp80-06-cropping-035.raw";
    test_vector_raw_str_arr[92] += "vp80-06-cropping-036.raw";
    test_vector_raw_str_arr[93] += "vp80-06-cropping-037.raw";
    test_vector_raw_str_arr[94] += "vp80-06-cropping-038.raw";
    test_vector_raw_str_arr[95] += "vp80-06-cropping-039.raw";
    test_vector_raw_str_arr[96] += "vp80-06-cropping-040.raw";
    test_vector_raw_str_arr[97] += "vp80-06-cropping-041.raw";
    test_vector_raw_str_arr[98] += "vp80-06-cropping-042.raw";
    test_vector_raw_str_arr[99] += "vp80-06-cropping-043.raw";
    test_vector_raw_str_arr[100] += "vp80-06-cropping-044.raw";
    test_vector_raw_str_arr[101] += "vp80-06-cropping-045.raw";

    test_vector_num = 0;
    std::string test_vector_txt[max_test_vector];

    while (test_vector_num < max_test_vector)
    {
        vpxt_remove_file_extension(
            test_vector_raw_str_arr[test_vector_num].c_str(),
            test_vector_txt[test_vector_num]);
        test_vector_txt[test_vector_num] += "MD5.txt";
        test_vector_num++;
    }

    std::string md5_key_str_arr[101];

    // New Test Vectors 2.0.0 MD5 Checksums
    /*vp80-00-comprehensive-001.raw*/
    md5_key_str_arr[0].assign("fad126074e1bd5363d43b9d1cadddb71");
    /*vp80-00-comprehensive-002.raw*/
    md5_key_str_arr[1].assign("182f03dd264ebac04e24c7c9499d7cdb");
    /*vp80-00-comprehensive-003.raw*/
    md5_key_str_arr[2].assign("e5fe668b033900022c3eb0ba76a44bd1");
    /*vp80-00-comprehensive-004.raw*/
    md5_key_str_arr[3].assign("95097ce9808c1d47e03f99c48ad111ec");
    /*vp80-00-comprehensive-005.raw*/
    md5_key_str_arr[4].assign("0f469e4fd1dea533e5580688b2d242ff");
    /*vp80-00-comprehensive-006.raw*/
    md5_key_str_arr[5].assign("2d5fa3ec2f88404ae7b305c1074036f4");
    /*vp80-00-comprehensive-007.raw*/
    md5_key_str_arr[6].assign("92526913d89b6a9b00f2d602def08bce");
    /*vp80-00-comprehensive-008.raw*/
    md5_key_str_arr[7].assign("bd4d46a9d14fe5a7fc9cfc8deac2d34c");
    /*vp80-00-comprehensive-009.raw*/
    md5_key_str_arr[8].assign("19201a2d535bd82f41c1a5658def5379");
    /*vp80-00-comprehensive-010.raw*/
    md5_key_str_arr[9].assign("61d05919a9883d9f215eb3f2db63eb13");
    /*vp80-00-comprehensive-011.raw*/
    md5_key_str_arr[10].assign("1a0afe5e70512a03323a8f1176bcf022");
    /*vp80-00-comprehensive-012.raw*/
    md5_key_str_arr[11].assign("4ea997c80dc2087e6deec81f1ecf6668");
    /*vp80-00-comprehensive-013.raw*/
    md5_key_str_arr[12].assign("93169305d3054327be3cc074f0773a75");
    /*vp80-00-comprehensive-014.raw*/
    md5_key_str_arr[13].assign("7280a64c51dfa557c1b9552dc1e1fbed");
    /*vp80-00-comprehensive-015.raw*/
    md5_key_str_arr[14].assign("23b9cc582e344726e76cda092b416bcf");
    /*vp80-00-comprehensive-016.raw*/
    md5_key_str_arr[15].assign("55e889d22f99718cf6936d55f8ade12b");
    /*vp80-00-comprehensive-017.raw*/
    md5_key_str_arr[16].assign("95a68ffb228d1d8c6ee54f16a10fb9eb");
    /*vp80-01-intra-1400.raw*/
    md5_key_str_arr[17].assign("53b08ac91398a5dd948434e41b31b47e");
    /*vp80-01-intra-1411.raw*/
    md5_key_str_arr[18].assign("8fa1762329e65c97245393a933cd0f00");
     /*vp80-01-intra-1416.raw*/
    md5_key_str_arr[19].assign("cffd1299fa7a0330264cb411d9482bb0");
    /*vp80-01-intra-1417.raw*/
    md5_key_str_arr[20].assign("0e6c13a78a203d95fe12d206a432f642");
    /*vp80-02-inter-1402.raw*/
    md5_key_str_arr[21].assign("184ee9c5cd6e32f2fe7b2f5a463d37b3");
    /*vp80-02-inter-1412.raw*/
    md5_key_str_arr[22].assign("6928dc8e7886914b0ba5825518e54bd7");
    /*vp80-02-inter-1418.raw*/
    md5_key_str_arr[23].assign("45302aa645ff5c139fe580ac30482245");
    /*vp80-02-inter-1424.raw*/
    md5_key_str_arr[24].assign("4816cb607488b930ceadeb2cdb034c49");
    /*vp80-03-segmentation-1401.raw*/
    md5_key_str_arr[25].assign("f7acb74e99528568714129e2994ceca5");
    /*vp80-03-segmentation-1403.raw*/
    md5_key_str_arr[26].assign("4e651f545a21863e97547185f93dbd7b");
    /*vp80-03-segmentation-1407.raw*/
    md5_key_str_arr[27].assign("fa76612d673cbfcb8ec58eda08400388");
    /*vp80-03-segmentation-1408.raw*/
    md5_key_str_arr[28].assign("886f15167bbdd5ea6c099e8b74452c7c");
    /*vp80-03-segmentation-1409.raw*/
    md5_key_str_arr[29].assign("780cc4d060eecec04c2746c98065ec6f");
    /*vp80-03-segmentation-1410.raw*/
    md5_key_str_arr[30].assign("f3468778cd11642f095b4e5dcb19fbda");
    /*vp80-03-segmentation-1413.raw*/
    md5_key_str_arr[31].assign("6a0564ccc1a655d929390a72ff558db9");
    /*vp80-03-segmentation-1414.raw*/
    md5_key_str_arr[32].assign("0f887b4bc1bb0aae670c50c9b7f0142f");
    /*vp80-03-segmentation-1415.raw*/
    md5_key_str_arr[33].assign("8b83e0a3ca0da9e8d7f47a06dc08e18b");
    /*vp80-03-segmentation-1425.raw*/
    md5_key_str_arr[34].assign("96ffacf0c3eae59b58252be24a60e9b2");
    /*vp80-03-segmentation-1426.raw*/
    md5_key_str_arr[35].assign("ab1062e4e45e6802d80313da52028af2");
    /*vp80-03-segmentation-1427.raw*/
    md5_key_str_arr[36].assign("761c3d8e23314516592a1f876865c22a");
    /*vp80-03-segmentation-1432.raw*/
    md5_key_str_arr[37].assign("c5a7776fdfe8908fcc64e58c317c8cf3");
    /*vp80-03-segmentation-1435.raw*/
    md5_key_str_arr[38].assign("36a77df963d0d8c3bc098827be403bdb");
    /*vp80-03-segmentation-1436.raw*/
    md5_key_str_arr[39].assign("bfd17a557ee1ba347c755a18ce5a64a6");
    /*vp80-03-segmentation-1437.raw*/
    md5_key_str_arr[40].assign("876e7c782ee4dd23866498b794856fd1");
    /*vp80-03-segmentation-1441.raw*/
    md5_key_str_arr[41].assign("d7a1e99d0ec80ac2b95cc7277bf4db3c");
    /*vp80-03-segmentation-1442.raw*/
    md5_key_str_arr[42].assign("1a23409897f51ad2920d5ddb999eac71");
    /*vp80-04-partitions-1404.raw*/
    md5_key_str_arr[43].assign("27837df047de5b5ae2dc8f2e9d318cb3");
    /*vp80-04-partitions-1405.raw*/
    md5_key_str_arr[44].assign("12fb1d187ee70738265d8f3a0a70ef26");
    /*vp80-04-partitions-1406.raw*/
    md5_key_str_arr[45].assign("2da53f93a051dcb8290f884a55272dd9");
    /*vp80-05-sharpness-1428.raw*/
    md5_key_str_arr[46].assign("14b537dae40c6013079fd3d25cb16e7a");
    /*vp80-05-sharpness-1429.raw*/
    md5_key_str_arr[47].assign("e836423126f8a7de2c6c9777e0a84214");
    /*vp80-05-sharpness-1430.raw*/
    md5_key_str_arr[48].assign("51503f7a786032d2cbed84ed11430ff3");
    /*vp80-05-sharpness-1431.raw*/
    md5_key_str_arr[49].assign("7f7f534b2d6e28002e119ed269c8f282");
    /*vp80-05-sharpness-1433.raw*/
    md5_key_str_arr[50].assign("a2ff07ccbb019f48e020507ca5f5ee90");
    /*vp80-05-sharpness-1434.raw*/
    md5_key_str_arr[51].assign("110a65e1729fc54e0a25dbf9cc367ccf");
    /*vp80-05-sharpness-1438.raw*/
    md5_key_str_arr[52].assign("cc468ac5ce042c85f04d62a8e09c97ff");
    /*vp80-05-sharpness-1439.raw*/
    md5_key_str_arr[53].assign("ebfc41ade751e96220e74491bffda736");
    /*vp80-05-sharpness-1440.raw*/
    md5_key_str_arr[54].assign("903a267bcc69ad5f8d886db6478d997a");
    /*vp80-05-sharpness-1443.raw*/
    md5_key_str_arr[55].assign("3c5c3c66cad414d6b79de77e977f115b");
    /*vp80-06-cropping-001.raw*/
    md5_key_str_arr[56].assign("9f8e73e634493ab3e56aad63321f8e11");
    /*vp80-06-cropping-002.raw*/
    md5_key_str_arr[57].assign("aef0cfaf282d07cd7ac360fb4f7a1f6b");
    /*vp80-06-cropping-003.raw*/
    md5_key_str_arr[58].assign("609e845f662e3455daf57c68b859a4f6");
    /*vp80-06-cropping-004.raw*/
    md5_key_str_arr[59].assign("c37c1c11bf3f5adcfa62a39b35cf3d07");
    /*vp80-06-cropping-005.raw*/
    md5_key_str_arr[60].assign("82641ff61fe1d6bb7ba8accfa5b399ad");
    /*vp80-06-cropping-006.raw*/
    md5_key_str_arr[61].assign("b8fa425191154685444164157bfbf53d");
    /*vp80-06-cropping-007.raw*/
    md5_key_str_arr[62].assign("ee19c6b8947c3521b810b66ae080368a");
    /*vp80-06-cropping-008.raw*/
    md5_key_str_arr[63].assign("1d99ee17a5ba002dbb185a00ea517427");
    /*vp80-06-cropping-009.raw*/
    md5_key_str_arr[64].assign("017e2e8066397e6f4fdf9522cc2d3428");
    /*vp80-06-cropping-010.raw*/
    md5_key_str_arr[65].assign("2e66d33de5a4550b168524b569799858");
    /*vp80-06-cropping-011.raw*/
    md5_key_str_arr[66].assign("3afdb776a6260ab3a3722d5ed0a51ea6");
    /*vp80-06-cropping-012.raw*/
    md5_key_str_arr[67].assign("ee370c45dd534c994e83454962f05e4a");
    /*vp80-06-cropping-013.raw*/
    md5_key_str_arr[68].assign("de95d19355f402c2ce9a75562ef45a88");
    /*vp80-06-cropping-014.raw*/
    md5_key_str_arr[69].assign("79d1c72208f9a40a770a065b48d7c9b6");
    /*vp80-06-cropping-015.raw*/
    md5_key_str_arr[70].assign("eac91c3f679b57fb0d41965a58515c58");
    /*vp80-06-cropping-016.raw*/
    md5_key_str_arr[71].assign("1ade1e1d87d045a3df99c1c4431c3770");
    /*vp80-06-cropping-017.raw*/
    md5_key_str_arr[72].assign("a92acb085407d9f38d6b86a25dcd31b6");
    /*vp80-06-cropping-018.raw*/
    md5_key_str_arr[73].assign("633ab126a1f74babedef090b7a9f79f3");
    /*vp80-06-cropping-019.raw*/
    md5_key_str_arr[74].assign("336b86b25f320eb63eb0476ab84229cd");
    /*vp80-06-cropping-020.raw*/
    md5_key_str_arr[75].assign("d4bf42f6671d06dbda405c105984f4a6");
    /*vp80-06-cropping-021.raw*/
    md5_key_str_arr[76].assign("a857394e5ad0b52ffcffd61d0be108ed");
    /*vp80-06-cropping-022.raw*/
    md5_key_str_arr[77].assign("2f985c573f020c5bac55d48fa5e24489");
    /*vp80-06-cropping-023.raw*/
    md5_key_str_arr[78].assign("8a56b8c71b671503018727917fcdb145");
    /*vp80-06-cropping-024.raw*/
    md5_key_str_arr[79].assign("2369693e71dcb565327b49c31fdc513e");
    /*vp80-06-cropping-025.raw*/
    md5_key_str_arr[80].assign("9be67b80d50f157bbae891e2a465909e");
    /*vp80-06-cropping-026.raw*/
    md5_key_str_arr[81].assign("20ed8b808ad8f3be05a9c80e0328f0e5");
    /*vp80-06-cropping-027.raw*/
    md5_key_str_arr[82].assign("d4178440ab559d89df2d036c6606cda6");
    /*vp80-06-cropping-028.raw*/
    md5_key_str_arr[83].assign("c65520731d9caae321de26fa41e77bf2");
    /*vp80-06-cropping-029.raw*/
    md5_key_str_arr[84].assign("097c3f56e8917306fa5f1a60e63c95b9");
    /*vp80-06-cropping-030.raw*/
    md5_key_str_arr[85].assign("f52c5fc26a9439aa8226d533bcdada98");
    /*vp80-06-cropping-031.raw*/
    md5_key_str_arr[86].assign("5df73922d1ed6680e4d2ac35d86b061a");
    /*vp80-06-cropping-032.raw*/
    md5_key_str_arr[87].assign("9d9a31179bfb0daf3a884625e7f1c116");
    /*vp80-06-cropping-033.raw*/
    md5_key_str_arr[88].assign("bbafe2fdf7b927ff0b403f91646ed80b");
    /*vp80-06-cropping-034.raw*/
    md5_key_str_arr[89].assign("b2d27a6f08e37e1a26bfd31e229684d8");
    /*vp80-06-cropping-035.raw*/
    md5_key_str_arr[90].assign("dd5138a4831475284d285a6e821387d6");
    /*vp80-06-cropping-036.raw*/
    md5_key_str_arr[91].assign("8fd17178a7d163a8eb9b439d9fa42797");
    /*vp80-06-cropping-037.raw*/
    md5_key_str_arr[92].assign("0e7960d1d71e1aa5a97d22f152c10a8f");
    /*vp80-06-cropping-038.raw*/
    md5_key_str_arr[93].assign("84b6553703153de46aefb09bc479bca5");
    /*vp80-06-cropping-039.raw*/
    md5_key_str_arr[94].assign("971f42513a93493c4c7bce8177a07da4");
    /*vp80-06-cropping-040.raw*/
    md5_key_str_arr[95].assign("98e3e9253baa99f5bf40316eb54d9e36");
    /*vp80-06-cropping-041.raw*/
    md5_key_str_arr[96].assign("1e57bb03f8872d07d64a3177d2a31ceb");
    /*vp80-06-cropping-042.raw*/
    md5_key_str_arr[97].assign("1a2360cc18b06016777fb9614ac4de61");
    /*vp80-06-cropping-043.raw*/
    md5_key_str_arr[98].assign("18924afc1f5acb60d3933184ff1f93c8");
    /*vp80-06-cropping-044.raw*/
    md5_key_str_arr[99].assign("62f5de3feb4346072c94d643d6946fb4");
    /*vp80-06-cropping-045.raw*/
    md5_key_str_arr[100].assign("8319b0f71a76787f894f2927b7923897");

    int print_key_track = 0;
    // if = 1 will delete decompressed tesetvectors if 0 will not
    int del_tv = 0;

    ///////////// OutPutfile ////////////
    std::string text_file_str = cur_test_dir_str + slashCharStr() + test_dir;

    if (test_type == kCompOnly || test_type == kFullTest)
        text_file_str += ".txt";
    else
        text_file_str += "_TestOnly.txt";


    FILE *fp;

    if ((fp = freopen(text_file_str.c_str(), "w", stderr)) == NULL)
    {
        tprintf(PRINT_STD, "Cannot open out put file: %s\n",
            text_file_str.c_str());
        exit(1);
    }

    ////////////////////////////////
    //////////////////////////////////////////////////////////

    if (test_type == kFullTest)
        print_header_full_test(argc, argv, main_test_dir_char);

    if (test_type == kCompOnly)
        print_header_compression_only(argc, argv, main_test_dir_char);

    if (test_type == kTestOnly)
        print_header_test_only(argc, argv, cur_test_dir_str);

    vpxt_cap_string_print(PRINT_BTH, "%s", test_dir);

    if (test_type == kTestOnly)
    {
        // This test requires no preperation before a Test Only Run
    }
    else
    {
        cur_test_vector = 1;
        while (cur_test_vector < max_test_vector)
        {
            tprintf(PRINT_BTH, "\n");

            if (vpxt_decompress_to_raw(
                test_vector_str_arr[cur_test_vector].c_str(),
                test_vector_raw_str_arr[cur_test_vector].c_str(), threads)
                == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }

            cur_test_vector++;
        }
    }

    std::vector<int> fail_vector;

    if (test_type == kCompOnly)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    // Compute MD5 CheckSums
    cur_test_vector = 1;

    while (cur_test_vector < max_test_vector)
    {
        tprintf(PRINT_STD, "\n\nComputing MD5 for Test Vector %i",
            cur_test_vector);
        tprintf(PRINT_ERR, "\n\nComputing MD5 for Test Vector %i\n",
            cur_test_vector);
        vpxt_dec_compute_md5(test_vector_str_arr[cur_test_vector].c_str(),
            test_vector_txt[cur_test_vector].c_str());
        cur_test_vector++;
    }

    if (del_tv == 1)
    {
        cur_test_vector = 1;

        while (cur_test_vector < max_test_vector)
        {
            if (remove(test_vector_raw_str_arr[cur_test_vector].c_str()) == 0)
            {
                /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
            }
            else
            {
                tprintf(PRINT_STD, "Error: %s Not Deleted\n\n",
                    test_vector_raw_str_arr[cur_test_vector].c_str());
            }

            cur_test_vector++;
        }
    }

    int x = 0;
    int test_state = kTestPassed;

    cur_test_vector = 1;

    while (cur_test_vector < max_test_vector)
    {
        char md5_input_file[255];
        char test_vect_file_name[255];
        snprintf(md5_input_file, 255, "%s",
            test_vector_txt[cur_test_vector].c_str());
        vpxt_file_name(test_vector_str_arr[cur_test_vector].c_str(),
            test_vect_file_name, 1);

        char buffer[1024];
        std::fstream infile;
        infile.open(md5_input_file);
        infile.getline(buffer, 1024);
        infile.close();

        tprintf(PRINT_BTH, "\n\n");

        if (md5_key_str_arr[x].compare(buffer) == 0)
        {
            tprintf(PRINT_BTH, " VP8 Test Vector %i: %s \n"
                    " Observed: %s \n"
                    " Expected: %s \n"
                    " -Pass \n\n"
                    , x + 1, test_vect_file_name, buffer,
                    md5_key_str_arr[x].c_str());
        }
        else
        {
            tprintf(PRINT_BTH, " VP8 Test Vector %i: %s \n"
                    " Observed: %s \n"
                    " Expected: %s \n"
                    " -Fail \n\n"
                    , x + 1, test_vect_file_name, buffer,
                    md5_key_str_arr[x].c_str());

            fail_vector.push_back(x + 1);
            test_state = kTestFailed;
        }

        cur_test_vector++;
        x++;
    }

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    if (test_state == kTestPassed)
    {
        vpxt_formated_print(RESPRT, "All decoded test vector MD5 checksum's "
            "match expected checksum's - Passed");
    }
    else
    {
        std::string fail_str = "Not all decoded test vectors match expected "
            "MD5 checksum's.";

        unsigned int q = 0;

        if (fail_vector.size() == 1)
            fail_str += " Test Vector ";
        else
            fail_str += " Test Vectors ";

        while (q < fail_vector.size())
        {
            if (q != 0)
            {
                if (q + 1 != fail_vector.size())
                    fail_str += ", ";
                else
                    fail_str += " and ";
            }

            char tv_fail_num_char[32] = "";
            vpxt_itoa_custom(fail_vector[q], tv_fail_num_char, 10);
            fail_str += tv_fail_num_char;
            q++;
        }

        if (fail_vector.size() == 1)
            fail_str += " does not match";
        else
            fail_str += " do not match";

        fail_str += " - Failed";
        vpxt_formated_print(RESPRT, fail_str.c_str());
    }

    if(test_state == kTestPassed)
        tprintf(PRINT_BTH, "\n\nPassed\n");
    if(test_state == kTestFailed)
        tprintf(PRINT_BTH, "\n\nFailed\n");

    if (delete_ivf)
        for (int z = 1; z < max_test_vector; ++z)
            vpxt_delete_files(1, test_vector_raw_str_arr[z].c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}