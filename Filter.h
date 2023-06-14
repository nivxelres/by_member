#pragma once

class Filter {
  using BitMask = size_t;
  static const int ANSI_SIZE = 256;
  struct Metric;

 public:
  bool Init(const char* pattern);
  bool IsMatch(const char* str);
  bool MatchProcess(char ch);

 private:
  void SetFullMask(const Metric& metric);
  void ExcludePatternBits(const Metric& metric);
  void ExcludeReplacedBits(const BitMask& mask);

 private:
  BitMask m_alphabet[ANSI_SIZE];
  BitMask m_specs;
  BitMask m_fullMask;
  BitMask m_current;
};