#include "StdAfxRegArchLib.h"
/*!
	\file cEgarch.cpp
	\brief sources for class cEgarch methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {

	cEgarch::cEgarch(int theNArch, int theNGarch)
	:cAbstCondVar(eEgarch)  // call constructor of cAbstCondVar with type eEgarch
	{
		mvEspAbsEpsilont = 2/SQRT_2_PI  ; // Bon seulement pour la loi normale
		mvCste = 0.0L ;
		mvArch.ReAlloc(theNArch) ;
		mvGarch.ReAlloc(theNGarch) ;
		mvTeta=0;
		mvGamma=0;
		MESS_CREAT("cEgarch") ;
	}

	cEgarch::cEgarch(cAbstResiduals* theResiduals, int theNArch, int theNGarch)
	:cAbstCondVar(eEgarch)
	{
		if (theResiduals != NULL)
			mvEspAbsEpsilont = theResiduals->ComputeEspAbsEps() ;
		mvCste = 0.0L ;
		mvArch.ReAlloc(theNArch) ;
		mvGarch.ReAlloc(theNGarch) ;
		mvTeta=0;
		mvGamma=0;
		MESS_CREAT("cEgarch") ;
	}

	cEgarch::cEgarch(const cEgarch& theEgarch)
	:cAbstCondVar(eEgarch) 
	{	*this = theEgarch;
		MESS_CREAT("cEgarch");
	}

	cEgarch::~cEgarch()
	{
		mvArch.Delete() ;
		mvGarch.Delete() ;
		MESS_DESTR("cEgarch") ;
	}

	/*!
	 * \fn cAbstCondVar* cEgarch::PtrCopy()
	 */

/*	cAbstCondVar* cEgarch::PtrCopy() const
	{
		//		 cConstCondVar *myConstCondVar = new cConstCondVar(*this);
		//		 return myConstCondVar;
		return cAbstCondVarPtrCopy<cEgarch>();
	}
*/
	void cEgarch::Delete(void)
	{	mvArch.Delete() ;
		mvGarch.Delete() ;
	}

	void cEgarch::ReAlloc(const uint theSize, const uint theNumParam)
	{
		if (theNumParam == 1)
			mvArch.ReAlloc(theSize) ;
		else
			mvGarch.ReAlloc(theSize) ;
	}

	void cEgarch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{
		if (theNumParam == 0)
			mvArch.ReAlloc((int)theVectParam[0]) ;
		else
			mvGarch.ReAlloc((int)theVectParam[0]) ;
	}

#ifndef _RDLL_
	void cEgarch::Print(ostream& theOut) const
	{
		theOut << "EGARCH(" << mvArch.GetSize() << ", " << mvGarch.GetSize() << ") model:" << endl ;
		theOut << "Const=" << mvCste << endl ;
	uint i ;
		for (i = 0 ; i < mvArch.GetSize() ; i++)
			theOut << "ARCH[" << i+1 << "]=" << mvArch[i] << endl ;
		for (i = 0 ; i < mvGarch.GetSize() ; i++)
			theOut << "GARCH[" << i+1 << "]=" << mvGarch[i] << endl ;
		theOut << "Teta=" << mvTeta << endl ;
		theOut << "Gamma=" << mvGamma << endl ;
	}
#else
	void cEgarch::Print(void)
	{
		Rprintf("EGARCH(%d, %d) model with:\n", mvArch.GetSize(), mvGarch.GetSize());
		Rprintf("Const=%f\n", mvCste);
	uint i;
		for (i = 0; i < mvArch.GetSize(); i++)
			Rprintf("ARCH[%d]=%f\n", i + 1, mvArch[i]);
		for (i = 0; i < mvGarch.GetSize(); i++)
			Rprintf("GARCH[%d]=%f\n", i + 1, mvGarch[i]);
		Rprintf("Teta=%f\n", mvTeta);
		Rprintf("Gamma=%f\n", mvGamma);
	}
