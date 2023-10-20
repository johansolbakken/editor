use crate::{renderer::Renderer, text_view::TextView};


pub struct App {
    text_view: TextView,
}

impl App {
    pub fn new() -> Self {
        Self {
            text_view: TextView::new(),
        }
    }

    pub fn update(&mut self, renderer: &mut Renderer) {
        self.text_view.render(renderer);
    }
}