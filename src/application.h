#pragma once

#include "text_view.h"

class App {
public:
	void input();
    void run();

private:
	bool m_running = true;
	TextView m_text_view;
};