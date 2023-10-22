#pragma once

#include "text_view.h"

class App {
public:
	void input();
    void run();

	void new_file();
	void open_file();
	void save_file();
	void save_file_as();

private:
	bool m_running = true;
	TextView m_text_view;
};