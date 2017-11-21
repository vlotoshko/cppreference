#ifndef NVI_AND_CRTP_H
#define NVI_AND_CRTP_H
#include <iostream>

void testNVI();
void testCRTP();

// --- NVI classes

class BaseNVI
{
private:
  virtual void helloWorld_impl () = 0;
public:
  virtual ~BaseNVI();
  void helloWorld ();
};


class DerivedNVI : public BaseNVI
{
  void helloWorld_impl()
  {
    std::cout << "derived nvi method" << std::endl;
  }
};

#endif // NVI_AND_CRTP_H
