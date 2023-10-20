use crate::renderer::{Renderer, TextSpec};


pub struct TextView {

}

impl TextView {
    pub fn new() -> Self {
        Self {

        }
    }

    pub fn render(&self, renderer: &mut Renderer) {
        renderer.draw_text(TextSpec{
            text: "Hello, World!",
            ..Default::default()
        });
    }
}