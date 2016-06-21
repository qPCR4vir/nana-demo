#include <nana/gui/wvl.hpp>
#include <nana/gui/drawing.hpp>
#include <nana/paint/image_process_selector.hpp>
#include <iostream>

using namespace nana;

std::ostream& operator << (std::ostream& os, const point& p)
{
	return os << "point{" << p.x << "," << p.y << "}";
}
std::ostream& operator<< (std::ostream& os, const nana::size& z)
{
	return os << "size{" << z.width << "," << z.height << "}";
}
std::ostream& operator<< (std::ostream& os, const rectangle& r)
{
	return os << "rect{" << r.position() << "," << r.dimension() << "}";
}


class tsform
    : public form
{
    nana::paint::image img_;
	paint::graphics gr_;

public:
    tsform()
    {
		if (!img_.open(("../Examples/bground.6states.bmp")))
			throw std::runtime_error("Imposible to open the image");    /// \todo Open the image file.
        
		drawing dw(*this);
		dw.draw([this](paint::graphics& graph)
		{
			rectangle r { point{ 0,0 }, img_.size() };
			rectangle rw{ point{ 0,0 }, size() };
			//nana::size sz{size()};


			graph.rectangle(rectangle{ 5, 5, 50, 50 }, true, colors::red);
			graph.line(point(5, 5), point(55, 55), colors::blue);
			graph.line_begin(200, 100);
			graph.line_to(point(300, 300));
			graph.line_to(point(100, 200));
			graph.line_to(point(300, 200));
			graph.line_to(point(100, 300));
			graph.line_to(point(200, 100));

			img_.stretch(r, gr_, rw	);

			std::cout << "Pict: " << r << ", Windows: " << rw << "\n";
		});

  //      // get the graphycs !?
		//if (!API::window_graphics(*this, gr_))
		//	throw std::runtime_error("Imposible to get the graph");
  //      

  //      //Copy the image to the window
		//img_.paste( rectangle{ point{0,0}, img_.size() },
		//			  gr_,
		//			  {0,0});
		//img_.stretch( rectangle{ point{0,0}, img_.size() },
		//			  gr_,
		//			  size()  );

		dw.update();
        //Register events
        events().click  ( [this](){_m_click();} );  
        //events().resized( [this](){_m_size ();} );  
    }
private:
    //Switchs the algorithm between two algorithms in every click on the form.
    void _m_click()
    {
        static bool interop;
        nana::paint::image_process::selector sl;
        sl.stretch(interop ? "bilinear interoplation" : "proximal interoplation");
        interop = !interop;
		std::cout << (interop ? "Click: bilinear interoplation\n" 
			                 : "Click: proximal interoplation\n") ;
    }

  //  //When the window size is changed, it stretches the image to fit the window.
  //  void _m_size()
  //  {
  //      drawing dw(*this);
  //     
  //      dw.clear();  //Before stretch, it should clear the operations that are generated before.
  //      
		//img_.stretch(rectangle{ point{ 0,0 }, img_.size() },
		//	gr_,
		//	size());

		//dw.update();
		//std::cout << "resize\n";
  //  }
};

int main()
{
    tsform fm;
    fm.show();
    exec();
}
