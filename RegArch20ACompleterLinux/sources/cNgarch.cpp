#include "StdAfxRegArchLib.h"
/*!
	\file cNgarch.cpp
	\brief sources for class cNgarch methods.

	\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
	\date dec-18-2006 - Last change feb-18-2011
*/
namespace RegArchLib {
	/*!
	 * \fn cNgarch::cNgarch(uint theNArch, uint theNGarch):cAbstCondVar(eNgarch)
	 * \param uint theNArch: number of ARCH lags
	 * \param uint theNGarch: number of GARCH lags
	*/
	cNgarch::cNgarch(uint theNArch, uint theNGarch)
		:cAbstCondVar(eNgarch)  // call constructor of cAbstCondVar with type eGarch
	{
		mvArch.ReAlloc(theNArch);
		mvGarch.ReAlloc(theNGarch);
		mvConst = mvTheta = 0.0;
		MESS_CREAT("cNgarch");
	}

	/*!
	 * \fn cNgarch::cNgarch(double theConst, cDVector& theArch, cDVector& theGarch):cAbstCondVar(eGarch)
	 * \param double theConst: constant part of the GARCH(p, q) model
	 * \param cDVector& theGarch theArch: ARCH parameters
	 * \param cDVector& theGarch theGarch: GARCH parameters
	*/
	cNgarch::cNgarch(double theConst, double theTheta, cDVector& theArch, cDVector& theGarch)
		:cAbstCondVar(eNgarch)
	{
		mvConst = theConst;
		mvTheta = theTheta;
		mvArch = theArch;
		mvGarch = theGarch;
		MESS_CREAT("cNgarch");
	}

	/*!
	* \fn cNgarch::cNgarch(const cAbsCondVar& theGarch):cAbstCondVar(eGarch)
	* \param cAbsCondVar& theEgarch: theGarch class
	*/
	cNgarch::cNgarch(const cNgarch& theNgarch)
		:cAbstCondVar(eNgarch)
	{
		*this = theNgarch;
		MESS_CREAT("cNgarch");
	}

	/*!
	 * \fn cNgarch::~cNgarch()
	*/
	cNgarch::~cNgarch()
	{
		mvArch.Delete();
		mvGarch.Delete();
		MESS_DESTR("cNgarch");
	}

	/*!
	 * \fn cAbstCondVar* cNgarch::PtrCopy()
	 */
/*	cAbstCondVar* cNgarch::PtrCopy() const
	{
		//		 cConstCondVar *myConstCondVar = new cConstCondVar(*this);
		//		 return myConstCondVar;
		return cAbstCondVarPtrCopy<cNgarch>();
	}
*/
	/*!
	 * \fn void cNgarch::Delete(void)
	 * \param void
	 * \details Free memory
	 */
	void cNgarch::Delete(void)
	{
		mvArch.Delete();
		mvGarch.Delete();
	}

	/*!
	 * \fn void cNgarch::Print(ostream& theOut) const
	 * \param ostream& theOut: the output stream, default cout.
	 */
#ifndef _RDLL_
	void cNgarch::Print(ostream& theOut) const
	{
		uint myNArch = mvArch.GetSize();
		uint myNGarch = mvGarch.GetSize();
		theOut << "NGARCH(" << myNArch << ", " << myNGarch << ") model with:" << endl;
		theOut << "\tCste=" << mvConst << endl;
		theOut << "\tTheta=" << mvTheta << endl;
		for (uint i = 0; i < myNArch; i++)
			theOut << "\tARCH[" << i + 1 << "]=" << mvArch[i] << endl;
		for (uint j = 0; j < myNGarch; j++)
			theOut << "\tGARCH[" << j + 1 << "]=" << mvGarch[j] << endl;
	}
#else
	void cNgarch::Print(void)
	{
	uint myNArch = mvArch.GetSize();
	uint myNGarch = mvGarch.GetSize();
		Rprintf("NGARCH(%d, %d) model with:\n", myNArch, myNGarch);
		Rprintf("\tCste=%f\n", mvConst);
		Rprintf("\tTheta=%f\n", mvTheta);
		for (uint i = 0; i < myNArch; i++)
			Rprintf("\tARCH[%d]=%f\n", i + 1, mvArch[i]);
		for (uint j = 0; j < myNGarch; j++)
			Rprintf("\tGARCH[%d]=%f\n", j + 1, mvGarch[j]);
	}
#endif //_RDLL_

