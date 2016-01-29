#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

class coinsData {
  public:
    int * denominations;
    int * numOfEach;
    int target;
    int numCoins;
    coinsData(int t, int denoms[], int numDenoms){
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


int main(int argc, char ** argv){
  if (argc != 2){
    std::cout << "Please call this program with the filename as argument.\n";
    return 1;
  }
  std::string outputFName = argv[1];
  int fNamePos = outputFName.find(".");
  outputFName = outputFName.substr(0, fNamePos);
  outputFName += "change.txt";
  std::cout << outputFName << '\n';
  std::ifstream file (argv[1]);
  std::ofstream output (outputFName.c_str());
  std::vector<int> denoms;
  int target;
  std::string line;
  while (getline (file, line)){
    int hasNumber = 0;
    int number = 0;
    for (int x = 0; x < line.length(); x++){
      //std::cout << line[x] << "\n";
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
    coinsData data (target, denomsArr, denoms.size());
    // Call Algorithm 1 here
    std::string outputArray = "[";
    for (int y = 0; y < denoms.size(); y++){
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
  output.close();
  file.close();
  return 0;
}