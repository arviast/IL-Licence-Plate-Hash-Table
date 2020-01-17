/*main.cpp*/

//
// Hashing program for specialized Illinois license plates,
// which processes an input file of license plates and fines.
// The output is the total fines per license plate, in 
// sorted order.
//
// Original author: Prof. Joe Hummel
// Modified by:     << Arvinbayar Jamsranjav >>
//
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <cassert>

#include "ILplates.h"

using namespace std;


//
// hashInput:
//
void hashInput(string basename, ILplates& hashplates)
{
  string infilename = basename + ".txt";
  ifstream infile(infilename);

  if (!infile.good())
  {
    cout << endl;
    cout << "**Error: unable to open input file '" << infilename << "', exiting." << endl;
    cout << endl;
    exit(-1);
  }

  //
  // input the plates and fines:
  //
  cout << "Reading '" << infilename << "'..." << endl;

  string fine;
  string plate;

  getline(infile, fine);

  //
  // for each pair (fine, license plate), hash and store/update fine:
  //
  while (!infile.eof())
  {
    getline(infile, plate);

    // 
    // is plate valid?  Only process valid plates:
    //
    if (hashplates.Hash(plate) >= 0)  // yes:
    {
      int amount = hashplates.Search(plate);

      if (amount < 0)  // not found:
      {
        hashplates.Insert(plate, stoi(fine));
      }
      else  // we found it, so update total in hash table:
      {
        amount += stoi(fine);
        hashplates.Insert(plate, amount);
      }

    }//valid

    getline(infile, fine);
  }
}

// insertion sort
// ...
void sortIt(vector<string> &plates, vector<int> &amounts) 
{
    string key;
    int value;
    
    for(unsigned int i = 1; i < plates.size(); i++) {
        key = plates[i];
        value = amounts[i];
        int j = i-1;
        
        while(j > -1 && plates[j] > key) {
            plates[j+1] = plates[j];
            amounts[j+1] = amounts[j];
            j = j-1;
        }
        plates[j+1] = key;
        amounts[j+1] = value;
    }
}

int main()
{
  int    N;        // = 10000;
  string basename; // = "tickets1";

  cout << "Enter hashtable size> ";
  cin >> N;

  hashtable<string, int>  ht(N);
  ILplates                hashplates(ht);

  cout << "Enter base filename> ";
  cin >> basename;
  cout << endl;

  //
  // process input file of fines and license plates:
  //
  hashInput(basename, hashplates);

  //
  // debugging:
  //
  vector<string> plates = ht.Keys();
  vector<int> amounts = ht.Values();
  
  // sort the vector
  sortIt(plates, amounts);
  cout << "Sorting...\n";
  // write to txt file 
  string outfilename = basename + "-output.txt";
  ofstream outfile(outfilename);
  cout << "Writing '" << outfilename << "'..." << endl;
    
  for (unsigned int i = 0; i < plates.size(); ++i) {
      outfile << "\"" << plates[i] << "\"" << " $" << amounts[i] << endl;
  }  

  //
  // done:
  //
  return 0;
}