	void cNgarch::SetDefaultInitPoint(double theMean, double theVar)
	{
		mvConst = theVar*0.1 ;
		mvTheta = 0.0;
	uint myNArch = mvArch.GetSize() ;
	uint myNGarch = mvGarch.GetSize() ;
	uint i ;
		for (i = 0 ; i < myNArch ; i++)
			mvArch[i] = 0.1/(double)myNArch ;
		for (i = 0 ; i < myNGarch ; i++)
			mvGarch[i] = 0.8/(double)myNGarch ;
	}

	void cNgarch::SetDefaultInitPoint(cRegArchValue& theValue)
	{
	double myVar;
		theValue.ComputeVar(myVar);
		mvConst = myVar*0.1;
		mvTheta = 0.0;
		uint myNArch = mvArch.GetSize();
	uint myNGarch = mvGarch.GetSize();
	uint i;
		for (i = 0; i < myNArch; i++)
			mvArch[i] = 0.1 / (double)myNArch;
		for (i = 0; i < myNGarch; i++)
			mvGarch[i] = 0.8 / (double)myNGarch;
	}

	/*!
	 * \fn void cNgarch::ReAlloc(const uint theSize, const uint theNumParam)
	 * \param const uint theSize: new size of mvArch or mvGarch
	 * \param const uint theNumParam: 0 for mvArch, 1 for mvGarch.
	 * \details new allocation of mvArch or mvGarch
	 */
	void cNgarch::ReAlloc(const uint theSize, const uint theNumParam)
	{
		switch (theNumParam)
		{	case 1 :
				mvArch.ReAlloc(theSize) ;
			break ;
			case 2 :
				mvGarch.ReAlloc(theSize) ;
			break ;
			default :
	//			throw cError("cNgarch::ReAlloc - theNumParam must be in 1, 2.") ;
			break ;
		}
	}

	/*!
	 * \fn void cNgarch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	 * \param const cDVector& theVectParam: the vector of Const, ARCH or GARCH coefficients
	 * \param const uint theNumParam: =0, the constant part; =1 the ARCH coefficients; =2 theGARCH Coefficients
	 * \details new allocation of mvArch or mvConst
	 */
	void cNgarch::ReAlloc(const cDVector& theVectParam, const uint theNumParam)
	{	switch (theNumParam)
		{	case 0: // mvConst
				if (theVectParam.GetSize() > 0)
					mvConst = theVectParam[0] ;
				else
					throw cError("cNgarch::ReAlloc - Size of theVectParam must be > 0") ;
			break ;
			case 1 :
				if (theVectParam.GetSize() > 0)
					mvTheta = theVectParam[0];
				else
					throw cError("cNgarch::ReAlloc - Size of theVectParam must be > 0");
			break;
			case 2 : // mvArch
				mvArch = theVectParam ;
			break ;
			case 3 : // mvGarch
				mvGarch = theVectParam ;
			break ;
			default :
				throw cError("cNgarch::ReAlloc - theNumParam must be in 0, 1, 2, 3") ;
			break ;
		}
	}

	/*!
	 * \fn void cNgarch::Set(const double theValue, const uint theIndex, const uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const double theValue: the value of the "theIndex" th lag. Default 0.
	 * \param const uint theIndex: the index.
	 * \param const uint theNumParam: =0, mvConst, =1, ARCH parameters; =2, GARCH parameters
	 * \details mvArch[theIndex] = theValue or mvGarch[theIndex]= theValue or mvConst = theValue
	 */
	void cNgarch::Set(const double theValue, const uint theIndex, const uint theNumParam)
	{	switch (theNumParam)
		{	case 0 :
				mvConst = theValue ;
			break ;
			case 1:
				mvTheta = theValue;
				break;
			case 2:
				if (theIndex < mvArch.GetSize())
					mvArch[theIndex] = theValue ;
				else
					throw cError("cNgarch::Set - wrong index") ;
			break ;
			case 3 :
				if (theIndex < mvGarch.GetSize())
					mvGarch[theIndex] = theValue ;
				else
					throw cError("cNgarch::Set - wrong index") ;
			break ;
			default:
				throw cError("cNgarch::Set - theNumParam must be in 0, 1, 2, 3");
			break;
		}
	}

