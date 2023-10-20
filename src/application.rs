use crate::{renderer::Renderer, text_view::TextView};

pub struct App {
    text_view: TextView,
    text_buffer: String,
}

impl App {
    pub fn new() -> Self {
        Self {
            text_view : TextView::new(),
            text_buffer: String::from(""),
        }
    }

    pub fn update(&mut self, renderer: &mut Renderer) {
        self.text_view.set_width(renderer.size().width as f64);
        self.text_view.set_height(renderer.size().height as f64);
        self.text_view.set_text(self.text_buffer.clone());
        self.text_view.render(renderer);
    }

    pub fn on_key_event(&mut self, key: winit::event::VirtualKeyCode, pressed: bool) {
        if pressed {
            match key {
                winit::event::VirtualKeyCode::Back => {
                    self.text_buffer.pop();
                }
                winit::event::VirtualKeyCode::Return => {
                    self.text_buffer.push('\n');
                }
                _ => {
                    let key_char = virtual_key_code_to_string(key);
                    if key_char != "Unknown" {
                        self.text_buffer.push_str(key_char);
                    }
                }
            }
        }
    }
}

fn virtual_key_code_to_string(key: winit::event::VirtualKeyCode) -> &'static str {
    match key {
        winit::event::VirtualKeyCode::Key1 => "1",
        winit::event::VirtualKeyCode::Key2 => "2",
        winit::event::VirtualKeyCode::Key3 => "3",
        winit::event::VirtualKeyCode::Key4 => "4",
        winit::event::VirtualKeyCode::Key5 => "5",
        winit::event::VirtualKeyCode::Key6 => "6",
        winit::event::VirtualKeyCode::Key7 => "7",
        winit::event::VirtualKeyCode::Key8 => "8",
        winit::event::VirtualKeyCode::Key9 => "9",
        winit::event::VirtualKeyCode::Key0 => "0",

        winit::event::VirtualKeyCode::A => "A",
        winit::event::VirtualKeyCode::B => "B",
        winit::event::VirtualKeyCode::C => "C",
        winit::event::VirtualKeyCode::D => "D",
        winit::event::VirtualKeyCode::E => "E",
        winit::event::VirtualKeyCode::F => "F",
        winit::event::VirtualKeyCode::G => "G",
        winit::event::VirtualKeyCode::H => "H",
        winit::event::VirtualKeyCode::I => "I",
        winit::event::VirtualKeyCode::J => "J",
        winit::event::VirtualKeyCode::K => "K",
        winit::event::VirtualKeyCode::L => "L",
        winit::event::VirtualKeyCode::M => "M",
        winit::event::VirtualKeyCode::N => "N",
        winit::event::VirtualKeyCode::O => "O",
        winit::event::VirtualKeyCode::P => "P",
        winit::event::VirtualKeyCode::Q => "Q",
        winit::event::VirtualKeyCode::R => "R",
        winit::event::VirtualKeyCode::S => "S",
        winit::event::VirtualKeyCode::T => "T",
        winit::event::VirtualKeyCode::U => "U",
        winit::event::VirtualKeyCode::V => "V",
        winit::event::VirtualKeyCode::W => "W",
        winit::event::VirtualKeyCode::X => "X",
        winit::event::VirtualKeyCode::Y => "Y",
        winit::event::VirtualKeyCode::Z => "Z",

        winit::event::VirtualKeyCode::Apostrophe => "'",
        winit::event::VirtualKeyCode::Backslash => "\\",
        winit::event::VirtualKeyCode::Comma => ",",
        winit::event::VirtualKeyCode::Equals => "=",
        winit::event::VirtualKeyCode::Grave => "`",
        winit::event::VirtualKeyCode::LBracket => "[",
        winit::event::VirtualKeyCode::Minus => "-",
        winit::event::VirtualKeyCode::Period => ".",
        winit::event::VirtualKeyCode::Plus => "+",
        winit::event::VirtualKeyCode::RBracket => "]",
        winit::event::VirtualKeyCode::Semicolon => ";",
        winit::event::VirtualKeyCode::Slash => "/",
        winit::event::VirtualKeyCode::NumpadSubtract => "-",

        winit::event::VirtualKeyCode::Space => " ",

        _ => "Unknown",
    }
}
