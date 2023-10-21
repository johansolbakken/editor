// Define the push constants block
struct PushConstants {
    mvp: mat4x4<f32>, // Model-View-Projection matrix
    color: vec4<f32>, // Color
};

@group(0) @binding(0) var<uniform> pushConstants: PushConstants;

struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
};

@vertex
fn vs_main(
    @builtin(vertex_index) in_vertex_index: u32,
) -> VertexOutput {
    var out: VertexOutput;
    let x = f32(1 - i32(in_vertex_index)) * 0.5;
    let y = f32(i32(in_vertex_index & 1u) * 2 - 1) * 0.5;
    let vertex_position = vec4<f32>(x, y, 0.0, 1.0);

    // Apply the Model-View-Projection matrix to the vertex position
    out.clip_position = pushConstants.mvp * vertex_position;
    
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    // Use the color from the push constants
    return pushConstants.color;
}