#endif // _RDLL_

	void cEgarch::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvCste = log(theVar) ;
	uint i ;
		for (i = 0 ; i < mvArch.GetSize() ; i++)
			mvArch[i] = 0.0 ;
		for (i = 0 ; i < mvGarch.GetSize() ; i++)
			mvGarch[i] = 0.0;
		mvTeta = mvGamma = 0.0 ;
	}

	void cEgarch::SetDefaultInitPoint(cRegArchValue& theValue)
	{
	double myVar;
		theValue.ComputeVar(myVar);
		mvCste = log(myVar);
	uint i;
		for (i = 0; i < mvArch.GetSize(); i++)
			mvArch[i] = 0.0;
		for (i = 0; i < mvGarch.GetSize(); i++)
			mvGarch[i] = 0.0;
		mvTeta = mvGamma = 0.0;
	}

	void cEgarch::Set(const cDVector& theVectParam, const uint theNumParam)
	{
		switch (theNumParam)
		{	case 0 :
				mvEspAbsEpsilont = theVectParam[0] ;
			break ;
			case 1 :
				mvCste = theVectParam[0] ;
			break ;
			case 2 :
				mvArch = theVectParam ;
			break ;
			case 3:
				mvGarch = theVectParam ;
			case 4:
				mvTeta = theVectParam[0];
			default :
				mvGamma = theVectParam[0];
			break ;
		}
	}

	void cEgarch::Set(const double theValue, const uint theIndex, const uint theNumParam) 
	{
	uint mySize ;
		switch (theNumParam)
		{	case 0 :
				mvEspAbsEpsilont = theValue ;
			break ;
			case 1 :
				mvCste = theValue ;
			break ;
			case 2 :
			{	mySize = mvArch.GetSize() ;
				if (mySize < theIndex)
					mvArch.ReAlloc(theIndex+1) ;
				mvArch[theIndex]= theValue ;
			}
			break ;
			case 3 :
			{	mySize = mvGarch.GetSize() ;
				if (mySize < theIndex)
					mvGarch.ReAlloc(theIndex+1) ;
				mvGarch[theIndex]= theValue ;
			}
			case 4:
				mvTeta = theValue ;
			break ;

			case 5:
				mvGamma = theValue;
			break ;

			default :
			break ;
		}
	}

	double  cEgarch::Get(const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{	case 0 :
				return mvCste ;
			break ;		
			case 1 :
				return mvArch[theIndex] ;
			break ;
			case 2 :
				return mvGarch[theIndex] ;
			break ;
			case 3 :
				return mvTeta ;
			break ;
			case 4 :
				return mvGamma ;
			break ;
		}
	}

	cDVector& cEgarch::Get(const uint theNumParam)
	{
	cDVector* myAux;
		switch (theNumParam)
		{
		case 0:
			myAux = new cDVector(1, mvCste);
			return *myAux;
			break;
		case 1:
			return mvArch;
			break;
		case 2:
			return mvGarch;
			break;
		case 3:
			myAux = new cDVector(1, mvTeta);
			return *myAux;
			break;
		case 4:
			myAux = new cDVector(1, mvGamma);
			return *myAux;
			break;
		}
	}

	cEgarch& cEgarch::operator =(const cEgarch& theSrc)
	{
		mvArch = theSrc.mvArch;
		mvGarch = theSrc.mvGarch;
		mvCste = theSrc.mvCste;
		mvGamma = theSrc.mvGamma;
		mvTeta = theSrc.mvTeta;
		mvEspAbsEpsilont = theSrc.mvEspAbsEpsilont;
		return *this ;
	}

	double cEgarch::ComputeVar(uint theDate, const cRegArchValue& theValue) const
	{
	uint myp = mvArch.GetSize();
	uint myq = mvGarch.GetSize() ;
	double myRes = mvCste ;
		for (uint i = 1 ; i <= MIN(myp, theDate) ; i++)
			myRes += mvArch[i-1] *( mvTeta*theValue.mEpst[theDate-i] + mvGamma * (fabs(theValue.mEpst[theDate-i])- mvEspAbsEpsilont)) ;
		for (uint j = 1 ; j <= MIN(myq, theDate) ; j++)
			myRes += mvGarch[j-1] * log(theValue.mHt[theDate-j]) ;

		return exp(myRes) ;
	}

	uint cEgarch::GetNParam(void) const 
	{
		return mvArch.GetSize() + mvGarch.GetSize() + 3 ;
	}

	uint cEgarch::GetNLags(void) const
	{
		return MAX(mvArch.GetSize(), mvGarch.GetSize()) ;
	}

	void cEgarch::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = GetNParam(),
		 k = theIndex ;

		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size") ;
		theDestVect[k++] = mvCste ;
		mvArch.SetSubVectorWithThis(theDestVect, k) ;
		k += mvArch.GetSize() ;
		mvGarch.SetSubVectorWithThis(theDestVect, k) ;
		k += mvGarch.GetSize() ;
		theDestVect[k++] = mvTeta	;
		theDestVect[k] = mvGamma	;
	}

	void cEgarch::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize(),
		k = theIndex					;
		if (GetNParam() + theIndex > mySize)
			throw cError("Wrong size") ;
		mvCste = theSrcVect[k++] ;
		mvArch.SetThisWithSubVector(theSrcVect, k) ;
		k += mvArch.GetSize() ;
		mvGarch.SetThisWithSubVector(theSrcVect, k) ;
		k += mvGarch.GetSize() ;
		mvTeta = theSrcVect[k++] ;
		mvGamma = theSrcVect[k] ;
	}

