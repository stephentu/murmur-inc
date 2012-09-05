#include "MurmurHash3.h"
#include "MurmurInc.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

#ifdef NDEBUG
#error "relies on assert()"
#endif /* NDEBUG */

static inline string
random_string(size_t len)
{
  string ret;
  ret.reserve(len);
  for (size_t i = 0; i < len; i++)
    ret.push_back((char)rand());
  return ret;
}

static string
hexify(const string& input)
{
  size_t len = input.length();
  const char* const lut = "0123456789ABCDEF";

  string output;
  output.reserve(2 * len);
  for (size_t i = 0; i < len; ++i) {
    const unsigned char c = (unsigned char) input[i];
    output.push_back(lut[c >> 4]);
    output.push_back(lut[c & 15]);
  }
  return output;
}

static void
do_test(size_t key_len, size_t step)
{
  string x = random_string(key_len);

  uint32_t a[4], b[4], c[4], d[4];

  MurmurHash3_x86_128(x.data(), x.size(), 0, a);

  // try all at once
  {
    murmur_inc h(0);
    h.hash(x.data(), x.size());
    h.finalize(b);
    assert(memcmp(a, b, sizeof(uint32_t) * 4) == 0);
  }

  // try in steps, for determinism (don't check against all-at-once,
  // because won't produce same hash)
  {
    murmur_inc h1(0), h2(0);

    const size_t n = x.size() / step + ( x.size() % step ? 1 : 0 );
    for (size_t i = 0; i < n; i++) {
      if ((i + 1) == n) {
        assert(x.size() > (i * step));
        assert((x.size() - (i * step)) <= step);
        h1.hash(x.data() + i * step, x.size() - (i * step));
        h2.hash(x.data() + i * step, x.size() - (i * step));
      } else {
        h1.hash(x.data() + i * step, step);
        h2.hash(x.data() + i * step, step);
      }
    }
    h1.finalize(c);
    h2.finalize(d);

    assert(memcmp(c, d, sizeof(uint32_t) * 4) == 0);
  }

  cout << "MurmurHash3_x86_128: " << hexify(string((const char *) a, sizeof(uint32_t) * 4)) << endl;
  cout << "MurmurInc: " << hexify(string((const char *) c, sizeof(uint32_t) * 4)) << endl;
}

int main(void)
{
  srand(time(NULL));

  do_test(130, 28);
  do_test(128, 2);
  do_test(256, 64);

  return 0;
}