	/*!
	 * \fn void cNgarch::Set(const cDVector& theVectParam, const uint theNumParam)
	 * \brief fill the parameters vector
	 * \param const cDVector& theVectParam: the vector of values
	 * \param const uint theNumParam: =0, mvConst, =1, ARCH parameters; =2, GARCH parameters
	 * \details mvAr = theValue
	 */
	void cNgarch::Set(const cDVector& theVectParam, const uint theNumParam)
	{	switch (theNumParam)
		{	case 0 :
				if (theVectParam.GetSize() > 0)
					mvConst = theVectParam[0] ;
				else
					throw cError("cNgarch::Set - Size of theVectParam must be > 0") ;
			break ;
			case 1:
				if (theVectParam.GetSize() > 0)
					mvTheta = theVectParam[0];
				else
					throw cError("cNgarch::Set - Size of theVectParam must be > 0");
			break;
			case 2:
				mvArch = theVectParam ;
			break ;
			case 3 :
				mvGarch = theVectParam ;
			break ;
			default:
				throw cError("cNgarch::Set - theNumParam must be in 0, 1, 2, 3") ;
			break ;
		}
	}

	double  cNgarch::Get(const uint theIndex, const uint theNumParam)
	{
		switch (theNumParam)
		{	case 0 :
				return mvConst ;
			break ;
			case 1:
				return mvTheta;
			break;
			case 2 :
				return mvArch[theIndex] ;
			break ;
			case 3:
				return mvGarch[theIndex] ;
			break ;
		}
	}

	cDVector& cNgarch::Get(const uint theNumParam)
	{
	cDVector *myAux;
		switch (theNumParam)
		{
			case 0:
				myAux = new cDVector(1, mvConst);
				return *myAux;
			break;
			case 1:
				myAux = new cDVector(1, mvTheta);
				return *myAux;
			break;			
			case 2:
				return mvArch;
			break;
			case 3:
				return mvGarch;
			break;
		}
	}

	cNgarch& cNgarch::operator =(const cNgarch& theSrc)
	{
		mvConst = theSrc.mvConst;
		mvTheta = theSrc.mvTheta;
		mvArch = theSrc.mvArch;
		mvGarch = theSrc.mvGarch;
		return *this;
	}

	/*!
	 * \fn double cNgarch::ComputeVar(uint theDate, const cRegArchValue& theData) const
	 * \param int theDate: date of computation
	 * \param const cRegArchValue& theData: past datas
	 * \details theData is not updated here.
	*/
	double cNgarch::ComputeVar(uint theDate, const cRegArchValue& theData) const 
	{
		uint myp = mvArch.GetSize();
		uint myq = mvGarch.GetSize() ;
		double myRes = mvConst ;
		for (uint i = 1 ; i <= MIN(myp, theDate) ; i++)
			myRes += mvArch[i-1] *(theData.mUt[theDate-i] - mvTheta*sqrt(theData.mHt[theDate-i]))*(theData.mUt[theDate-i] - mvTheta*sqrt(theData.mHt[theDate-i])) ;
		for (uint j = 1 ; j <= MIN(myq, theDate) ; j++)
			myRes += mvGarch[j-1] * theData.mHt[theDate-j];

		return myRes ;
	}

	uint cNgarch::GetNParam(void) const
	{
		return mvArch.GetSize() + mvGarch.GetSize() + 2 ;
	}

	uint cNgarch::GetNLags(void) const
	{
		return MAX(mvArch.GetSize(), mvGarch.GetSize()) ;
	}

