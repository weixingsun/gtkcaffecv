#ifndef MCVDRAW_H
#define MCVDRAW_H

#include <gtkmm.h>
#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
//#include <opencv2/xfeatures2d.hpp>

using namespace cv;

class McvDraw : public Gtk::DrawingArea
{
public:
    McvDraw();
    virtual ~McvDraw();

public:
    void  mcv_img_zoom(Mat  &image, Mat  &image_zoom, int  zwh);
    void  load_img(std::string  &str_img_fn);
    void  load_img(cv::Mat  &img);
    void  load_img(Glib::RefPtr<Gdk::Pixbuf>  pix_buf);
    void  get_img_roi(Glib::RefPtr<Gdk::Pixbuf>  &pix_buf);
    void  get_info_roi(int &x, int &y, int &w, int &h);

protected:
    //virtual bool  on_event(GdkEvent* dk_event);
    virtual void  on_show();
    virtual bool  on_button_press_event(GdkEventButton* button_event);
    virtual bool  on_motion_notify_event(GdkEventMotion* motion_event);
    virtual bool  on_button_release_event (GdkEventButton* release_event);

protected:
    //Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

    Cairo::RefPtr<Cairo::ImageSurface>   m_surface;
    Cairo::RefPtr<Cairo::Context>        m_cr;
    Glib::RefPtr<Gdk::Pixbuf>            m_pixbuf;
    Glib::RefPtr<Gdk::Pixbuf>            m_img_pixbuf;

    Mat                            m_image;
    Mat                            m_img_zoom;
    Mat                            m_img_show;

    int                            m_roi_x;
    int                            m_roi_y;
    int                            m_roi_w;
    int                            m_roi_h;

    int                            m_width;
    int                            m_height;

    bool                           m_drawing;
};

#endif // MCVDRAW_H
