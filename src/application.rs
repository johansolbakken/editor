use crate::{
    event::{Key, KeyEvent},
    renderer::Renderer,
    text_view::TextView,
};

pub struct App {
    text_view: TextView,
    super_pressed: bool,
    current_file : String,
}

impl App {
    pub fn new() -> Self {
        let mut text_view = TextView::new();
        text_view.set_font_size(24.0);
        Self { text_view, super_pressed: false, current_file: String::new() }
    }

    pub fn update(&mut self, renderer: &mut Renderer) {
        self.text_view.set_width(renderer.size().width as f64);
        self.text_view.set_height(renderer.size().height as f64);
        self.text_view.render(renderer);
    }

    pub fn character_event(&mut self, char: char) {
        if self.super_pressed {
            match char {
                'n' => {
                    self.new_file();
                }
                's' => {
                    if self.current_file == "" {
                        self.prompt_for_file();
                    }

                    self.save();
                }
                'o' => {
                    self.open();
                }
                _ => {}
            }
            return;
        }

        let escape = '\u{1b}';
        let backspace = '\u{7f}';
        let tab = '\t';

        if char == '\n' || char == '\r' || char == escape || char == backspace || char == tab {
            return;
        }
        self.text_view.insert_char(char);
    }

    fn new_file(&mut self) {
        self.text_view.set_text(String::new());
        self.prompt_for_file();
    }

    fn save(&self) {
        let text = self.text_view.text();
        std::fs::write(self.current_file.clone(), text).unwrap();
    }

    fn open(&mut self) {
        self.prompt_for_file();

        if std::fs::metadata(self.current_file.clone()).is_err() {
            println!("File does not exist");
            self.current_file = String::new();
            return;
        }

        let text = std::fs::read_to_string(self.current_file.clone()).unwrap();
        self.text_view.set_text(text);
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
                Key::Super => {
                    self.super_pressed = true;
                }
                _ => {}
            },
            KeyEvent::Released(key) => {
                match key {
                    Key::Super => {
                        self.super_pressed = false;
                    }
                    _ => {}
                }
            }
        }
    }

    pub fn prompt_for_file(&mut self) {
        println!("Enter file name: ");

        let mut file = String::new();
        std::io::stdin().read_line(&mut file).unwrap();

        self.current_file = file;
    }

    pub fn get_current_file(&self) -> String {
        self.current_file.clone()
    }
}
