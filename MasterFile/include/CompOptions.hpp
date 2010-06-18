//------------------------------------------------------------------------------
//
//  Copyright (c) 1999-2003  On2 Technologies Inc.  All Rights Reserved.
//
//------------------------------------------------------------------------------
//
//  $Workfile: CompOptions.hpp$
//  $Date: 9/8/2005 11:16:45 AM$
//
//------------------------------------------------------------------------------

#if !defined(COMPOPTIONS_HPP)
#define COMPOPTIONS_HPP


typedef enum
{
    USAGE_STREAM_FROM_SERVER    = 0x0,  //  INTER prediction, (0,0) motion vector implied.
    USAGE_LOCAL_FILE_PLAYBACK   = 0x1  //  INTER prediction, (0,0) motion vector implied.
} END_USAGE;


typedef enum
{
    MODE_REALTIME       = 0x0,
    MODE_GOODQUALITY    = 0x1,
    MODE_BESTQUALITY    = 0x2,
    MODE_FIRSTPASS      = 0x3,
    MODE_SECONDPASS     = 0x4,
    MODE_SECONDPASS_BEST = 0x5
} MODE;


typedef struct
{
    unsigned int FourCC;  // must be set to '17PV'
    unsigned int ModeSet; // must be set to 1
    unsigned int Used;    // must be set to 512

    // width of source
    int Width;

    // height of source
    int Height;

    // frame rate numerator/denominator
    int RateNum;
    int RateDenom;

    // mode ->
    //(0)=Realtime/Live Encoding. This mode is optimized for realtim encoding (for example, capturing
    //    a television signal or feed from a live camera). ( speed setting controls how fast )
    //(1)=Good Quality Fast Encoding. The encoder balances quality with the amount of time it takes to
    //    encode the output. ( speed setting controls how fast )
    //(2)=One Pass - Best Quality. The encoder places priority on the quality of the output over encoding
    //    speed. The output is compressed at the highest possible quality. This option takes the longest
    //    amount of time to encode. ( speed setting ignored )
    //(3)=Two Pass - First Pass. The encoder generates a file of statistics for use in the second encoding
    //    pass. ( speed setting controls how fast )
    //(4)=Two Pass - Second Pass. The encoder uses the statistics that were generated in the first encoding
    //    pass to create the compressed output. ( speed setting controls how fast )
    //(5)=Two Pass - Second Pass Best.  The encoder uses the statistics that were generated in the first
    //    encoding pass to create the compressed output using the highest possible quality, and taking a
    //    longer amount of time to encode.. ( speed setting ignored )
    MODE Mode;

    // speed determines how fast encoder works -> this is relevant only for
    // good quality modes and realtime encode modes..
    // for realtime quality the settings are 0 - 16
    // for good quality the settings can be 0 - 6
    int Speed;

    // EndUsage->
    //(0)Stream From A Server (CBR). Constant bitrate output suited for streaming over a TCP network.
    //(1)Local File Playback (VBR). Variable bitrate output for local playback or download and play.
    END_USAGE EndUsage;

    // Sharpness - 0-7 ( 0 recommended) - 0 fewest artifacts, 7 - most detail
    int Sharpness;

    // maintain interlacing
    int Interlaced;

    // auto keyframing enabled
    int AutoKeyFrameEnabled;

    // maximum number of frames between key frames
    int ForceKeyFrameEvery;

    // temporal noise reduction 0 -> off, 6 -> Very Strong denoiser
    int NoiseSensitivity;

    // when set proper parsing of the bitstream can occur after a
    // frame drop.   The frames might use the wrong data and artifacts can
    // occur, but may be localized to areas of high motion.
    int ErrorResilientMode;

    // bitrate in kilobits ( 1 kilobit = 1 bits)
    int TargetBitRate;

    // the encoder maintains a model of the decoders buffer fullness so that
    // it can maintain smooth playback during streaming.  It uses the
    // following parms to maintain this buffering.  Note these parms are
    // only relevant for streaming modes

    // The initial encoder buffer level
    int StartingBufferLevel;

    // The buffer level target we strive to reach / maintain.
    int OptimalBufferLevel;

    // The maximum encoder buffer level
    int MaximumBufferLevel;

    // use a fixed quality level
    int FixedQ;

    // The best allowed frame quality, 0-63. Set to a higher value if you don't want
    // to waste bits on encoding noise.
    int BestAllowedQ;

    // worst allowed frame quality (higher is worse) 0-63
    int QuantizerMax;

    // allow compressor to drop frames.
    int AllowDF;

    // Buffer fullness watermark for forced drop frames.
    int DropFramesWaterMark;

    // allow compressor to change internal size of compressed data as a datarate control
    int AllowSpatialResampling;

    // Buffer fullness watermark for downwards spacial re-sampling
    int ResampleDownWaterMark;

    // Buffer fullness watermark where returning to larger image size is consdered
    int ResampleUpWaterMark;

    // 0-100 set to something less than 100 to try and reserve bits for a tough section
    // that might come later
    int UnderShootPct;

    // The highest datarate (in kilobits/second) that can be streamed, and also the
    // highest datarate that the encoder will allow, no matter how difficult the
    // section is to encode.
    int MaxAllowedDatarate;

    // Bias -> 0- 100 CBR vs VBR ( 0 means cbr, 100 means vbr)
    int TwoPassVBRBias;

    // max datarate to shoot for on any section
    int TwoPassVBRMaxSection;

    // min datarate to shoot for on any section
    int TwoPassVBRMinSection;

    // delete the first pass file after use.. (0 no, 1 yes)
    int DeleteFirstPassFile;

    // name of first pass file
    char FirstPassFile[512];

    // name of settings file
    char SettingsFile[512];

    // root directory
    char RootDirectory[512];

} CompOptions;

#endif
