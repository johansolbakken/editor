
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Key {
    Up, Down, Left, Right,
    Enter, Backspace, Escape,
    Shift, Ctrl, Super, Alt,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum KeyEvent {
    Pressed(Key),
    Released(Key),
}