/*
	void cEgarch::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize();
	uint myq = mvGarch.GetSize();
	uint myBegIndex = theGradData.GetNMeanParam() ;
	uint i, j ;
		theGradData.mCurrentGradVar = 0.0L ;
	// CSTE
		theGradData.mCurrentGradVar[myBegIndex] = 1.0 ;
	// ARCH
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar[myBegIndex+i] = mvTeta*theData.mEpst[theDate-i] + mvGamma*(fabs(theData.mEpst[theDate-i]) - mvEspAbsEpsilont) ;
		myBegIndex += myp ;
	// GARCH
		for (j = 1 ; j <= MIN(myq, theDate) ; j++)
			theGradData.mCurrentGradVar[myBegIndex+j] = log(theData.mHt[theDate-j]) ;
		myBegIndex += myq ;
	// Teta et Gamma
		for ( i = 1 ; i <= MIN(myp, theDate) ; i++)
		{	theGradData.mCurrentGradVar[myBegIndex+1] += mvArch[i-1] * theData.mEpst[theDate-i] ;
			theGradData.mCurrentGradVar[myBegIndex+2] += mvArch[i-1] * (fabs(theData.mEpst[theDate-i]) - mvEspAbsEpsilont) ;
		}

	// Et la suite
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			theGradData.mCurrentGradVar += mvArch[i-1]*(mvTeta + mvGamma*SIGN(theData.mEpst[theDate-i]))*theGradData.mGradEpst[i-1] ;

		for (j = 1 ; j <= MIN(myq, theDate) ; j++)
			theGradData.mCurrentGradVar += mvGarch[j-1] / theData.mHt[theDate-j] * theGradData.mGradHt[j-1] ;

		// Et maintenant le gradient par rapport au(x) paramètre(s) du résidu

	int myNLawParam = (int)theGradData.GetNDistrParameter() ;
		if (myNLawParam > 0)
		{
		double myAux = -mvGamma * Sum(mvArch) ;
		cDVector myAuxGrad(myNLawParam) ;

			theResiduals->ComputeGradBetaEspAbsEps(myAuxGrad) ;
			myBegIndex = theGradData.GetNParam() - myNLawParam ;
			for (int i = 0 ; i < myNLawParam ; i++)
				theGradData.mCurrentGradVar[myBegIndex+i] += myAuxGrad[i]*myAux ;
		}

	// On a calculé  grad Nu = grad(Log h)^2, il faut calculer grad h	
	
		theGradData.mCurrentGradVar *= theData.mHt[theDate] ; 
	}
*/

	void cEgarch::ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize();
	uint myq = mvGarch.GetSize();
	uint myNParam = theGradData.GetNParam();
	uint myNDistrParam = theGradData.GetNDistrParameter();
	uint myNMeanParam = theGradData.GetNMeanParam();
	uint myNVarParam = theGradData.GetNVarParam();
	uint myBegIndex = myNMeanParam;

	cDVector* myDArch = new cDVector[myp];
	cDVector* myDGarch = new cDVector[myq];
		for (uint i = 0; i < myp; i++)
		{
			myDArch[i].ReAlloc(myNParam, 0.0);
			myDArch[i][myBegIndex + i + 1] = 1.0;
		}
		for (uint j = 0; j < myq; j++)
		{
			myDGarch[j].ReAlloc(myNParam, 0.0);
			myDGarch[j][myBegIndex + myp + j + 1] = 1.0;
		}
	cDVector myDOmega(myNParam);
	cDVector myDTeta(myNParam);
 	cDVector myDGamma(myNParam);
	cDVector myDEspAbsEps(myNParam);
		myDOmega[myBegIndex] = 1.0;
		myDTeta[myBegIndex + myp + myq + 1] = 1.0;
		myDGamma[myBegIndex + myp + myq + 2] = 1.0;
		if (myNDistrParam > 0)
		{
		uint myIndex = myNMeanParam + myNVarParam;
		cDVector myAuxGrad(myNDistrParam);
			theResiduals->ComputeGradBetaEspAbsEps(myAuxGrad);
			for (uint i = 0; i < myNDistrParam; i++)
				myDEspAbsEps[myIndex + i] = myAuxGrad[i];
		}
	cDVector myGradNu(myNParam);
		myGradNu = myDOmega;
		for (uint i = 0; i < MIN(myp, theDate); i++)
		{
		double myEps = theData.mEpst[theDate - i - 1];
		double mySign = SIGN(myEps);
			myGradNu += myDArch[i] * mvTeta * myEps;
			myGradNu += mvArch[i] * myDTeta *myEps;
			myGradNu += mvArch[i] * mvTeta * theGradData.mGradEpst[i];
			myGradNu += myDArch[i] * mvGamma * mySign * myEps; 
			myGradNu += mvArch[i] * myDGamma * mySign * myEps;
			myGradNu += mvArch[i] * mvGamma * mySign * theGradData.mGradEpst[i];
			myGradNu -= myDArch[i] * mvGamma * mvEspAbsEpsilont;
			myGradNu -= mvArch[i] * myDGamma * mvEspAbsEpsilont;
			myGradNu -= mvArch[i] * mvGamma * myDEspAbsEps;
		}
		for (uint j = 0; j < MIN(myq, theDate); j++)
		{
			myGradNu += myDGarch[j] * log(theData.mHt[theDate - j - 1]);
			myGradNu += mvGarch[j] * theGradData.mGradHt[j] / theData.mHt[theDate - j - 1];
		}
		// On a calculé  grad Nu = grad(Log h)^2, il faut calculer grad h	

		theGradData.mCurrentGradVar = theData.mHt[theDate] * myGradNu;
		for (uint i = 0; i < myp; i++)
		{
			myDArch[i].Delete();
		}
		for (uint j = 0; j < myq; j++)
		{
			myDGarch[j].Delete();
		}
		delete [] myDArch;
		delete [] myDGarch;
	}

	void cEgarch::SetEspAbsEps(double theEspAbsEps)
	{
		mvEspAbsEpsilont = theEspAbsEps ;
	}

