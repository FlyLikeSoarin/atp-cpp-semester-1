#include <cstdio>
#include <cstring>
#include <cstdlib>

size_t findPoint(const char* number) {
  size_t i = 0;
  while (number[i] && (number[i] != '.'))
    ++i;
  return i;
}
char getAns(const char sign, bool isBothNegative){
  if (!isBothNegative){
    return sign;
  }
  else if (sign=='<'){
    return '>';
  }
 return '<';
}

char compareBeforePoint(const char* arg1, const char* arg2, size_t pointIndex) {
  for (size_t i = 0; i < pointIndex; ++i) {
    if (arg1[i] < arg2[i])
      return '<';
    if (arg1[i] > arg2[i])
      return '>';
  }
  return '=';
}

char compareAfterPonit(const char* arg1, const char* arg2, size_t pointIndex){
  size_t i = pointIndex;
  while (arg1[i] && arg2[i]) {
    if (arg1[i] < arg2[i])
      return '<';
    if (arg1[i] > arg2[i])
      return '>';
    ++i;
  }

  while (arg2[i]) {
    if ((arg2[i] != '0') && (arg2[i] != '.'))
      return '<';
    ++i;
  }
  while (arg1[i]) {
    if ((arg1[i] != '0') && (arg1[i] != '.'))
      return '>';
    ++i;
  }
  return '=';
}




char compare(char* arg1,char* arg2){
  bool isArg1AndArg2Negative = (arg1[0] == '-' && arg2[0] == '-');
  if ((arg1[0] == '-') && (arg2[0] != '-')) {
    return '<';
  } else if ((arg1[0] != '-') && (arg2[0] == '-')) {
    return '>';
  }
  if (isArg1AndArg2Negative) {
    arg1[0] = '0';
    arg2[0] = '0';
  }
  size_t positionOfPointArg1 = findPoint(arg1);
  size_t positionOfPointArg2 = findPoint(arg2);

  if (positionOfPointArg1 != positionOfPointArg2)
    return getAns((positionOfPointArg1 < positionOfPointArg2) ? '<' : '>', isArg1AndArg2Negative);

  char resultBeforePoint = compareBeforePoint(arg1, arg2, positionOfPointArg1);
  if (resultBeforePoint != '=')
    return getAns(resultBeforePoint,isArg1AndArg2Negative);

  char resultAfterPoint = compareAfterPonit(arg1, arg2, positionOfPointArg1);
  if ( resultAfterPoint != '=')
    return getAns( resultAfterPoint,isArg1AndArg2Negative);
  return '=';
}


int main() {
  char arg1[10002];
  char arg2[10002];
  scanf("%s", arg1);
  scanf("%s", arg2);
  printf("%c", compare(arg1, arg2));

}