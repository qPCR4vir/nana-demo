#include <iostream>

#include <nana/gui.hpp>
#include <nana/threads/pool.hpp>

void foo()
{
      std::cout << "This function is 'called' in other thread created by thread pool.\n";
}

int main()
{
    nana::threads::pool thrpool;
    nana::form fm;
    fm.events().click(pool_push(thrpool, foo));
    fm.events().click(pool_push(thrpool, []{
                           std::cout << "A lambda is also allowed (called from yet another thread).\n";
                        }));
    std::cout << "Two click events were registered for the Nana windows. Click it to see..\n";
    fm.show();
    nana::exec();
}
