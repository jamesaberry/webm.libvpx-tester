#include "vpxt_test_declarations.h"

int test_noise_sensitivity(int argc,
                           const char** argv,
                           const std::string &working_dir,
                           const std::string sub_folder_str,
                           int test_type,
                           int delete_ivf)
{
    char *comp_out_str = "Noise Sensitivity";
    char *test_dir = "test_noise_sensitivity";
    int input_ver = vpxt_check_arg_input(argv[1], argc);

    if (input_ver < 0)
        return vpxt_test_help(argv[1], 0);

    std::string input = argv[2];
    int mode = atoi(argv[3]);
    int bit_rate = atoi(argv[4]);
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

    int max_noise = 1;
    int temp_denoise = 1;
    std::vector<std::string> noise_sense_vec;

    std::string noise_sense_base = cur_test_dir_str;
    noise_sense_base += slashCharStr();
    noise_sense_base += test_dir;
    noise_sense_base += "_compression_";

#if !(CONFIG_TEMPORAL_DENOISING)
        max_noise = 6;
        temp_denoise = 0;
#endif

    for(int i = 0; i < max_noise+1; ++i){
        char i_char[8];
        vpxt_itoa_custom(i, i_char, 10);

        std::string noise_sense = noise_sense_base;
        noise_sense += i_char;
        vpxt_enc_format_append(noise_sense, enc_format);
        noise_sense_vec.push_back(noise_sense);
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
        bit_rate = opt.target_bandwidth;
    }

    /////////////////////////////////////////////////////////

    opt.target_bandwidth = bit_rate;

    int noise = 0;
    long file_size[7];
    double noise_psnr[7];

    // print if temp denoise or not
    if(temp_denoise)
        printf("\n\n Not using Temporal Denoising\n");
    else
        printf("\n\n Using Temporal Denoising\n");

    // Run Test only (Runs Test, Sets up test to be run, or skips compresion of
    // files)
    if (test_type == kTestOnly)
    {
        while (noise != max_noise + 1)
        {
            tprintf(PRINT_BTH, "\n");
            noise_psnr[noise] = vpxt_psnr(input.c_str(),
                noise_sense_vec[noise].c_str(), 0, PRINT_BTH, 1, NULL);
            tprintf(PRINT_BTH, "\n");
            file_size[noise] = vpxt_file_size(noise_sense_vec[noise].c_str(),
                1);
            tprintf(PRINT_BTH, "\n");

            noise++;
        }
    }
    else
    {
        while (noise != max_noise + 1)
        {
            opt.Mode = mode;
            opt.noise_sensitivity = noise;

            if (vpxt_compress(input.c_str(), noise_sense_vec[noise].c_str(),
                speed, bit_rate, opt, comp_out_str, noise, 0, enc_format)== -1)
            {
                fclose(fp);
                record_test_complete(file_index_str, file_index_output_char,
                    test_type);
                return kTestIndeterminate;
            }

            if (test_type != 2)
            {
                tprintf(PRINT_BTH, "\n");
                noise_psnr[noise] = vpxt_psnr(input.c_str(),
                    noise_sense_vec[noise].c_str(), 0, PRINT_BTH, 1, NULL);
                tprintf(PRINT_BTH, "\n");
                file_size[noise] = vpxt_file_size(
                    noise_sense_vec[noise].c_str(), 1);
                tprintf(PRINT_BTH, "\n");
            }

            noise++;
        }

    }

    // Create Compression only stop test short.
    if (test_type == kCompOnly)
    {
        // Compression only run
        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestEncCreated;
    }

    // checks 0v1
    // or
    // checks 0v1 | 1v2 | 2v3 | 3v4 | 4v5 | 5v6
    int n = 0;
    int fail = 0;

    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    while (n < max_noise)
    {
        if (noise_psnr[n] == noise_psnr[n+1] && file_size[n] == file_size[n+1])
        {
            vpxt_formated_print(RESPRT, "Noise %i PSNR %.4f == Noise %i PSNR "
                "%.4f - Failed", n, noise_psnr[n], n + 1, noise_psnr[n+1]);
            tprintf(PRINT_BTH, "\n");
            fail = 1;

        }
        else
        {
            vpxt_formated_print(RESPRT, "Noise %i PSNR %.4f != Noise %i PSNR "
                "%.4f - Passed", n, noise_psnr[n], n + 1, noise_psnr[n+1]);
            tprintf(PRINT_BTH, "\n");
        }

        n++;
    }

    if (noise_psnr[0] <= noise_psnr[max_noise])
    {
        vpxt_formated_print(RESPRT, "Noise 0 PSNR: %.4f <= Noise %i PSNR: "
            "%.4f - Failed", noise_psnr[0], max_noise, noise_psnr[max_noise]);
        tprintf(PRINT_BTH, "\n");
        fail = 1;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Noise 0 PSNR: %.4f > Noise %i PSNR: "
            "%.4f - Passed", noise_psnr[0], max_noise, noise_psnr[max_noise]);
        tprintf(PRINT_BTH, "\n");
    }

    if (delete_ivf)
        for(n = 0; n < max_noise; n++)
            vpxt_delete_files(1, noise_sense_vec[n].c_str());

    if (fail == 0)
    {
        tprintf(PRINT_BTH, "\nPassed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestPassed;
    }
    else
    {
        tprintf(PRINT_BTH, "\nFailed\n");

        fclose(fp);
        record_test_complete(file_index_str, file_index_output_char, test_type);
        return kTestFailed;
    }

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return kTestError;
}