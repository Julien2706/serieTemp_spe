#include "StdAfxRegArchLib.h"
/*!
	\file cAbstCondMean.cpp
	\brief Sources for abstract class cAbstCondMean methods.
	\author Jean-Baptiste DURAND, Ollivier TARAMASCO
	\date dec-18-2006 - Last change feb-18-2011
*/

namespace RegArchLib {

	/*!
	 * \fn cAbstCondMean::cAbstCondMean(const eCondMeanEnum theType)
	 * \param const eCondMeanEnum theType. Code of the conditional mean type. Default value: eUnknown
	 * \details Set the real type code of conditional mean component
	 */
	cAbstCondMean::cAbstCondMean(const eCondMeanEnum theType)
	{
		mvCondMeanType = theType ;
  		MESS_CREAT("cAbstCondMean")
	}

	/*!
	 * \fn cAbstCondMean::~cAbstCondMean()
	 */
	cAbstCondMean::~cAbstCondMean()
	{
		mvCondMeanType = eUnknown ;
  		MESS_DESTR("cAbstCondMean")
	}

	/*!
	 * \fn  inline eCondMeanEnum cAbstCondMean::GetCondMeanType(void)
	 * \param void
	 * \return a eCondMeanEnum value.
	 */
	eCondMeanEnum cAbstCondMean::GetCondMeanType(void) const
	{
		return mvCondMeanType ;
	}

	/*!
	 * \fn  void cAbstCondMean::SetCondMeanType(eCondMeanEnum theType)
	 * \param eCondMeanEnum theType
	 * \details mvCondMeanType = theType
	 */
	void cAbstCondMean::SetCondMeanType(eCondMeanEnum theType)
	{	mvCondMeanType = theType ;
	}

	void cAbstCondMean::ComputeGradWithF(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals)
	{
	uint myNParam[3];
		GetNParamF(myNParam);
	cDVector myGradF(myNParam[0] + myNParam[1] + myNParam[2]);
		ComputeGradF(theDate, theData, myGradF);
		for (uint i = 0; i < myNParam[0]; i++)
			theGradData.mCurrentGradMu[theBegIndex + i] += myGradF[i];
		for (uint k = 0; k < MIN(myNParam[1], theDate); k++)
			theGradData.mCurrentGradMu -= myGradF[myNParam[0] + k] * theGradData.mGradMt[k];
		if (myNParam[2] > 0)
			theGradData.mCurrentGradMu += myGradF[myNParam[0] + myNParam[1]] * theGradData.mCurrentGradSigma;
	}

	void cAbstCondMean::ComputeHessWithF(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals)
	{
	uint myNParam[3];
		GetNParamF(myNParam);
	uint myN = myNParam[0] + myNParam[1] + myNParam[2];
	uint myNm1 = myNParam[0] + myNParam[1];
	uint myNParamTot = theGradData.GetNParam();
	cDVector myGradF(myN);
 		ComputeGradF(theDate, theData, myGradF);
	cDMatrix myHessF(myN, myN);
		ComputeHessF(theDate, theData, myHessF);
	cDMatrix myD2Fxx(myNParamTot, myNParamTot);
		for (uint i = 0; i < myNParam[0]; i++)
			for (uint j = 0; j < myNParam[0]; j++)
				myD2Fxx[i + theBegIndex][j + theBegIndex] = myHessF[i][j];
	cDVector* myD2Fxy = NULL;
		if (myNParam[1] > 0)
			myD2Fxy = new cDVector[myNParam[1]];
		for (uint i = 0; i < myNParam[1]; i++)
		{
			myD2Fxy[i].ReAlloc(myNParamTot);
			for (uint j = 0; j < myNParam[0]; j++)
				myD2Fxy[i][j + theBegIndex] = myHessF[i + myNParam[0]][j];
		}
	cDVector myD2Fxz;
		if (myNParam[2] > 0)
		{
			myD2Fxz.ReAlloc(myNParamTot);
			for (uint j = 0; j < myNParam[0]; j++)
				myD2Fxz[j + theBegIndex] = myHessF[myNm1][j];
		}
	cDMatrix myD2Fyy = cDMatrix(myNParam[1], myNParam[1]);
		for (uint i = 0; i < myNParam[1]; i++)
			for (uint j = 0; j < myNParam[1]; j++)
				myD2Fyy[i][j] = myHessF[i + myNParam[0]][j + myNParam[0]];
	double* myD2Fyz = NULL;
		if (myNParam[1] > 0)
		{
			myD2Fyz = new double[myNParam[1]];
			for (uint k = 0; k < myNParam[1]; k++)
			{
				myD2Fyz[k] = 0;
				if (myNParam[2] > 0)
					myD2Fyz[k] = myHessF[myNm1][k + myNParam[0]];
			}
		}
		theHessData.mCurrentHessMu += myD2Fxx;
		for (uint k = 0; k < myNParam[1]; k++)
			theHessData.mCurrentHessMu -= myD2Fxy[k] * Transpose(theGradData.mGradMt[k]);
		if (myNParam[2] > 0)
			theHessData.mCurrentHessMu += myD2Fxz * Transpose(theGradData.mCurrentGradSigma);

		for (uint k = 0; k < myNParam[1]; k++)
		{
		cDVector myAux = myD2Fxy[k];
			for (uint l = 0; l < myNParam[1]; l++)
				myAux -= myD2Fyy[k][l] * theGradData.mGradMt[l];
			if (myNParam[2] > 0)
				myAux += myD2Fyz[k] * theGradData.mCurrentGradSigma;
			theHessData.mCurrentHessMu -= myAux * Transpose(theGradData.mGradMt[k]);
			theHessData.mCurrentHessMu -= myGradF[myNParam[0] + k] * theHessData.mHessMt[k];
		}
		if (myNParam[2] > 0)
		{
		cDVector myAux = myD2Fxz;
			for (uint l = 0; l < myNParam[1]; l++)
				myAux -= myHessF[myNm1][myNParam[0]+l] * theGradData.mGradMt[l];
			myAux += myHessF[myNm1][myNm1] * theGradData.mCurrentGradSigma;
			theHessData.mCurrentHessMu += myAux * Transpose(theGradData.mCurrentGradSigma);
			theHessData.mCurrentHessMu += myHessF[myNm1][myNm1] * theHessData.mCurrentHessSigma;
		}
		if (myNParam[1] > 0)
		{
			for (uint i = 0; i < myNParam[1]; i++)
				myD2Fxy[i].Delete();
			delete myD2Fxy;
			delete myD2Fyz;
		}
	}

