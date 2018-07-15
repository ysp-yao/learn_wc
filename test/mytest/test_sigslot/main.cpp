#include "rtc_base/sigslot.h"

#include <iostream>

class Sig {
 public:
  sigslot::signal0<> sig0;

  sigslot::signal1<int> sig1;
  sigslot::signal1<int> sig2;
};

class Slot : public sigslot::has_slots<> {
 public:
  void OnSig0_0() { std::cout << "Slot::OnSig0_0()\n"; }
  void OnSig0_1() { std::cout << "Slot::OnSig0_1()\n"; }

  void OnSig1(int i) {
    std::cout << "Slot::OnSig1(int), from sig" << i << "\n";
  }
};

class Base {
 public:
  sigslot::signal1<std::string> sig;
};

class Derived : public Base, 
                public sigslot::has_slots<> {
 public:
  Derived() { 
    sig.connect(this, &Derived::OnSigInternal);
  }

  void OnSigInternal(std::string msg) { 
    std::cout << "Derived::OnSigInternal()\n";
    sig(std::string("from OnSigInternal"));
    return;
  }

  void OnSig(std::string msg) { 
    std::cout << "Derived::OnSig()\n";
  }
};

int main() {
  Sig sig;
  Slot slot;

  // 1. 一个signal可以connect多个slot
  // 2. slot的执行顺序由connect的顺序决定
  sig.sig0.connect(&slot, &Slot::OnSig0_0);
  sig.sig0.connect(&slot, &Slot::OnSig0_1);
  sig.sig0.connect(&slot, &Slot::OnSig0_0);
  sig.sig0();

  // 3. 多个signal可以connect到一个slot
  sig.sig1.connect(&slot, &Slot::OnSig1);
  sig.sig2.connect(&slot, &Slot::OnSig1);
  // 4. slot的执行顺序与sig调用顺序有关
  sig.sig2(2);
  sig.sig1(1);

  std::cout << "=========================================\n";

  //
  Derived derived;
  derived.sig.connect(&derived, &Derived::OnSig);
  derived.sig(std::string("from main"));

  while (1)
    ;

  return 0;
}