use crate::renderer::{Renderer, TextSpec};


pub struct TextView {
    text: String,
    font_size: f64,
}

impl TextView {
    pub fn new() -> Self {
        Self {
            text: String::from(""),
            font_size: 12.0,
        }
    }

    pub fn render(&self, renderer: &mut Renderer) {
        let text = self.text.clone();
        let scale = self.font_size * renderer.dpi_factor();
        renderer.draw_text(TextSpec{
            text,
            scale: scale as f32,
            ..Default::default()
        });
    }

    pub fn set_text(&mut self, text: String) {
        self.text = text;
    }
}