	void cAbstCondMean::ComputeGradAndHessWithF(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals)
	{
	uint myNParam[3];
		GetNParamF(myNParam);
	uint myN = myNParam[0] + myNParam[1] + myNParam[2];
	uint myNm1 = myNParam[0] + myNParam[1];
	uint myNParamTot = theGradData.GetNParam();

	cDVector myGradF(myN);
		ComputeGradF(theDate, theData, myGradF);
		for (uint i = 0; i < myNParam[0]; i++)
			theGradData.mCurrentGradMu[theBegIndex + i] += myGradF[i];
		for (uint k = 0; k < MIN(myNParam[1], theDate); k++)
			theGradData.mCurrentGradMu -= myGradF[myNParam[0] + k] * theGradData.mGradMt[k];
		if (myNParam[2] > 0)
			theGradData.mCurrentGradMu += myGradF[myNParam[0] + myNParam[1]] * theGradData.mCurrentGradSigma;
		
	cDMatrix myHessF(myN, myN);
		ComputeHessF(theDate, theData, myHessF);
	cDMatrix myD2Fxx(myNParamTot, myNParamTot);
	for (uint i = 0; i < myNParam[0]; i++)
		for (uint j = 0; j < myNParam[0]; j++)
			myD2Fxx[i + theBegIndex][j + theBegIndex] = myHessF[i][j];
	cDVector* myD2Fxy = NULL;
		if (myNParam[1] > 0)
			myD2Fxy = new cDVector[myNParam[1]];
		for (uint i = 0; i < myNParam[1]; i++)
		{
			myD2Fxy[i].ReAlloc(myNParam[0]);
			for (uint j = 0; j < myNParam[0]; j++)
				myD2Fxx[i][j + theBegIndex] = myHessF[i + myNParam[0]][j];
		}
	cDVector myD2Fxz ;
		if (myNParam[2] > 0)
		{
			myD2Fxz.ReAlloc(myNParamTot);
			for (uint j = 0; j < myNParam[0]; j++)
				myD2Fxz[j+theBegIndex] = myHessF[myNm1][j];
		}
	cDMatrix myD2Fyy = cDMatrix(myNParam[1], myNParam[1]);
		for (uint i = 0; i < myNParam[1]; i++)
			for (uint j = 0; j < myNParam[1]; j++)		
				myD2Fyy[i][j] = myHessF[i + myNParam[0]][j + myNParam[0]];
	double* myD2Fyz = NULL;
		if (myNParam[1] > 0)
		{
			myD2Fyz = new double[myNParam[1]];
			for (uint k = 0; k < myNParam[1]; k++)
			{
				myD2Fyz[k] = 0;
				if (myNParam[2] > 0)
					myD2Fyz[k] = myHessF[myNm1][k + myNParam[0]];
			}
		}
		theHessData.mCurrentHessMu += myD2Fxx;
		for (uint k = 0; k < myNParam[1]; k++)
			theHessData.mCurrentHessMu -= myD2Fxy[k] * Transpose(theGradData.mGradMt[k]);
		if (myNParam[2] > 0)
			theHessData.mCurrentHessMu += myD2Fxz * Transpose(theGradData.mCurrentGradSigma);
		
		for (uint k = 0; k < myNParam[1]; k++)
		{
		cDVector myAux = myD2Fxy[k];
			for (uint l = 0; l < myNParam[1]; l++)
				myAux -= myD2Fyy[k][l] * theGradData.mGradMt[l];
			if (myNParam[2] > 0)
				myAux += myD2Fyz[k] * theGradData.mCurrentGradSigma;
			theHessData.mCurrentHessMu -= myAux * Transpose(theGradData.mGradMt[k]);
			theHessData.mCurrentHessMu -= myGradF[myNParam[0] + k] * theHessData.mHessMt[k];
		}
		if (myNParam[2] > 0)
		{
		cDVector myAux = myD2Fxz;
			for (uint l = 0; l < myNParam[1]; l++)
				myAux -= myHessF[myNm1][myNParam[0]] * theGradData.mGradMt[l];
			myAux += myHessF[myNm1][myNm1] * theGradData.mCurrentGradSigma;
			theHessData.mCurrentHessMu += myAux * Transpose(theGradData.mCurrentGradSigma);
			theHessData.mCurrentHessMu += myHessF[myNm1][myNm1] * theHessData.mCurrentHessSigma;
		}
		if (myNParam[1] > 0)
		{
			for (uint i = 0; i < myNParam[1]; i++)
				myD2Fxy[i].Delete();
			delete myD2Fxy;
			delete myD2Fyz;
		}

	}


#ifndef _RDLL_
	/*!
	 * \fn ostream& operator <<(ostream& theOut, const cAbstCondMean& theAbstCondMean)
	 * \param ostream& theOut: output stream (file or screen). Default cout.
	 * \param const cAbstCondMean& theAbstCondMean: the mean component model.
	 * \details Uses cAbstCondMean::Print method.
	 */
	ostream& operator <<(ostream& theOut, const cAbstCondMean& theAbstCondMean)
	{
		theAbstCondMean.Print(theOut) ;
		return theOut ;
	}
#endif // _RDLL_

