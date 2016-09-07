#include <gtkmm.h>
#include <iostream>
#include <glog/logging.h>

#include "mcvwin.h"

int main(int argc, char** argv)
{
    // Initialize Google's logging library.
    //google::InitGoogleLogging(argv[0]);
    //FLAGS_log_dir = "./log";
    //LOG(INFO) << "argv[0]";

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.draw");

    //Load the GtkBuilder file and instantiate its widgets:
    auto refBuilder = Gtk::Builder::create();

    try
    {
        refBuilder->add_from_file("../glade/gtkwin.glade");
    }
    catch(const Glib::FileError& ex)
    {
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    }
    catch(const Glib::MarkupError& ex)
    {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    }
    catch(const Gtk::BuilderError& ex)
    {
        std::cerr << "BuilderError: " << ex.what() << std::endl; 
        return 1;
    }

    McvWin*  p_win = nullptr;

    refBuilder->get_widget_derived("applicationwindow1", p_win);

    if (p_win) app->run(*p_win);

    delete p_win;

    return 0;
}
