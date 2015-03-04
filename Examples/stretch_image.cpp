#include <nana/gui/wvl.hpp>
#include <nana/gui/drawing.hpp>
#include <nana/paint/image_process_selector.hpp>

using namespace nana::gui;

class tsform
    : public form
{
public:
    tsform()
    {

        img_.open(STR("image01.bmp"));    /// \todo Open the image file.

        //Copy the image to the window
        nana::size sz = size();
       
        drawing dw(*this);
        dw.bitblt(0, 0, sz.width, sz.height, img_, 0, 0);
        dw.update();

        //Register events
        make_event<events::click>(*this, &tsform::_m_click);
        make_event<events::size>(*this, &tsform::_m_size);
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
        dw.stretch(size(), img_, img_.size());
        dw.update();
    }
private:
    nana::paint::image img_;
};

int main()
{
    tsform fm;
    fm.show();
    exec();
}
