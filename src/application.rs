use crate::{renderer::Renderer, text_view::TextView, event::KeyEvent};

pub struct App {
    text_view: TextView,
}

impl App {
    pub fn new() -> Self {
        let mut text_view = TextView::new();
        text_view.set_font_size(24.0);
        Self {
            text_view,
        }
    }

    pub fn update(&mut self, renderer: &mut Renderer) {
        self.text_view.set_width(renderer.size().width as f64);
        self.text_view.set_height(renderer.size().height as f64);
        self.text_view.render(renderer);
    }

    pub fn character_event(&mut self, char: char) {
        self.text_view.insert_char(char);
    }

    pub fn key_event(&mut self, event: KeyEvent) {
        
    }
}