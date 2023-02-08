// clang++ -std=c++20 ~/w/gpt2cpp/t.cc -o /tmp/t && /tmp/t
#include <codecvt>
#include <iostream>
#include <string>
#include <unordered_map>

std::wstring utf8_to_wstring(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.from_bytes(str);
}

std::string wstring_to_utf8(const std::wstring &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.to_bytes(str);
}

std::string utf8(wchar_t c) {
  std::wstring w(1, c);
  return wstring_to_utf8(w);
}

/*
def bytes_to_unicode():
    bs = (
        list(range(ord("!"), ord("~") + 1)) + list(range(ord("¡"), ord("¬") +
1)) + list(range(ord("®"), ord("ÿ") + 1))
    )
    cs = bs[:]
    n = 0
    for b in range(2**8):
        if b not in bs:
            bs.append(b)
            cs.append(2**8 + n)
            n += 1
    cs = [chr(n) for n in cs]
    return dict(zip(bs, cs))
*/
void build_byte_to_utf8_map(std::unordered_map<uint8_t, wchar_t> *b2u,
                            std::unordered_map<wchar_t, uint8_t> *u2b) {
  auto _insert_range = [=](int start, int end) {
    for (int c = start; c <= end; c++) {
      b2u->insert({uint8_t(c), wchar_t(c)});
    }
  };

  b2u->clear();
  _insert_range(L'!', L'~');
  _insert_range(L'¡', L'¬');
  _insert_range(L'®', L'ÿ');

  int n = 0;
  for (int b = 0; b < 256; b++) {
    if (b2u->find(uint8_t(b)) == b2u->end()) {
      b2u->insert({uint8_t(b), wchar_t(256 + n)});
      n++;
    }
  }

  u2b->clear();
  for (auto e : (*b2u)) {
    u2b->insert({e.second, e.first});
  }
}

int main() {
  std::unordered_map<uint8_t, wchar_t> b2u;
  std::unordered_map<wchar_t, uint8_t> u2b;
  build_byte_to_utf8_map(&b2u, &u2b);
  assert(b2u.size() == 256);
  assert(b2u[0] == 0x100);
  assert(u2b.size() == 256);
  return 0;
}