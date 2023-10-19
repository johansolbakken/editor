use glyph_brush::{Section, Text};
use winit::{
    event::{Event, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
    window::WindowBuilder,
};

mod renderer;

fn main() {
    // Initialize the event loop
    let event_loop = EventLoop::new();

    // Create a window
    let window = WindowBuilder::new()
        .with_title("WGPU Text Editor")
        .build(&event_loop)
        .unwrap();

    let mut renderer = renderer::Renderer::new(&window);
    let mut size = window.inner_size();

    let mut staging_belt = wgpu::util::StagingBelt::new(1024);

    let cascadia: &[u8] = include_bytes!("font/Cascadia.ttf");
    let font = wgpu_glyph::ab_glyph::FontArc::try_from_slice(cascadia).unwrap();
    let mut glyph_brush =
        wgpu_glyph::GlyphBrushBuilder::using_font(font).build(renderer.device(), renderer.surface_format());

    // Main event loop
    event_loop.run(move |event, _, control_flow| {
        *control_flow = ControlFlow::Wait;

        match event {
            Event::MainEventsCleared => {
                window.request_redraw();
            }
            Event::RedrawRequested(window_id) if window_id == window.id() => {
                let output = renderer.surface().get_current_texture().unwrap();
                let view = output
                    .texture
                    .create_view(&wgpu::TextureViewDescriptor::default());
                let mut encoder = renderer.device().create_command_encoder(&wgpu::CommandEncoderDescriptor {
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
                                    r: 0.1,
                                    g: 0.2,
                                    b: 0.3,
                                    a: 1.0,
                                }),
                                store: true,
                            },
                        })],
                        depth_stencil_attachment: None,
                    });

                    render_pass.set_pipeline(renderer.render_pipeline());
                    render_pass.set_vertex_buffer(0, renderer.vertex_buffer().slice(..));
                    render_pass.draw(0..3, 0..1);
                }

                glyph_brush.queue(Section {
                    screen_position: (30.0, 30.0),
                    bounds: (size.width as f32, size.height as f32),
                    text: vec![Text::new("Hello wgpu_glyph!")
                        .with_color([0.0, 0.0, 0.0, 1.0])
                        .with_scale(40.0)],
                    ..Section::default()
                });

                glyph_brush.queue(Section {
                    screen_position: (30.0, 90.0),
                    bounds: (size.width as f32, size.height as f32),
                    text: vec![Text::new("Hello wgpu_glyph!")
                        .with_color([1.0, 1.0, 1.0, 1.0])
                        .with_scale(40.0)],
                    ..Section::default()
                });

                // Draw the text
                glyph_brush
                    .draw_queued(
                        renderer.device(),
                        &mut staging_belt,
                        &mut encoder,
                        &view,
                        size.width,
                        size.height,
                    )
                    .unwrap();

                // Remember to reset the staging belt at the end of the frame
                staging_belt.finish();

                // submit will accept anything that implements IntoIter
                renderer.queue().submit(std::iter::once(encoder.finish()));
                output.present();
            }
            Event::WindowEvent {
                event: WindowEvent::Resized(new_size),
                ..
            } => {
                size = new_size;
                let config = wgpu::SurfaceConfiguration {
                    width: size.width,
                    height: size.height,
                    ..renderer.surface_config().clone()
                };
                renderer.surface().configure(renderer.device(), &config);
            }
            Event::WindowEvent {
                event: WindowEvent::CloseRequested,
                ..
            } => *control_flow = ControlFlow::Exit,

            _ => {}
        }
    });
}
