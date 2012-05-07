#include "vpxt_test_declarations.h"

int test_noise_sensitivity(int argc,
                           const char** argv,
                           const std::string &working_dir,
                           const std::string sub_folder_str,
                           int test_type,
                           int delete_ivf,
                           int artifact_detection)
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
    int bitrate = 128;

    //////////// Formatting Test Specific directory ////////////
    std::string cur_test_dir_str;
    std::string file_index_str;
    char main_test_dir_char[255] = "";
    char file_index_output_char[255] = "";

    if (initialize_test_directory(argc, argv, test_type, working_dir, test_dir,
        cur_test_dir_str, file_index_str, main_test_dir_char,
        file_index_output_char, sub_folder_str) == 11)
        return kTestErrFileMismatch;

    int noise_sense_art_det[7];
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

        noise_sense_art_det[i] = artifact_detection;
    }

    ///////////// Open Output File and Print Header ////////////
    std::string text_file_str = cur_test_dir_str + slashCharStr() + test_dir;
    FILE *fp;

    vpxt_open_output_file(test_type, text_file_str, fp);
    vpxt_print_header(argc, argv, main_test_dir_char, cur_test_dir_str,
        test_dir, test_type);

    VP8_CONFIG opt;
    vpxt_default_parameters(opt);

    /////////////////// Use Custom Settings ///////////////////
    if(vpxt_use_custom_settings(argv, argc, input_ver, fp, file_index_str,
        file_index_output_char, test_type, opt, bitrate)
        == kTestIndeterminate)
        return kTestIndeterminate;

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
                noise_sense_vec[noise].c_str(), 0, PRINT_BTH, 1, 0, 0, 0, NULL,
                noise_sense_art_det[noise]);
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
                    noise_sense_vec[noise].c_str(), 0, PRINT_BTH, 1, 0, 0, 0,
                    NULL, noise_sense_art_det[noise]);
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
    int test_state = kTestPassed;
    tprintf(PRINT_BTH, "\n\nResults:\n\n");

    while (n < max_noise)
    {
        if (noise_psnr[n] == noise_psnr[n+1] && file_size[n] == file_size[n+1])
        {
            vpxt_formated_print(RESPRT, "Noise %i PSNR %.4f == Noise %i PSNR "
                "%.4f - Failed", n, noise_psnr[n], n + 1, noise_psnr[n+1]);
            tprintf(PRINT_BTH, "\n");
            test_state = kTestFailed;
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
        test_state = kTestFailed;
    }
    else
    {
        vpxt_formated_print(RESPRT, "Noise 0 PSNR: %.4f > Noise %i PSNR: "
            "%.4f - Passed", noise_psnr[0], max_noise, noise_psnr[max_noise]);
        tprintf(PRINT_BTH, "\n");
    }

    // handle possible artifact
    for(n = 0; n < max_noise; n++){
        if(noise_sense_art_det[n] == kPossibleArtifactFound)
        {
            tprintf(PRINT_BTH, "\nPossible Artifact\n");

            fclose(fp);
            record_test_complete(file_index_str, file_index_output_char,
                test_type);
            return kTestPossibleArtifact;
        }
    }

    if (test_state == kTestPassed)
        tprintf(PRINT_BTH, "\nPassed\n");
    if (test_state == kTestFailed)
        tprintf(PRINT_BTH, "\nFailed\n");

    if (delete_ivf)
        for(n = 0; n < max_noise; n++)
            vpxt_delete_files(1, noise_sense_vec[n].c_str());

    fclose(fp);
    record_test_complete(file_index_str, file_index_output_char, test_type);
    return test_state;
}