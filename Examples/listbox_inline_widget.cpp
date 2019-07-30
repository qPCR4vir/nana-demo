#include <nana/gui.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/textbox.hpp>

using namespace nana;

//Creates a textbox and button
//textbox shows the value of the sub item
//button is used to delete the item.
class inline_widget : public listbox::inline_notifier_interface
{
	inline_indicator * indicator_{ nullptr };
	index_type  pos_ ;
	textbox     txt_ ;
	button      btn_ ;

private:
	//Creates inline widget
	//listbox calls this method to create the widget
	//The position and size of widget can be ignored in this process
	void create(window wd) override
	{
		//Create listbox
		txt_.create(wd);
		txt_.events().click([this]
		{
			//Select the item when clicks the textbox
			indicator_->selected(pos_);
		});

		txt_.events().mouse_move([this]
		{
			//Highlight the item when hovers the textbox
			indicator_->hovered(pos_);
		});

		txt_.events().key_char([this](const arg_keyboard& arg)
		{
			if (arg.key == keyboard::enter)
			{
				//Modify the item when enter is pressed
				arg.ignore = true;
				indicator_->modify(pos_, txt_.caption());
			}
		});

		//Create button
		btn_.create(wd);
		btn_.caption("Delete");
		btn_.events().click([this]
		{
			//Delete the item when button is clicked
			auto & lsbox = dynamic_cast<listbox&>(indicator_->host());
			lsbox.erase(lsbox.at(pos_));
		});

		btn_.events().mouse_move([this]
		{
			//Highlight the item when hovers the button
			indicator_->hovered(pos_);
		});
	}

	//Activates the inline widget, bound to a certain item of the listbox
	//The inline_indicator is an object to operate the listbox item,
	//pos is an index object refers to the item of listbox
	void activate(inline_indicator& ind, index_type pos) override
	{
		indicator_ = &ind;
		pos_ = pos;
	}

	//Notify the item status
	//This method is introduced since 1.4.1
	void notify_status(status_type status, bool status_on) /*override*/ 
	{
		switch(status)
		{
		case status_type::selecting:
			//If status_on is true, the item is selecting.
			//If status on is false, the item is unselecting
			break;
		case status_type::checking:
			//If status_on is true, the item is checking
			//If status_on is false, the item is unchecking
			break;
		}
	}

	//Sets the inline widget size
	//dimension represents the max size can be set
	//The coordinate of inline widget is a logical coordinate to the sub item of listbox
	void resize(const size& dimension) override
	{
		auto sz = dimension;
		sz.width -= 50;
		txt_.size(sz);

		rectangle r(sz.width + 5, 0, 45, sz.height);
		btn_.move(r);
	}

	//Sets the value of inline widget with the value of the sub item
	void set(const value_type& value) override
	{
		txt_.caption(value);
	}

	//Determines whether to draw the value of sub item
	//e.g, when the inline widgets covers the whole background of the sub item,
	//it should return false to avoid listbox useless drawing
	bool whether_to_draw() const override
	{
		return false;
	}
private:
};

int main()
{
	using namespace nana;

	form fm;
	listbox lsbox(fm, rectangle{ 10, 10, 300, 200 });

	//Create two columns
	lsbox.append_header("column 0");
	lsbox.append_header("column 1");

	//Then append items
	lsbox.at(0).append({ std::string("Hello0"), std::string("World0") });
	lsbox.at(0).append({ std::string("Hello1"), std::string("World1") });
	lsbox.at(0).append({ std::string("Hello2"), std::string("World2") });
	lsbox.at(0).append({ std::string("Hello3"), std::string("World3") });

	//Create a new category
	lsbox.append("Category 1");

	//Append items for category 1
	lsbox.at(1).append({ std::string("Hello4"), std::string("World4") });
	lsbox.at(1).append({ std::string("Hello5"), std::string("World5") });
	lsbox.at(1).append({ std::string("Hello6"), std::string("World6") });
	lsbox.at(1).append({ std::string("Hello7"), std::string("World7") });

	//Set the inline_widget, the first column of category 0, the second column of category 1
	lsbox.at(0).inline_factory(0, pat::make_factory<inline_widget>());
	lsbox.at(1).inline_factory(1, pat::make_factory<inline_widget>());

	fm.show();
	exec();
}
