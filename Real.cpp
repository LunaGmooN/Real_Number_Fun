#include <iomanip>
#include <cstdlib>
#include <string>
#include <math.h>

#include "Real.h"
using namespace std;

/*********************************************************************************************/
/*****************Constructors/Destructor ****************************************************/

/*Default Constructor ************/
Real::Real ()
{
  wholeNum = decimalNum = 0;
  WholeNums.push_back('0');
  DecimalNums.push_back('0');

  wholeNumLength = 1;
  decimalNumLength = 1;

  isNeg = false;
  wholeNumZero = true;
}

/* Copy Constructor *************/
Real::Real(const Real & R)
{
  WholeNums = R.WholeNums;
  DecimalNums = R.DecimalNums;

  wholeNumLength = R.wholeNumLength;
  decimalNumLength = R.decimalNumLength;

  isNeg = R.isNeg;
  wholeNumZero = R.wholeNumZero;
}

/* Constructor using String *****/
Real::Real(const string & S)
{
  string wholeS, decimalS;
  size_t whole = S.find('.');
  wholeNumLength = decimalNumLength = 0;
  isNeg = false;

  //Separate whole and decimal nums in string
  wholeS = S.substr(0, whole);
  if (whole > S.length())
    decimalS = "0";
  else
    decimalS = S.substr(whole+1, S.length()-1);

  //Initialize Class
  init(wholeS, decimalS);

}

/* Constructor using double *****/
Real::Real(double D)
{
  //Convert to whole and decimal strings
  string S = to_string(D);
  string wholeS, decimalS;
  size_t whole = S.find('.');
  wholeNumLength = decimalNumLength = 0;
  isNeg = false;

  //Separate whole and decimal nums in string
  wholeS = S.substr(0, whole);
  if (whole > S.length())
    decimalS = "0";
  else
    decimalS = S.substr(whole+1, S.length()-1);

  //Initialize Class
  init(wholeS, decimalS);
}

/* Constructor using long long **/
Real::Real(long long W, long long D)
{
  string wholeS, decimalS;
  wholeNumLength = decimalNumLength = 0;
  isNeg = false;

  //Separate whole and decimal nums in string
  wholeS = to_string(W);
  decimalS = to_string(abs(D));

  //Initialize Class
  init(wholeS, decimalS);
}

/* Destructor *******************/
Real::~Real()
{

}

/*********************************************************************************************/
/*******************Assignment Operator ******************************************************/

/* Assignment operator *********/
Real & Real::operator = (const Real & R)
{
  WholeNums.clear();
  WholeNums = R.WholeNums;
  DecimalNums.clear();
  DecimalNums = R.DecimalNums;

  wholeNumLength = R.wholeNumLength;
  decimalNumLength = R.decimalNumLength;

  isNeg = R.isNeg;

  WholeNumSum();
  
  return *this;
}

/*********************************************************************************************/
/***********************Friend Operators *****************************************************/

/* Output Stream operator *********/
ostream & operator << (ostream & outs, const Real & R)
{
  if (R.isNeg )
    outs<< '-';
  for (int i = 0; i < R.wholeNumLength; i++)
    outs << R.WholeNums[i];
  outs << '.';
  for (int i = 0; i < R.decimalNumLength; i++)
    outs << R.DecimalNums[i];
      
  return outs;
}

/* Input Stream operator *********/
istream & operator >> (istream & ins, Real & R)
{
  string S;
  ins >> S;

  Real R_temp(S);
  R = R_temp;
  
  return ins;
}

/*********************************************************************************************/
/**********************Logical Operators *****************************************************/

