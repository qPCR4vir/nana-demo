//Nana's example
//This example illustrates how to show the line number of textbox.
//Nana 1.2 is required

#include <nana/gui.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/textbox.hpp>

int main()
{
	using namespace nana;

	form fm;

	//Define a panel widget to draw line numbers.
	panel<true> line(fm);

	textbox tbox(fm);
	//tbox.line_wrapper(true); //Add this line and input a very very long line of text, give it a try.

	//Layout management.
	place plc(fm);
    
    //Define two fields, the weight of line field will be changed.
	plc.div("margin=5 <line weight=15><text>");

	plc["line"] << line;
	plc["text"] << tbox;
	plc.collocate();

	//Draw the line numbers
	drawing{ line }.draw([&tbox, &plc](paint::graphics& graph)
	{
		//Returns the text position of each line that currently displays on screen.
		auto text_pos = tbox.text_position();

		int top = tbox.text_area().y;
		int right = static_cast<int>(graph.width()) - 5;

		for (auto & pos : text_pos)
		{
			auto line_num = std::to_wstring(pos.y + 1);
			auto pixels = graph.text_extent_size(line_num).width;

			//Check if the panel widget is not enough room to display a line number
			if (pixels + 5 > graph.width())
			{
				//Change the weight of 'line' field.
				std::stringstream ss;
				ss << "weight=" << pixels + 10;
				plc.modify("line", ss.str().data());
				plc.collocate();
				return;
			}

			//Draw the line number
			graph.string({ right - static_cast<int>(pixels), top }, line_num);

			top += tbox.line_pixels();
		}
	});

	//When the text is exposing in the textbox, refreshs
	//the panel widget to redraw new line numbers.
	tbox.events().text_exposed([&line]
	{
		API::refresh_window(line);
	});

	fm.show();
	exec();
}