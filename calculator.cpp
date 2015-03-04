/*
 *	Nana Calculator
 *	Nana 0.8 and C++11 is required.
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

	state	opstate{state::init};
	wchar_t operation{L'+'};
	double oprand{0};
	double outcome{0};
	label & procedure;
	label & result;

	stateinfo(label& proc, label& resl)
		: procedure(proc), result(resl)
	{}
};

void numkey_pressed(stateinfo& state, const arg_mouse& arg)
{
	if(state.opstate != stateinfo::state::init)
	{
		if(state.opstate == stateinfo::state::assigned)
		{
			state.outcome = 0;
			state.operation = '+';			
		}
		state.result.caption(STR(""));
		state.opstate = stateinfo::state::init;
	}

	nana::string rstr = state.result.caption();
	if(rstr == STR("0"))	rstr.clear();

	wchar_t d = API::window_caption(arg.window_handle)[0];
	if(d == '.')
	{
		if(rstr.find(L'.') == rstr.npos)
			state.result.caption(rstr.size() ? rstr + d : nana::string(STR("0.")));
	}
	else
		state.result.caption(rstr + d);
}

void opkey_pressed(stateinfo& state, const arg_mouse& arg)
{
	wchar_t d = API::window_caption(arg.window_handle)[0];
	if('C' == d)
	{
		state.result.caption(STR("0"));
		state.procedure.caption(STR(""));
		state.opstate = stateinfo::state::init;
		state.outcome = 0;
		state.operation = '+';
		return;
	}
	else if(0xB1 == d)
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
				state.operation = '=';
			}

			state.result.caption(s);
			state.opstate = stateinfo::state::init;
		}
		return;
	}
	else if('%' == d)
	{
		auto s = state.result.caption();
		if(s.size())
		{
			double d = std::stod(s);
			d = state.outcome * d / 100;
			state.result.caption(std::to_wstring(d));
			state.opstate = stateinfo::state::init;
		}
		return;			
	}
	else if(state.opstate == stateinfo::state::operated)
		return;

	nana::string oprandstr = state.result.caption();
	if(0 == oprandstr.size()) oprandstr = L'0';

	wchar_t pre_operation = state.operation;
	nana::string proc;
	if('=' != d)
	{
		state.operation = d;
		if(state.opstate != stateinfo::state::assigned)
			state.oprand = std::stod(oprandstr);
		else
			pre_operation = L'=';

		proc = state.procedure.caption() + oprandstr;
		if(('X' == d || '/' == d) && (proc.find_last_of(STR("+-")) != proc.npos))
		{
			proc.insert(0, L"(");
			((proc += STR(") ")) += d) += ' ';
		}
		else
			((proc += ' ') += d) += ' ';

		state.opstate = stateinfo::state::operated;
	}
	else
	{
		if(state.opstate == stateinfo::state::init)
			state.oprand = std::stod(oprandstr);

		state.opstate = stateinfo::state::assigned;
	}

	switch(pre_operation)
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

	std::wstring outstr = std::to_wstring(state.outcome);
	while(outstr.size() && ('0' == outstr.back()))
		outstr.pop_back();
	
	if(outstr.size() && (outstr.back() == '.'))
		outstr.pop_back();
	if(outstr.size() == 0) outstr += L'0';
	state.result.caption(outstr);
}

void go()
{
	form fm;
	fm.caption(STR("Calculator"));
	
	//Use class place to layout the widgets.
	place place(fm);
	place.div(	"vert<procedure weight=10%><result weight=15%>"
		"<weight=2><opkeys margin=2 grid=[4, 5] gap=2 collapse(0,4,2,1)>");

	label procedure(fm), result(fm);

	//Make the label right aligned.
	procedure.text_align(nana::align::right);
	result.text_align(nana::align::right);
	result.typeface(nana::paint::font(nullptr, 14, true));

	place.field("procedure")<<procedure;
	place.field("result")<<result;

	stateinfo state(procedure, result);
	std::vector<std::unique_ptr<nana::button>> op_keys;

	wchar_t keys[] = L"C\261%/789X456-123+0.=";
	nana::paint::font keyfont(nullptr, 10, true);
	for(auto key : keys)
	{
		op_keys.emplace_back(new button(fm));
		op_keys.back()->caption(string(1, key));
		op_keys.back()->typeface(keyfont);

		if('=' == key)
		{
			op_keys.back()->background(0x7ACC);
			op_keys.back()->foreground(0xFFFFFF);
		}
		place.field("opkeys") << *op_keys.back();

		//Make event answer for keys.
		if(('0' <= key && key <= '9') || ('.' == key))
			op_keys.back()->events().click.connect(std::bind(numkey_pressed, std::ref(state), std::placeholders::_1));
		else
			op_keys.back()->events().click.connect(std::bind(opkey_pressed, std::ref(state), std::placeholders::_1));
	}

	place.collocate();
	fm.show();
	exec();
}
