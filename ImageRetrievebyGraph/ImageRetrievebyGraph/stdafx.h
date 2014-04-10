// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

struct MaskResults 
{
	double Mask1;
	double Mask2;
	double Mask3;
	double Mask4;
	double Mask5;
};

struct Neighborhood 
{
	double Area1;
	double Area2;
	double Area3;
	double Area4;
};

struct FuzzyRules 
{
	int Input1;
	int Input2;
	int Input3;
	int Output;
};


// TODO: 在此处引用程序需要的其他头文件
