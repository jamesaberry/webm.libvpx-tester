#include "vpxt_test_declarations.h"

int test_max_quantizer(int argc,
                       const char** argv,
                       const std::string &working_dir,
                       const std::string sub_folder_str,
                       int test_type,
                       int delete_ivf,
                       int artifact_detection)
{
    char *comp_out_str = "Max Quantizer";
    char *test_dir = "test_max_quantizer";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bitrate = atoi(argv[4]);
    std::string enc_format = argv[5];

    int speed = 0;

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    std::string quant_out_base = cur_test_dir_str + slashCharStr() + test_dir +
        "_compression_";

    int file_num = 0;
    int quant = 4;
    int quant_arr[8];
    int quant_out_enc_art_det_arr[8];
    std::string quant_out_enc_arr[8];

    while (file_num < 8)
    {
        // set up file name
        char quant_char[2];
        vpxt_itoa_custom(quant, quant_char, 10);

        quant_out_enc_arr[file_num] = quant_out_base;
        quant_out_enc_arr[file_num] += quant_char;
        vpxt_enc_format_append(quant_out_enc_arr[file_num], enc_format);

        // set up quantizer value array and art detect array
        quant_arr[file_num] = quant;
        quant_out_enc_art_det_arr[file_num] = artifact_detection;

        quant += 8;
        ++file_num;
    }

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

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    /////////////////// Use Custom Settings ///////////////////
    if (input_ver == 2)
    {
        if (!vpxt_file_exists_check(argv[argc-1]))
        {
            tprintf(PRINT_BTH, "\nInput Settings file %s does not exist\n",
                argv[argc-1]);

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestIndeterminate;
        }

        opt = vpxt_input_settings(argv[argc-1]);
        bitrate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////
    // make sure constrained quantizer mode is off
    if(opt.end_usage == 2)
        opt.end_usage = 1;
    opt.target_bandwidth = bitrate;

    double psnr_arr[10];
    int max_q_verify_arr[10];

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        file_num = 0;
        while (file_num < 8)
        {
                psnr_arr[file_num] = vpxt_psnr(input.c_str(),
                    quant_out_enc_arr[file_num].c_str(), 0, PRINT_BTH, 1, 0, 0,
                    0, NULL, quant_out_enc_art_det_arr[file_num]);
                tprintf(PRINT_BTH, "\n");

                max_q_verify_arr[file_num] = vpxt_check_max_quantizer(
                    quant_out_enc_arr[file_num].c_str(), quant_arr[file_num]);
                tprintf(PRINT_BTH, "\n");

                ++file_num;
        }
    }
    else
    {
        file_num = 0;
        while (file_num < 8)
        {
            opt.worst_allowed_q = quant_arr[file_num];
            // make sure min q is less than max
            while(opt.best_allowed_q > opt.worst_allowed_q &&
                opt.best_allowed_q >= 1)
                opt.best_allowed_q = opt.worst_allowed_q - 1;

            opt.Mode = mode;
            if (vpxt_compress(input.c_str(), quant_out_enc_arr[file_num].c_str()
                , speed, bitrate, opt, comp_out_str, quant_arr[file_num], 1,
                enc_format) == -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }

            tprintf(PRINT_BTH, "\n");

            if (test_type != kCompOnly)
            {
                psnr_arr[file_num] = vpxt_psnr(input.c_str(),
                    quant_out_enc_arr[file_num].c_str(), 0, PRINT_BTH, 1, 0, 0,
                    0, NULL, quant_out_enc_art_det_arr[file_num]);
                tprintf(PRINT_BTH, "\n");

                max_q_verify_arr[file_num] = vpxt_check_max_quantizer(
                    quant_out_enc_arr[file_num].c_str(), quant_arr[file_num]);
                tprintf(PRINT_BTH, "\n");
            }

            ++file_num;
        }
    }

    // Create Compression only stop test short.
    if (test_type == kCompOnly)
    {
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    tprintf(PRINT_BTH, "\n");

    file_num = 0;
    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    // verify psnr numbers
    while (file_num < 7)
    {
        if (!(psnr_arr[file_num+1] <= psnr_arr[file_num]))
        {
            // if psnr_arr[file_num+1] greater than but within 1% - min pass
            if (psnr_arr[file_num+1] <= (psnr_arr[file_num] +
                (psnr_arr[file_num] * 0.01)))
            {
                vpxt_formated_print(RESPRT, "MaxQ %2i PSNR %.2f within 1%% of "
                    "MaxQ %2i PSNR %.2f - MinPassed", quant_arr[file_num+1],
                    psnr_arr[file_num+1], quant_arr[file_num],
                    psnr_arr[file_num]);
                tprintf(PRINT_BTH, "\n");
                test_state = kTestMinPassed;
            }
            else
            {
                vpxt_formated_print(RESPRT, "MaxQ %2i %.2f > %.2f MaxQ %2i - "
                    "Failed", quant_arr[file_num+1], psnr_arr[file_num+1],
                    psnr_arr[file_num], quant_arr[file_num]);
                tprintf(PRINT_BTH, "\n");
                test_state = kTestFailed;
            }
        }
        else
        {
            vpxt_formated_print(RESPRT, "MaxQ %2i %.2f <= %.2f MaxQ %2i - "
                "Passed", quant_arr[file_num+1], psnr_arr[file_num+1],
                psnr_arr[file_num], quant_arr[file_num]);
            tprintf(PRINT_BTH, "\n");
        }

        ++file_num;
    }

    // Verify that max Q not exceded for any frames
    file_num = 0;
    while (file_num < 8)
    {
        if (max_q_verify_arr[file_num] != -1)
        {
            vpxt_formated_print(RESPRT, "MaxQ value exceded for MaxQ %2i - "
                "frame %i - Failed", quant_arr[file_num],
                max_q_verify_arr[file_num]);
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
        }
        else
        {
            vpxt_formated_print(RESPRT, "MaxQ value not exceded for MaxQ %2i "
                "- Passed", quant_arr[file_num], max_q_verify_arr[file_num]);
            tprintf(PRINT_BTH, "\n");
        }

        ++file_num;
    }

    // handle possible artifact
    file_num = 0;
    while(file_num < 8)
    {
        if(quant_out_enc_art_det_arr[file_num] == kPossibleArtifactFound)
        {
            tprintf(PRINT_BTH, "\nPossible Artifact\n");

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestPossibleArtifact;
        }
        ++file_num;
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestMinPassed)
        tprintf(PRINT_BTH, "\nMin Passed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        for(file_num = 0; file_num < 8; file_num++)
            vpxt_delete_files(1, quant_out_enc_arr[file_num].c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}