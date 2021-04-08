/*
#include <LibGUI/Window.h>
#include <LibGUI/Application.h>
#include <LibGUI/Icon.h>
#include <LibGfx/Bitmap.h>
*/
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	/*
	if (pledge("stdio recvfd sendfd accept rpath unix cpath fattr", nullptr) < 0) {
		perror("pledge");
		return 1;
	}

	auto app = GUI::Application::construct(argc, argv);

	if (pledge("stdio recvfd sendfd accept rpath", nullptr) < 0) {
		perror("pledge");
		return 1;
	}

	if (unveil("/res", "r") < 0) {
		perror("unveil");
		return 1;
	}

	unveil(nullptr, nullptr);

	auto app_icon = GUI::Icon::default_icon("ladybug");
	
	auto window = GUI::Window::construct();
	window->set_resizable(false);
	window->set_title("Commander Keen");
	window->resize(400, 300);

	window->show();
    	window->set_icon(app_icon.bitmap_for_size(16));

	return app->exec();
	*/
	return 1;
}