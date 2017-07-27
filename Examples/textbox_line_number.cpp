//Nana's example
//This example illustrates how to show the line number of textbox.
//Nana 1.5.4 is required

#include <nana/gui.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/textbox.hpp>

int main()
{
	using namespace nana;

	form fm;

	//Define a panel widget to draw line numbers.
	panel<true> line(fm);

	textbox tbox(fm);
	//tbox.line_wrapped(true); //Add this line and input a very very long line of text, give it a try.

	//Layout management.
	place plc(fm);

	//Define two fields, the weight of line field will be changed.
	fm.div("margin=5 <line weight=15><text>");

	fm["line"] << line;
	fm["text"] << tbox;
	fm.collocate();

	//Draw the line numbers
	drawing{ line }.draw([&](paint::graphics& graph)
	{
		auto line_px = tbox.line_pixels();
		if (0 == line_px)
			return;

		//Returns the text position of each line that currently displays on screen.
		auto text_pos = tbox.text_position();

		//Textbox has supported smooth scrolling since 1.5. Therefore it would only render
		//the lower part of top displayed line if it is scrolled less than a line height.
		//
		//tbox.content_origin().y % line_px, get the height which is scrolled less than
		//a line height.
		int top = tbox.text_area().y - tbox.content_origin().y % line_px;
		int right = static_cast<int>(graph.width()) - 5;

		for (auto & pos : text_pos)
		{
			auto line_num = std::to_wstring(pos.y + 1);
			auto pixels = graph.text_extent_size(line_num).width;

			//Check if the panel widget is not enough room to display a line number
			if (pixels + 5 > graph.width())
			{
				//Change the weight of 'line' field.
				fm.get_place().modify("line", ("weight=" + std::to_string(pixels + 10)).c_str());
				fm.collocate();
				return;
			}

			//Draw the line number
			graph.string({ right - static_cast<int>(pixels), top }, line_num);

			top += line_px;
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
