// http://nanapro.codeplex.com/discussions/544169

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>

int main()
{
    using namespace nana;

    form fm;

    // Define a background for a button.
    element::bground bground;

    // Load the BMP file containing a series of pictures - one for each button state.
    // The second parameter is passed as true to indicate that the state pictures are vertically aligned.
    // The third parameter specifies the valid area of the picture. The whole picture is used when the width or height of the valid area is zero.
    bground.image(paint::image("../Examples/bground.6states.bmp"), true, {});

    //Defaultly the button will stretch the whole background of state to fit the widget,
    //but it's easy to make the border deformation occurred. Now we need to specified the pixels
    //of 4 borders, and these border areas will not be deformed when stretching.
    bground.stretch_parts(4, 4, 4, 4);  //left, top, right, bottom

    //Create the button
    button btn(fm, nana::rectangle(20, 20, 140, 40));

    //Set the bground object.
    btn.set_bground(bground);

    fm.show();
	exec(

#ifdef NANA_AUTOMATIC_GUI_TESTING

		1, 1, [&btn]() {click(btn); }
#endif

	);
}