/* Equal To operator *********/
bool Real::operator == (const Real & R) const
{
  bool isEqual = 1;

  /******Testing whole number*****/
  int numDiff = R.wholeNumLength - wholeNumLength;
  size_t shortWholeNumLen = wholeNumLength;
  size_t thisAdj, rAdj;
  thisAdj = rAdj = 0;

  if (numDiff != 0)//Test for leading 0's

    {
      isEqual = 0;
      if (numDiff > 0)
	{
	  //R's whole number is longer
	  rAdj = numDiff;
	  //shortWholeNumLen should be correct
	  for (int i = 0; i < numDiff; i++)
	    {
	      //If anything but leading 0's return false
	      if (R.WholeNums[i] != '0')
		return isEqual;
	    }
	}else 
	{
	  //This's whole number is longer
	  thisAdj = abs(numDiff);
	  shortWholeNumLen += numDiff;
	  for (int i = 0; i < abs(numDiff); i++)
	    {
	      //If anything but leading 0's return false
	      if (R.WholeNums[i] != '0')
		return isEqual;
	    }
	}
      isEqual = 1;
    }//Assume any length difference is only leading 0s
  
  for (size_t i = 0; i < shortWholeNumLen; i++)
    {
      if (WholeNums[i + thisAdj] != R.WholeNums[i + rAdj])
	{
	  isEqual = 0;
	  return isEqual;
	}
    }

  /*****Testing decimal number*****/
  numDiff = R.decimalNumLength - decimalNumLength;
  size_t shortDecimalNumLen = decimalNumLength;
  
  if (numDiff != 0)
    {
      isEqual = 0;
      if (numDiff > 0)
	{
	  //R's decimal number is longer
	  //shortWholeNumLen should be correct
	  //Test for trailing 0's
	  for (int i = shortDecimalNumLen - 1; i < numDiff; i++)
	    {
	      //If anything but leading 0's return false
	      if (R.WholeNums[i] != '0')
		return isEqual;
	    }
	}else 
	{
	  //This's decimal numberis longer
	  shortDecimalNumLen += numDiff;
	  //Test for leading 0's
	  for (int i = shortDecimalNumLen - 1; i < abs (numDiff); i++)
	    {
	      //If anything but leading 0's return false
	      if (R.WholeNums[i] != '0')
		return isEqual;
	    }
	}
      isEqual = 1;
    }//Assuming difference in length is attributable to trailing 0s
  
  for (size_t i = 0; i < shortDecimalNumLen; i++)
    {
      if (DecimalNums[i] != R.DecimalNums[i])
	{
	  isEqual = 0;
	  return isEqual;
	}
    }
  if (isNeg != R.isNeg)
    isEqual = false;
  
  return isEqual;
}

/* Not Equal To operator *********/
bool Real::operator != (const Real & R) const
{
  bool isNotEqual = !(*this == R);
  return isNotEqual;
}

/* Greater than operator *********/
bool Real::operator > (const Real & R) const
{
  bool isGreater = 1;

  if (isNeg && !R.isNeg)
    return false;
  else if (!isNeg && R.isNeg)
    return true;
  
  /******Testing whole number*****/
  if (wholeNumLength > R.wholeNumLength)
    return isGreater;
  if (R.wholeNumLength > wholeNumLength)
    return !isGreater;
  
  for (size_t i = 0; i < wholeNumLength; i++)
    {
      if (WholeNums[i] !=  R.WholeNums[i])
	{
	  if (WholeNums[i] <  R.WholeNums[i])
	    isGreater = 0;
	  return isGreater;
	}
    }

  /*****Testing decimal number*****/
  int numDiff = R.decimalNumLength - decimalNumLength;
  size_t shortDecimalNumLen = decimalNumLength;

  if (numDiff < 0)
    shortDecimalNumLen += numDiff;

  for (size_t i = 0; i < shortDecimalNumLen; i++)
    {
      if (DecimalNums[i] != R.DecimalNums[i])
	{
	  if (DecimalNums[i] <  R.DecimalNums[i])
	    isGreater = 0;
	  return isGreater;
	}
    }

  if (numDiff != 0)//Testing for trailing 0s
    {
      if (numDiff > 0)
	{
	  //R's decimal number is longer
	  isGreater = 0;
	  //Test for trailing 0's
	  for (int i = shortDecimalNumLen - 1; i < numDiff; i++)
	    {
	      //If anything but leading 0's return false
	      if (R.WholeNums[i] != '0')
		return isGreater;
	    }
	}else 
	{
	  //This's decimal number is longer
	  //Test for leading 0's
	  for (int i = shortDecimalNumLen - 1; i < abs (numDiff); i++)
	    {
	      //If anything but leading 0's return false
	      if (R.WholeNums[i] != '0')
		return isGreater;
	    }
	}
    }//Assuming difference in length is attributable to trailing 0s

  isGreater = 0;//Numbers must be equal
  return isGreater;
}

/* Greater than or Equal to operator *********/
bool Real::operator >= (const Real & R) const
{
  bool isGreater;
  bool isEqual = (*this == R);
  if (!isEqual)
    isGreater = (*this > R);

  return isEqual || isGreater;
}

/* Less than operator *********/
bool Real::operator < (const Real & R) const
{
  bool isNotGreaterEqual = !(*this >= R);
  return isNotGreaterEqual;
}

/* Less than Equal to operator *********/
bool Real::operator <= (const Real & R) const
{
  bool isNotGreater = !(*this > R);
  return isNotGreater;
}


