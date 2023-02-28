#include "StdAfxRegArchLib.h"

namespace RegArchLib {
static cPolynome ourPolynome = cPolynome();

	cPolynome::cPolynome(uint theDegree)
	{
		mCoeff.ReAlloc(theDegree+1);
	}

	cPolynome::cPolynome(uint theDegree, double* theCoeff)
	{
		mCoeff.ReAlloc(theDegree + 1, theCoeff);
	}

	cPolynome::cPolynome(cDVector& theCoeff)
	{
		mCoeff = theCoeff;
	}
		
	cPolynome::cPolynome(const cPolynome &theSrc)
	{
		mCoeff = theSrc.mCoeff;
	}

	cPolynome::~cPolynome()
	{
		if (mCoeff.GetSize() > 0)
			mCoeff.Delete();
	}

	void cPolynome::Delete(void)
	{
		if (mCoeff.GetSize() > 0)
			mCoeff.Delete();
	}

	void cPolynome::Resize(uint theDegree)
	{
		mCoeff.ReAlloc(theDegree + 1);
	}

	uint cPolynome::GetNDegree(void) const
	{
		return mCoeff.GetSize() - 1;
	}

	double& cPolynome::operator[](int theIndex) const
	{
		return mCoeff[theIndex];
	}

	cPolynome& cPolynome::operator=(const cPolynome& theSrc)
	{
		mCoeff = theSrc.mCoeff;
		return *this;
	}

	cPolynome& cPolynome::operator+(const cPolynome& theP)
	{
	uint myp1 = GetNDegree();
	uint myp2 = theP.GetNDegree();
	uint myp = MAX(myp1, myp2);
		ourPolynome.Resize(myp);
		for (uint i = 0; i <= myp; i++)
		{
			if (i <= myp1)
				ourPolynome.mCoeff[i] =  mCoeff[i];
			if (i <= myp2)
				ourPolynome.mCoeff[i] += (theP.mCoeff)[i];
		}
		return ourPolynome;
	}

	cPolynome& cPolynome::operator-(const cPolynome& theP)
	{
		uint myp1 = GetNDegree();
		uint myp2 = theP.GetNDegree();
		uint myp = MAX(myp1, myp2);
		ourPolynome.Resize(myp);
		for (uint i = 0; i <= myp; i++)
		{
			if (i <= myp1)
				ourPolynome.mCoeff[i] = mCoeff[i];
			if (i <= myp2)
				ourPolynome.mCoeff[i] -= (theP.mCoeff)[i];
		}
		return ourPolynome;
	}

	cPolynome& cPolynome::operator*(const cPolynome& theP)
	{
	uint myp1 = GetNDegree();
	uint myp2 = theP.GetNDegree();
	uint myp = myp1 + myp2;
		ourPolynome.Resize(myp);
		for (uint i = 0; i <= myp1; i++)
			for (uint j = 0; j <= myp2; j++)
				ourPolynome.mCoeff[i + j] += mCoeff[i] * (theP.mCoeff)[j];
		return ourPolynome;
	}

#ifndef _RDLL_
	void cPolynome::Print(void)
	{
	uint myDegree = GetNDegree();
		if (mCoeff[myDegree] != 0)
			cout << mCoeff[myDegree] << "*x^" << myDegree;
		for (uint i = myDegree - 1; i >= 1; i--)
		{
			if (mCoeff[i] > 0)
				cout << "+" << mCoeff[i] << "*x^" << i;
			else
				if (mCoeff[i] < 0)
					cout << mCoeff[i] << "*x^" << i;
		}
		if (mCoeff[0] > 0)
			cout << "+" << mCoeff[0] << endl;
		else
			if (mCoeff[0] < 0)
				cout << mCoeff[0] << endl;
			else 
				cout << endl;
	}
#else
	void cPolynome::Print(void)
	{
		uint myDegree = GetNDegree();
		if ((mCoeff)[myDegree] != 0)
			Rprintf("%f*x^%d", (mCoeff)[myDegree],myDegree);
		for (uint i = myDegree - 1; i >= 1; i--)
		{
			if ((mCoeff)[i] > 0)
				Rprintf("+%f*x^%d", (mCoeff)[i], i);
			else
				if ((mCoeff)[i] < 0)
					Rprintf("%f*x^%d", (mCoeff)[i], i);
		}
		if ((mCoeff)[0] > 0)
			Rprintf("+%f\n", (mCoeff)[0]);
		else
			if ((mCoeff)[0] < 0)
				Rprintf("%f\n", (mCoeff)[0]);
			else 
				Rprintf("\n");
	}
#endif //_RDLL_

	cPolynome& operator *(cPolynome& theP, double theVal)
	{
	uint myp = theP.GetNDegree();
		ourPolynome.Resize(myp);

		for (uint i = 0; i <= myp; i++)
			ourPolynome.mCoeff[i] = (theP.mCoeff)[i] * theVal;
		return ourPolynome;
	}

