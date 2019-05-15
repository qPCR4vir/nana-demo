/**
* Created by ravenspoint on 08.04.2019 to illustrate place error checking.
* Modiffied by qPCR4vir
*/

#include <iostream>
#include <nana/gui.hpp>
#include <nana/gui/widgets/checkbox.hpp>

int main()try
{
    // construct application form
    nana::form fm( nana::rectangle( 100,100, 300, 300 ));

    nana::checkbox  cb1{fm, "one"  },
                    cb2{fm, "two"  },
                    cb3{fm, "three"},
                    cb4{fm, "four" };

        nana::place plc{fm}; //fm is an instance of nana::form
        try
        {
            plc.div("p1 <a> <b> > p2 <a> <d> >");
            plc["a"]<<cb1;
            plc["b"]<<cb2;
            plc["c"]<<cb3;
            plc["d"]<<cb4;
            plc.collocate();
        }
        catch( std::runtime_error& e )
        {
            std::cout <<"We have a runtime_error: "<< e.what();
            nana::msgbox m( "Sorry... we have a runtime_error !!");
            m<<e.what() ;
            m.show();
            throw;
        }
        catch( std::invalid_argument& e )
        {
            std::cout <<"We have a invalid_argument: " << e.what();
            nana::msgbox m( "Sorry... we have an invalid_argument error !!");
            m<<e.what() ;
            m.show();
            throw;
        }
        catch( std::exception& e )
        {
            std::cout <<"We have an exception: " << e.what();
            nana::msgbox m( "Sorry... we have an exception error !!");
            m<<e.what() ;
            m.show();
            throw;
        }
        // show & run
        fm.show();
        nana::exec();
}
catch( std::runtime_error& e )
{
    std::cout << "\nruntime_error: " << e.what();
}
catch( std::invalid_argument& e )
{
    std::cout << "\ninvalid argument: " << e.what() << "\n";
}
catch( std::exception& e )
{
    std::cout << "\nexception: " << e.what() << "\n";
}
catch( ... )
{
    std::cout << "\nunkown exception thrown\n";
}
