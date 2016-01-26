/*
 *	Nana Calculator
 *	Nana 1.3 and C++11 is required.
 *	This is a demo for Nana C++ Library.
 *	It creates an intermediate level graphical calculator with few code.
 */

#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/place.hpp>

using namespace nana;

struct stateinfo
{
	enum class state{init, operated, assigned};

	state	opstate {state::init};
	std::string    operation {"+"};
	double  oprand  {0};
	double  outcome {0};
	label & procedure;
	label & result;

	stateinfo(label& proc, label& resl)
		: procedure(proc), result(resl)
	{}
};

void numkey_pressed(stateinfo& state, const arg_click& arg)
{
	if(state.opstate != stateinfo::state::init)
	{
		if(state.opstate == stateinfo::state::assigned)
		{
			state.outcome = 0;
			state.operation = "+";			
		}
		state.result.caption("");
		state.opstate = stateinfo::state::init;
	}

	std::string rstr = state.result.caption();
	if(rstr == "0")	rstr.clear();

	std::string d = API::window_caption(arg.window_handle);
	if(d == ".")
	{
		if(rstr.find('.') == rstr.npos)
			state.result.caption(rstr.size() ? rstr + d : std::string("0."));
	}
	else
		state.result.caption(rstr + d);
}

void opkey_pressed(stateinfo& state, const arg_click& arg)
{
	std::string d = API::window_caption(arg.window_handle) ;
	if("C" == d)
	{
		state.result.caption("0");
		state.procedure.caption("");
		state.opstate = stateinfo::state::init;
		state.outcome = 0;
		state.operation = "+";
		return;
	}
	else if( "\u00b1" == d) // 0xB1    u8"\261"
	{
		auto s = state.result.caption();
		if(s.size())
		{
			if(s[0] == '-')
				s.erase(0, 1);
			else
				s.insert(0, 1, '-');

			if(state.opstate == stateinfo::state::assigned)
			{
				state.outcome = -state.outcome;
				state.operation = "=";
			}

			state.result.caption(s);
			state.opstate = stateinfo::state::init;
		}
		return;
	}
	else if("%" == d)
	{
		auto s = state.result.caption();
		if(s.size())
		{
			double d = std::stod(s);
			d = state.outcome * d / 100;
			state.result.caption(std::to_string(d));
			state.opstate = stateinfo::state::init;
		}
		return;			
	}
	else if(state.opstate == stateinfo::state::operated)
		return;

	std::string oprandstr = state.result.caption();
	if(0 == oprandstr.size()) oprandstr = '0';

	std::string pre_operation = state.operation;
	std::string proc;
	if("=" != d)
	{
		state.operation = d;
		if(state.opstate != stateinfo::state::assigned)
			state.oprand = std::stod(oprandstr);
		else
			pre_operation = "=";

		proc =  state.procedure.caption()  + oprandstr ;
		if(("X" == d || "/" == d) && (proc.find_last_of("+-") != proc.npos))
		{
			proc.insert(0, "(");
			(( proc += ") " )  += d) += " ";
		}
		else
			((proc += " ") += d) += " ";

		state.opstate = stateinfo::state::operated;
	}
	else
	{
		if(state.opstate == stateinfo::state::init)
			state.oprand = std::stod(oprandstr);

		state.opstate = stateinfo::state::assigned;
	}

	switch(pre_operation[0])
	{
	case '+':
		state.outcome += state.oprand;
		break;
	case '-':
		state.outcome -= state.oprand;
		break;
	case 'X':
		state.outcome *= state.oprand;
		break;
	case '/':
		state.outcome /= state.oprand;
		break;
	}

	state.procedure.caption(proc);

	std::string outstr = std::to_string(state.outcome);
	while(outstr.size() && ('0' == outstr.back()))
		outstr.pop_back();
	
	if(outstr.size() && (outstr.back() == '.'))
		outstr.pop_back();
	if(outstr.size() == 0) outstr += '0';
	state.result.caption(outstr);
}

void go()
{
	form fm;
	fm.caption(("Calculator"));
	
	//Use class place to layout the widgets.
	place place(fm);
	place.div(	"vert<procedure weight=10%><result weight=15%>"
		"<weight=2><opkeys margin=2 grid=[4, 5] gap=2 collapse(0,4,2,1)>");

	label procedure(fm), result(fm);

	//Make the label right aligned.
	procedure.text_align(nana::align::right);
	result.text_align(nana::align::right);
	result.typeface(nana::paint::font("", 14, true));

	place.field("procedure")<<procedure;
	place.field("result")<<result;

	stateinfo state(procedure, result);
	std::vector<std::unique_ptr<nana::button>> op_keys;

	char keys[] = "Cm%/789X456-123+0.="; // \261
	nana::paint::font keyfont("", 10, true);
	//constexpr char k[]{ u8"\261" };
	for(auto key : keys)
	{

		std::string Key;
		if (key == 'm')
			Key = "\u00b1";  // in MSVC2015 u8"\261"; in ISO Latin 1 Character set: unsigned char 177; xB1 ; &plusmn;
			                 // http://daniel-hug.github.io/characters/
		else
			Key = std::string(1, key);

		op_keys.emplace_back(new button(fm));
		op_keys.back()->caption( Key);
		op_keys.back()->typeface(keyfont);

		if('=' == key)
		{
			op_keys.back()->bgcolor ( color_rgb(  0x7ACC));
			op_keys.back()->fgcolor ( color_rgb(0xFFFFFF));
		}
		place.field("opkeys") << *op_keys.back();

		//Make event answer for keys.
		if((L'0' <= key && key <= L'9') || (L'.' == key))
			op_keys.back()->events().click.connect(std::bind(numkey_pressed, std::ref(state), std::placeholders::_1));
		else
			op_keys.back()->events().click.connect(std::bind(opkey_pressed, std::ref(state), std::placeholders::_1));
	}

	place.collocate();
	fm.show();
	exec();
}

int main()
{
    go();
}