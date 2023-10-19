use wgpu::{Backends, Instance};
use winit::{
    event::{Event, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
    window::WindowBuilder,
};

fn main() {
    // Initialize the event loop
    let event_loop = EventLoop::new();

    // Create a window
    let window = WindowBuilder::new()
        .with_title("WGPU Text Editor")
        .build(&event_loop)
        .unwrap();

    // Initialize WGPU
    let instance = Instance::new(wgpu::InstanceDescriptor {
        backends: Backends::PRIMARY,
        ..Default::default()
    });
    let surface = unsafe { instance.create_surface(&window) }.unwrap();
    let adapter = pollster::block_on(instance.request_adapter(&wgpu::RequestAdapterOptions {
        power_preference: wgpu::PowerPreference::LowPower, // or HighPerformance
        compatible_surface: Some(&surface),
        force_fallback_adapter: false, // Adjust as per your needs
    }))
    .expect("Failed to find a suitable GPU adapter");

    // Main event loop
    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Wait;

        match event {
            Event::WindowEvent {
                event: WindowEvent::CloseRequested,
                ..
            } => *control_flow = ControlFlow::Exit,
            _ => {}
        }
    });
}
