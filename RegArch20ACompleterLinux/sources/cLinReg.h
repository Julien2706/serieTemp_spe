#pragma once 
#ifndef _CLINREG_H_
#define _CLINREG_H_

	/*!
		\file cLinReg.h
		\brief Definition of the cLinReg class
	
		\author Jean-Baptiste DURAND, Ollivier TARAMASCO 
		\date dec-18-2006 - Last change feb-18-2011
	*/
#include "cAbstCondMean.h"
#include "cRegArchValue.h"
#include "cRegArchGradient.h"

namespace RegArchLib {
	class cLinReg : public cAbstCondMean
	{
	private :
		cDVector mvBeta ;
	public :
		cLinReg(int theNLinReg = 0) ;
        cLinReg(const cDVector& theBeta) ;
		cLinReg(const cLinReg& theLinReg) ; /// Recopy constructor
		virtual ~cLinReg() ;
		//cAbstCondMean* PtrCopy(void) ; /// < Return a copy of *this
		void Delete(void) ;
	#ifdef _RDLL_
		void Print(void);
	#else
		void Print(ostream& theOut = cout) const;
	#endif //_RDLL_
		void SetDefaultInitPoint(double theMean, double theVar) ;
		void SetDefaultInitPoint(cRegArchValue& theValue);
		void Set(const double theValue, const uint theIndex = 0, const uint theNumParam = 0); ///< Set model parameters.
		void Set(const cDVector& theVectParam, const uint theNumParam=0) ; ///< Set model parameters.
		double Get(const uint theIndex, const uint theNumParam) ;
		cDVector& Get(const uint theNumParam);
		void ReAlloc(const cDVector& theModel, const uint theNumParam=0) ;
		void ReAlloc(const uint theModel, const uint theNumParam=0) ;
		cLinReg& operator=(const cLinReg& theSrc) ;
		void ReAllocProxyMeanParameters(uint theOldNParam=0) {};
		void UpdateProxyMeanParameters(void) {};
		double ComputeMean(uint theDate, const cRegArchValue& theData) const ;
		uint GetNParam(void) const ; ///< Return number of parameters
		uint GetNLags(void) const ;
		void RegArchParamToVector(cDVector& theDestVect, uint theIndex = 0) ;
		void VectorToRegArchParam(const cDVector& theSrcVect, uint theIndex = 0) ;
		void ComputeGrad(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData, uint theBegIndex, cAbstResiduals* theResiduals) ;
		void GetNParamF(uint theNParam[3]) const;
		void ComputeGradF(uint theDate, const cRegArchValue& theData, cDVector& theGradF);
		void ComputeHess(uint theDate, const cRegArchValue& theData, cRegArchGradient& theGradData,cRegArchHessien& theHessData, uint theBegIndex, cAbstResiduals* theResiduals) ;
		void ComputeHessF(uint theDate, const cRegArchValue& theData, cDMatrix& theHessF);
		void GetParamName(uint theIndex, char** theName);
		void GetParamName(uint theIndex, string theName[]);
	} ;

}
#endif // _CLinReg_H_
