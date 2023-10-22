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

	void parse_command();

	void open_file(const std::string& file_path);
	void save_file(const std::string& file_path);
	void new_file();

private:
	bool m_running = true;
	TextView m_text_view;
	TextInput m_text_input;
	std::string m_file_path;
};