#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <limits.h>

class coinsData {
  public:
    int * denominations;
    int * numOfEach;
    int *bestNumOfEach;
    int target;
    int numCoins;
    int bestNumCoins; //used for recursive base case
    int numDenoms;
    coinsData(int t, int denoms[], int numDs){
      numDenoms = numDs;
      bestNumCoins = INT_MAX;
      target = t;
      denominations = new int[numDenoms];
      numOfEach = new int[numDenoms];
      bestNumOfEach = new int[numDenoms];
      for (int x = 0; x < numDenoms; x++){
        numOfEach[x] = 0;
        bestNumOfEach[x] = 0;
        denominations[x] = denoms[x];
        numCoins = 0;

      }
    }
};

/*
//  This "greedy" change-maker lives in the moment, simply dishing out whatever coin
//  looks like it will minimize the owed change the most.
*/
void yoloCoins(coinsData & data){
  while (data.target != 0){
    std::cout << data.target << '\n';
    int addedCoin = 0;
    for (int x = 0; x < data.numDenoms; x++){
      if (data.denominations[x] > data.target && !addedCoin && data.target){
        std::cout << "subtracting " << data.denominations[x-1] << '\n';
        data.numOfEach[x-1]++;
        data.target -= data.denominations[x-1];
        data.numCoins++;
        addedCoin = 1;
      }
    }
    if (!addedCoin && data.target){
      std::cout << "subtracting " << data.denominations[data.numDenoms-1] << '\n';
      data.numOfEach[data.numDenoms-1]++;
      data.target -= data.denominations[data.numDenoms-1];
      data.numCoins++;
      addedCoin = 1;
    }
  }
  return;
}

<<<<<<< HEAD
/*
//	This change-making algorithm builds a table from the bottom up to calculate
//	the optimal change.
*/
void dynCoins(coinsData & data){
	int i, j, k;
	int *minCoin = new int[data.target];

	//Initialize and zero out 2D array count
	int **count = new int *[data.target + 1];
	for(i = 0; i < data.target + 1; i++){
		count[i] = new int[data.numDenoms];
		for(j = 0; j < data.numDenoms; j++){
			count[i][j] = 0;
		}
	} 
	
	int min;

	minCoin[0] = 0;

  //For each value from 1 to the target
	for(i = 1; i <= data.target; i++){
		min = 9999;
    //For each denomination
		for(j = 0; j < data.numDenoms; j++){
      //If the denomination is less than or equal to the target
			if(data.denominations[j] <= i){
        //If the minimum is larger than the memoized value of i - denomination + 1
				if(min > minCoin[i - data.denominations[j]] + 1){
          //Set min to the memoized value + 1
					min = minCoin[i - data.denominations[j]] + 1;
					for(k = 0; k < data.numDenoms; ++k){
            //Get the counts from the memoized i - denomination
						count[i][k] = count[i - data.denominations[j]][k];
					}
          //Increment the denomination used by one
					count[i][j]++;
				}
			}
		}
    //Memo the minimum for this value of i
		minCoin[i] = min;
	}

  //Store result for output
  data.numCoins = minCoin[data.target];
  //printf("Minimum coins required %d \n", minCoin[data.target]);
  for(i = 0; i < data.numDenoms; i++){
    data.numOfEach[i] = count[data.target][i];
    //printf("%d: %d\n", data.denominations[i], count[data.target][i]);
  }

	return;
=======
/*****************************
 * changeSlow(coinsData &data)
 * This function recursively makes change for target cents
 * Recursively checks target value against array of available
 * coin denominations. This method is quite slow, as it will
 * Check each solution many times. As number of coin values grow the number of combos 
 * grows exponentially.
 * Params: initialized coinsData object
 * Returns: void
*****************************/
void changeSlow(coinsData &data){
  //Base case, we have made enough change
  if (data.target == 0){
    if (data.numCoins < data.bestNumCoins){
      data.bestNumCoins = data.numCoins;
      data.bestNumOfEach = data.numOfEach;
    }
    return;
  }

  if (data.numCoins > data.bestNumCoins){
    return;
  }
  
  //Iterates through array of coin denominations
  //By iterating backwards we can start with largest value
  //If we find a coing <= target starting with largest we subtract it
  //from the target.
  for (int i = data.numDenoms - 1; i >= 0; i--)
  {
    if(data.target >= data.denominations[i]){
      data.numCoins += 1;
      data.numOfEach[i]++;
      data.target -= data.denominations[i];
      bruteCoins(data);
    }
  }
>>>>>>> origin/master
}

void outputResults(std::string text, int arrayNum, coinsData data, std::ofstream &output){
  output << text << " for test #" << arrayNum << '\n';
  std::string outputArray = "[";
    for (int y = 0; y < data.numDenoms; y++){
      char temp[100];
      sprintf(temp, "%d", data.numOfEach[y]);
      outputArray += temp;
      outputArray += ",";
    }
    outputArray.erase(outputArray.size() - 1);
    outputArray += "]";
    output << outputArray << '\n';
    output << data.numCoins << '\n';
}

int main(int argc, char ** argv){
  if (argc != 2){
    std::cout << "Please call this program with the filename as argument.\n";
    return 1;
  }
  std::string outputFName = argv[1];
  int fNamePos = outputFName.find(".");
  outputFName = outputFName.substr(0, fNamePos);
  outputFName += "change.txt";
  std::ifstream file (argv[1]);
  std::ofstream output (outputFName.c_str());
  std::string line;
  int arrayNum = 0;
  while (getline (file, line)){
    arrayNum++;
    std::vector<int> denoms;
    int target;
    int hasNumber = 0;
    int number = 0;
    for (int x = 0; x < line.length(); x++){
      if (isdigit(line[x])){
        hasNumber = 1;
        number *= 10;
        number += (line[x] - '0');
      } else {
        if (hasNumber){
          denoms.push_back(number);
          number = 0;
          hasNumber = 0;
        }
      }
    }
    getline(file, line);
    target = atoi(line.c_str());
    // Copy the vector to an array
    int * denomsArr = new int[denoms.size()];
    std::copy(denoms.begin(), denoms.end(), denomsArr);
    coinsData greedyData (target, denomsArr, denoms.size());
    coinsData dynData (target, denomsArr, denoms.size());
    coinsData divAndConqData (target, denomsArr, denoms.size());

    yoloCoins(greedyData);
    outputResults("Greedy output", arrayNum, greedyData, output);
<<<<<<< HEAD
  
    dynCoins(dynData);
    outputResults("Dynamic programming output", arrayNum, dynData, output);

    // uncomment your section when your algorithm is complete.
    
    // divAndConqCoins(divAndConqData);
    // outputResults("Divide and conquer output", arrayNum divAndConqData, output);
=======

    // uncomment your section when your algorithm is complete.

    // dynCoins(dynData);
    // outputResults("Dynamic programming output", arrayNum, dynData, output);

    changeSlow(divAndConqData);
    outputResults("Divide and conquer output", arrayNum, divAndConqData, output);

>>>>>>> origin/master
  }
  output.close();
  file.close();

  return 0;
}
