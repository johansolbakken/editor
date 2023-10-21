
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Key {
    Up, Down, Left, Right
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum KeyEvent {
    Pressed(Key),
    Released(Key),
}