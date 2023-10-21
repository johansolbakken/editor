use crate::{
    event::{Key, KeyEvent},
    renderer::Renderer,
    text_view::TextView,
};

pub struct App {
    text_view: TextView,
}

impl App {
    pub fn new() -> Self {
        let mut text_view = TextView::new();
        text_view.set_font_size(24.0);
        Self { text_view }
    }

    pub fn update(&mut self, renderer: &mut Renderer) {
        self.text_view.set_width(renderer.size().width as f64);
        self.text_view.set_height(renderer.size().height as f64);
        self.text_view.render(renderer);
    }

    pub fn character_event(&mut self, char: char) {
        let escape = '\u{1b}';
        let backspace = '\u{7f}';
        let tab = '\t';

        if char == '\n' || char == '\r' || char == escape || char == backspace || char == tab {
            return;
        }
        self.text_view.insert_char(char);
    }

    pub fn key_event(&mut self, event: KeyEvent) {
        match event {
            KeyEvent::Pressed(key) => match key {
                Key::Left => {
                    self.text_view.move_cursor_left();
                }
                Key::Right => {
                    self.text_view.move_cursor_right();
                }
                Key::Up => {
                    self.text_view.move_cursor_up();
                }
                Key::Down => {
                    self.text_view.move_cursor_down();
                }
                Key::Backspace => {
                    self.text_view.delete_char();
                }
                Key::Enter => {
                    self.text_view.insert_enter();
                }
                _ => {}
            },
            KeyEvent::Released(key) => {}
        }
    }
}
