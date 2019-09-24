// https://sourceforge.net/p/nanapro/blog/2016/03/nana-example---using-scrollbar/
// This example illustrates how to scroll a picture by using scrollbars.

#include <nana/gui.hpp>
#include <nana/gui/widgets/scroll.hpp>

int main()
{
    using namespace nana;

    form fm;
    scroll<true> vert_s(fm, true);
    scroll<false> horz_s(fm, true);

    //layout management for scrollbars
    place plc(fm);
    plc.div("vert<<><weight=16 vert_scroll>><weight=16 horz_bottom <horz_scroll><weight=16 corner>>");
    plc["vert_scroll"] << vert_s;
    plc["horz_scroll"] << horz_s;

    //left-upper position of image
    point pos;

    //when scrolls the scrollbars, move the picture to the corresponding position
    vert_s.events().value_changed([&]
    {
        pos.y = -static_cast<int>(vert_s.value());
        API::refresh_window(fm);
    });

    horz_s.events().value_changed([&]
    {
        pos.x = -static_cast<int>(horz_s.value());
        API::refresh_window(fm);
    });

    //Draw the image and set scrollbars.
    drawing(fm).draw([&](paint::graphics& graph)
    {
        paint::image img("image.bmp");

        const auto img_sz = img.size();

        //The area to show the picture(without scrollbars)
        auto screen_sz = graph.size();

        if (screen_sz.height < img_sz.height)
        {
            screen_sz.width -= 16;
            if (screen_sz.width < img_sz.width)
                screen_sz.height -= 16;
        }
        else if (screen_sz.width < img_sz.width)
        {
            screen_sz.height -= 16;
            if (screen_sz.height < img_sz.height)
                screen_sz.width -= 16;
        }

        if (pos.x + img_sz.width < screen_sz.width)
        {
            pos.x = static_cast<int>(screen_sz.width) - static_cast<int>(img_sz.width);
            if (pos.x > 0)
                pos.x = 0;
        }

        if (pos.y + img_sz.height < screen_sz.height)
        {
            pos.y = static_cast<int>(screen_sz.height) - static_cast<int>(img_sz.height);
            if (pos.y > 0)
                pos.y = 0;
        }

        bool has_vert = screen_sz.height < img_sz.height;
        plc.field_display("vert_scroll", has_vert);

        if (has_vert)
        {
            vert_s.range(screen_sz.height);
            vert_s.amount(img_sz.height);
            vert_s.value(-pos.y);
        }
        else
            pos.y = 0;

        bool has_horz = screen_sz.width < img_sz.width;
        //set display of "horz_bottom", not display of "horz_scroll".
        plc.field_display("horz_bottom", has_horz);

        if (has_horz)
        {
            horz_s.range(screen_sz.width);
            horz_s.amount(img_sz.width);

            horz_s.value(-pos.x);
        }
        else
            pos.x = 0;

        plc.field_display("corner", has_vert&& has_horz);

        plc.collocate();

        //Copy the picture to the form.
        img.paste(graph, pos);

        //Draw the corner
        if (has_vert && has_horz)
            graph.rectangle(rectangle( graph.width() - 16, graph.height() - 16, 16, 16 ), true, colors::white_smoke);
    });

    fm.show();

    exec();
}