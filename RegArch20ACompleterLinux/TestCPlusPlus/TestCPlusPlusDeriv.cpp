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
int _wtmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif // _MSC_VER
{

	cout.precision(4);

	/*
	cNormResiduals myGauss;
	cGarch myGarch1(2, 1);
		myGarch1.Set(0.1, 0, 0);
		myGarch1.Set(0.1, 0, 1);
		myGarch1.Set(0.2, 1, 1);
		myGarch1.Set(0.6, 0, 2);
	cAr myAr1(2);
		myAr1.Set(0.2, 0, 0);
		myAr1.Set(-0.3, 1, 0);
	cConst myConst1(5.0);
	cStudentResiduals mySt1(10);
	cRegArchModel myModel0;
		myModel0.SetResid(mySt1);
		myModel0.AddOneMean(myAr1);
		myModel0.AddOneMean(myConst1);
		myModel0.SetVar(myGarch1);
		myModel0.Print();
	cDVector myXX(myModel0.GetNParam());
		myModel0.RegArchParamToVector(myXX);
		myXX.Print();
	cRegArchModel myModel01 = myModel0;
		myModel01.Print();

		return 0;
	*/
	/*
	double myX0 = 0.2;
	double myp = .5;
	double mySigma1 = 1;
	double mySigma2 = 2;
	double myh0 = 0.0001;

	double myLogDens0 = MixNormLogDensity(myX0, myp, mySigma1, mySigma2);
	double myLogDens1 = MixNormLogDensity(myX0*(1+myh0), myp, mySigma1, mySigma2);
	double myLogDens2 = MixNormLogDensity(myX0*(1 + 2*myh0), myp, mySigma1, mySigma2);
	double myDiffLogDensNum = (myLogDens1 - myLogDens0) / (myh0*myX0);
		cout << "Derivee num: " << myDiffLogDensNum << endl;
	double myDiffLogDens = MixNormDiffLogDensity(myX0, myp, mySigma1, mySigma2);
		cout << "Derivee the: " << myDiffLogDens << endl;
	double myDiff2LogDensNum = (myLogDens2 - 2*myLogDens1 + myLogDens0) / (pow(myh0*myX0,2));
	cout << "Derivee seconde  num: " << myDiff2LogDensNum << endl;
	double myDiff2LogDens = MixNormDiff2LogDensity(myX0, myp, mySigma1, mySigma2);
	cout << "Derivee seconde the: " << myDiff2LogDens << endl;
	cDVector myGradLogDens(3), myGradLogDensNum(3);
		myLogDens1 = MixNormLogDensity(myX0, myp*(1 + myh0), mySigma1, mySigma2);
		myGradLogDensNum[0] = (myLogDens1 - myLogDens0) / (myh0*myp);
		myLogDens1 = MixNormLogDensity(myX0, myp, mySigma1*(1 + myh0), mySigma2);
		myGradLogDensNum[1] = (myLogDens1 - myLogDens0) / (myh0*mySigma1);
		myLogDens1 = MixNormLogDensity(myX0, myp, mySigma1, mySigma2*(1 + myh0));
		myGradLogDensNum[2] = (myLogDens1 - myLogDens0) / (myh0*mySigma2);
		MixNormGradLogDensity(myX0, myp, mySigma1, mySigma2, myGradLogDens);
		cout << "Gradient num: " << endl << myGradLogDensNum << endl;
		cout << "Gradient the: " << endl << myGradLogDens << endl;
	cDMatrix myHessBeta(3, 3), myHessBetaNum(3, 3);
	double myLogDens3;
		MixNormHessLogDensity(myX0, myp, mySigma1, mySigma2, myHessBeta);
		myLogDens1 = MixNormLogDensity(myX0, myp*(1 + myh0), mySigma1, mySigma2);
		myLogDens2 = MixNormLogDensity(myX0, myp*(1 + 2*myh0), mySigma1, mySigma2);
		myHessBetaNum[0][0] = (myLogDens2 - 2 * myLogDens1 + myLogDens0) / (pow(myh0*myp, 2));
		myLogDens2 = MixNormLogDensity(myX0, myp, mySigma1*(1 + myh0), mySigma2);
		myLogDens3 = MixNormLogDensity(myX0, myp*(1+myh0), mySigma1*(1+myh0), mySigma2);
		myHessBetaNum[0][1] = myHessBetaNum[1][0] = (myLogDens3 - myLogDens2 -  myLogDens1 + myLogDens0) / (pow(myh0, 2)*myp*mySigma1);
		myLogDens2 = MixNormLogDensity(myX0, myp, mySigma1, mySigma2*(1 + myh0));
		myLogDens3 = MixNormLogDensity(myX0, myp*(1+myh0), mySigma1, mySigma2*(1 + myh0));
		myHessBetaNum[0][2] = myHessBetaNum[2][0] = (myLogDens3 - myLogDens2 - myLogDens1 + myLogDens0) / (pow(myh0, 2)*myp*mySigma2);
		myLogDens1 = MixNormLogDensity(myX0, myp, mySigma1*(1 + myh0), mySigma2);
		myLogDens2 = MixNormLogDensity(myX0, myp, mySigma1*(1 + 2 * myh0), mySigma2);
		myHessBetaNum[1][1] = (myLogDens2 - 2 * myLogDens1 + myLogDens0) / (pow(myh0*mySigma1, 2));
		myLogDens2 = MixNormLogDensity(myX0, myp, mySigma1, mySigma2*(1 + myh0));
		myLogDens3 = MixNormLogDensity(myX0, myp, mySigma1*(1+myh0), mySigma2*(1 + myh0));
		myHessBetaNum[1][2] = myHessBetaNum[2][1] = (myLogDens3 - myLogDens2 - myLogDens1 + myLogDens0) / (pow(myh0, 2)*mySigma1*mySigma2);
		myLogDens1 = MixNormLogDensity(myX0, myp, mySigma1, mySigma2*(1 + myh0));
		myLogDens2 = MixNormLogDensity(myX0, myp, mySigma1, mySigma2*(1 + 2 * myh0));
		myHessBetaNum[2][2] = (myLogDens2 - 2 * myLogDens1 + myLogDens0) / (pow(myh0*mySigma2, 2));
		cout << "Hessien num:" << endl << myHessBetaNum << endl;
		cout << "Hessien the:" << endl << myHessBeta << endl;
	cDVector myGradDiffLog(3);
		MixNormGradDiffLogDens(myX0, myp, mySigma1, mySigma2, myGradDiffLog);
	cDVector myGradDiffLogNum(3);
		myLogDens1 = MixNormLogDensity(myX0*(1+myh0), myp, mySigma1, mySigma2);
		myLogDens2 = MixNormLogDensity(myX0, myp*(1 + myh0), mySigma1, mySigma2);
		myLogDens3 = MixNormLogDensity(myX0*(1+myh0), myp*(1 + myh0), mySigma1, mySigma2);
		myGradDiffLogNum[0] = (myLogDens3 - myLogDens2 - myLogDens1 + myLogDens0)/(pow(myh0,2)*myX0*myp);
		myLogDens2 = MixNormLogDensity(myX0, myp, mySigma1*(1 + myh0), mySigma2);
		myLogDens3 = MixNormLogDensity(myX0*(1 + myh0), myp, mySigma1*(1 + myh0), mySigma2);
		myGradDiffLogNum[1] = (myLogDens3 - myLogDens2 - myLogDens1 + myLogDens0) / (pow(myh0, 2)*myX0*mySigma1);
		myLogDens2 = MixNormLogDensity(myX0, myp, mySigma1, mySigma2*(1 + myh0));
		myLogDens3 = MixNormLogDensity(myX0*(1 + myh0), myp, mySigma1, mySigma2*(1 + myh0));
		myGradDiffLogNum[2] = (myLogDens3 - myLogDens2 - myLogDens1 + myLogDens0) / (pow(myh0, 2)*myX0*mySigma2);
		cout << "Grad DiffLogDens Num:" << endl << myGradDiffLogNum << endl;
		cout << "Grad DiffLogDens the:" << endl << myGradDiffLog << endl;
	*/

	cConst myConst(10);

	cAr	myAr(2);
	myAr.Set(.8, 0);
	myAr.Set(-.2, 1);

	cMa myMa(1);
	myMa.Set(0.4, 0);
	//	myMa.Set(0.6, 1) ;

//	cArfima myArfima(2, 2, 0.3, 3);
//	myArfima.Set(.3, 0, 0);
//	myArfima.Set(-.2, 1, 0);
//	myArfima.Set(-.4, 0, 1);
//	myArfima.Set(.5, 1, 1);

	cStdDevInMean myStdDevInMean(.6);
	cVarInMean myVarInMean(.6);
	cLinReg myBeta(2);
	myBeta.Set(3, 0, 0);
	myBeta.Set(-0.5, 0, 1);
	bool myLinRegBool = false;

	cConstCondVar myConstVar(2.0);

	cArch myArch(1);
	myArch.Set(0.4, 0, 0);
	myArch.Set(0.5, 0, 1);

	cGarch myGarch(1, 1);
	myGarch.Set(0.1, 0, 0);
	myGarch.Set(0.1, 0, 1);
	myGarch.Set(0.8, 0, 2);


	cNormResiduals myNormResid;

	cAparch myAparch(1, 1);
	myAparch.Set(0.1, 0, 0); // cste ;
	myAparch.Set(1.3, 0, 1); // Delta ;
	myAparch.Set(0.1, 0, 2); // Arch ;
	myAparch.Set(0.3, 0, 3); // Gamma ;
	myAparch.Set(0.8, 0, 4); // Garch ;


	cTarch myTarch(1);
	myTarch.Set(0.1, 0, 0);
	myTarch.Set(0.8, 0, 1);
	myTarch.Set(0.7, 0, 2);

	cUgarch myUgarch(false, 2, 1, 1);
	myUgarch.Set(.1, 0, 1);
	myUgarch.Set(.2, 1, 1);
	myUgarch.Set(.2, 0, 2);
	myUgarch.Set(.7, 0, 3);
	bool myUgarchBool = false;

//	cFigarch myFigarch(1, 1, 0.3, 50);
//	myFigarch.Set(.1, 0, 0);
//	myFigarch.Set(.1, 0, 1);
	//		myFigarch.Set(.2, 1, 1);
//s	myFigarch.Set(.5, 0, 2);
	//		myFigarch.Set(.1, 1, 2);


	cStudentResiduals myStudent(5, true);
	cGedResiduals myGedResiduals(1.5);
	cMixNormResiduals myMixNorm(.5, 1, 2);

	cEgarch myEgarch(&myNormResid, 1, 1);
	//cEgarch myEgarch(&myStudent, 1, 1);	myEgarch.Set(-0.1217, 0, 1); // cste
	//cEgarch myEgarch(&myMixNorm, 1, 1);	myEgarch.Set(-0.1217, 0, 1); // cste
	//cEgarch myEgarch(&myGedResiduals, 1, 1);	myEgarch.Set(-0.1217, 0, 1); // cste
	myEgarch.Set(0.06046, 0, 2); // ARCH
	myEgarch.Set(0.985106, 0, 3); // GARCH
	myEgarch.Set(3.198596, 0, 4); // TETA
	myEgarch.Set(1.0, 0, 5); //GAMMA


	cCondMean myCondMean;

	//	myCondMean.AddOneMean(myConst);
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
	// myModel.SetVar(myGarch);
	//	myModel.SetVar(myEgarch);
	//	myModel.SetVar(myAparch);
	//	myModel.SetVar(myTarch);
//		myModel.SetVar(myFigarch) ;
	//	myModel.SetVar(myUgarch) ;
	//	myUgarchBool = true;

	 myModel.SetResid(myNormResid);
	//	myModel.SetResid(myStudent) ;
	//	myModel.SetResid(myMixNorm);
	//	myModel.SetResid(myGedResiduals);

	cout << "Modele : " << endl;
	myModel.Print();

	uint myNSimul = 10;
	cDMatrix myXt = cDMatrix(myNSimul, 2, 1);
	if (myLinRegBool)
	{
		for (uint i = 0; i < myNSimul; i++)
		{
			myXt[i][0] = (double)(i + 1);
			myXt[i][1] = (double)(i + 1)*(i + 1);
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

	RegArchSimul(myNSimul, myModel, myValue);
//		cout << myValue << endl;

	cRegArchGradient myGradient = cRegArchGradient(&myModel);
	cRegArchGradient myGradNum = cRegArchGradient(&myModel);
	cRegArchHessien myHessien = cRegArchHessien(&myModel);
	cRegArchHessien myHessNum = cRegArchHessien(&myModel);
	uint myNParam = myModel.GetNParam();
	uint myNDistrParam = myModel.mResids->GetNParam();
	uint myNMeanVarParam = myNParam - myNDistrParam;
	cDVector myGradlt(myNParam);
	cDMatrix myHesslt(myNParam, myNParam);
	double myh = 0.001;
	cDVector myVectParam(myNParam);
	cNumericDerivative myNumDeriv = cNumericDerivative(myNParam, myNDistrParam, myh, myValue.mYt, &myValue.mXt, &myValue.mXvt);
	uint myDate = 4;
	cRegArchValue myAuxValue(myValue);
	myModel.mVar->UpdateProxyVarParameters();
	if (myModel.mMean != NULL)
		myModel.mMean->UpdateProxyMeanParameters();
	for (uint t = 0; t < myDate; t++)
	{
		myModel.ComputeGradAndHess(t, myValue, myGradient, myHessien, myModel.mResids);
		myModel.NumericComputeGradAndHess(t, myAuxValue, myGradNum, myHessNum, myModel.mResids, myNumDeriv);
		cout << "t = " << t << endl;
		cout << "------" << endl;
		cout << "Grad Mu" << endl;
		cout << myGradient.mCurrentGradMu << endl;
		cout << "Grad Mu Numerique" << endl;
		cout << myGradNum.mCurrentGradMu << endl;
		cout << "Grad Var" << endl;
		cout << myGradient.mCurrentGradVar << endl;
		cout << "Grad Var Numerique" << endl;
		cout << myGradNum.mCurrentGradVar << endl << endl;
		cout << "Grad Sigma" << endl;
		cout << myGradient.mCurrentGradSigma << endl;
		cout << "Grad Sigma Numerique" << endl;
		cout << myGradNum.mCurrentGradSigma << endl << endl;
		cout << "Grad Eps" << endl;
		cout << myGradient.mCurrentGradEps << endl;
		cout << "Grad Eps Numerique" << endl;
		cout << myGradNum.mCurrentGradEps << endl << endl;
		cout << "Grad Dens" << endl;
		cout << myGradient.mCurrentGradLogDens << endl;
		cout << "Grad Dens Numerique" << endl;
		cout << myGradNum.mCurrentGradLogDens << endl << endl;
		cout << "Grad diff LogDens" << endl;
		cout << myGradient.mCurrentDiffLogDensity << endl;
		cout << "Grad diff LogDens Numerique" << endl;
		cout << myGradNum.mCurrentDiffLogDensity << endl << endl;
		cout << "Hessien Mu" << endl;
		cout << myHessien.mCurrentHessMu << endl;
		cout << "Hessien numérique Mu" << endl;
		cout << myHessNum.mCurrentHessMu << endl;
		cout << "Hessien Var" << endl;
		cout << myHessien.mCurrentHessVar << endl;
		cout << "Hessien numérique Var" << endl;
		cout << myHessNum.mCurrentHessVar << endl;
		cout << "Hessien Sigma" << endl;
		cout << myHessien.mCurrentHessSigma << endl;
		cout << "Hessien numérique Sigma" << endl;
		cout << myHessNum.mCurrentHessSigma << endl;
		cout << "Hessien Eps" << endl;
		cout << myHessien.mCurrentHessEps << endl;
		cout << "Hessien numérique Eps" << endl;
		cout << myHessNum.mCurrentHessEps << endl;
		cout << "Grad Diff LogDens" << endl;
		cout << myHessien.mCurrentGradDiffLogDensity << endl;
		cout << "Grad Diff LogDens numérique" << endl;
		cout << myHessNum.mCurrentGradDiffLogDensity << endl;
		cout << "Hessien Dens" << endl;
		cout << myHessien.mCurrentHessDens << endl;
		cout << "Hessien numérique Dens" << endl;
		cout << myHessNum.mCurrentHessDens << endl;
		myGradient.Update();
		myHessien.Update();
		myGradNum.Update();
		myHessNum.Update();
		/*
		uint myNPast = myGradNum.GetNPast();
					if (myNPast > 0)
					{
						for (int k = 0 ; k < myNPast ; k++)
						{
							myGradNum.mGradHt[k] = myGradient.mGradHt[k];
							myGradNum.mGradMt[k] = myGradient.mGradMt[k];
							myGradNum.mGradEpst[k] = myGradient.mGradEpst[k];
							myHessNum.mHessHt[k] = myHessien.mHessHt[k];
							myHessNum.mHessMt[k] = myHessien.mHessMt[k];
							myHessNum.mHessEpst[k] = myHessien.mHessEpst[k];
						}
					}
					myHessNum.mCurrentHessVar = myHessien.mCurrentHessVar;
					myHessNum.mCurrentHessMu = myHessien.mCurrentHessMu;
					myHessNum.mCurrentHessSigma = myHessien.mCurrentHessSigma;
					myHessNum.mCurrentHessDens = myHessien.mCurrentHessDens;
					myHessNum.mCurrentGradDiffLogDensity = myHessien.mCurrentGradDiffLogDensity;


					myAuxValue.mMt[t] = myValue.mMt[t];
					myAuxValue.mHt[t] = myValue.mHt[t];
					myAuxValue.mUt[t] = myValue.mUt[t];
					myAuxValue.mEpst[t] = myValue.mEpst[t];
		*/
	}
	return 0;
}
