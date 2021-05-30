#ifndef IRWrapper_h
#define IRWrapper_h

#include <memory>

class Wrapper {

  public:
    Wrapper(int, int);

    void tryRemote(bool enable);
    void readSignal();
  private:
    class IRData;

};

#endif
