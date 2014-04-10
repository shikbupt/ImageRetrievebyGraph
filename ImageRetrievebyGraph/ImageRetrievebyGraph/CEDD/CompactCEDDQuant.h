#pragma once

class CompactCEDDQuant
{
public:
	CompactCEDDQuant();
	~CompactCEDDQuant();

public:
	void Apply(double* Local_Edge_Histogram, double* Edge_HistogramElement);

private:
	double *QuantTable;
	double *QuantTable2;
    double *QuantTable3;
    double *QuantTable4;
	double *QuantTable5;
	double *QuantTable6;
};