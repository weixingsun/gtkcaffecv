#include "mcvdraw.h"

using namespace cv;

McvDraw::McvDraw()
{
    m_width = 800;
    m_height = 600;

    m_surface = Cairo::ImageSurface::create(Cairo::FORMAT_RGB24, m_width, m_height);

    m_cr = Cairo::Context::create(m_surface);

    this->add_events(Gdk::LEAVE_NOTIFY_MASK |
                     Gdk::BUTTON_PRESS_MASK |
                     Gdk::BUTTON_RELEASE_MASK |
                     Gdk::POINTER_MOTION_MASK |
                     Gdk::POINTER_MOTION_HINT_MASK);

    this->signal_motion_notify_event().connect(
        sigc::mem_fun(*this, &McvDraw::on_motion_notify_event));

    this->signal_button_press_event().connect(
        sigc::mem_fun(*this, &McvDraw::on_button_press_event));

    m_drawing = false;
}

McvDraw::~McvDraw()
{

}

void  McvDraw::mcv_img_zoom(Mat  &image, Mat  &image_zoom, int  zwh)
{
    int    width01;
    int    high01;

    int    *p_l = &width01;
    int    *p_s = &high01;

    int    img_ls = image.cols;
    int    img_ss = image.rows;

    if (image.cols < image.rows)
    {
        p_s =  &width01;
        p_l =  &high01;
        img_ls = image.rows;
        img_ss = image.cols;
    }

    *p_l = zwh;
    float  zoom = *p_l*1.0/img_ls;
    *p_s = img_ss*zoom + 0.5;

    //resize(image, image_zoom, Size(width01, high01), CV_INTER_LINEAR);
    //resize(image, image_zoom, Size(width01, high01), CV_INTER_AREA);
    resize(image, image_zoom, Size(width01, high01), CV_INTER_CUBIC);
    //resize(image, image_zoom, Size(width01, high01), CV_INTER_LANCZOS4);
}

bool McvDraw::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    /*
    Cairo::RefPtr<Cairo::Context> cr_01 = this->get_window()->create_cairo_context();
    cr_01->set_source_rgb(0.0, 0.0, 0.8);
    cr_01->rectangle(80, 80, 32, 32);
    cr_01->fill();
    cr_01->stroke();
    return true;
    */

    if (!m_pixbuf) return false;

    //Gtk::Allocation allocation = get_allocation();
    //const int width = allocation.get_width();
    //const int height = allocation.get_height();

    // Draw the image in the middle of the drawing area, or (if the image is
    // larger than the drawing area) draw the middle part of the image.
    //Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf, (width - m_pixbuf->get_width())/2, (height - m_pixbuf->get_height())/2);

    Gdk::Cairo::set_source_pixbuf(cr, m_pixbuf, 0, 0);
    cr->paint();

    cr->set_source_rgb(0.8, 0.8, 0.0);

    cr->rectangle(m_roi_x, m_roi_y, m_roi_w, m_roi_h);

    cr->stroke();

    return true;
}

void  McvDraw::load_img(std::string  &str_img_fn)
{
    m_image = imread(str_img_fn.c_str(), IMREAD_COLOR);

    mcv_img_zoom(m_image, m_img_zoom, 600);

    cvtColor(m_img_zoom, m_img_show, CV_BGR2RGB);

    if( m_img_show.empty() )
    {
        std::cout <<  "Could not open or find the image" << std::endl;
    }


    m_pixbuf = Gdk::Pixbuf::create_from_data(
                   (guint8*)m_img_show.data,
                   Gdk::COLORSPACE_RGB,
                   false,
                   8,
                   m_img_show.cols,
                   m_img_show.rows,
                   m_img_show.step);

    //m_width = m_pixbuf->get_width();
    //m_height = m_pixbuf->get_height();

    //Gdk::Cairo::set_source_pixbuf(m_cr, m_pixbuf, 0, 0);
    //m_cr->paint();

    m_roi_x = 0;
    m_roi_y = 0;
    m_roi_w = 0;
    m_roi_h = 0;
}

void  McvDraw::load_img(cv::Mat  &img)
{
    m_image = img;

    mcv_img_zoom(m_image, m_img_zoom, 600);

    cvtColor(m_img_zoom, m_img_show, CV_BGR2RGB);

    if( m_img_show.empty() )
    {
        std::cout <<  "Could not open or find the image" << std::endl;
    }

    m_pixbuf = Gdk::Pixbuf::create_from_data(
                   (guint8*)m_img_show.data,
                   Gdk::COLORSPACE_RGB,
                   false,
                   8,
                   m_img_show.cols,
                   m_img_show.rows,
                   m_img_show.step);

    m_width = m_pixbuf->get_width();
    m_height = m_pixbuf->get_height();

    Gdk::Cairo::set_source_pixbuf(m_cr, m_pixbuf, 0, 0);
    m_cr->paint();

    m_roi_x = 0;
    m_roi_y = 0;
    m_roi_w = 0;
    m_roi_h = 0;
}

