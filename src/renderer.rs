use glyph_brush::{Section, Text};
use glyph_brush_layout::ab_glyph::FontArc;
use wgpu::{util::DeviceExt, Backends, CommandEncoder, Instance, TextureView};

// lib.rs
#[repr(C)]
#[derive(Copy, Clone, Debug, bytemuck::Pod, bytemuck::Zeroable)]
struct Vertex {
    position: [f32; 3],
}

impl Vertex {
    fn desc() -> wgpu::VertexBufferLayout<'static> {
        wgpu::VertexBufferLayout {
            array_stride: std::mem::size_of::<Vertex>() as wgpu::BufferAddress,
            step_mode: wgpu::VertexStepMode::Vertex,
            attributes: &[wgpu::VertexAttribute {
                offset: 0,
                shader_location: 0,
                format: wgpu::VertexFormat::Float32x3,
            }],
        }
    }
}

pub struct Renderer {
    instance: wgpu::Instance,
    surface: wgpu::Surface,
    surface_format: wgpu::TextureFormat,
    config: wgpu::SurfaceConfiguration,
    adapter: wgpu::Adapter,
    device: wgpu::Device,
    queue: wgpu::Queue,
    size: winit::dpi::PhysicalSize<u32>,
    render_pipeline: wgpu::RenderPipeline,
    vertex_buffer: wgpu::Buffer,
    text_brush: Option<wgpu_glyph::GlyphBrush<()>>,
    dpi_factor: f64,
}

#[derive(Debug)]
pub struct TextSpec {
    pub screen_position: (f32, f32),
    pub color: [f32; 4],
    pub scale: f32,
    pub text: String,
    pub bounds: (f32, f32),
}

impl Default for TextSpec {
    fn default() -> Self {
        Self {
            screen_position: (0.0, 0.0),
            color: [1.0, 1.0, 1.0, 1.0],
            scale: 16.0,
            text: String::from(""),
            bounds: (0.0, 0.0),
        }
    }
}

