// Нечёткое сравнение Ву и Менбера (произвольный порядок)

#include <iostream>

#include "Filter.h"

int main() {
  const char* pattern = "a*b*d";       // образец
  const char* matchStr = "abXXd";      // текст
  const char* notMatchStr = "abXXdX";  // текст

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