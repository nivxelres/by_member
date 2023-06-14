// �������� ��������� �� � ������� (������������ �������)

#include <iostream>

#include "Filter.h"

int main() {
  const char* pattern = "a*b*d";       // �������
  const char* matchStr = "abXXd";      // �����
  const char* notMatchStr = "abXXdX";  // �����

  Filter filter;
  filter.Init(pattern);

  std::cout << std::boolalpha;

  auto result = filter.IsMatch(matchStr);
  std::cout << "pattern = " << pattern << std::endl;
  std::cout << "matchStr = " << matchStr << std::endl;
  std::cout << "result = " << result << std::endl << std::endl;

  result = filter.IsMatch(notMatchStr);
  std::cout << "pattern = " << pattern << std::endl;
  std::cout << "notMatchStr = " << notMatchStr << std::endl;
  std::cout << "result = " << result << std::endl;

  return 0;
}