	void cNgarch::ComputeGrad(uint theDate, const cRegArchValue& theValue, cRegArchGradient& theGradData, cAbstResiduals* theResiduals)
	{
	uint myp = mvArch.GetSize(),
		myq = mvGarch.GetSize(),
		myBegIndex = theGradData.GetNMeanParam() ;
	uint i, j ;
		theGradData.mCurrentGradVar = 0.0L ;
		theGradData.mCurrentGradVar[myBegIndex] = 1.0 ;
	//Derivé sur THETA
	//sum_{i=1}{p}{Alpha_i*(-2*U(t-i)*Sigma(t-i) + 2 * mvTheta * H(t-i))}

	for(i = 1 ; i <=  MIN(myp, theDate); i++){
		theGradData.mCurrentGradVar[myBegIndex+1] += mvArch[i-1]*(-2.0*theValue.mUt[theDate-i]*sqrt(theValue.mHt[theDate-i])+ 2.0*mvTheta*theValue.mHt[theDate-i]) ;
	}

	//Derivé de la partie ARCH
		for (i = 1 ; i <=  MIN(myp, theDate) ; i++)
			//Derivé sur alpha_i : [U(t-i) - mvTheta*Sigma(t-i)]**2
			theGradData.mCurrentGradVar[myBegIndex+1+i] = (theValue.mUt[theDate-i]-mvTheta*sqrt(theValue.mHt[theDate-i]))*(theValue.mUt[theDate-i]-mvTheta*sqrt(theValue.mHt[theDate-i])) ;
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			//Derivé de U**2 : - Alpha_i*2*U(t-i)*gradM(t-i)
			theGradData.mCurrentGradVar += - 2.0 * mvArch[i-1] * theValue.mUt[theDate-i] * theGradData.mGradMt[i-1];
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			//Derivé de 2*U*sigma*mvTheta : - Alpha_i*2*U(t-i)*gradSigma(t-i) + Alpha_i*2*Sigma(t-i)*gradM(t-i)
			theGradData.mCurrentGradVar += - 2.0 * mvArch[i-1] * theValue.mUt[theDate-i] * mvTheta * (theGradData.mGradHt[i-1]/(2*sqrt(theValue.mHt[theDate-i]))) - 2 * mvArch[i-1] * mvTheta * sqrt(theValue.mHt[theDate-i])*(-1*theGradData.mGradMt[i-1]);
		for (i = 1 ; i <= MIN(myp, theDate) ; i++)
			//Derivé de [sigma*mvTheta]**2 : - Alpha_i*mvTheta**2 *gradH(t-i)
			theGradData.mCurrentGradVar += mvArch[i-1] * mvTheta * mvTheta * theGradData.mGradHt[i-1];
	//Dérivé de la partie GARCH
		for (j = 1; j <= MIN(myq, theDate); j++)
			//Derivé sur Beta_i : gradH(t-i)
			theGradData.mCurrentGradVar[myBegIndex + myp + 1 + j] += theValue.mHt[theDate - j];
		for (j = 1; j <= MIN(myq, theDate); j++)
			//Derivé de Beta_i * H(t-i) : Beta_i * gradH(t-i)
			theGradData.mCurrentGradVar += mvGarch[j-1] * theGradData.mGradHt[j-1] ;
	}

	void cNgarch::RegArchParamToVector(cDVector& theDestVect, uint theIndex)
	{
	uint mySize = GetNParam() ;
		if (theDestVect.GetSize() < mySize + theIndex)
			throw cError("Wrong size") ;
		theDestVect[theIndex] = mvConst ;
		theDestVect[theIndex+1] = mvTheta;
		mvArch.SetSubVectorWithThis(theDestVect, theIndex + 2) ;
		mvGarch.SetSubVectorWithThis(theDestVect, theIndex + 2 + mvArch.GetSize()) ;
	}