/*********************************************************************************************/
/*****************Mathamatical Operators *****************************************************/

/* Plus operator *********/
Real Real::operator + (const Real & R) const
{
  return AddSub(R, 1);
}

/* Plus Equals operator *********/
Real Real::operator += (const Real & R)
{
  *this = *this + R;
  return *this;
}

/* Post-Increment operator *********/
Real Real::operator ++ ()
{
  Real one ("1.0");
  *this = *this + one;
    
  return *this;
}

/* Pre-Increment operator *********/
Real Real::operator ++ (int)
{
  Real temp = *this;
  ++*this;
  return temp;
}

/* Minus operator *********/
Real Real::operator - (const Real & R) const
{
  return AddSub(R, -1); 
}

/* Minus equals operator *********/
Real Real::operator -= (const Real & R)
{
  *this = *this - R;
  return *this;
}

/* Post Decrement operator *********/
Real Real::operator -- ()
{
  Real one ("-1.0");
  *this = *this + one;

  return *this;

}

/* Pre Decrement operator *********/
Real Real::operator -- (int)
{
  Real temp = *this;
  --*this;

  return *this;

}

/* Multiplication operator *********/
Real Real::operator * (const Real & R) const
{
  Real Prod;

  string temp = "";
  int carry = 0;
  int numDecimals = decimalNumLength + R.decimalNumLength;
  int num0= 0;
  
  //Set up First and Second multipliers
  int mult1, mult2;
  mult1 = 1;
  mult2 = 1;
  if (isNeg)
    mult1 = -1;
  if (R.isNeg)
    mult2 = -1;

  int j = decimalNumLength - 1;
  int prod_temp;

  for (int j = decimalNumLength  - 1; j >-1; j--, num0++)
    {
      for (int i = R.decimalNumLength - 1; i > -1; i--)
	{
	  prod_temp = (DecimalNums[j] - '0') * (R.DecimalNums[i] - '0') + carry;
	  //reset carry
	  carry = 0;
	  
	  if (prod_temp > 9)
	    {
	      carry = prod_temp / 10;
	      prod_temp -= carry * 10;
	    }
	  temp += to_string(prod_temp);
	}
      for (int i = R.wholeNumLength - 1; i > -1; i--)
	{
	  prod_temp = (DecimalNums[j] - '0') * (R.WholeNums[i] - '0') + carry;
	  //reset carry
	  carry = 0;
	  
	  if (prod_temp > 9)
	    {
	      carry = prod_temp / 10;
	      prod_temp -= carry * 10;
	    }
	  temp += to_string(prod_temp);
	}
      //Finalize string
      int n = temp.length();
      for (int left = 0, right= n - 1; left < right; left++, right--)
	swap(temp[left], temp[right]);
      for (int left = 0; left < num0; left++)
	temp += '0';
      temp.insert(temp.length() - numDecimals, ".");

      Real Temp (temp);
      temp = "";
      Prod += Temp;
    }
  for (int j = wholeNumLength - 1; j >-1; j--, num0++)
    {
      for (int i = R.decimalNumLength - 1; i > -1; i--)
	{
	  prod_temp = (WholeNums[j] - '0') * (R.DecimalNums[i] - '0') + carry;
	  
	  //reset carry
	  carry = 0;
	  
	  if (prod_temp > 9)
	    {
	      carry = prod_temp / 10;
	      prod_temp -= carry * 10;
	    }
	  temp += to_string(prod_temp);
	}
      
      for (int i = R.wholeNumLength - 1; i > -1; i--)
	{
	  prod_temp = (WholeNums[j] - '0') * (R.WholeNums[i] - '0') + carry;
	  //reset carry
	  carry = 0;
	  
	  if (prod_temp > 9)
	    {
	      carry = prod_temp / 10;
	      prod_temp -= carry * 10;
	    }
	  temp += to_string(prod_temp);
	}
      //Finalize string
      int n = temp.length();
      for (int left = 0, right= n - 1; left < right; left++, right--)
	swap(temp[left], temp[right]);
      for (int left = 0; left < num0; left++)
	temp += '0';
      temp.insert(temp.length() - numDecimals, ".");

      Real Temp (temp);
      Prod += Temp;
    }
    
  //Eliminate excess 0s and check for 0 whole num
  Prod.WholeNumSum();
  return Prod;
  
  return *this;

}

/*********************************PRIVATE*****************************************/
/*********************************************************************************/