void  McvDraw::load_img(Glib::RefPtr<Gdk::Pixbuf>  pix_buf)
{
    m_pixbuf = pix_buf->copy();

    m_width = m_pixbuf->get_width();
    m_height = m_pixbuf->get_height();

    Gdk::Cairo::set_source_pixbuf(m_cr, m_pixbuf, 0, 0);
    m_cr->paint();

    m_roi_x = 0;
    m_roi_y = 0;
    m_roi_w = 0;
    m_roi_h = 0;
}

void  McvDraw::get_img_roi(Glib::RefPtr<Gdk::Pixbuf>  &pix_buf)
{
    m_pixbuf->copy_area(m_roi_x, m_roi_y, m_roi_w, m_roi_h, pix_buf, 0, 0);
}

void  McvDraw::get_info_roi(int &x, int &y, int &w, int &h)
{
    x = m_roi_x;
    y = m_roi_y;
    w = m_roi_w;
    h = m_roi_h;
}

void  McvDraw::on_show()
{
    Gtk::DrawingArea::on_show();

    std::cout << "McvDraw on_show" << std::endl;
}

/*
bool  McvDraw::on_button_press_event(GdkEventButton* button_event)
{
    std::cout << "McvDraw button_press : " << button_event->x << "  " << button_event->y << std::endl;

    int  x = button_event->x;
    int  y = button_event->y;

    Gdk::Rectangle update_rect(x - 3, y - 3, 6, 6);

    //Gdk::Cairo::set_source_pixbuf(m_cr, m_pixbuf, 0, 0);

    Cairo::RefPtr<Cairo::Context> m_cr = this->get_window()->create_cairo_context();

    m_cr->paint();

    m_cr->set_source_rgb(0.0, 0.0, 0.8);

    m_cr->rectangle(x, y, 12, 12);

    m_cr->fill();

    m_cr->stroke();

    m_cr->paint();

    return  true;
}
*/

bool  McvDraw::on_button_press_event(GdkEventButton* button_event)
{
    std::cout << "McvDraw button_press : " << button_event->x << "  " << button_event->y << std::endl;

    //int  x = button_event->x;
    //int  y = button_event->y;

    m_roi_x = button_event->x;
    m_roi_y = button_event->y;

    m_drawing = true;

    return  true;
}


bool  McvDraw::on_motion_notify_event(GdkEventMotion* motion_event)
{
    //std::cout << "McvDraw  motion_notify : " << motion_event->x << "  " << motion_event->y << std::endl;

    if (m_drawing == false) return true;

    //int  x = motion_event->x;
    //int  y = motion_event->y;

    m_roi_w = motion_event->x - m_roi_x;
    m_roi_h = motion_event->y - m_roi_y;

    if (m_roi_w < 0) m_roi_w = 0;
    if (m_roi_h < 0) m_roi_h = 0;

    Glib::RefPtr<Gdk::Window> win = this->get_window();

    if (win)
    {
        Gdk::Rectangle r(0, 0, 800, 600);
        win->invalidate_rect(r, false);
        //this->show();
    }

    /*
    Gdk::Cairo::set_source_pixbuf(m_cr, m_pixbuf, 0, 0);
    m_cr->paint();

    m_cr->set_source_rgb(0.8, 0.8, 0.0);

    m_cr->rectangle(m_roi_x, m_roi_y, m_roi_w, m_roi_h);

    m_cr->fill();

    m_cr->stroke();

    Glib::RefPtr<Gdk::Pixbuf>     pixbuf = Gdk::Pixbuf::create(m_surface, 0, 0, m_width, m_height);

    Cairo::RefPtr<Cairo::Context> cr = this->get_window()->create_cairo_context();
    Gdk::Cairo::set_source_pixbuf(cr, pixbuf, 0, 0);
    cr->paint();
    */

    return  true;
}

bool  McvDraw::on_button_release_event(GdkEventButton* release_event)
{
    std::cout << "McvDraw  button_release : " << release_event->x << "  " << release_event->y << std::endl;

    if (release_event->x<0 || release_event->y<0) return true;

    //int  x = release_event->x;
    //int  y = release_event->y;

    m_roi_w = release_event->x - m_roi_x;
    m_roi_h = release_event->y - m_roi_y;

    if (m_roi_w<0 || m_roi_h<0)
    {
        m_roi_w = 0;
        m_roi_h = 0;
    }

    Glib::RefPtr<Gdk::Window> win = this->get_window();

    if (win)
    {
        Gdk::Rectangle r(0, 0, 800, 600);
        win->invalidate_rect(r, false);
    }

    m_drawing = false;

    return  true;
}
