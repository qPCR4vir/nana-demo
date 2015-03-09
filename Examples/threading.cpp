#include <nana/gui/wvl.hpp>
#include <nana/threads/pool.hpp>
void foo()
{
  //This function will be "called" in other thread created by thread pool.
}
int main()
{
  using namespace nana;
  using namespace nana::threads;
    pool thrpool;
    form fm;
    fm.events().click(pool_push(thrpool, foo));
    fm.events().click(pool_push(thrpool, []{
                                              //A lambda is also allowed.
                                      }));
    fm.show();
    exec();
}