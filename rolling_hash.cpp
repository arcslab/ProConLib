#include<algorithm>
#include<iostream>
#include<random>
#include<vector>
#include<string>
#include<cassert>
#include <iomanip>

using std::cout;
using std::endl;
using std::cerr;
using std::cin;

constexpr int64_t EuclidMod(int64_t v, int64_t m){
  if(0 <= v && v < m){
    return v;
  }else if(-m <= v && v < 0){
    return v + m;
  }else{
    int64_t result = v % m;
    if(result < 0){
      result += m;
    }
    return result;
  }
}

int64_t ModInv(int64_t a, int64_t m){
  int64_t b = m, u = 1, v = 0;
  while(b){
    int64_t t = a / b;
    a -= t * b;
    std::swap(a, b);
    u -= t * v;
    std::swap(u, v);
  }
  EuclidMod(u, m);
  return u;
}

template<int64_t mod>
class ModInt{
  int64_t _value;

public:
  constexpr explicit ModInt(const int64_t x = 0): _value(x % mod){
  };

  constexpr ModInt& operator+=(const ModInt& another){
    _value += another._value;
    _value = EuclidMod(_value, mod);
    return *this;
  }

  constexpr ModInt& operator-=(const ModInt& another){
    _value -= another._value;
    _value = EuclidMod(_value, mod);
    return *this;
  }

  constexpr ModInt& operator*=(const ModInt& another){
    _value *= another._value;
    _value = EuclidMod(_value, mod);
    return *this;
  }

  constexpr ModInt& operator/=(const ModInt& another){
    _value = _value * ModInv(another._value, mod);
    _value = EuclidMod(_value, mod);
    return *this;
  }

  constexpr ModInt& operator++(){
    _value += ModInt(1);
    return *this;
  }

  constexpr ModInt operator++(int){
    ModInt tmp = *this;
    ++tmp;
    return tmp;
  }

  int64_t GetValue() const{
    return _value;
  }

};

template<int64_t mod>
constexpr ModInt<mod>
operator+(const ModInt<mod>& left, const ModInt<mod>& right){
  return ModInt<mod>(left) += right;
}

template<int64_t mod>
constexpr ModInt<mod>
operator-(const ModInt<mod>& left, const ModInt<mod>& right){
  return ModInt<mod>(left) -= right;
}

template<int64_t mod>
constexpr ModInt<mod>
operator/(const ModInt<mod>& left, const ModInt<mod>& right){
  return ModInt<mod>(left) /= right;
}

template<int64_t mod>
constexpr ModInt<mod>
operator*(const ModInt<mod>& left, const ModInt<mod>& right){
  return ModInt<mod>(left) *= right;
}

template<int64_t mod>
std::ostream& operator<<(std::ostream& stream, const ModInt<mod>& mod_int){
  stream << mod_int.GetValue();
  return stream;
}

class RandomIntNumber {
  std::mt19937_64 mt_maker_;
  std::uniform_int_distribution<int64_t> range_maker_;

public:
  RandomIntNumber(int64_t min, int64_t max) {
    std::random_device seed_maker;
    mt_maker_.seed(seed_maker());
    // range_maker_.min = min;
    // range_maker_.max = max;
    std::uniform_int_distribution<int64_t>::param_type param(min, max);
    range_maker_.param(param);
  }
  int64_t Make(void) {
    return range_maker_(mt_maker_);
  }
  int64_t operator()(void) {
    return Make();
  }
};

// return base_^exponent (MOD. mod).
int64_t RepeatedPowMod(int64_t base, int64_t exponent, int64_t mod) {
  if (exponent == 0)
    return 1;
  else if (exponent % 2 == 0) {
    int64_t root = RepeatedPowMod(base, exponent / 2, mod);
    return (root * root) % mod;
  } else {
    return (base * RepeatedPowMod(base, exponent - 1, mod)) % mod;
  }
}

class RollingHash{
  static const int64_t MOD_=1000000007;
  std::vector<ModInt<MOD_>> hash_;
  std::string str_;
  RandomIntNumber random_int_number_;
  int64_t base_;

public:
  RollingHash(const std::string& str):random_int_number_(2,MOD_-2),str_(str){
    base_=random_int_number_.Make();
    for(int i=0;i<str.size();i++){
      if(i==0){
        hash_.push_back(ModInt<MOD_>(str[0]));
      }else{
        ModInt<MOD_> result(hash_[i-1]);
        result*=ModInt<MOD_>(base_);
        result+=ModInt<MOD_>(str[i]);
        hash_.push_back(result);
      }
    }
  }
  RollingHash(const std::string& str,int64_t base):random_int_number_(2,MOD_-2),str_(str),base_(base){
    for(int i=0;i<str.size();i++){
      if(i==0){
        hash_.push_back(ModInt<MOD_>(str[0]));
      }else{
        ModInt<MOD_> result(hash_[i-1]);
        result*=ModInt<MOD_>(base_);
        result+=ModInt<MOD_>(str[i]);
        hash_.push_back(result);
      }
    }

  }
  int64_t GetHash(int front,int back){
    if(front>back){
      cerr<<"RollingHash::GetHash() Error: argument\"front\" is larger than \"back\""<<endl;
      abort();
    }
    if(front==0) return hash_[back].GetValue();
    ModInt<MOD_> result=hash_[back]-hash_[front-1]*ModInt<MOD_>(RepeatedPowMod(base_,back-front+1,MOD_));
    return result.GetValue();
  }
  int64_t GetBase(){
    return base_;
  }
};

bool IsContaining(std::string a,std::string b){
  if(a.size()<b.size()){
    std::swap(a,b);
  }
  RollingHash hash_a(a);
  RollingHash hash_b(b,hash_a.GetBase());

  for(int front=0;front+b.size()-1<a.size();front++){
    int back=front+b.size()-1;
    if(hash_a.GetHash(front,back)==hash_b.GetHash(0,b.size()-1)) return true;
  }
  return false;
}

int main(void){
  cout << std::fixed << std::setprecision(10);
  std::ios::sync_with_stdio(false);
  cin.tie(0);
  std::string t,p;
  cin>>t>>p;
  // cerr<<"read"<<endl;
  RollingHash hash_t(t);
  // cerr<<"hashed"<<endl;
  RollingHash hash_p(p,hash_t.GetBase());
  // cerr<<"hashed"<<endl;
  for(int front=0;front+p.size()-1<t.size();front++){
    int back=front+p.size()-1;
    if(hash_t.GetHash(front,back)==hash_p.GetHash(0,p.size()-1)){
      printf("%d\n",front);
    }
  }
  return 0;
}

// int main(void){
//   std::string s("aafafaa");
//   RollingHash hash(s);
//   for(int i=0;i<=5;i++){
//     cout<<hash.GetHash(i,i+1)<<endl;
//   }
// }

