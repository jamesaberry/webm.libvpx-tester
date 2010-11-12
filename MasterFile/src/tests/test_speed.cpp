#include "vpxt_test_declarations.h"

int test_speed(int argc, char *argv[], string WorkingDir, string FilesAr[], int TestType)
{
    char *CompressString = "CpuUsed";

    if (!(argc == 6 || argc == 7))
    {
        printf(
            "  SpeedTest \n\n"
            "    <inputfile>\n"
            "    <Mode>\n"
            "          (0)Realtime/Live Encoding\n"
            "          (1)Good Quality Fast Encoding\n"
            "    <Target Bit Rate>\n"
            "    <Lag In Frames>\n"
            "    <Optional Settings File>\n"
        );
        return 0;
    }

    char *input = argv[2];
    int Mode = atoi(argv[3]);
    int BitRate = atoi(argv[4]);
    int LagInFramesInput = atoi(argv[5]);

    ////////////Formatting Test Specific Directory////////////
    string WorkingDirString = "";


    char WorkingDir3[255] = "";
    char *MyDir = "SpeedTest";
    string MainDirString = "";
    char File1[255] = "";


    if (initialize_test_directory(argc, argv, TestType, WorkingDir, MyDir, WorkingDirString, MainDirString, WorkingDir3, File1, FilesAr) == 11)
        return 11;

    string SpeedTestGoodQBase = WorkingDirString;
    string SpeedTestRealTimeBase = WorkingDirString;

    SpeedTestGoodQBase.append(slashCharStr());
    SpeedTestGoodQBase.append("SpeedTestGoodQ_CpuUsed");
    SpeedTestRealTimeBase.append(slashCharStr());
    SpeedTestRealTimeBase.append("SpeedTestRealTime_CpuUsed");

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
        print_header_full_test(argc, argv, WorkingDirString);
    }

    if (TestType == 2)
    {
        print_header_compression_only(argc, argv, WorkingDir3);
    }

    if (TestType == 3)
    {
        print_header_test_only(argc, argv, WorkingDirString);
    }

    tprintf("Speed Test");

    if (Mode != 0 && Mode != 1)
    {
        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    int speed = 0;
    int Fail = 0;
    int Fail2 = 0;
    int Failb = 0;
    int Fail2b = 0;

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    ///////////////////Use Custom Settings///////////////////
    if (argc == 7)
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
        BitRate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////
    int CompressInt = opt.cpu_used;
    opt.target_bandwidth = BitRate;
    opt.Mode = MODE_GOODQUALITY;
    opt.lag_in_frames = LagInFramesInput;

    tprintf("\nLagInFrames: %i\n", LagInFramesInput);

    unsigned int GoodTotalcpu_tick[7];
    unsigned int RealTotalcpu_tick[17];
    unsigned int RealTotalcpu_tickPos[18];

    double GoodPSNRArr[7];
    double RealPSNRArr[17];
    double RealPSNRArrPos[18];

    //Run Test only (Runs Test, Sets up test to be run, or skips compresion of files)
    if (TestType == 3)
    {
        int counter = 0;

        if (Mode == 1)
        {
            while (counter < 6)
            {
                char CounterChar[4];
                vpx_itoa_custom(counter, CounterChar, 10);

                string SpeedTestGoodQ = SpeedTestGoodQBase;
                SpeedTestGoodQ.append(CounterChar);
                SpeedTestGoodQ.append(".ivf");

                GoodTotalcpu_tick[counter] = vpxt_cpu_tick_return(SpeedTestGoodQ.c_str(), 0);
                GoodPSNRArr[counter] = vpxt_ivf_psnr(input, SpeedTestGoodQ.c_str(), 1, 0, 1, NULL);
                counter++;
            }
        }

        if (Mode == 0)
        {
            counter = -1;
            int counter2 = 0;

            while (counter > -17)
            {
                char CounterChar[4];
                vpx_itoa_custom(counter, CounterChar, 10);

                string SpeedTestRealTime = SpeedTestRealTimeBase;
                SpeedTestRealTime.append(CounterChar);
                SpeedTestRealTime.append(".ivf");

                RealTotalcpu_tick[counter2] = vpxt_cpu_tick_return(SpeedTestRealTime.c_str(), 0);
                RealPSNRArr[counter2] = vpxt_ivf_psnr(input, SpeedTestRealTime.c_str(), 1, 0, 1, NULL);
                counter--;
                counter2++;
            }

            counter = 0;

            while (counter < 17)
            {
                char CounterChar[4];
                vpx_itoa_custom(counter, CounterChar, 10);

                string SpeedTestRealTime = SpeedTestRealTimeBase;
                SpeedTestRealTime.append(CounterChar);
                SpeedTestRealTime.append(".ivf");

                RealTotalcpu_tickPos[counter] = vpxt_cpu_tick_return(SpeedTestRealTime.c_str(), 0);
                RealPSNRArrPos[counter] = vpxt_ivf_psnr(input, SpeedTestRealTime.c_str(), 1, 0, 1, NULL);
                counter++;
            }
        }
    }
    else
    {

        int counter = 0;

        if (Mode == 1)
        {
            while (counter < 6)
            {
                char CounterChar[4];
                vpx_itoa_custom(counter, CounterChar, 10);

                string SpeedTestGoodQ = SpeedTestGoodQBase;
                SpeedTestGoodQ.append(CounterChar);
                SpeedTestGoodQ.append(".ivf");

                opt.cpu_used = counter;
                CompressInt = opt.cpu_used;
                opt.Mode = MODE_GOODQUALITY;
                unsigned int Time = vpxt_time_compress_ivf_to_ivf(input, SpeedTestGoodQ.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, GoodTotalcpu_tick[counter]);

                if (Time == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    record_test_complete(MainDirString, File1Str, TestType);
                    return 2;
                }

                if (TestType != 2 && TestType != 3)
                {
                    GoodPSNRArr[counter] = vpxt_ivf_psnr(input, SpeedTestGoodQ.c_str(), 1, 0, 1, NULL);
                }

                counter++;
            }
        }

        if (Mode == 0)
        {
            counter = -1;
            int counter2 = 0;

            while (counter > -17)
            {
                char CounterChar[4];
                vpx_itoa_custom(counter, CounterChar, 10);

                string SpeedTestRealTime = SpeedTestRealTimeBase;
                SpeedTestRealTime.append(CounterChar);
                SpeedTestRealTime.append(".ivf");

                opt.cpu_used = counter;
                CompressInt = opt.cpu_used;
                opt.Mode = MODE_REALTIME;
                unsigned int Time = vpxt_time_compress_ivf_to_ivf(input, SpeedTestRealTime.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, RealTotalcpu_tick[counter2]);

                if (Time == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    record_test_complete(MainDirString, File1Str, TestType);
                    return 2;
                }

                if (TestType != 2 && TestType != 3)
                {
                    RealPSNRArr[counter2] = vpxt_ivf_psnr(input, SpeedTestRealTime.c_str(), 1, 0, 1, NULL);
                }

                counter--;
                counter2++;
            }

            counter = 0;

            while (counter < 17)
            {
                char CounterChar[4];
                vpx_itoa_custom(counter, CounterChar, 10);

                string SpeedTestRealTime = SpeedTestRealTimeBase;
                SpeedTestRealTime.append(CounterChar);
                SpeedTestRealTime.append(".ivf");

                opt.cpu_used = counter;
                CompressInt = opt.cpu_used;
                opt.Mode = MODE_REALTIME;
                unsigned int Time = vpxt_time_compress_ivf_to_ivf(input, SpeedTestRealTime.c_str(), speed, BitRate, opt, CompressString, CompressInt, 0, RealTotalcpu_tickPos[counter]);

                if (Time == -1)
                {
                    fclose(fp);
                    string File1Str = File1;
                    record_test_complete(MainDirString, File1Str, TestType);
                    return 2;
                }

                if (TestType != 2 && TestType != 3)
                {
                    RealPSNRArrPos[counter] = vpxt_ivf_psnr(input, SpeedTestRealTime.c_str(), 1, 0, 1, NULL);
                }

                counter++;
            }
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

    cout << "\n";
    cerr << "\n";

    int counter = 1;
    int pass = 1;

    if (Mode == 1)
    {
        while (counter < 6)
        {
            if (GoodTotalcpu_tick[counter] < GoodTotalcpu_tick[counter-1])
            {
                tprintf("      CpuUsed %*i Encode Tick: %i < CpuUsed %*i Encode Tick: %i\n", 2, counter, GoodTotalcpu_tick[counter], 2, counter - 1, GoodTotalcpu_tick[counter-1]);
            }
            else
            {
                tprintf("Fail: CpuUsed %*i Encode Tick: %i > CpuUsed %*i Encode Tick: %i\n", 2, counter, GoodTotalcpu_tick[counter], 2, counter - 1, GoodTotalcpu_tick[counter-1]);
                Fail = 1;
            }

            float PSNRPercent;
            PSNRPercent = vpxt_abs_float(GoodPSNRArr[counter] - GoodPSNRArr[counter-1]) / GoodPSNRArr[counter-1];

            if (PSNRPercent < 0.1)
            {
                tprintf("      CpuUsed %*i PSNR: %4.2f within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, GoodPSNRArr[counter], 2, counter - 1, GoodPSNRArr[counter-1]);
            }
            else
            {
                tprintf("Fail: CpuUsed %*i PSNR: %4.2f not within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, GoodPSNRArr[counter], 2, counter - 1, GoodPSNRArr[counter-1]);
                Fail2 = 1;
            }

            counter++;
        }
    }

    if (Mode == 0)
    {
        counter = 1;

        while (counter < 16)
        {
            if (RealTotalcpu_tick[counter] < RealTotalcpu_tick[counter-1])
            {
                tprintf("      CpuUsed -%*i Encode Tick: %i < CpuUsed -%*i Encode Tick: %i\n", 2, counter + 1, RealTotalcpu_tick[counter], 2, counter, RealTotalcpu_tick[counter-1]);
            }
            else
            {
                tprintf("Fail: CpuUsed -%*i Encode Tick: %i > CpuUsed -%*i Encode Tick: %i\n", 2, counter + 1, RealTotalcpu_tick[counter], 2, counter, RealTotalcpu_tick[counter-1]);
                Fail++;

                float TimePercent = (RealTotalcpu_tick[counter] - RealTotalcpu_tick[counter-1]);

                if (TimePercent < 0)
                {
                    TimePercent = TimePercent * (-1);
                }

                if (TimePercent > (RealTotalcpu_tick[counter-1] / 10))
                {
                    Failb++;
                }
                else
                {
                }
            }

            float PSNRPercent;
            PSNRPercent = vpxt_abs_float(RealPSNRArr[counter] - RealPSNRArr[counter-1]) / RealPSNRArr[counter-1];

            if (PSNRPercent < 0.1)
            {
                tprintf("      CpuUsed -%*i PSNR: %4.2f within 10%% of CpuUsed -%*i PSNR: %4.2f\n", 2, counter + 1, RealPSNRArr[counter], 2, counter, RealPSNRArr[counter-1]);
            }
            else
            {
                tprintf("Fail: CpuUsed -%*i PSNR: %4.2f not within 10%% of CpuUsed -%*i PSNR: %4.2f\n", 2, counter + 1, RealPSNRArr[counter], 2, counter, RealPSNRArr[counter-1]);
                Fail2++;
            }

            counter++;
        }

        tprintf("\n\n");

        counter = 1;

        while (counter < 17)
        {
            if (RealTotalcpu_tickPos[counter] < RealTotalcpu_tickPos[counter-1])
            {
                tprintf("      CpuUsed %*i Encode Tick: %i < CpuUsed %*i Encode Tick: %i\n", 2, counter, RealTotalcpu_tickPos[counter], 2, counter - 1, RealTotalcpu_tickPos[counter-1]);
            }
            else
            {

                tprintf("Fail: CpuUsed %*i Encode Tick: %i > CpuUsed %*i Encode Tick: %i\n", 2, counter, RealTotalcpu_tickPos[counter], 2, counter - 1, RealTotalcpu_tickPos[counter-1]);
                Fail++;

                float TimePercent = (RealTotalcpu_tickPos[counter] - RealTotalcpu_tickPos[counter-1]);

                if (TimePercent < 0)
                {
                    TimePercent = TimePercent * (-1);
                }

                if (TimePercent > (RealTotalcpu_tickPos[counter-1] / 10))
                {
                    Failb++;
                }
                else
                {
                }
            }

            float PSNRPercent;
            PSNRPercent = vpxt_abs_float(RealPSNRArrPos[counter] - RealPSNRArrPos[counter-1]) / RealPSNRArrPos[counter-1];

            if (PSNRPercent < 0.1)
            {
                tprintf("      CpuUsed %*i PSNR: %4.2f within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, RealPSNRArrPos[counter], 2, counter - 1, RealPSNRArrPos[counter-1]);
            }
            else
            {
                tprintf("Fail: CpuUsed %*i PSNR: %4.2f not within 10%% of CpuUsed %*i PSNR: %4.2f\n", 2, counter, RealPSNRArrPos[counter], 2, counter - 1, RealPSNRArrPos[counter-1]);
                Fail2++;
            }

            counter++;
        }
    }


    if (Mode == 0)
    {
        tprintf("\n\nResults:\n\n");

        if (Fail == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All encode ticks decrease as CpuUsed increases - Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");

            //printf("All encode ticks decrease as CpuUsed increases - Passed");
        }

        if (Fail < 4 && Fail != 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Enough encode ticks decrease as CpuUsed increases - Min Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            pass = 2;
        }

        if (Fail >= 4)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Not enough encode ticks decrease as CpuUsed increases - Failed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            pass = 0;
        }

        if (Fail2 == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All PSNRs are within 10%% - Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }

        if (Fail2 < 2 && Fail2 != 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Enough PSNRs are within 10%% - Min Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            pass = 2;
        }

        if (Fail2 >= 2)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Not enough PSNRs are within 10%% - Failed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            pass = 0;
        }

        if (Failb != 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Not all Encode speeds are within 10%% - Failed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            pass = 0;
        }
    }



    if (Mode == 1)
    {
        tprintf("\n\nResults:\n\n");

        if (Fail == 1)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Not all encode ticks decrease as CpuUsed increases - Failed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            pass = 0;
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All encode ticks decrease as CpuUsed increases - Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }

        if (Fail2 == 0)
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "All PSNR values are within 10%% of eachother - Passed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
        }
        else
        {
            char OutputChar1[255];
            snprintf(OutputChar1, 255, "Not all PSNR values are within 10%% of eachother - Failed");
            string OutputChar1str = OutputChar1;
            formated_print(OutputChar1str, 5);
            tprintf("\n");
            pass = 0;
        }
    }

    if (pass == 0)
    {
        tprintf("\nFailed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 0;
    }

    if (pass == 1)
    {
        tprintf("\nPassed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 1;
    }
    else
    {
        tprintf("\nMin Passed\n");

        fclose(fp);
        string File1Str = File1;
        record_test_complete(MainDirString, File1Str, TestType);
        return 8;
    }

    fclose(fp);
    string File1Str = File1;
    record_test_complete(MainDirString, File1Str, TestType);
    return 6;
}