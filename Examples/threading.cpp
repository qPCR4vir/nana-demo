#include <nana/gui/wvl.hpp>
#include <nana/threads/pool.hpp>
#include <iostream> 

void foo()
{
      std::cout << "This function is 'called' in other thread created by thread pool.\n";
}
int main()
{
  using namespace nana;
  using namespace nana::threads;
    pool thrpool;
    form fm;
    fm.events().click(pool_push(thrpool, foo));
    fm.events().click(pool_push(thrpool, []{
                                              std::cout << "A lambda is also allowed (called from yet another thread).\n";
                                      }));
    std::cout << "Two click events were registered for the Nana windows. Click it to see..\n";
    fm.show();
    exec();
}