// Test.cpp : définit le point d'entrée pour l'application console.
//

#include "StdAfxTestCPlusPlus.h"
 
using namespace std;
using namespace ErrorNameSpace;
using namespace VectorAndMatrixNameSpace;
using namespace WrapperGslCppNameSpace;
using namespace WrapperNloptCppNamespace;
using namespace RegArchLib ;





#ifdef _MSC_VER
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif // _MSC_VER
{
/*ifstream myFile("CACWE.txt", ios::in) ;
int myNObs = 0;
double myRt;
int myd1, myd2, myd3, myd4, myd5;
char myBuf[500];
while (myFile.getline(myBuf, 500))
{
	myNObs++;
}
myFile.close();
myFile.open("CACWE.txt", ios::in);
int kk = 0;
cDVector myYt(myNObs);
cDMatrix myXt(myNObs, 5);
while (myFile.getline(myBuf, 500))
{
	sscanf(myBuf, "%lf %d %d %d %d %d", &myRt, &myd1, &myd2, &myd3, &myd4, &myd5);
	myYt[kk] = myRt*100;
	myXt[kk][0] = myd1;
	myXt[kk][1] = myd2;
	myXt[kk][2] = myd3;
	myXt[kk][3] = myd4;
	myXt[kk][4] = myd5;
	kk++;
}

*/
cConst myConst(1);
cAr	myAr(1);
	myAr.Set(.3, 0);
	//myAr.Set(-.2, 1);

cMa myMa(1);
	myMa.Set(-0.4, 0);
	//	myMa.Set(0.6, 1) ;

//cArfima myArfima(2, 2, .2, 100);
//	myArfima.Set(.3, 0, 0);
//	myArfima.Set(.2, 1, 0);
//	myArfima.Set(.4, 0, 1);
//	myArfima.Set(.5, 1, 1);

cStdDevInMean myStdDevInMean(3);
cVarInMean myVarInMean(.6);
cLinReg myBeta(2);
	myBeta.Set(1, 0, 0);
	myBeta.Set(-3, 0, 1);
bool myLinRegBool = false;

cConstCondVar myConstVar(2.0);

cArch myArch(1);
	myArch.Set(0.4, 0, 0);
	myArch.Set(0.5, 0, 1);

cGarch myGarch(1, 1);
	myGarch.Set(0.1, 0, 0);
	myGarch.Set(0.3, 0, 1);
	myGarch.Set(0.6, 0, 2);


cNormResiduals myNormResid;

cAparch myAparch(1, 1);
	myAparch.Set(0.1, 0, 0); // cste ;
	myAparch.Set(1.3, 0, 1); // Delta ;
	myAparch.Set(0.1, 0, 2); // Arch ;
	myAparch.Set(0.3, 0, 3); // Gamma ;
	myAparch.Set(0.8, 0, 4); // Garch ;


cTarch myTarch(1);
	myTarch.Set(0.1, 0, 0);
	myTarch.Set(0.3, 0, 1);
	myTarch.Set(0.5, 0, 2);

cUgarch myUgarch(false, 2, 1, 1);
	myUgarch.Set(.1, 0, 1);
	myUgarch.Set(.2, 1, 1);
	myUgarch.Set(.2, 0, 2);
	myUgarch.Set(.7, 0, 3);
	bool myUgarchBool = false;

//cFigarch myFigarch(1, 1, 0.5, 100);
//	myFigarch.Set(.1, 0, 0);
//	myFigarch.Set(.1, 0, 1);
//	myFigarch.Set(.8, 0, 2);


cStudentResiduals myStudent(10, true);
	//cSkewtResiduals mySkewt(10, 1.5);
cGedResiduals myGedResiduals(3);
cMixNormResiduals myMixNorm(.5, 1, 2);

cEgarch myEgarch(&myNormResid, 1, 1);
	myEgarch.Set(-0.1217, 0, 1); // cste
	myEgarch.Set(0.06046, 0, 2); // ARCH
	myEgarch.Set(0.985106, 0, 3); // GARCH
	myEgarch.Set(3.198596, 0, 4); // TETA
	myEgarch.Set(1.0, 0, 5); //GAMMA


cCondMean myCondMean;

		myCondMean.AddOneMean(myConst);
	//	myCondMean.AddOneMean(myMa);
	//	myCondMean.AddOneMean(myAr);
	//	myCondMean.AddOneMean(myArfima);
	//	myCondMean.AddOneMean(myStdDevInMean);
	//	myCondMean.AddOneMean(myVarInMean);
	//	myCondMean.AddOneMean(myBeta);
	//	myLinRegBool = true;

	cRegArchModel myModel;

	myModel.SetMean(myCondMean);


		myModel.SetVar(myConstVar);	
	//	myModel.SetVar(myArch);
	//	myModel.SetVar(myGarch);
	//  myModel.SetVar(myEgarch);
	//	myModel.SetVar(myAparch);
	//	myModel.SetVar(myTarch);
	//	myModel.SetVar(myFigarch) ;
	//	myModel.SetVar(myUgarch) ;
	//	myUgarchBool = true;

	  myModel.SetResid(myNormResid);
	//	myModel.SetResid(myStudent) ;
	//	myModel.SetResid(mySkewt);
	//	myModel.SetResid(myMixNorm);
	//	myModel.SetResid(myGedResiduals);



	cout << "Modele : ";
	myModel.Print();
	//	return 0 ;


uint myNSimul = 1000;
cDMatrix myXt = cDMatrix(myNSimul, 2, 1);
	if (myLinRegBool)
	{
		for (uint i = 0; i < myNSimul; i++)
		{
			myXt[i][0] = (double)i/10;
			myXt[i][1] = myXt[i][0]* myXt[i][1];
		}
	}

	cRegArchValue myValue(myNSimul);
	if (myLinRegBool)
		myValue.mXt = myXt;
	else
		myValue.mXt = NULL;

	if (myUgarchBool)
		myValue.mXvt = myXt;
	else
		myValue.mXvt = NULL;


	//cRegArchValue myValue(&myYt, &myXt);
	RegArchSimul(myNSimul, myModel, myValue);


	
cRegArchModel myResModel(myModel);
	myResModel.SetDefaultInitPoint(myValue);


uint myNParam = myModel.GetNParam();

	
cDVector myInitPoint = cDVector(myNParam, 0.0) ;
	myResModel.RegArchParamToVector(myInitPoint);

double myLLH = RegArchLLH(myResModel, myValue);
	cout << "LLH=" << myLLH << endl;


sGSLMultiMinResult myResGSLStruct;
sGSLMultiMinAlgoParam myGSLAlgoParam;
	myGSLAlgoParam.mAlgoType = eSimplexNMTwo;
	myGSLAlgoParam.mNMaxIter = 1000;
	myGSLAlgoParam.mStepSize = 0.001;
	myGSLAlgoParam.mStopValue = 0.001;
	myGSLAlgoParam.mTol = 0.001;
	myGSLAlgoParam.mVerbose = false;
	myGSLAlgoParam.mNMaxSeconds = 20;


	RegArchEstim(myModel, myValue, myResGSLStruct, myResModel, &myInitPoint, &myGSLAlgoParam);
	cout << endl << myResGSLStruct.mConvergenceComment << endl;
	myResModel.Print();

cDMatrix myTable;
	RegArchStatTable(myResModel, myValue, myTable);
	string* myParamName = new string[myNParam];
	myModel.GetParamName(myParamName);
	cout << "Parameter\tValue\tStd.Err\t t stat\tP Value" << endl;
	for (uint i = 0; i < myTable.GetNRow(); i++)
		cout << myParamName[i] << "\t" << myTable[i][0] << "\t" << myTable[i][1] << "\t" << myTable[i][2] << "\t" << myTable[i][3] << endl;

cNLOPTResult myResNloptStruct(myNParam);
cNLOPTAlgoParam myNloptAlgoParam = cNLOPTAlgoParam();
	myNloptAlgoParam.mAlgo = NLOPT_LD_LBFGS;
	myNloptAlgoParam.mMaxComputeTime = 30;
	myNloptAlgoParam.mMinimisation = false;
	myNloptAlgoParam.mStopVal = 1e299;
	myNloptAlgoParam.mfTol = 1e-5;
	myNloptAlgoParam.mxTol = 1e-5;
	myNloptAlgoParam.mVerbose = false;

	RegArchEstim(myModel, myValue, myResNloptStruct, myResModel, &myInitPoint, &myNloptAlgoParam);

	cout << endl << myResNloptStruct.mConvergenceString << endl << "LLH = " << myResNloptStruct.mFunctValue << endl << "Temps : " << myResNloptStruct.mComputeTime << " s" << endl;
	myResModel.Print();

	RegArchStatTable(myResModel, myValue, myTable);

	myModel.GetParamName(myParamName);
	cout << "Parameter\tValue\tStd.Err\t t stat\tP Value" << endl;
	for (uint i = 0; i < myTable.GetNRow(); i++)
		cout << myParamName[i] << "\t" << myTable[i][0] << "\t" << myTable[i][1] << "\t" << myTable[i][2] << "\t" << myTable[i][3] << endl;
	
	delete[] myParamName;

	return 0;

}
