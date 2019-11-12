//#include <iostream>
//#include <gecode/driver.hh>
//#include <gecode/int.hh>
//#include <gecode/minimodel.hh>
//#include "catch.h"
//
///* Future Gecode formulation of the minimum SPR distance problem*/
//using namespace Gecode;
//
//class spr_model : public IntMinimizeScript {
// protected:
//  const int n;
//  IntVarArray a;
//  IntVarArray b;
//  IntVar q;
//
// public:
//  spr_model(const SizeOptions &opt)
//      : IntMinimizeScript(opt),
//        n(opt.size()), a(*this, n, 0, 1), b(*this, n, 0, 1), q(*this, 0, 10) {
//    linear(*this, a, IRT_GQ, 2);
//    linear(*this, b, IRT_GQ, 1);
//
//    rel(*this, ((a[1], a[2] > 0) >> (b[0] + b[1] + b[2] > 0)));
//    rel(*this, ((a[0] + a[1] + a[3] > 0) >> (b[1] + b[3] + b[3] > 0)));
//    rel(*this, ((a[0] + a[2] + a[3] > 0) >> (b[2] + b[3] > 0)));
//    rel(*this, (q == (sum(a) + sum(b))));
//    for (int i = 0; i < 10; i++) {
//      branch(*this, b, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
//      branch(*this, a, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
//      branch(*this, q, INT_VAL_SPLIT_MIN());
//    }
//
//  }
//
//  virtual IntVar cost(void) const {
//    return q;
//  }
//
//  spr_model(bool share, spr_model &s)
//      : IntMinimizeScript(share, s), n(s.n) {
//    a.update(*this, share, s.a);
//    b.update(*this, share, s.b);
//    q.update(*this, share, s.q);
//  }
//
//  virtual Space *
//  copy(bool share) {
//    return new spr_model(share, *this);
//  }
//
//  virtual void
//  print(std::ostream &os) const {
//    std::cout << a << b << q << std::endl;
//  }
//};
//
//
//TEST_CASE("Other Tests", "other") {
//
//}