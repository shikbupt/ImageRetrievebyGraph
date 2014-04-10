#pragma once
#include "../stdafx.h"
class Fuzzy10Bin
{
public:
	Fuzzy10Bin(bool bVaule);
	~Fuzzy10Bin();

public:
	bool KeepPreviuesValues;

protected:

	double* HueMembershipValues;
	double* SaturationMembershipValues;
	double* ValueMembershipValues;

public:

	FuzzyRules Fuzzy10BinRules[48];

	double HueActivation[8];
	double SaturationActivation[2];
	double ValueActivation[3];

private:
	void FindMembershipValueForTriangles(double Input, double* Triangles, double* MembershipFunctionToSave, int TrianglesLength); 
	void LOM_Defazzificator(FuzzyRules* Rules, double* Input1, double* Input2, double* Input3, double* ResultTable, int RulesLength);
	void MultiParticipate_Equal_Defazzificator(FuzzyRules* Rules, double* Input1, double* Input2, double* Input3, double* ResultTable, int RulesLength);
	void MultiParticipate_Defazzificator(FuzzyRules* Rules, double* Input1, double* Input2, double* Input3, double* ResultTable, int RulesLength);

public:
	void ApplyFilter(double Hue, double Saturation, double Value, int Method, double *ReturnValue);

};