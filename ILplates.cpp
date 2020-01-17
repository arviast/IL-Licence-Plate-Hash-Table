/*ILplates.cpp*/

//
// Hashing functions to store (license plate, amount) pairs
// using linear probing.
//
// << Arvinbayar Jamsranjav >>
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project 05
//

#include <iostream>
#include <string>

#include "ILplates.h"

using namespace std;


//
// Hash:
//
// Given a specialized Illinois license plate, returns an index into
// the underyling hash table.  If the given plate does not follow the
// formatting rules given below, -1 is returned.
//
// Personalized:
//   letters and numbers, with a space between the letters 
//   and numbers.  Format: 1-5 letters plus 1..99 *OR* 
//   6 letters plus 1..9
//
//   Examples: A 1, B 99, ZZZZZ 1, ABCDEF 3
//
// Vanity:
//   Format: 1-3 numbers *OR* 1-7 letters
// 
//   Examples: 007, 1, 42, X, AAA, ZZZEFGH

//
//--------------------------------------------------------------------------------------
//                                HOW I CHOOSE MY INDEX
// after playing for a while with the indexes I recognized that closer the numbers of 
// indexes then probes are less, which means we are using less spaces => my method was 
// multiply lower indexes with high number to make them same digit, also dividing them 
// by 2 helped me a lot.
//--------------------------------------------------------------------------------------
//

int ILplates::Hash(string plate)
{
  unsigned long long index = 0;
    
  unsigned int countNum = 0;              // for counting numbers in the string
  unsigned int countLetter = 0;           // for counting letters in the string
    
  for(unsigned int i = 0; i < plate.size(); i++) {
      if(isdigit(plate[i])) {
          countNum++;
      }
      else if (isalpha(plate[i])) {
          countLetter++;
      }
  }  
  
  // checking if its the Vanity licence plate
  // for example AAA, 123 ...
  // ...
  if( ((countNum > 0 && countNum < 4) || (countLetter > 0 && countLetter < 8)) &&
      (countNum == plate.size() || countLetter == plate.size()) ){
      // all digits
      // ...
      if (countNum == plate.size()) {
          index = stoll(plate);
          //convert to int
          //play with index
          return (index*600)/2 % HT.Size();
      }
      // all capital letters
      // ...
      else if (countLetter == plate.size()) {
          string temp;
          for(unsigned int i = 0; i < countLetter; i++) {
              if(plate[i] < 'A' || plate[i] > 'Z') {
                  return -1;
              }
              int tempIndex = (int)plate[i] - 'A';
              string pushIndex2 = to_string(tempIndex);
              for (unsigned int i = 0; i < pushIndex2.size(); i++) {
                  temp.push_back(pushIndex2[i]);
              }
          }
          // convert to integer
          // play with index
          index = stoll(temp);
          return (index*75)/2 % HT.Size();
      }
  }
  // checking if it's the Personalized licence plate
  // for example ABCDEF 1
  // cases are 1 number and 6 letter or 1-2 numbers and 1-5 letters
  // first and last characters cant be space
  // size cant be more than 1 cause it means a lot of spaces 
  // ...
  else if ( ((countNum == 1 && countLetter == 6) || 
          (countLetter > 0 && countLetter < 6 && countNum > 0 && countNum < 3)) && 
           (!isdigit(plate[0])) && ( plate.size() == (countLetter + countNum + 1)) && (plate[0] != ' ') 
          && (plate[plate.size() - 1] != ' ') ) {
      string temp1;              // created new string to save ascii's
      int tempIndex1 = 0;        // int that saves ASCII of the char
      
      // checks if there is no characters after space
      // for example: ABCDE 9F, ABCDE F9 both incorrect
      // ...
      for(unsigned int i = countLetter; i < plate.size()+1; i++) {
          if(isalpha(plate[i])) {
              return -1;
          }
      }
      // if num then push
      // else if alpha then convert before push 
      // also checks for capital or not
      // ....
      for(unsigned int i = 0; i < plate.size(); i++) {
          if(isdigit(plate[i])) {
              temp1.push_back(plate[i]);
          }
          else if(isalpha(plate[i])) {
              if(plate[i] < 'A' || plate[i] > 'Z') {
                  return -1;
              }
              tempIndex1 = (int)plate[i] - 'A';
              string pushIndex = to_string(tempIndex1);
              for (unsigned int i = 0; i < pushIndex.size(); i++) {
                  temp1.push_back(pushIndex[i]);
              }  
          }
      }
      // convert to int
      // play with index
      index = stoll(temp1);
      return (index*75)/2 % HT.Size();
  }
  // if reached here then licence plate is wrong
  // ...  
  else {
      return -1;
  }
    
  return -1;
}



//
// Search
// 
// Hashes and searches for the given license plate; returns the 
// associated value for this plate if found, or -1 if not found.
//
int ILplates::Search(string plate)
{
  bool empty;
  string key;
  int value;
  int hashIndex = Hash(plate);
  int counter = 0;
    
  HT.Get(hashIndex, empty, key, value);
    
  while( (counter < HT.Size()) && (!empty) ) {
      if(key == plate) {
          return value;
      }
      hashIndex = (hashIndex + 1) % HT.Size();
      counter++;
      HT.Get(hashIndex, empty, key, value);
  }
  return -1;
}


//
// Insert
//
// Inserts the given (plate, newValue) into the hash table,
// overwriting an existing value if there.
//
void ILplates::Insert(string plate, int newValue)
{
  int hashIndex1 = Hash(plate);
  bool empty1;
  int value1;
  string key1;
  int counter1 = 0;
    
  if(hashIndex1 == -1) {
      return;
  }  
 
  while(counter1 < HT.Size()) {  
      HT.Get(hashIndex1, empty1, key1, value1);
      if((empty1 == true) || (key1 == plate)) {
          HT.Set(hashIndex1, plate, newValue);
          return;
      }
      hashIndex1 = (hashIndex1 + 1) % HT.Size();
      counter1++;
  }
  return;
}