	cPolynome& operator *(double theVal, cPolynome& theP)
	{
	uint myp = theP.GetNDegree();
		ourPolynome.Resize(myp);

		for (uint i = 0; i <= myp ; i++)
			ourPolynome.mCoeff[i] = (theP.mCoeff)[i] * theVal;
		return ourPolynome;
	}

	cPolynome& cPolynome::operator +=(const cPolynome& theP)
	{
		*this = *this + theP;
		return *this;
	}

	cPolynome& cPolynome::operator -=(const cPolynome& theP)
	{
		*this = *this - theP;
		return *this;
	}

	cPolynome& cPolynome::operator *=(const cPolynome& theP)
	{
	cPolynome myAuxPol(0);
		myAuxPol = (*this) * theP;
		*this = myAuxPol;
		return *this;
	}

	cPolynome& cPolynome::operator *=(double theVal)
	{
		this->mCoeff *= theVal;
		return *this;
	}

	void ComputeDeltaPowD(double theD, uint theDegree, cPolynome& theRes)
	{
		theRes.Resize(theDegree);
		theRes[0] = 1.0;
		for (uint i = 1; i <= theDegree; i++)
			theRes[i] = -theRes[i - 1] * (theD - (double)(i - 1)) / (double)i;
	}

	void ComputeLogDelta(uint theDegree, cPolynome& theRes)
	{
		theRes.Resize(theDegree);
		theRes[0] = 0.0;
		for (uint i = 1; i <= theDegree; i++)
			theRes[i] = -1.0 / (double)i;
	}

	/*
	void IncrPowDiv(cPolynome& theNum, cPolynome& theDen, uint theOrder, cPolynome& theQuot, cPolynome& theRest)
	{
		//Division of two polynomials by increasing powers

		uint p1 = theNum.GetNDegree();

		theQuot.Resize(theOrder + p1);
		theRest.Resize(theOrder + p1);
		cPolynome myAux(theOrder + p1);
		cPolynome myMonome(theOrder + p1);

		for (uint i = 0; i <= p1; i++)
			theRest[i] = theNum[i];
		uint j = 0;
		while (j <= theOrder)
		{
			uint k = j;
		bool myFini = (k > theOrder + p1);
			while (! myFini)
			{
				if (theRest[k] == 0)
				{
					k++;
					myFini = (k > theOrder + p1);
				}
				else
					myFini = true;
			}
			if (k <= theOrder + p1)
			{
				(myMonome.mCoeff)[k] = (theQuot.mCoeff)[k] = theRest[k] / theDen[0];
				myAux = myMonome * theDen;
				(myMonome.mCoeff)[k] = 0;
				theRest -= myAux;
				j++;
			}
			else
				j = theOrder + 1;

		}
		myAux.Delete();
		myMonome.Delete();
	}
*/
	void IncrPowDiv(cPolynome& theNum, cPolynome& theDen, uint theOrder, cPolynome& theQuot, cPolynome& theRest)
	{
	uint myp1 = theNum.GetNDegree();
	uint myp2 = theDen.GetNDegree();
		theQuot.Resize(theOrder);
		theRest.Resize(MAX(theOrder + myp2, myp1));
		for (uint k = 0; k <= myp1; k++)
			theRest[k] = theNum[k];
		for (uint k = 0; k <= theOrder; k++)
		{
		double myCoeff = theQuot[k] = theRest[k] / theDen[0];
			if (myCoeff != 0)
			{
				for (uint j = 0; j <= myp2; j++)
					theRest[j+k] -= myCoeff * theDen[j];

			}
		}
	}


	cPolynome& TrunkMult(cPolynome& theP, cPolynome& theQ, uint theMaxDegree)
	{
		ourPolynome.Resize(theMaxDegree);
		for (uint i = 0; i <= theP.GetNDegree(); i++)
			for (uint j = 0; j <= theQ.GetNDegree(); j++)
				if (i + j <= theMaxDegree)
					ourPolynome.mCoeff[i + j] += (theP.mCoeff)[i] * (theQ.mCoeff)[j];
		return ourPolynome;
	}

	double cPolynome::BackwardPolOp(const cDVector& theYt, uint theIndex0, double thePow) const
	{
	int myDegree = GetNDegree();
	double myRes = mCoeff[0];
		if (thePow == 1.0)
		{
			for (int i = 1; i <= MIN(myDegree, (int)theIndex0); i++)
				myRes += mCoeff[i] * theYt[(int)theIndex0 - i];
		}
		else
		{
			for (int i = 1; i <= MIN(myDegree, (int)theIndex0); i++)
				myRes += mCoeff[i] * pow(theYt[(int)theIndex0 - i], thePow);
		}

		return myRes;
	}

	cPolynome& TrunkPoly(cPolynome& theP, uint theMaxDegree)
	{
		ourPolynome.Resize(MIN(theP.GetNDegree(), theMaxDegree));
		for (uint i = 0; i <= MIN(theP.GetNDegree(), theMaxDegree); i++)
			ourPolynome[i] = theP[i];
		return ourPolynome;
	}


} //namespace
