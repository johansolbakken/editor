#pragma once

#include <vector>
#include <string>

class TextView {
public:
    void render();
    void update();

    std::string text() const;
    void set_width(float width) { m_width = width; }
    void set_height(float height) { m_height = height; }

/*
    pub fn insert_char(&mut self, c: char);
    pub fn move_cursor_left(&mut self);
    pub fn move_cursor_right(&mut self);
    pub fn move_cursor_up(&mut self);
    pub fn move_cursor_down(&mut self);
    pub fn delete_char(&mut self);
    pub fn insert_enter(&mut self);
    pub fn text(&self) -> String;
    pub fn set_text(&mut self, text: String);
    pub fn set_focused(&mut self, focused: bool);
    pub fn scroll_horizontal(&mut self, delta: f64);
    pub fn scroll_vertical(&mut self, delta: f64);
    */

private:
    std::vector<std::string> m_text;
    float m_x = 0;
    float m_y = 0;
    float m_width = 0;
    float m_height = 0;

    /*s
    x: 0.0,
            y: 0.0,
            width: 0.0,
            height: 0.0,
            cursor: 0,
            line: 0,
            focused: false,
            scroll_x: 0.0,
            scroll_y: 0.0,
    */
};