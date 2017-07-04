#include "extcode.h"
#pragma pack(push)
#pragma pack(1)

#ifdef __cplusplus
extern "C" {
#endif

double __cdecl Labdll_Imod(double Imod, double K4, double K3, double K2, 
	double K1, double K0, double start, double end);
void __cdecl Labdll_PolyFit(double X[], double Y[], int32_t polynomialOrder, 
	double PolynomialCoefficients[], double *mse, int32_t len_x, int32_t len_y, 
	int32_t len_coef);
int32_t __cdecl labdll_ExcelGET_ED(char FilePath[], LVBoolean *haveA2, 
	uint8_t A0[], int32_t len_A0, uint8_t A2[], int32_t len_A2);
int32_t __cdecl Labdll_FQC_report(char report_path[], char report_no[], 
	char PN[], char SNstr[], double AOP[], int32_t len_AOP, LVBoolean fillFlag[], 
	int32_t len_FillFlag, double ER[], int32_t len_ER, uint8_t fillERFlag);
int32_t __cdecl Labdll_Version(void);

long __cdecl LVDLLStatus(char *errStr, int errStrLen, void *module);

#ifdef __cplusplus
} // extern "C"
#endif

#pragma pack(pop)

