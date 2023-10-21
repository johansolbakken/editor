use crate::renderer::{RectSpec, Renderer, TextSpec};

pub struct TextView {
    text: String,
    font_size: f64,
    x: f64,
    y: f64,
    width: f64,
    height: f64,
    cursor: usize,
}

impl TextView {
    pub fn new() -> Self {
        Self {
            text: String::from(""),
            font_size: 12.0,
            x: 0.0,
            y: 0.0,
            width: 0.0,
            height: 0.0,
            cursor: 0,
        }
    }

    pub fn render(&self, renderer: &mut Renderer) {
        let text = self.text.clone();
        let scale = self.font_size * renderer.dpi_factor();
        let screen_position = (self.x as f32, self.y as f32);
        let bounds = (self.width as f32, self.height as f32);
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
        if !cursor_blink_state {
            return;
        }

        // cursor
        let cursor_x = self.x + renderer.text_width(&self.text[..self.cursor], scale as f32) as f64 - 9.0;
        let cursor_y = self.y;
        renderer.draw_text(TextSpec {
            text: String::from("|"),
            scale: scale as f32,
            screen_position: (cursor_x as f32, cursor_y as f32),
            bounds,
            ..Default::default()
        });
    }

    pub fn set_text(&mut self, text: String) {
        self.text = text;
    }

    pub fn set_x(&mut self, x: f64) {
        self.x = x;
    }

    pub fn set_y(&mut self, y: f64) {
        self.y = y;
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

    pub fn set_cursor(&mut self, cursor: usize) {
        self.cursor = cursor;
    }

    pub fn cursor(&self) -> usize {
        self.cursor
    }
}
