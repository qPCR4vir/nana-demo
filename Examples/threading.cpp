#include <nana/gui/wvl.hpp>
#include <nana/threads/pool.hpp>
#include <iostream> 

void foo()
{
      std::cout << "This function will be 'called' in other thread created by thread pool.\n";
}
int main()
{
  using namespace nana;
  using namespace nana::threads;
    pool thrpool;
    form fm;
    fm.events().click(pool_push(thrpool, foo));
    fm.events().click(pool_push(thrpool, []{
                                              std::cout << "A lambda is also allowed.\n";
                                      }));
    fm.show();
    exec();
}