#include <nana/gui/wvl.hpp>
#include <nana/gui/drawing.hpp>
#include <nana/paint/image_process_selector.hpp>

using namespace nana;

class tsform
    : public form
{
    nana::paint::image img_;

public:
    tsform()
    {

        img_.open(STR("../Examples/bground.6states.bmp"));    /// \todo Open the image file.

        //Copy the image to the window
        nana::size sz = size();
       
        drawing dw(*this);

        // get the graphycs !?
        dw.bitblt(0, 0, sz.width, sz.height, img_, 0, 0);
        dw.update();

        //Register events
        events().click  ( [this](){_m_click();} );  
        events().resized( [this](){_m_size ();} );  
    }
private:
    //Switchs the algorithm between two algorithms in every click on the form.
    void _m_click()
    {
        static bool interop;
        nana::paint::image_process::selector sl;
        sl.stretch(interop ? "bilinear interoplation" : "proximal interoplation");
        interop = !interop;
    }

    //When the window size is changed, it stretches the image to fit the window.
    void _m_size()
    {
        drawing dw(*this);
       
        dw.clear();  //Before stretch, it should clear the operations that are generated before.
        // get the graphycs !?
        dw.stretch(size(), img_, img_.size());
        dw.update();
    }
};

int main()
{
    tsform fm;
    fm.show();
    exec();
}