/*
	void cEgarch::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize();
	uint myq = mvGarch.GetSize();
	uint myBegIndex = theGradData.GetNMeanParam();
	uint i, j;
		theHessData.mCurrentHessVar = 0;
	uint myNParam = theGradData.GetNParam();
	uint myNLawParam = theGradData.GetNDistrParameter();
	cDVector myAuxGrad(myNLawParam);
	cDVector myGradSup(myNParam);

		if (myNLawParam > 0)
		{
			theResiduals->ComputeGradBetaEspAbsEps(myAuxGrad);
		uint myIndex = myNParam - myNLawParam;
			for (i = 0; i < myNLawParam; i++)
				myGradSup[i + myIndex] = myAuxGrad[i];
		}
	cDMatrix myGradSuite(myNParam, myNParam);
	// dSuite/dCst
		// rien pour le moment	
		myBegIndex++;
	// dsuite /dArchi
		for (i = 0 ; i < MIN(myp, theDate) ; i++)
		{	for (j = 0 ; j < myNParam ; j++)
			{
				myGradSuite[j][myBegIndex + i] = (mvTeta + mvGamma * SIGN(theData.mEpst[theDate - i - 1])) * theGradData.mGradEpst[i][j] - mvGamma * myGradSup[j];
			}
		}
		myBegIndex += myp;
	// dsuite/dGgarchj
		for (j = 0; j < MIN(myq, theDate); j++)
		{	for (i = 0; i < myNParam; i++)
			{
				myGradSuite[i][myBegIndex + j] = theGradData.mGradHt[j][i]/ theData.mHt[theDate - j - 1];
			}
		}
		myBegIndex += myq;
	// suite/dTeta
		for (i = 0; i < MIN(myp, theDate); i++)
		{
			for (j = 0; j < myNParam; j++)
			{
				myGradSuite[j][myBegIndex] += mvArch[i] * theGradData.mGradEpst[i][j];
			}
		}
		myBegIndex++;
	// dsuite/dGamma
		for (i = 0; i < MIN(myp, theDate) ; i++)
		{
			for (j = 0; j < myNParam; j++)
			{
				myGradSuite[j][myBegIndex] += mvArch[i] * (SIGN(theData.mEpst[theDate-i-1])*theGradData.mGradEpst[i][j] - myGradSup[j]);
			}
		}
	// Et on continue	
		myBegIndex = theGradData.GetNMeanParam();
		for (i = myBegIndex; i < myNParam; i++)
		{
			for (j = 0; j < MIN(myq, theDate); i++)
			{
				double myAux = -2.0 / pow(theData.mHt[theDate - j - 1], 1.5)*theGradData.mGradHt[j][i] + 1 / theData.mHt[theDate - j - 1] * theHessData.mHessHt[j][j][i];
				myGradSuite[i][j] += mvGarch[j] * myAux;
			}
		}
	// Et maintenant la hessienne
	// d2 Var / dcst dTeta

	}
*/

