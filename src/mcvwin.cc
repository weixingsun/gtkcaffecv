#include "mcvwin.h"

using namespace cv;

McvWin::McvWin(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    : Gtk::ApplicationWindow(cobject),
      m_refBuilder(refBuilder)
{
    p_button01 = nullptr;
    m_refBuilder->get_widget("toolbutton1", p_button01);

    p_button03 = nullptr;
    m_refBuilder->get_widget("toolbutton3", p_button03);

    //p_button04 = nullptr;
    //m_refBuilder->get_widget("toolbutton4", p_button04);

    p_button_quit = nullptr;
    m_refBuilder->get_widget("toolbutton6", p_button_quit);

    p_entry = nullptr;
    m_refBuilder->get_widget("entry1", p_entry);

    p_choverlay = nullptr;
    m_refBuilder->get_widget("choverlay01", p_choverlay);

    p_text01 = nullptr;
    m_refBuilder->get_widget("textview01", p_text01);

    p_draw = new McvDraw();

    //std::string     str_fn = "../imgs/imgs01.jpg";
    std::string     str_fn = "../imgs/2007_000256.jpg";

    Glib::ustring   ustr_text = str_fn;

    p_entry->set_text(ustr_text);

    p_draw->load_img(str_fn);

    p_choverlay->add(*p_draw);

    if(p_button01)
    {
        p_button01->signal_clicked().connect( sigc::mem_fun(*this, &McvWin::on_button01_clicked) );
    }

    if(p_button03)
    {
        p_button03->signal_clicked().connect( sigc::mem_fun(*this, &McvWin::on_button03_clicked) );
    }

    if(p_button_quit)
    {
        p_button_quit->signal_clicked().connect( sigc::mem_fun(*this, &McvWin::on_button_exit) );
    }

    //p_edit01 = new Gtk::Entry();

    //std::string  model_file   = "../data/deploy.prototxt";
    //std::string  trained_file = "../data/bvlc_reference_caffenet.caffemodel";
    //std::string  mean_file    = "../data/imagenet_mean.binaryproto";
    //std::string  label_file   = "../data/synset_words.txt";

    std::string  model_file   = "../data/squeezenet_v1.1.prototxt";
    std::string  trained_file = "../data/squeezenet_v1.1.caffemodel";
    std::string  mean_file    = "../data/imagenet_mean.binaryproto";
    std::string  label_file   = "../data/synset_words.txt";


    mptr_caffe.reset(new Classifier(model_file, trained_file, mean_file, label_file));

    mptr_caffe->signal_connect(this);

    caffe_ready = true;

    m_ref_textbuf01 = Gtk::TextBuffer::create();
}

McvWin::~McvWin()
{
    std::cout << "~McvWin" << std::endl;

    //mp_thread->join();

    //delete  mp_thread;
    //delete  p_caffe;
}

void McvWin::on_show()
{
    ApplicationWindow::on_show();

    show_all();

    std::string  str_show = "Please draw a rectangle Region Of Interest in image, click run toolbutton.";
 
    m_ref_textbuf01->set_text(str_show);

    p_text01->set_buffer(m_ref_textbuf01);

    std::cout << "McvWin on_show" << std::endl;
}

void McvWin::on_button01_clicked()
{
    std::cout << "on_button01_clicked" << std::endl;
    //LOG(INFO) << "gtkdnn : " << " on_button01_clicked ...";

    Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:

    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("Image files");
    filter_text->add_mime_type("image/jpg");
    filter_text->add_mime_type("image/jpeg");
    filter_text->add_mime_type("image/png");
    dialog.add_filter(filter_text);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch(result)
    {
    case(Gtk::RESPONSE_OK):
    {
        std::string  str_fn = dialog.get_filename();

        std::cout << "Select clicked." << std::endl;
        std::cout << "Folder selected: " << str_fn << std::endl;

        p_entry->set_text(str_fn);

        p_draw->load_img(str_fn);

        break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
        std::cout << "Cancel clicked." << std::endl;
        break;
    }
    default:
    {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
    }
    }
}

void McvWin::on_button03_clicked()
{
    int x, y, w, h;

    p_draw->get_info_roi(x, y, w, h);

    std::string  str_show = "Please draw a rectangle Region Of Interest in image, click run toolbutton.";
 
    m_ref_textbuf01->set_text(str_show);

    p_text01->set_buffer(m_ref_textbuf01);

    if (w<=0 || h<=0) return;

    //Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 600, 400);
    Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_RGB24, w, h);

    //Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

    Glib::RefPtr<Gdk::Pixbuf>   pix_buf = Gdk::Pixbuf::create(surface, 0, 0, w, h);
    //Glib::RefPtr<Gdk::Pixbuf>   pix_buf = Gdk::Pixbuf::create(surface, 0, 0, 600, 400);

    //Glib::RefPtr<Gdk::Pixbuf>   pix_buf;

    p_draw->get_img_roi(pix_buf);

    Mat  img_buf(cv::Size(pix_buf->get_width(), pix_buf->get_height()), CV_8UC3, (uchar*)pix_buf->get_pixels(), pix_buf->get_rowstride());

    cvtColor(img_buf, img_buf, CV_BGR2RGB);

    //imwrite("pix_buf.jpg", img_buf);
    //pix_buf->save("pix_buf.png", "png");

    //double time_b = (double)getTickCount();
    //double time_e = (double)getTickCount();
    //double time_nn = (time_e - time_b)/getTickFrequency()*1000.0;

    mptr_caffe->load_image(img_buf);

    //std::unique_ptr<std::thread>   ptr_thread(new std::thread(&Classifier::fun, p_caffe));

    //mptr_thread=std::move(ptr_thread);

    if (caffe_ready) { mptr_thread.reset(new std::thread(&Classifier::fun, &(*mptr_caffe))); }

    caffe_ready = false;

    //std::cout << "Caffe time : " << time_nn << " ms" << std::endl;
}

void McvWin::on_button04_clicked()
{
    std::cout << "on_button04_clicked : "<< std::endl;

    return;
}


void McvWin::on_classifier_signal(Classifier  *p_class)
{
    mptr_thread->join();
    mptr_thread.release();

    caffe_ready = true;

    std::string  str_class = "\n";

    p_class->get_str_class(str_class);

    std::cout << str_class << std::endl;
 
    m_ref_textbuf01->set_text(str_class);

    p_text01->set_buffer(m_ref_textbuf01);
}

void McvWin::on_button_exit()
{

    hide(); //hide() will cause main::run() to end.
}

