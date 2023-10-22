#pragma once

#include "text_view.h"
#include "text_input.h"

class App {
public:
	App();
	void run();
	void input();
	void update();
	void render();


	void new_file();
	void open_file();
	void save_file();
	void save_file_as();

private:
	bool m_running = true;
	TextView m_text_view;
	TextInput m_text_input;
	std::string m_file_path;
};