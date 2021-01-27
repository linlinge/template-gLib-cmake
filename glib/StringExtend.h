#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
using namespace std;
void Str2Vec(string str, string delimiters, vector<int>& dat);
void StrSplit(string str, string delimiters, vector<string>& dat);
void StrPrint(vector<int>& dat);
int get_num_in_string(string str);
