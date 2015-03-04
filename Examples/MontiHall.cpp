#include <nana/gui/wvl.hpp> 
#include <nana/gui/widgets/label.hpp> 
#include <nana/gui/widgets/button.hpp> 
#include <nana/system/platform.hpp> 

class monty_hall 
      : public nana::gui::form 
{ 
    enum state_t{state_begin, state_picked, state_over}; 
  public: 
    monty_hall(); 
  private: 
    void _m_pick_door  (const nana::gui::eventinfo& ei); 
    void _m_play       (int door); 
    void _m_remove_door(int exclude); 
  private: 
    state_t    state_; 
    int        door_has_car_; 
    nana::gui::label  label_; 
    nana::gui::button door_[3]; 
 }; 
 int main() 
 { 
    monty_hall mh; 
    mh.show(); 
    nana::gui::exec(); 
 }

monty_hall::monty_hall() 
      : nana::gui::form(  nana::gui::API::make_center(400, 150)
	                      , appear::decorate<appear::taskbar>()   ) 
       ,state_(state_begin) 
 { 
  this->caption(STR("The Monty Hall Problem")); 
  nana::string text = STR("Hi, I am the host, you are on a Game Show:\n") 
  STR("You are given the choice of one of tree Doors.\n") 
  STR("One door has a new Car behind it and the other two: Goats.\n") 
  STR("Now pick a door to win the Car."); 

  label_.create(*this, nana::size(400, 100)); 
  label_.caption(text); 

  nana::string door_name[3] = 
                            {STR("Door No.&1"), STR("Door No.&2"), STR("Door No.&3")}; 
  for(int i = 0; i < 3; ++i) 
  { 
    door_[i].create(*this, nana::rectangle(50 + 110 * i, 110, 100, 24)); 
    door_[i].caption(door_name[i]); 
    door_[i].make_event<nana::gui::events::click>(*this, 
                                                        &monty_hall::_m_pick_door); 
  } 
}

 void monty_hall::_m_pick_door(const nana::gui::eventinfo& ei) 
{ 
 int index = 0; 
 for(; index < 3; ++index) 
 { 
     if(door_[index] == ei.window) 
        break; 
 } 
 _m_play(index); 
}

void monty_hall::_m_play(int door) 
{ 
   switch(state_) 
   { 
      case state_begin: 
                     door_has_car_ = (nana::system::timestamp() / 1000) % 3; 
                     _m_remove_door(door); 
                     state_ = state_picked; 
                break; 
      case state_picked: 
                     label_.caption(door_has_car_ == door ? 
                                       STR("Yes, you win the new Car!!") : STR("Sign, you are lost!")); 
                     state_ = state_over; 
                break; 
      } 
   }

void monty_hall::_m_remove_door(int exclude) 
{ 
     std::vector<int> doors; 
     for(int i = 0; i < 3; ++i) 
     { 
         if(i != exclude) 
             doors.push_back(i); 
     } 
     unsigned ts = (nana::system::timestamp() / 1000) % 2; 
     if(door_has_car_ == doors[ts]) 
         ts = (ts ? 0: 1); 
     door_[doors[ts]].enabled(false); 
     doors.erase(doors.begin() + ts); 
     nana::string text = STR("I know what's behind all the doors and") 
         STR("I remove a door which a goat behind it. \n") 
         STR("And now, do you want to stick with your decision") 
         STR(" of Door No.X or do you want to change your choice") 
         STR(" to Door No.Y?"); 
     nana::char_t door_char = '1' + exclude; 
     nana::string::size_type pos = text.find(STR("Door No.X")); 
     text.replace(pos + 8, 1, 1, door_char); 
     door_char = '1' + doors[0]; 
     pos = text.find(STR("Door No.Y")); 
     text.replace(pos + 8, 1, 1, door_char); 
     label_.caption(text); 
} 
