//
// Created by ravenspoint on 08.04.2019 to illustrate place error checking.
//

#include <iostream>
#include <nana/gui.hpp>
#include <nana/gui/widgets/checkbox.hpp>

int main()
{
    try
    {
        // construct application form
        nana::form fm( nana::rectangle( 100,100, 300, 300 ));

        nana::checkbox cb1( fm);
        cb1.caption("one");
        nana::checkbox cb2(fm);
        cb2.caption("two");
        nana::checkbox cb3(fm);
        cb3.caption("three");
        nana::checkbox cb4(fm);
        cb4.caption("four");

        nana::place plc{fm}; //fm is an instance of nana::form
        try
        {

            plc.div("p1 <a> <a> > p2 <a> <d> >");
            plc["a"]<<cb1;
            plc["b"]<<cb2;
            plc["c"]<<cb3;
            plc["d"]<<cb4;
            plc.collocate();
        }
        catch( std::runtime_error& e )
        {
            std::cout << e.what();
            nana::msgbox m( e.what() );
            m.show();
            throw;
        }
        catch( std::invalid_argument& e )
        {
            throw;
        }

        // show & run
        fm.show();
        nana::exec();
    }
    catch( std::runtime_error& e )
    {
        std::cout << e.what();
    }
    catch( std::invalid_argument& e )
    {
        std::cout << "invalid argument " << e.what() << "\n";
    }
    catch( ... )
    {
        std::cout << "unkown exception thrown\n";
    }
}