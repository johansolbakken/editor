use std::collections::LinkedList;

use crate::renderer::{Renderer, TextSpec};

pub struct TextView {
    text: LinkedList<String>,
    font_size: f64,
    x: f64,
    y: f64,
    width: f64,
    height: f64,
    cursor: usize,
    line: usize,
    focused: bool,
    scroll_x: f64,
    scroll_y: f64,
}

impl TextView {
    pub fn new() -> Self {
        let mut text = LinkedList::new();
        text.push_back(String::new());
        Self {
            text,
            font_size: 12.0,
            x: 0.0,
            y: 0.0,
            width: 0.0,
            height: 0.0,
            cursor: 0,
            line: 0,
            focused: false,
            scroll_x: 0.0,
            scroll_y: 0.0,
        }
    }

    pub fn render(&self, renderer: &mut Renderer) {
        let text = self.text();
        let scale = self.font_size * renderer.dpi_factor();
        let screen_position = (self.x as f32 - self.scroll_x as f32, self.y as f32 - self.scroll_y as f32);
        let bounds = (f32::MAX, f32::MAX);
        renderer.draw_text(TextSpec {
            text,
            scale: scale as f32,
            screen_position,
            bounds,
            ..Default::default()
        });

        let current_system_seconds = std::time::SystemTime::now()
            .duration_since(std::time::UNIX_EPOCH)
            .unwrap()
            .as_secs_f64();

        let cursor_blink_rate = 0.2;
        let cursor_blink_state = (current_system_seconds / cursor_blink_rate).sin() > 0.0;
        if !self.focused || !cursor_blink_state {
            return;
        }

        // cursor
        let text = &self.text.iter().nth(self.line).unwrap()[..self.cursor];
        let cursor_x = self.x
            + renderer.text_width(
                text,
                scale as f32,
            ) as f64
            - self.scroll_x
            - renderer.text_width("|", scale as f32) as f64 / 2.0;
        let cursor_y = self.y + self.line as f64 * scale;
        renderer.draw_text(TextSpec {
            text: String::from("|"),
            scale: scale as f32,
            screen_position: (cursor_x as f32, cursor_y as f32),
            bounds,
            ..Default::default()
        });
    }

    pub fn insert_char(&mut self, c: char) {
        if !self.focused {
            return;
        }
        let line = self.text.iter_mut().nth(self.line).unwrap();
        line.insert(self.cursor, c);
        self.cursor += 1;
    }

    pub fn set_width(&mut self, width: f64) {
        self.width = width;
    }

    pub fn set_height(&mut self, height: f64) {
        self.height = height;
    }

    pub fn set_font_size(&mut self, font_size: f64) {
        self.font_size = font_size;
    }

    pub fn move_cursor_left(&mut self) {
        if !self.focused {
            return;
        }
        if self.cursor == 0 {
            return;
        }
        self.cursor -= 1;
    }
    pub fn move_cursor_right(&mut self) {
        if !self.focused {
            return;
        }
        if self.cursor == self.text.iter().nth(self.line).unwrap().len() {
            return;
        }
        self.cursor += 1;
    }
    pub fn move_cursor_up(&mut self) {
        if !self.focused {
            return;
        }
        if self.line == 0 {
            return;
        }
        self.line -= 1;
        if self.cursor > self.text.iter().nth(self.line).unwrap().len() {
            self.cursor = self.text.iter().nth(self.line).unwrap().len();
        }
    }

    pub fn move_cursor_down(&mut self) {
        if !self.focused {
            return;
        }
        if self.line == self.text.len() - 1 {
            return;
        }

        self.line += 1;
        if self.cursor > self.text.iter().nth(self.line).unwrap().len() {
            self.cursor = self.text.iter().nth(self.line).unwrap().len();
        }
    }

    pub fn delete_char(&mut self) {
        if !self.focused {
            return;
        }
        if self.line == 0 && self.cursor == 0 {
            return;
        }

        if self.cursor == 0 {
            let mut new_text: LinkedList<String> = LinkedList::new();
            for (i, line) in self.text.iter().enumerate() {
                if i == self.line - 1 {
                    let mut new_line = String::new();
                    new_line.push_str(line);
                    new_line.push_str(&self.text.iter().nth(self.line).unwrap());
                    new_text.push_back(new_line);
                } else if i == self.line {
                    continue;
                } else {
                    new_text.push_back(line.clone());
                }
            }
            self.text = new_text;
            self.line -= 1;
            self.cursor = self.text.iter().nth(self.line).unwrap().len();
            return;
        }

        let mut new_text: LinkedList<String> = LinkedList::new();
        for (i, line) in self.text.iter().enumerate() {
            if i == self.line {
                let mut new_line = String::new();
                for (j, c) in line.chars().enumerate() {
                    if j == self.cursor - 1 {
                        continue;
                    }
                    new_line.push(c);
                }
                new_text.push_back(new_line);
            } else {
                new_text.push_back(line.clone());
            }
        }
        self.text = new_text;
        self.cursor -= 1;
    }

    pub fn insert_enter(&mut self) {
        if !self.focused {
            return;
        }
        if self.line == self.text.len() - 1 && self.cursor == self.text.iter().nth(self.line).unwrap().len() {
            self.text.push_back(String::new());
            self.line += 1;
            self.cursor = 0;
            return;
        }
        let mut new_text: LinkedList<String> = LinkedList::new();
        for (i, line) in self.text.iter_mut().enumerate() {
            if i == self.line {
                let mut new_line = String::new();
                let mut new_line2 = String::new();
                for (j, c) in line.chars().enumerate() {
                    if j < self.cursor {
                        new_line.push(c);
                    } else {
                        new_line2.push(c);
                    }
                }
                new_text.push_back(new_line);
                new_text.push_back(new_line2);
            } else {
                new_text.push_back(line.clone());
            }
        }
        self.text = new_text;
        self.line += 1;
        self.cursor = 0;
    }

    pub fn text(&self) -> String {
        let mut text = String::new();
        for (i,line) in self.text.iter().enumerate() {
            text.push_str(line);
            if i != self.text.len() - 1 {
                text.push_str("\n");
            }
        }
        text
    }

    pub fn set_text(&mut self, text: String) {
        self.text = LinkedList::new();
        let lines = text.split("\n");
        for line in lines {
            self.text.push_back(String::from(line));
        }
        self.line = 0;
        self.cursor = 0;
    }

    pub fn set_focused(&mut self, focused: bool) {
        self.focused = focused;
    }

    pub fn is_inside(&self, x: f64, y: f64) -> bool {
        if x < self.x {
            return false;
        }
        if x > self.x + self.width {
            return false;
        }
        if y < self.y {
            return false;
        }
        if y > self.y + self.height {
            return false;
        }
        true
    }

    pub fn scroll_horizontal(&mut self, delta: f64) {
        self.scroll_x += delta;
        if self.scroll_x < 0.0 {
            self.scroll_x = 0.0;
        }
    }

    pub fn scroll_vertical(&mut self, delta: f64) {
        self.scroll_y += delta;
        if self.scroll_y < 0.0 {
            self.scroll_y = 0.0;
        }
    }

    pub fn font_size(&self) -> f64 {
        self.font_size
    }

    pub fn set_x(&mut self, x: f64) {
        self.x = x;
    }

    pub fn set_y(&mut self, y: f64) {
        self.y = y;
    }
}
