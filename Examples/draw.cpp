#include <nana/gui.hpp>

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
		nana::paint::image img("test.bmp");
		if (img.empty())
		{
			graph.line(point(100, 100), point(300, 100), colors::red);
			graph.save_as_file("test.bmp");
		}
		else 
			img.paste(nana::rectangle(img.size()), graph, nana::point());

		graph.save_as_file("testGraphics.bmp");
    });
	nana::paint::graphics graphics;
	nana::paint::image img("test.bmp");
	if (!img.empty())
	{
		img.paste(nana::rectangle(img.size()), graphics, nana::point());
		graphics.save_as_file("testGraphics2.bmp");
	}
	dw.update();
    fm.show();
    ::nana::exec();
}