impl Renderer {
    pub fn new(window: &winit::window::Window) -> Self {
        // Initialize WGPU
        let instance = Instance::new(wgpu::InstanceDescriptor {
            backends: Backends::PRIMARY,
            ..Default::default()
        });
        let surface = unsafe { instance.create_surface(window) }.unwrap();
        let adapter = pollster::block_on(instance.request_adapter(&wgpu::RequestAdapterOptions {
            power_preference: wgpu::PowerPreference::LowPower, // or HighPerformance
            compatible_surface: Some(&surface),
            force_fallback_adapter: false, // Adjust as per your needs
        }))
        .expect("Failed to find a suitable GPU adapter");

        let (device, queue) = pollster::block_on(adapter.request_device(
            &wgpu::DeviceDescriptor {
                label: None,
                features: wgpu::Features::empty(),
                limits: wgpu::Limits::default(),
            },
            None,
        ))
        .expect("Failed to create device");

        let size = window.inner_size();
        let surface_caps = surface.get_capabilities(&adapter);
        // Shader code in this tutorial assumes an sRGB surface texture. Using a different
        // one will result all the colors coming out darker. If you want to support non
        // sRGB surfaces, you'll need to account for that when drawing to the frame.
        let surface_format = surface_caps
            .formats
            .iter()
            .copied()
            .find(|f| f.is_srgb())
            .unwrap_or(surface_caps.formats[0]);
        let config = wgpu::SurfaceConfiguration {
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
            format: surface_format,
            width: size.width,
            height: size.height,
            present_mode: surface_caps.present_modes[0],
            alpha_mode: surface_caps.alpha_modes[0],
            view_formats: vec![],
        };
        surface.configure(&device, &config);

        let shader = device.create_shader_module(wgpu::include_wgsl!("shader.wgsl"));

        let render_pipeline_layout =
            device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some("Render Pipeline Layout"),
                bind_group_layouts: &[],
                push_constant_ranges: &[],
            });

        let render_pipeline = device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
            label: Some("Render Pipeline"),
            layout: Some(&render_pipeline_layout),
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: "vs_main", // 1.
                buffers: &[
                    Vertex::desc(), // 2.
                ], // 2.
            },
            fragment: Some(wgpu::FragmentState {
                // 3.
                module: &shader,
                entry_point: "fs_main",
                targets: &[Some(wgpu::ColorTargetState {
                    // 4.
                    format: config.format,
                    blend: Some(wgpu::BlendState::REPLACE),
                    write_mask: wgpu::ColorWrites::ALL,
                })],
            }),
            primitive: wgpu::PrimitiveState {
                topology: wgpu::PrimitiveTopology::TriangleList, // 1.
                strip_index_format: None,
                front_face: wgpu::FrontFace::Ccw, // 2.
                cull_mode: Some(wgpu::Face::Back),
                // Setting this to anything other than Fill requires Features::NON_FILL_POLYGON_MODE
                polygon_mode: wgpu::PolygonMode::Fill,
                // Requires Features::DEPTH_CLIP_CONTROL
                unclipped_depth: false,
                // Requires Features::CONSERVATIVE_RASTERIZATION
                conservative: false,
            },
            depth_stencil: None, // 1.
            multisample: wgpu::MultisampleState {
                count: 1,                         // 2.
                mask: !0,                         // 3.
                alpha_to_coverage_enabled: false, // 4.
            },
            multiview: None, // 5.
        });

        // lib.rs
        const VERTEX_DATA: &[Vertex] = &[
            Vertex {
                position: [-0.5, -0.5, 0.0],
            },
            Vertex {
                position: [0.5, -0.5, 0.0],
            },
            Vertex {
                position: [0.0, 0.5, 0.0],
            },
        ];

        let vertex_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Vertex Buffer"),
            contents: bytemuck::cast_slice(VERTEX_DATA),
            usage: wgpu::BufferUsages::VERTEX,
        });

        let dpi_factor = window.scale_factor();

        Self {
            instance,
            surface,
            surface_format,
            config,
            adapter,
            device,
            queue,
            size,
            render_pipeline,
            vertex_buffer,
            text_brush: None,
            dpi_factor,
        }
    }

    pub fn device(&self) -> &wgpu::Device {
        &self.device
    }

    pub fn queue(&self) -> &wgpu::Queue {
        &self.queue
    }

    pub fn surface_format(&self) -> wgpu::TextureFormat {
        self.surface_format
    }

    pub fn surface(&self) -> &wgpu::Surface {
        &self.surface
    }

    pub fn render_pipeline(&self) -> &wgpu::RenderPipeline {
        &self.render_pipeline
    }

    pub fn vertex_buffer(&self) -> &wgpu::Buffer {
        &self.vertex_buffer
    }

    pub fn resize(&mut self, new_size: winit::dpi::PhysicalSize<u32>) {
        self.size = new_size;
        let config = wgpu::SurfaceConfiguration {
            width: self.size.width,
            height: self.size.height,
            ..self.config.clone()
        };
        self.surface.configure(&self.device, &self.config);
        self.config = config;
    }

    pub fn init_font(&mut self, font: FontArc) {
        let glyph_brush = wgpu_glyph::GlyphBrushBuilder::using_font(font)
            .build(&self.device, self.surface_format);
        self.text_brush = Some(glyph_brush);
    }

    pub fn draw_text(&mut self, text_spec: TextSpec) {
        match &mut self.text_brush {
            Some(text_brush) => {
                text_brush.queue(Section {
                    screen_position: text_spec.screen_position,
                    bounds: text_spec.bounds,
                    text: vec![Text::new(text_spec.text.as_str())
                        .with_color(text_spec.color)
                        .with_scale(text_spec.scale)],
                    ..Section::default()
                });
            }
            None => {
                println!("[WARNING] No text brush set");
            }
        }
    }

    pub fn set_text_brush(&mut self, text_brush: wgpu_glyph::GlyphBrush<()>) {
        self.text_brush = Some(text_brush);
    }

    pub fn flush_text(&mut self, view: &TextureView, size: winit::dpi::PhysicalSize<u32>, mut staging_belt: &mut wgpu::util::StagingBelt, mut encoder: &mut CommandEncoder) {
        match &mut self.text_brush {
            Some(text_brush) => {
                text_brush.draw_queued(
                    &self.device,
                    &mut staging_belt,
                    &mut encoder,
                    &view,
                    size.width,
                    size.height,
                )
                .unwrap();
            }
            None => {
                println!("[WARNING] No text brush set");
            }
        }
    }

    pub fn dpi_factor(&self) -> f64 {
        self.dpi_factor
    }

    pub fn size(&self) -> winit::dpi::PhysicalSize<u32> {
        self.size
    }
}
