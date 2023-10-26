#pragma once

#include "text_view.h"
#include "text_input.h"
#include "list_view.h"
#include "search.h"

struct AppSpec {
	std::string directory;
};

class App {
public:
	App(const AppSpec& spec);
	~App();
	void run();
	void input();
	void update();
	void render();

	void parse_command();

	void open_file(const std::string& file_path);
	void save_file(const std::string& file_path);
	void new_file();

private:
	AppSpec m_spec;
	bool m_running = true;
	TextView m_text_view;
	TextInput m_text_input;
	ListView m_list_view;
	std::string m_file_path;
	Search m_search;
};