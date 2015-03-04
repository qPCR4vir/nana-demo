#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/button.hpp>
int main()
		{
		  nana::gui::form form;
		  nana::gui::button btn(form, nana::rectangle(5, 5, 94, 23));
		  btn.image(STR("button_image.bmp"));
		  btn.image_valid_area(nana::arrange::horizontal, nana::rectangle(0, 0, 94 * 5, 23));
		  form.show();
		  nana::gui::exec();
		}
/*!\todo provide button_image.bmp*/
