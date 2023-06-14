#include "Filter.h"

#include <cassert>
#include <cstring>

namespace {
// ограничение на длинну фильтра 64 символа
// NOTE: расширяется увеличением слов.
constexpr int MAX_LEN = sizeof(size_t) * 8;

namespace spec {
constexpr char eol = '\0';
constexpr char any = '*';
constexpr char repl = '?';
}  // namespace spec

constexpr std::size_t operator"" sz(unsigned long long n) { return n; }

}  // namespace

struct Filter::Metric {
  unsigned char data[MAX_LEN]{};
  int len{0};
  int specsCount{0};

  int GetPatternLen() const {
    // символ замещения строки <*> не рассматривается как символ паттерна
    // для их хранеия используется отдельный битовый массив
    return len - specsCount;
  }

  bool Prepare(const char* pattern) {
    if (!pattern) {
      return false;
    }
    len = specsCount = 0;

    // Настройка алгоритма на поиск совпадения с начала строки.
    // (первым символом любой строки подается spec::eol)
    data[len++] = spec::eol;

    while (*pattern && len < MAX_LEN) {
      data[len++] = *pattern;
      if (*pattern != spec::any) {
        pattern++;
        continue;
      }

      specsCount++;
      while (*pattern == spec::any) {
        //пропуск подряд идущих <*>
        pattern++;
      };
    }

    if (len + 1 >= MAX_LEN) {
      return false;
    }

    // Настрока алгоритма на поиск совпадения в конце строки.
    if (data[len] != spec::any) {
      data[++len] = spec::eol;
    }

    return true;
  }
};

bool Filter::Init(const char* pattern) {
  Metric metric{};
  if (!metric.Prepare(pattern)) {
    return false;
  }

  SetFullMask(metric);
  ExcludePatternBits(metric);
  return true;
}

bool Filter::IsMatch(const char* str) {
  int textLen = strlen(str);
  m_current = m_fullMask;
  bool result = MatchProcess(spec::eol);
  while (*str && !result) {
    result = MatchProcess(*str++);
  }

  if (result) {
    return true;
  }

  result = MatchProcess(spec::eol);

  return result;
}

bool Filter::MatchProcess(char ch) {
  const BitMask prev = m_current;
  m_current = (m_current >> 1) | m_alphabet[ch];
  m_current = m_current & (prev | m_specs);

  if (!(m_current & 1sz)) {
    return true;
  }
  return false;
}

void Filter::SetFullMask(const Metric& metric) {
  const auto len = metric.GetPatternLen();
  m_fullMask = (1sz << len) - 1;
  m_specs = m_fullMask;

  for (size_t i = 0; i < ANSI_SIZE; i++) {
    m_alphabet[i] = m_fullMask;
  }
}

// выключение битов символов шаблона: 0-й символ в старшем бите.
void Filter::ExcludePatternBits(const Metric& metric) {
  const auto len = metric.GetPatternLen();
  for (size_t i = 0, pos = 0; i < metric.len; i++, pos++) {
    if (metric.data[i] == spec::any) {
      m_specs ^= 1sz << (len - pos);
      pos--;
      continue;
    }

    const auto mask = 1sz << (len - pos - 1);

    if (metric.data[i] == spec::repl) {
      ExcludeReplacedBits(mask);
      continue;
    }

    m_alphabet[metric.data[i]] ^= mask;
  }
}

// Для символа замещения <?> сбрасываются биты позиции всего алфавита.
// Так получаем равенство с любым символом в текущей позиции.
void Filter::ExcludeReplacedBits(const BitMask& mask) {
  for (size_t i = 0; i < ANSI_SIZE; i++) {
    m_alphabet[i] ^= mask;
  }
}