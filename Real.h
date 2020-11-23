#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Real
{
public:
  Real();
  Real(const Real & R);
  Real(const string & S);
  Real (long long W, long long D);
  Real (double D);
  ~Real();

  Real & operator = (const Real & R);

  friend ostream & operator << (ostream & outs, const Real & R);
  friend istream & operator >> (istream & ins, Real & R);

  bool operator == (const Real & R) const;
  bool operator != (const Real & R) const;
  bool operator > (const Real & R) const;
  bool operator >= (const Real & R) const;
  bool operator < (const Real & R) const;
  bool operator <= (const Real & R) const;

  Real operator + (const Real & R) const;
  Real operator += (const Real & R);
  Real operator ++ ();
  Real operator ++ (int);

  Real operator - (const Real & R) const;
  Real operator -= (const Real & R);
  Real operator -- ();
  Real operator -- (int);

  Real operator * (const Real & R) const;
  Real operator *= (const Real & R);
  Real operator / (const Real & R) const; // Extra Credit
  Real operator /= (const Real & R); // Extra Credit

private:
  void init(string & WS, string & DS);

  bool AbsGreaterThan(const Real & R)const;
  Real AddSub (const Real & R, int sign) const;

  void WholeNumSum();
  
  double wholeNum, decimalNum;
  vector<char> WholeNums;
  vector<char> DecimalNums;
  size_t wholeNumLength, decimalNumLength;
  bool isNeg;
  bool wholeNumZero;
  /*
  int carry;
  int improperFracNum;
  int precision;
  string decimal;
  */
};