	template<class T>
	T* TemplateCreateOneRealCondMean(void)
	{
//	static std::unique_ptr<T> myCondMean(new T());
//		return &*myCondMean;
		return new T();
	}

	template<class T>
	T* TemplateCreateOneRealCondMean(cAbstCondMean* theAbstCondMean)
	{
	T* mySrc = static_cast<T *>(theAbstCondMean);
		if (mySrc)
		{
		T* myCondMean;
			myCondMean = new T(*mySrc);
			return myCondMean;
	
//	static std::shared_ptr<T> myCondMean(new T(*mySrc));
//			return &*myCondMean;
		}
		else
			throw cError("Wrong contional mean in TemplateCreateOneRealCondMean");
	}

	/*!
	* \fn cAbstCondMean* CreateOneRealCondMean(const eCondMeanEnum theType)
	* \param const eCondMeanEnum theType: code of the real condtional mean component.
	* \details This function has to be changed when adding a new conditional mean type.
	*/
	cAbstCondMean* CreateOneRealCondMean(eCondMeanEnum theType)
	{
		switch (theType)
		{
		case eConst:
			return TemplateCreateOneRealCondMean<cConst>();
			break;
		case eAr:
			return TemplateCreateOneRealCondMean<cAr>();
			break;
		case eMa:
			return TemplateCreateOneRealCondMean<cMa>();
			break;
		case eLinReg:
			return TemplateCreateOneRealCondMean<cLinReg>();
			break;
		case eStdDevInMean:
			return TemplateCreateOneRealCondMean<cStdDevInMean>();
			break;
		case eVarInMean:
			return TemplateCreateOneRealCondMean<cVarInMean>();
			break;
		case eArfima:
//			return TemplateCreateOneRealCondMean<cArfima>();
			break;
		default:
			throw cError("CreateOneRealCondMean: unknown conditional mean type");
			break;
		}
	}

	cAbstCondMean* CreateOneRealCondMean(cAbstCondMean& theAbstCondMean)
	{
		switch (theAbstCondMean.GetCondMeanType())
		{
		case eConst:
			return TemplateCreateOneRealCondMean<cConst>(&theAbstCondMean);
			break;
		case eAr:
			return TemplateCreateOneRealCondMean<cAr>(&theAbstCondMean);	
			break;
		case eMa:
			return TemplateCreateOneRealCondMean<cMa>(&theAbstCondMean);
			break;
		case eLinReg:
			return TemplateCreateOneRealCondMean<cLinReg>(&theAbstCondMean);
			break;
		case eStdDevInMean:		
			return TemplateCreateOneRealCondMean<cStdDevInMean>(&theAbstCondMean);
			break;
		case eVarInMean:
			return TemplateCreateOneRealCondMean<cVarInMean>(&theAbstCondMean);
			break;
		case eArfima:
//			return TemplateCreateOneRealCondMean<cArfima>(&theAbstCondMean);
			break;
		default:
			throw cError("CreateOneRealCondMean: unknown conditional mean type");
			break;
		}
	}
	
	cAbstCondMean* cAbstCondMean::PtrCopy(void) 	
	{
		return CreateOneRealCondMean(*this);
	}
	
}//namespace
