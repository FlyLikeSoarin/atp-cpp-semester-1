#include <iostream>
#include <string>
size_t PosOPointAndNumberFormating(std::string& arg) {
  size_t positionOfPoint = arg.find('.');
  if (positionOfPoint == std::string::npos) {
    arg.push_back('.');
    positionOfPoint = arg.length() - 1;
  }
  return positionOfPoint;
}
int main() {
  std::string arg1;
  std::string arg2;
  std::cin >> arg1 >> arg2;
  bool isArg1AndArg2Negative = (arg1[0] == '-' && arg2[0] == '-');
  if (isArg1AndArg2Negative) {
    arg1[0] = '0';
    arg2[0] = '0';
  }
  size_t positionOfPointArg1 = PosOPointAndNumberFormating(arg1);
  size_t positionOfPointArg2 = PosOPointAndNumberFormating(arg2);

  std::string leadingZeroes;
  if (positionOfPointArg2 > positionOfPointArg1) {
    for (size_t i = positionOfPointArg1; i < positionOfPointArg2; ++i) {
      leadingZeroes += '0';
    }
    arg1 = leadingZeroes + arg1;
  } else {
    for (size_t i = positionOfPointArg2; i < positionOfPointArg1; ++i) {
      leadingZeroes += '0';
    }
    arg2 = leadingZeroes + arg2;
  }
  if (arg1.size() < arg2.size()) {
    for (size_t i = arg1.size(); i < arg2.size(); ++i) {
      arg1 += '0';
    }
  } else {
    for (size_t i = arg2.size(); i < arg1.size(); ++i) {
      arg2 += '0';
    }
  }

  if (arg1==arg2){
    std::cout<<'=';
  } else if(arg1<arg2){
    std::cout<< ((isArg1AndArg2Negative) ? '>' : '<');
  } else {
    std::cout<<((isArg1AndArg2Negative) ? '<' : '>');;
  }
}