/* Initialize class************/
void Real::init(string & wholeS, string & decimalS)
{
  //determine WholeNum and save to vector
  size_t i = 0;
  if (wholeS[0] == '+' || wholeS[0] == '-')
    {
      i++;
      if (wholeS[0] == '-')
	isNeg = true;
    }
  for (i; i < wholeS.length(); i++)
    {
      if(!isdigit(wholeS[i]))
	{
	  WholeNums.clear();
	  WholeNums.push_back('0');
	  wholeNumLength = 1;
	  break;
	}
      WholeNums.push_back(wholeS[i]);
      wholeNumLength++;
    }

  //determine DecimalNum and save to vector
  for (i = 0; i < decimalS.length(); i++)
    {
      if(!isdigit(decimalS[i]))
	{
	  DecimalNums.clear();
	  DecimalNums.push_back('0');
	  decimalNumLength = 1;
	  break;
	}
      DecimalNums.push_back(decimalS[i]);
      decimalNumLength++;
    }
  
  WholeNumSum();
}

/* WholeNumSum *******************/
bool Real::AbsGreaterThan(const Real & R)const
{
  bool isGreater = 1;
  
  /******Testing whole number*****/
  if (wholeNumLength > R.wholeNumLength)
    return isGreater;
  if (R.wholeNumLength > wholeNumLength)
    return !isGreater;
  
  for (size_t i = 0; i < wholeNumLength; i++)
    {
      if (WholeNums[i] !=  R.WholeNums[i])
	{
	  if (WholeNums[i] <  R.WholeNums[i])
	    isGreater = 0;
	  return isGreater;
	}
    }

  /*****Testing decimal number*****/
  int numDiff = R.decimalNumLength - decimalNumLength;
  size_t shortDecimalNumLen = decimalNumLength;

  if (numDiff < 0)
    shortDecimalNumLen += numDiff;

  for (size_t i = 0; i < shortDecimalNumLen; i++)
    {
      if (DecimalNums[i] != R.DecimalNums[i])
	{
	  if (DecimalNums[i] <  R.DecimalNums[i])
	    isGreater = 0;
	  return isGreater;
	}
    }

  if (numDiff != 0)//Testing for trailing 0s
    {
      if (numDiff > 0)
	{
	  //R's decimal number is longer
	  isGreater = 0;
	  //Test for trailing 0's
	  for (int i = shortDecimalNumLen - 1; i < numDiff; i++)
	    {
	      //If anything but leading 0's return false
	      if (R.WholeNums[i] != '0')
		return isGreater;
	    }
	}else 
	{
	  //This's decimal number is longer
	  //Test for leading 0's
	  for (int i = shortDecimalNumLen - 1; i < abs (numDiff); i++)
	    {
	      //If anything but leading 0's return false
	      if (R.WholeNums[i] != '0')
		return isGreater;
	    }
	}
    }//Assuming difference in length is attributable to trailing 0s

  isGreater = 0;//Numbers must be equal
  return isGreater;
}

