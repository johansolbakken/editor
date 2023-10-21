use winit::{
    event::{Event, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
    window::WindowBuilder,
};

mod renderer;
mod application;
mod text_view;

fn main() {
    // Initialize the event loop
    let event_loop = EventLoop::new();

    // Create a window
    let window = WindowBuilder::new()
        .with_title("WGPU Text Editor")
        .build(&event_loop)
        .unwrap();

    let mut renderer = renderer::Renderer::new(&window);
    let size = window.inner_size();

    let mut staging_belt = wgpu::util::StagingBelt::new(1024);

    let cascadia: &[u8] = include_bytes!("font/Cascadia.ttf");
    let font = wgpu_glyph::ab_glyph::FontArc::try_from_slice(cascadia).unwrap();
    renderer.init_font(font.clone());
    let glyph_brush = wgpu_glyph::GlyphBrushBuilder::using_font(font)
        .build(renderer.device(), renderer.surface_format());
    renderer.set_text_brush(glyph_brush);

    let mut app = application::App::new();

    // Main event loop
    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Wait;

        match event {
            Event::MainEventsCleared => {
                window.request_redraw();
            }
            Event::WindowEvent {  event: WindowEvent::ReceivedCharacter(char),.. } => {
                app.character_event(char);
            },
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

                // End render pass
                renderer.flush_text(&view, size, &mut staging_belt, &mut encoder);
                staging_belt.finish();
                //renderer.flush_rects(&view,&mut encoder);
                renderer.queue().submit(std::iter::once(encoder.finish()));
                output.present();
            }
            Event::WindowEvent {
                event: WindowEvent::Resized(new_size),
                ..
            } => renderer.resize(new_size),
            Event::WindowEvent {
                event: WindowEvent::CloseRequested,
                ..
            } => *control_flow = ControlFlow::Exit,

            _ => {}
        }
    });
}
