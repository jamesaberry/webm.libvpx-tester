
#ifndef TOOLS_H_GUARD
#define TOOLS_H_GUARD

int ComprIVF2IVF(int argc, char *argv[], string WorkingDir);
int CopyIVF2IVF(int argc, char *argv[], string WorkingDir);
int DecIVF2IVF(int argc, char *argv[]);
int DecIVF2Raw(int argc, char *argv[]);
int IVFDataRateTool(int argc, char *argv[]);
int IVFPSNRrun(int argc, char *argv[]);
int IVFCheckPBMrun(int argc, char *argv[]);
int compareIVF(int argc, char *argv[]);
int RawDataIVF(char *input, char *output);
int WriteIndividualFramesOut(int argc, char *argv[]);
int IVF2Raw(char *inputFile, char *outputDir);
int Raw2IVF(int argc, char *argv[]);
int CutIVFTool(int argc, char *argv[]);
int CropRawIVFTool(int argc, char *argv[]);
int vpxt_paste_ivf(int argc, char *argv[]);
int CombineIndvFrames(int argc, char *argv[]);
int Playvpxt_compare_ivf(int argc, char *argv[]);
int PlayDecIVF(int argc, char *argv[]);
int DecoderCheck(int argc, char *argv[]);
int IVFParseandDelete(char *DirName);
int DeleteAllIVFFiles(int argc, char *argv[]);
int TxtParseandCopy(char *DirName, char *BaseOutputDir, int BaseInputStrLength);
int CopyAllTxtFiles(int argc, char *argv[]);
int SideBySideText(int argc, char *argv[]);
int CompressionEquiv(int argc, char *argv[], string WorkingDir);
int ConvertParmFileToIVFenc(char *input, char *output);
int CompMatchesIVFenc(int argc, char *argv[]);
int compare_code_coverage(int argc, char *argv[]);
int FormatCodeCoverageFile(int argc, char *argv[]);
int TestVectorIndex(int argc, char *argv[]);
int APICOMPRESS(int argc, char *argv[]);
int APIDECOMPRESS(int argc, char *argv[]);
int IVFDECtest_vector_test(int argc, char *argv[]);
int ArrayCovFailListToFullList(int argc, char *argv[]);
int ArrayCovSummaryFile(int argc, char *argv[]);
int WinMemMonFormat(int argc, char *argv[]);
int WinMemMonGraph(int argc, char *argv[]);
int CreateRandParFile(int argc, char *argv[]);
int RunIVFDec(int argc, char *argv[]);
int RunIVFEnc(int argc, char *argv[]);
int SolveQuad();
int PrintCPUInfo();
#endif