#include <nana/gui/screen.hpp>
#include <nana/gui.hpp>

int main()
{
    using namespace nana;

    auto s = screen();
    auto pa = s.get_primary().workarea();
    form fm0, fm1, fm2, fm3;

    fm0.move(pa.x, pa.y);
    fm0.outline_size({pa.width / 2, pa.height / 2});
    fm0.caption("screen 0");

    fm1.move(pa.x + pa.width / 2, pa.y);
    fm1.outline_size({pa.width / 2, pa.height / 2});
    fm1.caption("screen 1");

    fm2.move(pa.x, pa.y + pa.height / 2);
    fm2.outline_size({pa.width / 2, pa.height / 2});
    fm2.caption("screen 2");

    fm3.move(pa.x + pa.width / 2, pa.y + pa.height / 2);
    fm3.outline_size({pa.width / 2, pa.height / 2});
    fm3.caption("screen 3");

    fm0.show();
    fm1.show();
    fm2.show();
    fm3.show();

    exec();
}