use event::Key;
use winit::{
    event::{Event, VirtualKeyCode, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
    window::WindowBuilder,
};

mod application;
mod event;
mod renderer;
mod text_view;

fn main() {
    // Initialize the event loop
    let event_loop = EventLoop::new();

    // Create a window
    let window = WindowBuilder::new()
        .with_title("WGPU Text Editor")
        .with_inner_size(winit::dpi::LogicalSize::new(1024, 768))
        .build(&event_loop)
        .unwrap();

    let mut renderer = renderer::Renderer::new(&window);
    let size = window.inner_size();

    renderer.init_font();

    let mut app = application::App::new();

    // Main event loop
    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Wait;

        match event {
            Event::MainEventsCleared => {
                window.request_redraw();
            }
            Event::WindowEvent {
                event: WindowEvent::ReceivedCharacter(char),
                ..
            } => {
                app.character_event(char);
            }
            Event::WindowEvent {
                event: WindowEvent::MouseInput { state, button, .. },
                ..
            } => {
                let pos = match button {
                    winit::event::MouseButton::Left => {
                        let pos = winit::dpi::PhysicalPosition::new(0.0, 0.0);
                        let pos = pos.to_logical(window.scale_factor());
                        pos
                    }
                    _ => {
                        return;
                    }
                };
                let pressed = match state {
                    winit::event::ElementState::Pressed => {
                        true
                    }
                    winit::event::ElementState::Released => {
                        false
                    }
                };
                app.mouse_event(pos.x, pos.y, pressed);
            }

            Event::WindowEvent {
                event: WindowEvent::KeyboardInput { input, .. },
                ..
            } => {
                let key = match input.virtual_keycode {
                    Some(VirtualKeyCode::Right) => Some(Key::Right),
                    Some(VirtualKeyCode::Left) => Some(Key::Left),
                    Some(VirtualKeyCode::Up) => Some(Key::Up),
                    Some(VirtualKeyCode::Down) => Some(Key::Down),

                    Some(VirtualKeyCode::Back) => Some(Key::Backspace),
                    Some(VirtualKeyCode::Return) => Some(Key::Enter),
                    Some(VirtualKeyCode::Escape) => Some(Key::Escape),

                    Some(VirtualKeyCode::LShift) | Some(VirtualKeyCode::RShift) => Some(Key::Shift),
                    Some(VirtualKeyCode::LControl) | Some(VirtualKeyCode::RControl) => { Some(Key::Ctrl) }
                    Some(VirtualKeyCode::LAlt) | Some(VirtualKeyCode::RAlt) => Some(Key::Alt),
                    Some(VirtualKeyCode::LWin) | Some(VirtualKeyCode::RWin) => Some(Key::Super),
                    _ => None,
                };
                match key {
                    Some(key) => match input.state {
                        winit::event::ElementState::Pressed => {
                            app.key_event(event::KeyEvent::Pressed(key));
                        }
                        winit::event::ElementState::Released => {
                            app.key_event(event::KeyEvent::Released(key));
                        }
                    },
                    None => {}
                }
            }
            Event::WindowEvent {
                event: WindowEvent::MouseWheel { delta, .. },
                ..
            } => {
                let (x, y) = match delta {
                    winit::event::MouseScrollDelta::LineDelta(x, y) => (x as f64, y as f64),
                    winit::event::MouseScrollDelta::PixelDelta(pos) => (pos.x as f64, pos.y as f64),
                };
                app.scroll_event(x, y);
            }
            Event::RedrawRequested(window_id) if window_id == window.id() => {
                let output = renderer.surface().get_current_texture().unwrap();
                let view = output
                    .texture
                    .create_view(&wgpu::TextureViewDescriptor::default());
                let mut encoder =
                    renderer
                        .device()
                        .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                            label: Some("Render Encoder"),
                        });
                {
                    let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                        label: Some("Render Pass"),
                        color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                            view: &view,
                            resolve_target: None,
                            ops: wgpu::Operations {
                                load: wgpu::LoadOp::Clear(wgpu::Color {
                                    r: 0.0,
                                    g: 0.0,
                                    b: 0.0,
                                    a: 1.0,
                                }),
                                store: true,
                            },
                        })],
                        depth_stencil_attachment: None,
                    });

                    render_pass.set_pipeline(renderer.render_pipeline());
                    //render_pass.set_vertex_buffer(0, renderer.vertex_buffer().slice(..));
                    //render_pass.draw(0..3, 0..1);
                }

                app.update(&mut renderer);

                let current_file = app.get_current_file();
                window.set_title(format!("Text Editor - {}", current_file).as_str());

                // End render pass
                renderer.flush_text(&view, size, &mut encoder);
                
                //renderer.flush_rects(&view,&mut encoder);
                renderer.queue().submit(std::iter::once(encoder.finish()));
                output.present();
            }
            Event::WindowEvent {
                event: WindowEvent::Resized(new_size),
                ..
            } => {
                renderer.resize(new_size);
            },
            Event::WindowEvent {
                event: WindowEvent::CloseRequested,
                ..
            } => *control_flow = ControlFlow::Exit,

            _ => {}
        }
    });
}