/*
	void cEgarch::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize();
	uint myq = mvGarch.GetSize();
	uint myNParam = theGradData.GetNParam();
	uint myNDistrParam = theGradData.GetNDistrParameter();
	cDMatrix mydArch(myNParam, myp);
	cDMatrix mydGarch(myNParam, myq);
	uint myBegIndex = theGradData.GetNMeanParam();
		for (uint i = 0; i < myp; i++)
			mydArch[myBegIndex + 1 + i][i] = 1.0;
		for (uint j = 0; j < myq ; j++)
			mydGarch[myBegIndex + 1 + j + myp ][j] = 1.0;
	cDVector mydTeta(myNParam);
		mydTeta[myBegIndex + myp + myq + 1] = 1.0;
	cDVector mydGamma(myNParam);
		mydGamma[myBegIndex + myp + myq + 2] = 1.0;
	cDVector myGradEspEps(myNParam);
	cDMatrix myHessEspEps(myNParam, myNParam);
		if (myNDistrParam > 0)
		{
		cDVector myAuxGrad(myNDistrParam);
			theResiduals->ComputeGradBetaEspAbsEps(myAuxGrad);
		cDMatrix myAuxHess(myNDistrParam, myNDistrParam);
			theResiduals->ComputeHessBetaEspAbsEps(myAuxHess);
		uint myIndex = myNParam - myNDistrParam;
			for (uint i = 0; i < myNDistrParam; i++)
			{
				myGradEspEps[myIndex + i] = myAuxGrad[i];
				for (uint j = 0; j < myNDistrParam; j++)
					myHessEspEps[myIndex + i][myIndex + j] = myAuxHess[i][j];
			}
		}
		
		theHessData.mCurrentHessVar = 0.0;
		for (uint m = 0 ; m < myNParam ; m++)
			for (uint n = m; n < myNParam; n++)
			{
				for (uint i = 0; i < MIN(myp, theDate); i++)
				{
					theHessData.mCurrentHessVar[m][n] += mydArch[m][i] * (mydTeta[n] * theData.mEpst[theDate - i - 1] + mvTeta * theGradData.mGradEpst[i][n]);
					theHessData.mCurrentHessVar[m][n] += mydArch[n][i] * (mydTeta[m] * theData.mEpst[theDate - i - 1] + mvTeta * theGradData.mGradEpst[i][m]);
					theHessData.mCurrentHessVar[m][n] += mvArch[i] * (mydTeta[n] * theGradData.mGradEpst[i][m] + mydTeta[m] * theGradData.mGradEpst[i][n] + mvTeta * theHessData.mHessEpst[i][m][n]);
				double mySign = SIGN(theData.mEpst[theDate - i - 1]);
					theHessData.mCurrentHessVar[m][n] += mydArch[m][i] * mySign * (mydGamma[n] * theData.mEpst[theDate - i - 1] + mvGamma * theGradData.mGradEpst[i][n]);
					theHessData.mCurrentHessVar[m][n] += mydArch[n][i] * mySign * (mydGamma[m] * theData.mEpst[theDate - i - 1] + mvGamma * theGradData.mGradEpst[i][m]);
					theHessData.mCurrentHessVar[m][n] += mvArch[i] * mySign * (mydGamma[n] * theGradData.mGradEpst[i][m] + mydGamma[m] * theGradData.mGradEpst[i][n] + mvGamma * theHessData.mHessEpst[i][m][n]);
					
					theHessData.mCurrentHessVar[m][n] += mydArch[m][i] * (mydGamma[n] * mvEspAbsEpsilont + mvGamma * myGradEspEps[n]);
					theHessData.mCurrentHessVar[m][n] += mydArch[n][i] * (mydGamma[m] * mvEspAbsEpsilont + mvGamma * myGradEspEps[m]);
					theHessData.mCurrentHessVar[m][n] += mvArch[i] * (mydGamma[n] * myGradEspEps[m] + mydGamma[m] * myGradEspEps[n] + mvGamma * myHessEspEps[m][n]);
				}
				for (uint j = 0; j < MIN(myq, theDate); j++)
				{
					theHessData.mCurrentHessVar[m][n] += mydGarch[m][j] * theGradData.mGradHt[j][n] / theData.mHt[theDate - j - 1];
					theHessData.mCurrentHessVar[m][n] += mydGarch[n][j] * theGradData.mGradHt[j][m] / theData.mHt[theDate - j - 1];
					theHessData.mCurrentHessVar[m][n] += mvGarch[j] * (-2.0 / pow(theData.mHt[theDate - j - 1], 1.5) * theGradData.mGradHt[j][n] + theHessData.mHessHt[j][m][n] / theData.mHt[theDate - j - 1]);
				}

			}
	}
*/

	void cEgarch::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize();
	uint myq = mvGarch.GetSize();
	uint myNParam = theGradData.GetNParam();
	uint myNDistrParam = theGradData.GetNDistrParameter();
	uint myNMeanParam = theGradData.GetNMeanParam();
	uint myNVarParam = theGradData.GetNMeanParam();
	uint myBegIndex = myNMeanParam;

	cDVector* myDArch = new cDVector[myp];
	cDVector* myDGarch = new cDVector[myq];
		for (uint i = 0; i < myp; i++)
		{
			myDArch[i].ReAlloc(myNParam, 0.0);
			myDArch[i][myBegIndex + i + 1] = 1.0;
		}
		for (uint j = 0; j < myq; j++)
		{
			myDGarch[j].ReAlloc(myNParam, 0.0);
			myDGarch[j][myBegIndex + myp + j + 1] = 1.0;
		}
	cDVector myDTeta(myNParam);
	cDVector myDGamma(myNParam);
	cDVector myDEspAbsEps(myNParam,0.0);
	cDMatrix myD2EspAbsEps(myNParam, myNParam, 0.0);
		myDTeta[myBegIndex + myp + myq + 1] = 1.0;
		myDGamma[myBegIndex + myp + myq + 2] = 1.0;
		if (myNDistrParam > 0)
		{
		uint myIndex = myNMeanParam + myNVarParam;
		cDVector myAuxGrad(myNDistrParam);
		cDMatrix myAuxHess(myNDistrParam, myNDistrParam);
			theResiduals->ComputeGradBetaEspAbsEps(myAuxGrad);
			theResiduals->ComputeHessBetaEspAbsEps(myAuxHess);
			for (uint i = 0; i < myNDistrParam; i++)
			{
				myDEspAbsEps[myIndex + i] = myAuxGrad[i];
				for (uint j = 0; j < myNDistrParam; j++)
					myD2EspAbsEps[myIndex + i][myIndex + j] = myAuxHess[i][j];
			}
		}
	cDMatrix myHessNu(myNParam, myNParam);
		for (uint i = 0; i < MIN(myp, theDate) ; i++)
		{
		double myEps = theData.mEpst[theDate - i - 1];
		double mySign = SIGN(myEps);
		cDVector myGradEps = theGradData.mGradEpst[i];
			myHessNu += myEps * (myDArch[i] * Transpose(myDTeta) + myDTeta * Transpose(myDArch[i]));
			myHessNu += mvTeta * (myDArch[i] * Transpose(myGradEps) + myGradEps * Transpose(myDArch[i]));
			myHessNu += mvArch[i] * (myDTeta * Transpose(myGradEps) + myGradEps * Transpose(myDTeta));			
			myHessNu += mvArch[i] * mvTeta * theHessData.mHessEpst[i];
			myHessNu += myEps * mySign * (myDArch[i] * Transpose(myDGamma) + myDGamma * Transpose(myDArch[i]));
			myHessNu += mvGamma * mySign * (myDArch[i] * Transpose(myGradEps) + myGradEps * Transpose(myDArch[i]));
			myHessNu += mvArch[i] * mySign * (myDGamma * Transpose(myGradEps) + myGradEps * Transpose(myDGamma));			
			myHessNu += mvArch[i] * mySign * mvGamma * theHessData.mHessEpst[i];
			myHessNu -= mvEspAbsEpsilont * (myDArch[i] * Transpose(myDGamma) + myDGamma * Transpose(myDArch[i]));
			myHessNu -= mvGamma * (myDArch[i] * Transpose(myDEspAbsEps) + myDEspAbsEps * Transpose(myDArch[i]));
			myHessNu -= mvArch[i] * (myDGamma * Transpose(myDEspAbsEps) + myDEspAbsEps * Transpose(myDGamma));			
			myHessNu -= mvArch[i] * mvGamma * myD2EspAbsEps;
		}
		for (uint j = 0; j < MIN(myq, theDate); j++)
		{
		cDVector myDNu = theGradData.mGradHt[j] / theData.mHt[theDate - j - 1];
		cDMatrix myD2Nu = theHessData.mHessHt[j] / theData.mHt[theDate - j - 1] - theGradData.mGradHt[j] * Transpose(theGradData.mGradHt[j]) / pow(theData.mHt[theDate - j - 1],2.0);
			myHessNu += myDGarch[j] * Transpose(myDNu) + myDNu * Transpose(myDGarch[j]);
			myHessNu -= mvGarch[j] * myD2Nu;
		}
	// On a calculé  Hess Nu = Hess(Log h), il faut calculer Hess h	
	cDVector myDNu = theGradData.mCurrentGradVar / theData.mHt[theDate];
		theHessData.mCurrentHessVar = myHessNu + myDNu * Transpose(myDNu);
		theHessData.mCurrentHessVar *= theData.mHt[theDate];
		for (uint i = 0; i < myp; i++)
		{
			myDArch[i].Delete();
		}
		for (uint j = 0; j < myq; j++)
		{
			myDGarch[j].Delete();
		}
		delete[] myDArch;
		delete[] myDGarch;
	}

	void cEgarch::GetParamName(uint theIndex, char** theName)
	{
		uint myIndex = theIndex;
		sprintf(theName[myIndex++], "CST VAR");
		for (uint i = 0; i < mvArch.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "ARCH[%d]", i + 1);

		}
		for (uint i = 0; i < mvGarch.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "GARCH[%d]", i + 1);

		}
		sprintf(theName[myIndex++], "THETA");
		sprintf(theName[myIndex++], "GAMMA");
	}
	
	void cEgarch::GetParamName(uint theIndex, string theName[])
	{
	uint myIndex = theIndex;
	char myChar[100];
		sprintf(myChar, "CST VAR");
		theName[myIndex++] = myChar;
		for (uint i = 0; i < mvArch.GetSize(); i++)
		{
			sprintf(myChar, "ARCH[%d]", i + 1);
			theName[myIndex++] = myChar;

		}
		for (uint i = 0; i < mvGarch.GetSize(); i++)
		{
			sprintf(myChar, "GARCH[%d]", i + 1);
			theName[myIndex++] = myChar;

		}
		sprintf(myChar, "THETA");
		theName[myIndex++] = myChar;
		sprintf(myChar, "GAMMA");
		theName[myIndex++] = myChar;
	}



}//namespace
