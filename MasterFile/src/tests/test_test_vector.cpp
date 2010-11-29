#include "vpxt_test_declarations.h"

int test_vector_test(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "Test Vector Check";
    char *MyDir = "test_test_vector";

    if (!(argc == 3))
    {
        vpxt_cap_string_print(PRINT_STD, "  %s", MyDir);
        printf(
            "\n\n"
            "    <Input Directory>\n"
            "\n"
        );
        return 0;
    }

    char *input = argv[2];

    int CurTestVector = 1;
    int LastTestVector = 102;

    ////////////Formatting Test Specific Directory////////////

    string CurTestDirStr = ""; // <- All Options need to set a value for this
    string FileIndexStr = "";
    char MainTestDirChar[255] = "";
    char FileIndexOutputChar[255] = "";

    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, CurTestDirStr, FileIndexStr, MainTestDirChar, FileIndexOutputChar, FilesAr) == 11)
        return 11;

    string TestVectorFolder = argv[2];
    string TestVectorOutFolder = CurTestDirStr.c_str();

    int TestVectorNum = 0;
    string TestVector[102];

    while (TestVectorNum < 102)
    {
        TestVector[TestVectorNum] = TestVectorFolder;
        TestVectorNum++;
    }

    TestVectorNum = 0;
    string TestVector_Raw[102];

    while (TestVectorNum < 102)
    {
        TestVector_Raw[TestVectorNum] = (TestVectorOutFolder.c_str());
        TestVectorNum++;
    }

    TestVector[1].append(slashCharStr());
    TestVector[1].append("vp80-00-comprehensive-001.ivf");
    TestVector[2].append(slashCharStr());
    TestVector[2].append("vp80-00-comprehensive-002.ivf");
    TestVector[3].append(slashCharStr());
    TestVector[3].append("vp80-00-comprehensive-003.ivf");
    TestVector[4].append(slashCharStr());
    TestVector[4].append("vp80-00-comprehensive-004.ivf");
    TestVector[5].append(slashCharStr());
    TestVector[5].append("vp80-00-comprehensive-005.ivf");
    TestVector[6].append(slashCharStr());
    TestVector[6].append("vp80-00-comprehensive-006.ivf");
    TestVector[7].append(slashCharStr());
    TestVector[7].append("vp80-00-comprehensive-007.ivf");
    TestVector[8].append(slashCharStr());
    TestVector[8].append("vp80-00-comprehensive-008.ivf");
    TestVector[9].append(slashCharStr());
    TestVector[9].append("vp80-00-comprehensive-009.ivf");
    TestVector[10].append(slashCharStr());
    TestVector[10].append("vp80-00-comprehensive-010.ivf");
    TestVector[11].append(slashCharStr());
    TestVector[11].append("vp80-00-comprehensive-011.ivf");
    TestVector[12].append(slashCharStr());
    TestVector[12].append("vp80-00-comprehensive-012.ivf");
    TestVector[13].append(slashCharStr());
    TestVector[13].append("vp80-00-comprehensive-013.ivf");
    TestVector[14].append(slashCharStr());
    TestVector[14].append("vp80-00-comprehensive-014.ivf");
    TestVector[15].append(slashCharStr());
    TestVector[15].append("vp80-00-comprehensive-015.ivf");
    TestVector[16].append(slashCharStr());
    TestVector[16].append("vp80-00-comprehensive-016.ivf");
    TestVector[17].append(slashCharStr());
    TestVector[17].append("vp80-00-comprehensive-017.ivf");
    TestVector[18].append(slashCharStr());
    TestVector[18].append("vp80-01-intra-1400.ivf");
    TestVector[19].append(slashCharStr());
    TestVector[19].append("vp80-01-intra-1411.ivf");
    TestVector[20].append(slashCharStr());
    TestVector[20].append("vp80-01-intra-1416.ivf");
    TestVector[21].append(slashCharStr());
    TestVector[21].append("vp80-01-intra-1417.ivf");
    TestVector[22].append(slashCharStr());
    TestVector[22].append("vp80-02-inter-1402.ivf");
    TestVector[23].append(slashCharStr());
    TestVector[23].append("vp80-02-inter-1412.ivf");
    TestVector[24].append(slashCharStr());
    TestVector[24].append("vp80-02-inter-1418.ivf");
    TestVector[25].append(slashCharStr());
    TestVector[25].append("vp80-02-inter-1424.ivf");
    TestVector[26].append(slashCharStr());
    TestVector[26].append("vp80-03-segmentation-1401.ivf");
    TestVector[27].append(slashCharStr());
    TestVector[27].append("vp80-03-segmentation-1403.ivf");
    TestVector[28].append(slashCharStr());
    TestVector[28].append("vp80-03-segmentation-1407.ivf");
    TestVector[29].append(slashCharStr());
    TestVector[29].append("vp80-03-segmentation-1408.ivf");
    TestVector[30].append(slashCharStr());
    TestVector[30].append("vp80-03-segmentation-1409.ivf");
    TestVector[31].append(slashCharStr());
    TestVector[31].append("vp80-03-segmentation-1410.ivf");
    TestVector[32].append(slashCharStr());
    TestVector[32].append("vp80-03-segmentation-1413.ivf");
    TestVector[33].append(slashCharStr());
    TestVector[33].append("vp80-03-segmentation-1414.ivf");
    TestVector[34].append(slashCharStr());
    TestVector[34].append("vp80-03-segmentation-1415.ivf");
    TestVector[35].append(slashCharStr());
    TestVector[35].append("vp80-03-segmentation-1425.ivf");
    TestVector[36].append(slashCharStr());
    TestVector[36].append("vp80-03-segmentation-1426.ivf");
    TestVector[37].append(slashCharStr());
    TestVector[37].append("vp80-03-segmentation-1427.ivf");
    TestVector[38].append(slashCharStr());
    TestVector[38].append("vp80-03-segmentation-1432.ivf");
    TestVector[39].append(slashCharStr());
    TestVector[39].append("vp80-03-segmentation-1435.ivf");
    TestVector[40].append(slashCharStr());
    TestVector[40].append("vp80-03-segmentation-1436.ivf");
    TestVector[41].append(slashCharStr());
    TestVector[41].append("vp80-03-segmentation-1437.ivf");
    TestVector[42].append(slashCharStr());
    TestVector[42].append("vp80-03-segmentation-1441.ivf");
    TestVector[43].append(slashCharStr());
    TestVector[43].append("vp80-03-segmentation-1442.ivf");
    TestVector[44].append(slashCharStr());
    TestVector[44].append("vp80-04-partitions-1404.ivf");
    TestVector[45].append(slashCharStr());
    TestVector[45].append("vp80-04-partitions-1405.ivf");
    TestVector[46].append(slashCharStr());
    TestVector[46].append("vp80-04-partitions-1406.ivf");
    TestVector[47].append(slashCharStr());
    TestVector[47].append("vp80-05-sharpness-1428.ivf");
    TestVector[48].append(slashCharStr());
    TestVector[48].append("vp80-05-sharpness-1429.ivf");
    TestVector[49].append(slashCharStr());
    TestVector[49].append("vp80-05-sharpness-1430.ivf");
    TestVector[50].append(slashCharStr());
    TestVector[50].append("vp80-05-sharpness-1431.ivf");
    TestVector[51].append(slashCharStr());
    TestVector[51].append("vp80-05-sharpness-1433.ivf");
    TestVector[52].append(slashCharStr());
    TestVector[52].append("vp80-05-sharpness-1434.ivf");
    TestVector[53].append(slashCharStr());
    TestVector[53].append("vp80-05-sharpness-1438.ivf");
    TestVector[54].append(slashCharStr());
    TestVector[54].append("vp80-05-sharpness-1439.ivf");
    TestVector[55].append(slashCharStr());
    TestVector[55].append("vp80-05-sharpness-1440.ivf");
    TestVector[56].append(slashCharStr());
    TestVector[56].append("vp80-05-sharpness-1443.ivf");
    TestVector[57].append(slashCharStr());
    TestVector[57].append("vp80-06-cropping-001.ivf");
    TestVector[58].append(slashCharStr());
    TestVector[58].append("vp80-06-cropping-002.ivf");
    TestVector[59].append(slashCharStr());
    TestVector[59].append("vp80-06-cropping-003.ivf");
    TestVector[60].append(slashCharStr());
    TestVector[60].append("vp80-06-cropping-004.ivf");
    TestVector[61].append(slashCharStr());
    TestVector[61].append("vp80-06-cropping-005.ivf");
    TestVector[62].append(slashCharStr());
    TestVector[62].append("vp80-06-cropping-006.ivf");
    TestVector[63].append(slashCharStr());
    TestVector[63].append("vp80-06-cropping-007.ivf");
    TestVector[64].append(slashCharStr());
    TestVector[64].append("vp80-06-cropping-008.ivf");
    TestVector[65].append(slashCharStr());
    TestVector[65].append("vp80-06-cropping-009.ivf");
    TestVector[66].append(slashCharStr());
    TestVector[66].append("vp80-06-cropping-010.ivf");
    TestVector[67].append(slashCharStr());
    TestVector[67].append("vp80-06-cropping-011.ivf");
    TestVector[68].append(slashCharStr());
    TestVector[68].append("vp80-06-cropping-012.ivf");
    TestVector[69].append(slashCharStr());
    TestVector[69].append("vp80-06-cropping-013.ivf");
    TestVector[70].append(slashCharStr());
    TestVector[70].append("vp80-06-cropping-014.ivf");
    TestVector[71].append(slashCharStr());
    TestVector[71].append("vp80-06-cropping-015.ivf");
    TestVector[72].append(slashCharStr());
    TestVector[72].append("vp80-06-cropping-016.ivf");
    TestVector[73].append(slashCharStr());
    TestVector[73].append("vp80-06-cropping-017.ivf");
    TestVector[74].append(slashCharStr());
    TestVector[74].append("vp80-06-cropping-018.ivf");
    TestVector[75].append(slashCharStr());
    TestVector[75].append("vp80-06-cropping-019.ivf");
    TestVector[76].append(slashCharStr());
    TestVector[76].append("vp80-06-cropping-020.ivf");
    TestVector[77].append(slashCharStr());
    TestVector[77].append("vp80-06-cropping-021.ivf");
    TestVector[78].append(slashCharStr());
    TestVector[78].append("vp80-06-cropping-022.ivf");
    TestVector[79].append(slashCharStr());
    TestVector[79].append("vp80-06-cropping-023.ivf");
    TestVector[80].append(slashCharStr());
    TestVector[80].append("vp80-06-cropping-024.ivf");
    TestVector[81].append(slashCharStr());
    TestVector[81].append("vp80-06-cropping-025.ivf");
    TestVector[82].append(slashCharStr());
    TestVector[82].append("vp80-06-cropping-026.ivf");
    TestVector[83].append(slashCharStr());
    TestVector[83].append("vp80-06-cropping-027.ivf");
    TestVector[84].append(slashCharStr());
    TestVector[84].append("vp80-06-cropping-028.ivf");
    TestVector[85].append(slashCharStr());
    TestVector[85].append("vp80-06-cropping-029.ivf");
    TestVector[86].append(slashCharStr());
    TestVector[86].append("vp80-06-cropping-030.ivf");
    TestVector[87].append(slashCharStr());
    TestVector[87].append("vp80-06-cropping-031.ivf");
    TestVector[88].append(slashCharStr());
    TestVector[88].append("vp80-06-cropping-032.ivf");
    TestVector[89].append(slashCharStr());
    TestVector[89].append("vp80-06-cropping-033.ivf");
    TestVector[90].append(slashCharStr());
    TestVector[90].append("vp80-06-cropping-034.ivf");
    TestVector[91].append(slashCharStr());
    TestVector[91].append("vp80-06-cropping-035.ivf");
    TestVector[92].append(slashCharStr());
    TestVector[92].append("vp80-06-cropping-036.ivf");
    TestVector[93].append(slashCharStr());
    TestVector[93].append("vp80-06-cropping-037.ivf");
    TestVector[94].append(slashCharStr());
    TestVector[94].append("vp80-06-cropping-038.ivf");
    TestVector[95].append(slashCharStr());
    TestVector[95].append("vp80-06-cropping-039.ivf");
    TestVector[96].append(slashCharStr());
    TestVector[96].append("vp80-06-cropping-040.ivf");
    TestVector[97].append(slashCharStr());
    TestVector[97].append("vp80-06-cropping-041.ivf");
    TestVector[98].append(slashCharStr());
    TestVector[98].append("vp80-06-cropping-042.ivf");
    TestVector[99].append(slashCharStr());
    TestVector[99].append("vp80-06-cropping-043.ivf");
    TestVector[100].append(slashCharStr());
    TestVector[100].append("vp80-06-cropping-044.ivf");
    TestVector[101].append(slashCharStr());
    TestVector[101].append("vp80-06-cropping-045.ivf");

    TestVector_Raw[1].append(slashCharStr());
    TestVector_Raw[1].append("vp80-00-comprehensive-001.raw");
    TestVector_Raw[2].append(slashCharStr());
    TestVector_Raw[2].append("vp80-00-comprehensive-002.raw");
    TestVector_Raw[3].append(slashCharStr());
    TestVector_Raw[3].append("vp80-00-comprehensive-003.raw");
    TestVector_Raw[4].append(slashCharStr());
    TestVector_Raw[4].append("vp80-00-comprehensive-004.raw");
    TestVector_Raw[5].append(slashCharStr());
    TestVector_Raw[5].append("vp80-00-comprehensive-005.raw");
    TestVector_Raw[6].append(slashCharStr());
    TestVector_Raw[6].append("vp80-00-comprehensive-006.raw");
    TestVector_Raw[7].append(slashCharStr());
    TestVector_Raw[7].append("vp80-00-comprehensive-007.raw");
    TestVector_Raw[8].append(slashCharStr());
    TestVector_Raw[8].append("vp80-00-comprehensive-008.raw");
    TestVector_Raw[9].append(slashCharStr());
    TestVector_Raw[9].append("vp80-00-comprehensive-009.raw");
    TestVector_Raw[10].append(slashCharStr());
    TestVector_Raw[10].append("vp80-00-comprehensive-010.raw");
    TestVector_Raw[11].append(slashCharStr());
    TestVector_Raw[11].append("vp80-00-comprehensive-011.raw");
    TestVector_Raw[12].append(slashCharStr());
    TestVector_Raw[12].append("vp80-00-comprehensive-012.raw");
    TestVector_Raw[13].append(slashCharStr());
    TestVector_Raw[13].append("vp80-00-comprehensive-013.raw");
    TestVector_Raw[14].append(slashCharStr());
    TestVector_Raw[14].append("vp80-00-comprehensive-014.raw");
    TestVector_Raw[15].append(slashCharStr());
    TestVector_Raw[15].append("vp80-00-comprehensive-015.raw");
    TestVector_Raw[16].append(slashCharStr());
    TestVector_Raw[16].append("vp80-00-comprehensive-016.raw");
    TestVector_Raw[17].append(slashCharStr());
    TestVector_Raw[17].append("vp80-00-comprehensive-017.raw");
    TestVector_Raw[18].append(slashCharStr());
    TestVector_Raw[18].append("vp80-01-intra-1400.raw");
    TestVector_Raw[19].append(slashCharStr());
    TestVector_Raw[19].append("vp80-01-intra-1411.raw");
    TestVector_Raw[20].append(slashCharStr());
    TestVector_Raw[20].append("vp80-01-intra-1416.raw");
    TestVector_Raw[21].append(slashCharStr());
    TestVector_Raw[21].append("vp80-01-intra-1417.raw");
    TestVector_Raw[22].append(slashCharStr());
    TestVector_Raw[22].append("vp80-02-inter-1402.raw");
    TestVector_Raw[23].append(slashCharStr());
    TestVector_Raw[23].append("vp80-02-inter-1412.raw");
    TestVector_Raw[24].append(slashCharStr());
    TestVector_Raw[24].append("vp80-02-inter-1418.raw");
    TestVector_Raw[25].append(slashCharStr());
    TestVector_Raw[25].append("vp80-02-inter-1424.raw");
    TestVector_Raw[26].append(slashCharStr());
    TestVector_Raw[26].append("vp80-03-segmentation-1401.raw");
    TestVector_Raw[27].append(slashCharStr());
    TestVector_Raw[27].append("vp80-03-segmentation-1403.raw");
    TestVector_Raw[28].append(slashCharStr());
    TestVector_Raw[28].append("vp80-03-segmentation-1407.raw");
    TestVector_Raw[29].append(slashCharStr());
    TestVector_Raw[29].append("vp80-03-segmentation-1408.raw");
    TestVector_Raw[30].append(slashCharStr());
    TestVector_Raw[30].append("vp80-03-segmentation-1409.raw");
    TestVector_Raw[31].append(slashCharStr());
    TestVector_Raw[31].append("vp80-03-segmentation-1410.raw");
    TestVector_Raw[32].append(slashCharStr());
    TestVector_Raw[32].append("vp80-03-segmentation-1413.raw");
    TestVector_Raw[33].append(slashCharStr());
    TestVector_Raw[33].append("vp80-03-segmentation-1414.raw");
    TestVector_Raw[34].append(slashCharStr());
    TestVector_Raw[34].append("vp80-03-segmentation-1415.raw");
    TestVector_Raw[35].append(slashCharStr());
    TestVector_Raw[35].append("vp80-03-segmentation-1425.raw");
    TestVector_Raw[36].append(slashCharStr());
    TestVector_Raw[36].append("vp80-03-segmentation-1426.raw");
    TestVector_Raw[37].append(slashCharStr());
    TestVector_Raw[37].append("vp80-03-segmentation-1427.raw");
    TestVector_Raw[38].append(slashCharStr());
    TestVector_Raw[38].append("vp80-03-segmentation-1432.raw");
    TestVector_Raw[39].append(slashCharStr());
    TestVector_Raw[39].append("vp80-03-segmentation-1435.raw");
    TestVector_Raw[40].append(slashCharStr());
    TestVector_Raw[40].append("vp80-03-segmentation-1436.raw");
    TestVector_Raw[41].append(slashCharStr());
    TestVector_Raw[41].append("vp80-03-segmentation-1437.raw");
    TestVector_Raw[42].append(slashCharStr());
    TestVector_Raw[42].append("vp80-03-segmentation-1441.raw");
    TestVector_Raw[43].append(slashCharStr());
    TestVector_Raw[43].append("vp80-03-segmentation-1442.raw");
    TestVector_Raw[44].append(slashCharStr());
    TestVector_Raw[44].append("vp80-04-partitions-1404.raw");
    TestVector_Raw[45].append(slashCharStr());
    TestVector_Raw[45].append("vp80-04-partitions-1405.raw");
    TestVector_Raw[46].append(slashCharStr());
    TestVector_Raw[46].append("vp80-04-partitions-1406.raw");
    TestVector_Raw[47].append(slashCharStr());
    TestVector_Raw[47].append("vp80-05-sharpness-1428.raw");
    TestVector_Raw[48].append(slashCharStr());
    TestVector_Raw[48].append("vp80-05-sharpness-1429.raw");
    TestVector_Raw[49].append(slashCharStr());
    TestVector_Raw[49].append("vp80-05-sharpness-1430.raw");
    TestVector_Raw[50].append(slashCharStr());
    TestVector_Raw[50].append("vp80-05-sharpness-1431.raw");
    TestVector_Raw[51].append(slashCharStr());
    TestVector_Raw[51].append("vp80-05-sharpness-1433.raw");
    TestVector_Raw[52].append(slashCharStr());
    TestVector_Raw[52].append("vp80-05-sharpness-1434.raw");
    TestVector_Raw[53].append(slashCharStr());
    TestVector_Raw[53].append("vp80-05-sharpness-1438.raw");
    TestVector_Raw[54].append(slashCharStr());
    TestVector_Raw[54].append("vp80-05-sharpness-1439.raw");
    TestVector_Raw[55].append(slashCharStr());
    TestVector_Raw[55].append("vp80-05-sharpness-1440.raw");
    TestVector_Raw[56].append(slashCharStr());
    TestVector_Raw[56].append("vp80-05-sharpness-1443.raw");
    TestVector_Raw[57].append(slashCharStr());
    TestVector_Raw[57].append("vp80-06-cropping-001.raw");
    TestVector_Raw[58].append(slashCharStr());
    TestVector_Raw[58].append("vp80-06-cropping-002.raw");
    TestVector_Raw[59].append(slashCharStr());
    TestVector_Raw[59].append("vp80-06-cropping-003.raw");
    TestVector_Raw[60].append(slashCharStr());
    TestVector_Raw[60].append("vp80-06-cropping-004.raw");
    TestVector_Raw[61].append(slashCharStr());
    TestVector_Raw[61].append("vp80-06-cropping-005.raw");
    TestVector_Raw[62].append(slashCharStr());
    TestVector_Raw[62].append("vp80-06-cropping-006.raw");
    TestVector_Raw[63].append(slashCharStr());
    TestVector_Raw[63].append("vp80-06-cropping-007.raw");
    TestVector_Raw[64].append(slashCharStr());
    TestVector_Raw[64].append("vp80-06-cropping-008.raw");
    TestVector_Raw[65].append(slashCharStr());
    TestVector_Raw[65].append("vp80-06-cropping-009.raw");
    TestVector_Raw[66].append(slashCharStr());
    TestVector_Raw[66].append("vp80-06-cropping-010.raw");
    TestVector_Raw[67].append(slashCharStr());
    TestVector_Raw[67].append("vp80-06-cropping-011.raw");
    TestVector_Raw[68].append(slashCharStr());
    TestVector_Raw[68].append("vp80-06-cropping-012.raw");
    TestVector_Raw[69].append(slashCharStr());
    TestVector_Raw[69].append("vp80-06-cropping-013.raw");
    TestVector_Raw[70].append(slashCharStr());
    TestVector_Raw[70].append("vp80-06-cropping-014.raw");
    TestVector_Raw[71].append(slashCharStr());
    TestVector_Raw[71].append("vp80-06-cropping-015.raw");
    TestVector_Raw[72].append(slashCharStr());
    TestVector_Raw[72].append("vp80-06-cropping-016.raw");
    TestVector_Raw[73].append(slashCharStr());
    TestVector_Raw[73].append("vp80-06-cropping-017.raw");
    TestVector_Raw[74].append(slashCharStr());
    TestVector_Raw[74].append("vp80-06-cropping-018.raw");
    TestVector_Raw[75].append(slashCharStr());
    TestVector_Raw[75].append("vp80-06-cropping-019.raw");
    TestVector_Raw[76].append(slashCharStr());
    TestVector_Raw[76].append("vp80-06-cropping-020.raw");
    TestVector_Raw[77].append(slashCharStr());
    TestVector_Raw[77].append("vp80-06-cropping-021.raw");
    TestVector_Raw[78].append(slashCharStr());
    TestVector_Raw[78].append("vp80-06-cropping-022.raw");
    TestVector_Raw[79].append(slashCharStr());
    TestVector_Raw[79].append("vp80-06-cropping-023.raw");
    TestVector_Raw[80].append(slashCharStr());
    TestVector_Raw[80].append("vp80-06-cropping-024.raw");
    TestVector_Raw[81].append(slashCharStr());
    TestVector_Raw[81].append("vp80-06-cropping-025.raw");
    TestVector_Raw[82].append(slashCharStr());
    TestVector_Raw[82].append("vp80-06-cropping-026.raw");
    TestVector_Raw[83].append(slashCharStr());
    TestVector_Raw[83].append("vp80-06-cropping-027.raw");
    TestVector_Raw[84].append(slashCharStr());
    TestVector_Raw[84].append("vp80-06-cropping-028.raw");
    TestVector_Raw[85].append(slashCharStr());
    TestVector_Raw[85].append("vp80-06-cropping-029.raw");
    TestVector_Raw[86].append(slashCharStr());
    TestVector_Raw[86].append("vp80-06-cropping-030.raw");
    TestVector_Raw[87].append(slashCharStr());
    TestVector_Raw[87].append("vp80-06-cropping-031.raw");
    TestVector_Raw[88].append(slashCharStr());
    TestVector_Raw[88].append("vp80-06-cropping-032.raw");
    TestVector_Raw[89].append(slashCharStr());
    TestVector_Raw[89].append("vp80-06-cropping-033.raw");
    TestVector_Raw[90].append(slashCharStr());
    TestVector_Raw[90].append("vp80-06-cropping-034.raw");
    TestVector_Raw[91].append(slashCharStr());
    TestVector_Raw[91].append("vp80-06-cropping-035.raw");
    TestVector_Raw[92].append(slashCharStr());
    TestVector_Raw[92].append("vp80-06-cropping-036.raw");
    TestVector_Raw[93].append(slashCharStr());
    TestVector_Raw[93].append("vp80-06-cropping-037.raw");
    TestVector_Raw[94].append(slashCharStr());
    TestVector_Raw[94].append("vp80-06-cropping-038.raw");
    TestVector_Raw[95].append(slashCharStr());
    TestVector_Raw[95].append("vp80-06-cropping-039.raw");
    TestVector_Raw[96].append(slashCharStr());
    TestVector_Raw[96].append("vp80-06-cropping-040.raw");
    TestVector_Raw[97].append(slashCharStr());
    TestVector_Raw[97].append("vp80-06-cropping-041.raw");
    TestVector_Raw[98].append(slashCharStr());
    TestVector_Raw[98].append("vp80-06-cropping-042.raw");
    TestVector_Raw[99].append(slashCharStr());
    TestVector_Raw[99].append("vp80-06-cropping-043.raw");
    TestVector_Raw[100].append(slashCharStr());
    TestVector_Raw[100].append("vp80-06-cropping-044.raw");
    TestVector_Raw[101].append(slashCharStr());
    TestVector_Raw[101].append("vp80-06-cropping-045.raw");

    TestVectorNum = 0;
    string TestVector_Text[102];

    while (TestVectorNum < 102)
    {
        TestVector_Text[TestVectorNum] = TestVector_Raw[TestVectorNum];
        TestVector_Text[TestVectorNum].erase(TestVector_Text[TestVectorNum].end() - 4, TestVector_Text[TestVectorNum].end());
        TestVector_Text[TestVectorNum].append("_MD5.txt");
        TestVectorNum++;
    }

    string MD5Key[101];

    //New Test Vectors 2.0.0 MD5 Checksums
    MD5Key[0].assign("fad126074e1bd5363d43b9d1cadddb71");   /*vp80-00-comprehensive-001.raw*/
    MD5Key[1].assign("182f03dd264ebac04e24c7c9499d7cdb");   /*vp80-00-comprehensive-002.raw*/
    MD5Key[2].assign("e5fe668b033900022c3eb0ba76a44bd1");   /*vp80-00-comprehensive-003.raw*/
    MD5Key[3].assign("95097ce9808c1d47e03f99c48ad111ec");   /*vp80-00-comprehensive-004.raw*/
    MD5Key[4].assign("0f469e4fd1dea533e5580688b2d242ff");   /*vp80-00-comprehensive-005.raw*/
    MD5Key[5].assign("2d5fa3ec2f88404ae7b305c1074036f4");   /*vp80-00-comprehensive-006.raw*/
    MD5Key[6].assign("92526913d89b6a9b00f2d602def08bce");   /*vp80-00-comprehensive-007.raw*/
    MD5Key[7].assign("bd4d46a9d14fe5a7fc9cfc8deac2d34c");   /*vp80-00-comprehensive-008.raw*/
    MD5Key[8].assign("19201a2d535bd82f41c1a5658def5379");   /*vp80-00-comprehensive-009.raw*/
    MD5Key[9].assign("61d05919a9883d9f215eb3f2db63eb13");   /*vp80-00-comprehensive-010.raw*/
    MD5Key[10].assign("1a0afe5e70512a03323a8f1176bcf022");  /*vp80-00-comprehensive-011.raw*/
    MD5Key[11].assign("4ea997c80dc2087e6deec81f1ecf6668");  /*vp80-00-comprehensive-012.raw*/
    MD5Key[12].assign("93169305d3054327be3cc074f0773a75");  /*vp80-00-comprehensive-013.raw*/
    MD5Key[13].assign("7280a64c51dfa557c1b9552dc1e1fbed");  /*vp80-00-comprehensive-014.raw*/
    MD5Key[14].assign("23b9cc582e344726e76cda092b416bcf");  /*vp80-00-comprehensive-015.raw*/
    MD5Key[15].assign("55e889d22f99718cf6936d55f8ade12b");  /*vp80-00-comprehensive-016.raw*/
    MD5Key[16].assign("95a68ffb228d1d8c6ee54f16a10fb9eb");  /*vp80-00-comprehensive-017.raw*/
    MD5Key[17].assign("53b08ac91398a5dd948434e41b31b47e");  /*vp80-01-intra-1400.raw*/
    MD5Key[18].assign("8fa1762329e65c97245393a933cd0f00");  /*vp80-01-intra-1411.raw*/
    MD5Key[19].assign("cffd1299fa7a0330264cb411d9482bb0");  /*vp80-01-intra-1416.raw*/
    MD5Key[20].assign("0e6c13a78a203d95fe12d206a432f642");  /*vp80-01-intra-1417.raw*/
    MD5Key[21].assign("184ee9c5cd6e32f2fe7b2f5a463d37b3");  /*vp80-02-inter-1402.raw*/
    MD5Key[22].assign("6928dc8e7886914b0ba5825518e54bd7");  /*vp80-02-inter-1412.raw*/
    MD5Key[23].assign("45302aa645ff5c139fe580ac30482245");  /*vp80-02-inter-1418.raw*/
    MD5Key[24].assign("4816cb607488b930ceadeb2cdb034c49");  /*vp80-02-inter-1424.raw*/
    MD5Key[25].assign("f7acb74e99528568714129e2994ceca5");  /*vp80-03-segmentation-1401.raw*/
    MD5Key[26].assign("4e651f545a21863e97547185f93dbd7b");  /*vp80-03-segmentation-1403.raw*/
    MD5Key[27].assign("fa76612d673cbfcb8ec58eda08400388");  /*vp80-03-segmentation-1407.raw*/
    MD5Key[28].assign("886f15167bbdd5ea6c099e8b74452c7c");  /*vp80-03-segmentation-1408.raw*/
    MD5Key[29].assign("780cc4d060eecec04c2746c98065ec6f");  /*vp80-03-segmentation-1409.raw*/
    MD5Key[30].assign("f3468778cd11642f095b4e5dcb19fbda");  /*vp80-03-segmentation-1410.raw*/
    MD5Key[31].assign("6a0564ccc1a655d929390a72ff558db9");  /*vp80-03-segmentation-1413.raw*/
    MD5Key[32].assign("0f887b4bc1bb0aae670c50c9b7f0142f");  /*vp80-03-segmentation-1414.raw*/
    MD5Key[33].assign("8b83e0a3ca0da9e8d7f47a06dc08e18b");  /*vp80-03-segmentation-1415.raw*/
    MD5Key[34].assign("96ffacf0c3eae59b58252be24a60e9b2");  /*vp80-03-segmentation-1425.raw*/
    MD5Key[35].assign("ab1062e4e45e6802d80313da52028af2");  /*vp80-03-segmentation-1426.raw*/
    MD5Key[36].assign("761c3d8e23314516592a1f876865c22a");  /*vp80-03-segmentation-1427.raw*/
    MD5Key[37].assign("c5a7776fdfe8908fcc64e58c317c8cf3");  /*vp80-03-segmentation-1432.raw*/
    MD5Key[38].assign("36a77df963d0d8c3bc098827be403bdb");  /*vp80-03-segmentation-1435.raw*/
    MD5Key[39].assign("bfd17a557ee1ba347c755a18ce5a64a6");  /*vp80-03-segmentation-1436.raw*/
    MD5Key[40].assign("876e7c782ee4dd23866498b794856fd1");  /*vp80-03-segmentation-1437.raw*/
    MD5Key[41].assign("d7a1e99d0ec80ac2b95cc7277bf4db3c");  /*vp80-03-segmentation-1441.raw*/
    MD5Key[42].assign("1a23409897f51ad2920d5ddb999eac71");  /*vp80-03-segmentation-1442.raw*/
    MD5Key[43].assign("27837df047de5b5ae2dc8f2e9d318cb3");  /*vp80-04-partitions-1404.raw*/
    MD5Key[44].assign("12fb1d187ee70738265d8f3a0a70ef26");  /*vp80-04-partitions-1405.raw*/
    MD5Key[45].assign("2da53f93a051dcb8290f884a55272dd9");  /*vp80-04-partitions-1406.raw*/
    MD5Key[46].assign("14b537dae40c6013079fd3d25cb16e7a");  /*vp80-05-sharpness-1428.raw*/
    MD5Key[47].assign("e836423126f8a7de2c6c9777e0a84214");  /*vp80-05-sharpness-1429.raw*/
    MD5Key[48].assign("51503f7a786032d2cbed84ed11430ff3");  /*vp80-05-sharpness-1430.raw*/
    MD5Key[49].assign("7f7f534b2d6e28002e119ed269c8f282");  /*vp80-05-sharpness-1431.raw*/
    MD5Key[50].assign("a2ff07ccbb019f48e020507ca5f5ee90");  /*vp80-05-sharpness-1433.raw*/
    MD5Key[51].assign("110a65e1729fc54e0a25dbf9cc367ccf");  /*vp80-05-sharpness-1434.raw*/
    MD5Key[52].assign("cc468ac5ce042c85f04d62a8e09c97ff");  /*vp80-05-sharpness-1438.raw*/
    MD5Key[53].assign("ebfc41ade751e96220e74491bffda736");  /*vp80-05-sharpness-1439.raw*/
    MD5Key[54].assign("903a267bcc69ad5f8d886db6478d997a");  /*vp80-05-sharpness-1440.raw*/
    MD5Key[55].assign("3c5c3c66cad414d6b79de77e977f115b");  /*vp80-05-sharpness-1443.raw*/
    MD5Key[56].assign("9f8e73e634493ab3e56aad63321f8e11");  /*vp80-06-cropping-001.raw*/
    MD5Key[57].assign("aef0cfaf282d07cd7ac360fb4f7a1f6b");  /*vp80-06-cropping-002.raw*/
    MD5Key[58].assign("609e845f662e3455daf57c68b859a4f6");  /*vp80-06-cropping-003.raw*/
    MD5Key[59].assign("c37c1c11bf3f5adcfa62a39b35cf3d07");  /*vp80-06-cropping-004.raw*/
    MD5Key[60].assign("82641ff61fe1d6bb7ba8accfa5b399ad");  /*vp80-06-cropping-005.raw*/
    MD5Key[61].assign("b8fa425191154685444164157bfbf53d");  /*vp80-06-cropping-006.raw*/
    MD5Key[62].assign("ee19c6b8947c3521b810b66ae080368a");  /*vp80-06-cropping-007.raw*/
    MD5Key[63].assign("1d99ee17a5ba002dbb185a00ea517427");  /*vp80-06-cropping-008.raw*/
    MD5Key[64].assign("017e2e8066397e6f4fdf9522cc2d3428");  /*vp80-06-cropping-009.raw*/
    MD5Key[65].assign("2e66d33de5a4550b168524b569799858");  /*vp80-06-cropping-010.raw*/
    MD5Key[66].assign("3afdb776a6260ab3a3722d5ed0a51ea6");  /*vp80-06-cropping-011.raw*/
    MD5Key[67].assign("ee370c45dd534c994e83454962f05e4a");  /*vp80-06-cropping-012.raw*/
    MD5Key[68].assign("de95d19355f402c2ce9a75562ef45a88");  /*vp80-06-cropping-013.raw*/
    MD5Key[69].assign("79d1c72208f9a40a770a065b48d7c9b6");  /*vp80-06-cropping-014.raw*/
    MD5Key[70].assign("eac91c3f679b57fb0d41965a58515c58");  /*vp80-06-cropping-015.raw*/
    MD5Key[71].assign("1ade1e1d87d045a3df99c1c4431c3770");  /*vp80-06-cropping-016.raw*/
    MD5Key[72].assign("a92acb085407d9f38d6b86a25dcd31b6");  /*vp80-06-cropping-017.raw*/
    MD5Key[73].assign("633ab126a1f74babedef090b7a9f79f3");  /*vp80-06-cropping-018.raw*/
    MD5Key[74].assign("336b86b25f320eb63eb0476ab84229cd");  /*vp80-06-cropping-019.raw*/
    MD5Key[75].assign("d4bf42f6671d06dbda405c105984f4a6");  /*vp80-06-cropping-020.raw*/
    MD5Key[76].assign("a857394e5ad0b52ffcffd61d0be108ed");  /*vp80-06-cropping-021.raw*/
    MD5Key[77].assign("2f985c573f020c5bac55d48fa5e24489");  /*vp80-06-cropping-022.raw*/
    MD5Key[78].assign("8a56b8c71b671503018727917fcdb145");  /*vp80-06-cropping-023.raw*/
    MD5Key[79].assign("2369693e71dcb565327b49c31fdc513e");  /*vp80-06-cropping-024.raw*/
    MD5Key[80].assign("9be67b80d50f157bbae891e2a465909e");  /*vp80-06-cropping-025.raw*/
    MD5Key[81].assign("20ed8b808ad8f3be05a9c80e0328f0e5");  /*vp80-06-cropping-026.raw*/
    MD5Key[82].assign("d4178440ab559d89df2d036c6606cda6");  /*vp80-06-cropping-027.raw*/
    MD5Key[83].assign("c65520731d9caae321de26fa41e77bf2");  /*vp80-06-cropping-028.raw*/
    MD5Key[84].assign("097c3f56e8917306fa5f1a60e63c95b9");  /*vp80-06-cropping-029.raw*/
    MD5Key[85].assign("f52c5fc26a9439aa8226d533bcdada98");  /*vp80-06-cropping-030.raw*/
    MD5Key[86].assign("5df73922d1ed6680e4d2ac35d86b061a");  /*vp80-06-cropping-031.raw*/
    MD5Key[87].assign("9d9a31179bfb0daf3a884625e7f1c116");  /*vp80-06-cropping-032.raw*/
    MD5Key[88].assign("bbafe2fdf7b927ff0b403f91646ed80b");  /*vp80-06-cropping-033.raw*/
    MD5Key[89].assign("b2d27a6f08e37e1a26bfd31e229684d8");  /*vp80-06-cropping-034.raw*/
    MD5Key[90].assign("dd5138a4831475284d285a6e821387d6");  /*vp80-06-cropping-035.raw*/
    MD5Key[91].assign("8fd17178a7d163a8eb9b439d9fa42797");  /*vp80-06-cropping-036.raw*/
    MD5Key[92].assign("0e7960d1d71e1aa5a97d22f152c10a8f");  /*vp80-06-cropping-037.raw*/
    MD5Key[93].assign("84b6553703153de46aefb09bc479bca5");  /*vp80-06-cropping-038.raw*/
    MD5Key[94].assign("971f42513a93493c4c7bce8177a07da4");  /*vp80-06-cropping-039.raw*/
    MD5Key[95].assign("98e3e9253baa99f5bf40316eb54d9e36");  /*vp80-06-cropping-040.raw*/
    MD5Key[96].assign("1e57bb03f8872d07d64a3177d2a31ceb");  /*vp80-06-cropping-041.raw*/
    MD5Key[97].assign("1a2360cc18b06016777fb9614ac4de61");  /*vp80-06-cropping-042.raw*/
    MD5Key[98].assign("18924afc1f5acb60d3933184ff1f93c8");  /*vp80-06-cropping-043.raw*/
    MD5Key[99].assign("62f5de3feb4346072c94d643d6946fb4");  /*vp80-06-cropping-044.raw*/
    MD5Key[100].assign("8319b0f71a76787f894f2927b7923897");  /*vp80-06-cropping-045.raw*/

    int printKeyTracker = 0;
    int DeleteTV = 0; // if = 1 will delete decompressed tesetvectors if 0 will not

    /////////////OutPutfile////////////
    string TextfileString(CurTestDirStr.c_str());
    TextfileString.append(slashCharStr());
    TextfileString.append(MyDir);

    if (TestType == COMP_ONLY || TestType == TEST_AND_COMP)
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

    if (TestType == TEST_AND_COMP)
        print_header_full_test(argc, argv, MainTestDirChar);

    if (TestType == COMP_ONLY)
        print_header_compression_only(argc, argv, MainTestDirChar);

    if (TestType == TEST_ONLY)
        print_header_test_only(argc, argv, CurTestDirStr);

    vpxt_cap_string_print(PRINT_BOTH, "%s", MyDir);

    if (TestType == TEST_ONLY)
    {
        //This test requires no preperation before a Test Only Run
    }
    else
    {
        CurTestVector = 1;

        while (CurTestVector < LastTestVector)
        {
            tprintf("\n\nTestVector %i\nAPI - Decompressing VP8 IVF File to Raw File: \n", CurTestVector);

            if (vpxt_decompress_ivf_to_raw(TestVector[CurTestVector].c_str(), TestVector_Raw[CurTestVector].c_str()) == -1)
            {
                fclose(fp);
                record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
                return 2;
            }

            CurTestVector++;
        }
    }

    vector<int> FailVector;

    if (TestType == COMP_ONLY)
    {
        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 10;
    }

    //Compute MD5 CheckSums
    CurTestVector = 1;

    while (CurTestVector < LastTestVector)
    {
        tprintf("\n\nComputing MD5 for TestVector %i", CurTestVector);
        vpxt_dec_compute_md5(TestVector[CurTestVector].c_str(), TestVector_Text[CurTestVector].c_str());
        CurTestVector++;
    }

    if (DeleteTV == 1)
    {
        CurTestVector = 1;

        while (CurTestVector < LastTestVector)
        {
            if (remove(TestVector_Raw[CurTestVector].c_str()) == 0)
            {
                /*printf("%s Successfully Deleted\n\n", outputString.c_str());*/
            }
            else
            {
                printf("Error: %s Not Deleted\n\n", TestVector_Raw[CurTestVector].c_str());
            }

            CurTestVector++;
        }
    }

    int x = 0;
    int Fail = 0;

    CurTestVector = 1;

    while (CurTestVector < LastTestVector)
    {
        char TextInput[255];
        char TestVectFileName[255];
        snprintf(TextInput, 255, "%s", TestVector_Text[CurTestVector].c_str());
        vpxt_file_name(TestVector[CurTestVector].c_str(), TestVectFileName, 1);

        char buffer[1024];
        fstream infile;
        infile.open(TextInput);
        infile.getline(buffer, 1024);
        infile.close();

        tprintf("\n\n");

        if (MD5Key[x].compare(buffer) == 0)
        {
            tprintf(" VP8 Test Vector %i: %s \n"
                    " Observed: %s \n"
                    " Expected: %s \n"
                    " -Pass \n\n"
                    , x + 1, TestVectFileName, buffer, MD5Key[x].c_str());
        }
        else
        {
            tprintf(" VP8 Test Vector %i: %s \n"
                    " Observed: %s \n"
                    " Expected: %s \n"
                    " -Fail \n\n"
                    , x + 1, TestVectFileName, buffer, MD5Key[x].c_str());

            FailVector.push_back(x + 1);
            Fail = 1;
        }

        CurTestVector++;
        x++;
    }

    tprintf("\n\nResults:\n\n");

    if (Fail == 0)
    {
        vpxt_formated_print(RESPRT, "All decoded test vector MD5 checksum's match expected checksum's - Passed");

        tprintf("\n\nPassed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 1;
    }
    else
    {
        string FailStr = "Not all decoded test vectors match expected MD5 checksum's.";

        int q = 0;

        if (FailVector.size() == 1)
        {
            FailStr.append(" Test Vector ");
        }
        else
        {
            FailStr.append(" Test Vectors ");
        }

        while (q < FailVector.size())
        {


            if (q != 0)
            {
                if (q + 1 != FailVector.size())
                {
                    FailStr.append(", ");
                }
                else
                {
                    FailStr.append(" and ");
                }
            }

            char TVFailNumChar[32] = "";
            vpxt_itoa_custom(FailVector[q], TVFailNumChar, 10);
            FailStr.append(TVFailNumChar);
            q++;
        }

        if (FailVector.size() == 1)
            FailStr.append(" does not match");
        else
            FailStr.append(" do not match");

        FailStr.append(" - Failed");
        vpxt_formated_print(RESPRT, FailStr.c_str());

        tprintf("\n\nFailed\n");

        fclose(fp);
        record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
        return 0;
    }

    fclose(fp);
    record_test_complete(FileIndexStr, FileIndexOutputChar, TestType);
    return 6;
}