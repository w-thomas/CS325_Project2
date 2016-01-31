/*
*   Can be used to gather coin and timing data for experimental results
*/

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

class coinsData {
  public:
    int * denominations;
    int * numOfEach;
    int target;
    int numCoins;
    int numDenoms;
    coinsData(int t, int denoms[], int numDs){
      numDenoms = numDs;
      target = t;
      denominations = new int[numDenoms];
      numOfEach = new int[numDenoms];
      for (int x = 0; x < numDenoms; x++){
        numOfEach[x] = 0;
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
  std::ofstream output (outputFName.c_str());
  
  /***************** DENOMINATION ARRAY *****************/
  int denomsArr[] = {1,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};
  clock_t t;
  for(int i = 10000; i <= 1000000; i = i + 10000){
    
    coinsData myData (i, denomsArr, 16);
    t = clock();

    /***************** Select your algorithm and choose to output coin data or timing data (milliseconds) *****************/
    /***************** (Comment out what you don't need) *****************/
    //yoloCoins(myData);
    //outputResults("Greedy output", i, myData, output);
    
    dynCoins(myData);
    //outputResults("Dynamic programming output", i, myData, output);
    
    //divAndConqCoins(divAndConqData);
    //outputResults("Divide and conquer output", arrayNum, myData, output);

    t = clock() - t;
    output << ((float)(1000*t))/CLOCKS_PER_SEC << std::endl;
  }
  return 0;
}