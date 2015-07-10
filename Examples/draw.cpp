#include <nana/gui/wvl.hpp>

int main()
{
    using namespace nana;

    form fm {API::make_center(340, 340)};
    drawing dw(fm);
    dw.draw([](paint::graphics& graph)
    {
        graph.rectangle(rectangle{5, 5, 50, 50}, true, colors::red );
        graph.line(point(5, 5), point(55, 55), colors::blue);
        graph.line_begin(200,100);
        graph.line_to(point(300,300));
        graph.line_to(point(100,200));
        graph.line_to(point(300,200));
        graph.line_to(point(100,300));
        graph.line_to(point(200,100));
		graph.save_as_file("graphics.bmp");
		nana::paint::image img(STR("C:\\Prog\\ExtLib\\nana-demo\\bin\\test.bmp"));
		img.paste(img.size(), graph, nana::point());
		graph.save_as_file("C:\\Prog\\ExtLib\\nana-demo\\bin\\testGraphics.bmp");
    });
	nana::paint::graphics graphics;
	nana::paint::image img(STR("C:\\Prog\\ExtLib\\nana-demo\\bin\\test.bmp"));
	img.paste(img.size(), graphics, nana::point());
	graphics.save_as_file("C:\\Prog\\ExtLib\\nana-demo\\bin\\testGraphics2.bmp");
    dw.update();
    fm.show();
    ::nana::exec();
}