	void cNgarch::VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex)
	{
	uint mySize = theSrcVect.GetSize() ;
		if (GetNParam() + theIndex > mySize)
			throw cError("Wrong size") ;
		mvConst = theSrcVect[theIndex] ;
		mvTheta = theSrcVect[theIndex+1];
		mvArch.SetThisWithSubVector(theSrcVect, theIndex+2) ;
		mvGarch.SetThisWithSubVector(theSrcVect, theIndex + 2 + mvArch.GetSize()) ;
	}

	void cNgarch::ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, cAbstResiduals* theResiduals)
	{
		uint myp = mvArch.GetSize(), 
		myq = mvGarch.GetSize() ;
	uint myBegIndex = theGradData.GetNMeanParam();
	printf("curent var avant\n");
	theHessData.mCurrentHessVar.Print();
		theHessData.mCurrentHessVar = 0.0;
	uint i, j ;
	// Gradient du vecteur  
		cDMatrix myMat = theHessData.mCurrentHessVar;
		double sumTheta = 0;
		// Derivé seconde par rapport a theta : sum_{i=1}{p}{Alpha_i * 2 * H(t-i)}
		// Derivé seconde par rapport a theta et alpha_i : -2*U(t-i)*Sigma(t-i) + 2 * mvTheta * H(t-i)
		for (i = 1; i <= MIN(myp, theDate); i++){
			sumTheta +=  mvArch[i-1]*2.0*theData.mHt[theDate-i];
			double result = -2.0*theData.mUt[theDate-i]*sqrt(theData.mHt[theDate-i])+2.0*mvTheta*theData.mHt[theDate-i];
			myMat.Set(result, myBegIndex+1, myBegIndex+1+i);
			myMat.Set(result, myBegIndex+1+i, myBegIndex+1);
		}
		myMat.Set(sumTheta, myBegIndex+1, myBegIndex+1);
		theHessData.mCurrentHessVar += myMat;

	// Derivé sur THETA 
	myMat = 0.0;
	cDVector* myVect = new cGSLVector(myMat.GetNRow(), 0.0);

	// Derivé de sum_{i=1}{p}{Alpha_i * Theta**2 * gradH(t-i)} : sum_{i=1}{p}{Alpha_i * 2 * Theta* * gradH(t-i)}
	for (i = 1 ; i <= MIN(myp, theDate) ; i++)
		*myVect = *myVect + mvArch[i-1] * 2 * mvTheta * theGradData.mGradHt[i-1];

	// Derivé de sum_{i=1}{p}{-2 * Alpha_i * U(t-i) * Theta * gradSigma(t-i) -2 * Alpha_i * gradU(t-i) * Theta * Sigma(t-i)} : 
	// -2 * Alpha_i * U(t-i)  * gradSigma(t-i) -2 * Alpha_i * gradU(t-i)  * Sigma(t-i)
	for (i = 1 ; i <= MIN(myp, theDate) ; i++)
		*myVect = *myVect - 2.0 * mvArch[i-1] * theData.mUt[theDate-i] * (theGradData.mGradHt[i-1]/(2*sqrt(theData.mHt[theDate-i]))) - 2 * mvArch[i-1] * sqrt(theData.mHt[theDate-i])*(-1*theGradData.mGradMt[i-1]);
	
	myMat.SetRow(myBegIndex + 1, *myVect);
	theHessData.mCurrentHessVar += myMat + Transpose(myMat);

	//Dévrié de la partie ARCH
	myMat = 0.0;
	for (i = 1; i <= MIN(myp, theDate); i++){
		//Derivé sur alpha_i : 
		// 2 * U(t-i) * gradU(t-i) 
		// -2 * U(t-i) * Theta * gradSigma(t-i) 
		// -2 * gradU(t-i) * Theta * Sigma(t-i) 
		// + * gradH(t-i) * Theta**2
		myMat.SetRow(myBegIndex + 1 + i, -2.0 * theData.mUt[theDate - i] * theGradData.mGradMt[i - 1] - 2.0 * theData.mUt[theDate - i] * mvTheta * (theGradData.mGradHt[i-1]/(2*sqrt(theData.mHt[theDate-i]))) + 2.0 * sqrt(theData.mHt[theDate - i]) * mvTheta * theGradData.mGradMt[i - 1] + mvTheta * mvTheta* theGradData.mGradHt[i - 1]);
	}
	theHessData.mCurrentHessVar += myMat + Transpose(myMat);

	for (i = 1; i <= MIN(myp, theDate); i++){
		// Dérivé de : 2 * alpha_i * U(t-i) * gradU(t-i) :
			// 2 * alpha_i * U(t-i) * hessU(t-i)
			theHessData.mCurrentHessVar -= 2.0 * mvArch[i - 1] * theData.mUt[theDate - i] * theHessData.mHessMt[i - 1];
			// 2 * alpha_i * gradU(t-i) * gradU(t-i)
			theHessData.mCurrentHessVar += 2.0 * mvArch[i - 1] * theGradData.mGradMt[i-1]*Transpose(theGradData.mGradMt[i-1]);
		// Dérivé de : -2 * alpha_i * Theta * U(t-i) * gradSigma(t-i) :
			// -2 * alpha_i * Theta * U(t-i) * hessSigma(t-i)
			theHessData.mCurrentHessVar -= 2.0 * mvArch[i - 1] * mvTheta * theData.mUt[theDate - i] * ((theHessData.mHessHt[i - 1]*2*sqrt(theData.mHt[theDate - i]) - (theGradData.mGradHt[i-1]*Transpose(theGradData.mGradHt[i-1]))/sqrt(theData.mHt[theDate-i]))/(4*theData.mHt[theDate-i]));
			// -2 * alpha_i * Theta * gradU(t-i) * gradSigma(t-i)
			theHessData.mCurrentHessVar += 2.0 * mvArch[i - 1] * mvTheta * theGradData.mGradMt[i-1]*Transpose((theGradData.mGradHt[i-1]/(2*sqrt(theData.mHt[theDate-i]))));
		// Dérivé de : -2 * alpha_i * Theta * Sigma(t-i) * gradU(t-i) :
			// -2 * alpha_i * Theta *  Sigma(t-i) * hessU(t-i) 
			theHessData.mCurrentHessVar += 2.0 * mvArch[i - 1] * mvTheta * theHessData.mHessMt[i - 1]*sqrt(theData.mHt[theDate - i]) ;
			// -2 * alpha_i * Theta *  gradSigma(t-i) * gradU(t-i) 
			theHessData.mCurrentHessVar += 2.0 * mvArch[i - 1] * mvTheta * (theGradData.mGradHt[i-1]/(2*sqrt(theData.mHt[theDate-i]))) * Transpose(theGradData.mGradMt[i-1]);
		// Dérivé de : alpha_i * Theta**2 * gradH(t-i) :
			// alpha_i * Theta**2 * hessH(t-i) 
			theHessData.mCurrentHessVar += mvArch[i - 1] * mvTheta * mvTheta * theHessData.mHessHt[i - 1];
	
	}	
		
	// Dérivé de la partie GARCH
		myMat = 0.0;
		for (j = 1; j <= MIN(myq, theDate); j++)
		//Derivé sur beta_i de : beta_i * gradH(t-i)
			// gradH(t-i)
			myMat.SetRow(myBegIndex + 1 + myp + j, theGradData.mGradHt[j - 1]);
		theHessData.mCurrentHessVar += myMat + Transpose(myMat);

		for (j = 1; j <= MIN(myq, theDate); j++)
		// Dérivé de : beta_i * gradH(t-i) :
			// beta_i * hessH(t-i)
			theHessData.mCurrentHessVar += mvGarch[j - 1] * theHessData.mHessHt[j - 1];
	}
	
	void cNgarch::GetParamName(uint theIndex, char** theName)
	{
	uint myIndex = theIndex;
		sprintf(theName[myIndex++], "CST VAR");
		sprintf(theName[myIndex++], "THETA");
		for (uint i = 0; i < mvArch.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "ARCH[%d]", i + 1);

		}
		for (uint i = 0; i < mvGarch.GetSize(); i++)
		{
			sprintf(theName[myIndex++], "GARCH[%d]", i + 1);

		}
	}

	void cNgarch::GetParamName(uint theIndex, string theName[])
	{
	uint myIndex = theIndex;
	char myChar[100];
		sprintf(myChar, "CST VAR");
		theName[myIndex++] = myChar;
		sprintf(myChar, "THETA");
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
	}

}//namespace
