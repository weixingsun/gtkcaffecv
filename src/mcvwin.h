#ifndef MCVWIN_H
#define MCVWIN_H

#include <gtkmm.h>
#include <iostream>
#include <vector>

#include "mcvdraw.h"
#include "mcvcaffe.h"

#include <opencv2/opencv.hpp>

using namespace cv;

class McvWin : public Gtk::ApplicationWindow
{
public:
    McvWin(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
    virtual ~McvWin();

    void on_classifier_signal(Classifier  *p_class);

protected:
    void on_show();
    void on_button01_clicked();
    void on_button03_clicked();
    void on_button04_clicked();
    void on_button_exit();
 

    Glib::RefPtr<Gtk::Builder>  m_refBuilder;

    Gtk::ToolButton           *p_button01;
    Gtk::ToolButton           *p_button03;
    Gtk::ToolButton           *p_button04;
    //Gtk::ToggleToolButton   *p_toggle_button05;
    //Gtk::ToggleToolButton     *p_toggle_property;
    Gtk::ToolButton           *p_button_quit;

    Gtk::Entry         *p_entry;
    Gtk::Alignment     *p_con;

    //Gtk::ScrolledWindow      *p_swin;
    Gtk::Entry               *p_edit;

    McvDraw                  *p_draw;

    Gtk::Overlay             *p_choverlay;
    //Gtk::Overlay             *p_choverlay02;
    //Gtk::Box                 *p_chbox01;
    Gtk::TextView            *p_text01;
    
    Glib::RefPtr<Gtk::TextBuffer>  m_ref_textbuf01;

    Gtk::Entry               *p_edit01;

    //Classifier               *p_caffe;
    //std::thread              *mp_thread;

    bool                      caffe_ready;

    std::unique_ptr<Classifier>    mptr_caffe;
    std::unique_ptr<std::thread>   mptr_thread;
};

#endif // MCVWIN_H