/* Plus operator ******************/
Real Real::AddSub (const Real & R, int sign) const
{
  //Find which has larger magnitude
  bool thisIsBigger = 0;
  if (AbsGreaterThan(R))
    thisIsBigger = 1;
  
  //Sign will be used to signal btw add and sub
  Real Sum;
  vector <char> *ptrD = &Sum.DecimalNums;
  vector <char> *ptrW = &Sum.WholeNums;
  int carry = 0;
  int pull = 0;

  //Set up First and Second multipliers
  int mult1, mult2, add1, add2;
  mult1 = 1;
  mult2 = 1;
  
  //track size differences
  int small = 0;
  
  if (thisIsBigger)
    {
      Sum = *this;
      if (isNeg)
	  mult1 = -1;
      if (R.isNeg)
	mult2 = -1 * sign;
      else
	mult2 = sign;
      small = R.decimalNumLength;
    }
  else
    {
      Sum = R;
      if (R.isNeg)
	  mult1 = -1 * sign;
      else
	mult1 = sign;
      if (isNeg)
	mult2 = -1;
      if (sign < 0)
	Sum.isNeg = !Sum.isNeg;
      small = decimalNumLength;
    }

  /*********Decimal Sum**********/
  //Add trailing 0s
  if (thisIsBigger)
    {
      for (int i = decimalNumLength - 1; i < R.decimalNumLength; i ++)
	{
	  ptrD->push_back('0');
	  Sum.decimalNumLength++;
	}    }
  else
    {
      for (int i = R.decimalNumLength; i < decimalNumLength; i ++)
	{
	  ptrD->push_back('0');
	  Sum.decimalNumLength++;
	}    }
  //Add
  for (int i = small - 1; i > -1; i--)
    {
      int sum_temp;
      if (thisIsBigger)
	{
	  add1 = ptrD->at(i) - '0';
	  add2 = R.DecimalNums[i] - '0';  
	}
      else
	{
	  add1 = ptrD->at(i) - '0';
	  add2  = DecimalNums[i] - '0';
	}
      sum_temp = (mult1 * add1) + (mult2 *add2) + carry + mult1 * pull;
      
      //Check carry
      if (sum_temp > 9)
	carry = 1;
      else if  (sum_temp - pull < -9)
	carry = -1;
      else
	carry = 0;
      
      //Check pull
      if (mult1 != mult2 && add1 < add2)
	{
	  pull = -1;
	  if (i != 0 || !Sum.wholeNumZero)
	      sum_temp += 10;
	}
      else
	pull = 0;
      
      sum_temp = abs(sum_temp) % 10;
      string s_temp = to_string(sum_temp);
      ptrD->at(i) =  s_temp[0];

    }
  /*********Whole Num Sum*********/
  int adj1 = wholeNumLength - R.wholeNumLength;
  adj1 = abs(adj1);
  int numZeros = 0;
  int endPoint = Sum.wholeNumLength;

  if (thisIsBigger)
    {
      if (!R.wholeNumZero)
	small = R.wholeNumLength;
      else
	small = 0;
    }
  else
    {
      if (!wholeNumZero)
	small = wholeNumLength;
      else
	small = 0;
    }
  
  //Add
  for (int i = small - 1; i > -1; i--)
    {
      int sum_temp;
      if (thisIsBigger)
	{
	  add1 = ptrW->at(i + adj1) - '0';
	  add2 = R.WholeNums[i] - '0';  
	}
      else
	{
	  add1 = ptrW->at(i + adj1) - '0';
	  add2  = WholeNums[i] - '0';
	}
      sum_temp = (mult1 * add1) + (mult2 *add2) + carry + (mult1 *pull);

      //Check carry
      if (sum_temp > 9)
	carry = 1;
      else if (sum_temp - pull < -9)
	carry = -1;
      else
	carry = 0;
      
      //Check pull
      if (mult1 != mult2 && add1 < add2)
	{
	  pull = -1;
	  if (i + small - 1 < ptrW->size())
	      sum_temp += mult1 * 10;
	}
      else
	pull = 0;

      sum_temp = abs(sum_temp) % 10;
      string s_temp = to_string(sum_temp);
      ptrW->at(i + adj1) =  s_temp[0];
      endPoint = i + adj1;
    }
  
  /*********Deal with Carry/pull*********/
  while (carry)
    {
      if (--endPoint >-1)
	{
	  add1 = ptrW->at(endPoint) - '0';
	  int sum_temp = (mult1 * add1) + carry;
	  if (sum_temp > 9)
	    carry = 1;
	  else if (sum_temp < -9)
	    carry = -1;
	  else
	    carry = 0;
	  sum_temp = abs(sum_temp) %10;
	  string s_temp = to_string(sum_temp);
	  ptrW->at(endPoint) = s_temp[0];
	}
      else
	{
	  ptrW->insert(ptrW->begin(), '1');
	  carry = 0;
	  Sum.wholeNumLength++;
	}
    }
  while (pull)
    {
      if (--endPoint > -1)
	{
	  add1 = ptrW->at(endPoint) - '0';
	  int sum_temp = mult1 * (add1 + pull);
	  if (mult1 * sum_temp < 0)
	    sum_temp += 10;
	  else
	    pull = 0;
	  sum_temp = abs(sum_temp) % 10;
	  string s_temp = to_string(sum_temp);
	  ptrW->at(endPoint) = s_temp[0];
	}
      else
	{
	  ptrW->erase(ptrW->begin());
	  Sum.wholeNumLength--;
	  pull = 0;
	}
    }

  //Eliminate excess 0s and check for 0 whole num
  Sum.WholeNumSum();
  return Sum;
}

/* WholeNumSum *******************/
void Real::WholeNumSum()
{
  int sum = 0;
  for (int i = 0; i < wholeNumLength; i++)
    {
      if (WholeNums[i] == '0' && i < wholeNumLength - 1)
	{
	  WholeNums.erase(WholeNums.begin());
	  wholeNumLength--;
	  i--;
	}
      else
	{
	  
	  sum += WholeNums[i] - '0';
	  if (sum != 0)
	    {
	      wholeNumZero = false;
	      return;
	    }
	}
    }
  if (wholeNumLength == 0)
    {
      WholeNums.push_back('0');
      wholeNumLength++;
    }
  wholeNumZero = true;
}



