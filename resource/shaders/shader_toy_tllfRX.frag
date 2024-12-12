#version 330 core

uniform vec2 iResolution; // 注意：这里将 vec3 改为 vec2，因为 C++ 只传递了宽和高
uniform float iTime;      // 时间变量 (秒)
uniform mat4 model;       // 模型矩阵
uniform mat4 view;        // 视图矩阵
uniform mat4 projection;  // 投影矩阵

out vec4 fragColor;

#define NUM_LAYERS 8.0
#define TAU 6.28318
#define PI 3.141592
#define Velocity 0.025
#define StarGlow 0.025
#define CanvasView 20.0

// 星星光晕
float Star(vec2 uv, float flare) {
    float d = length(uv);
    float m = sin(StarGlow * 1.2) / d;  
    float rays = max(0.0, 0.5 - abs(uv.x * uv.y * 1000.0)); 
    m += (rays * flare) * 2.0;
    m *= smoothstep(1.0, 0.1, d);
    return m;
}

// 简单哈希函数，用于生成随机值
float Hash21(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

// 星星层
vec3 StarLayer(vec2 uv) {
    vec3 col = vec3(0);
    vec2 gv = fract(uv);
    vec2 id = floor(uv);
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            vec2 offs = vec2(x, y);
            float n = Hash21(id + offs);
            float size = fract(n);
            float star = Star(gv - offs - vec2(n, fract(n * 34.0)) + 0.5, 
                              smoothstep(0.1, 0.9, size) * 0.46);
            vec3 color = sin(vec3(0.2, 0.3, 0.9) * fract(n * 2345.2) * TAU) * 0.25 + 0.75;
            color *= vec3(0.9, 0.59, 0.9 + size);
            star *= sin(iTime * 0.6 + n * TAU) * 0.5 + 0.5;
            col += star * size * color;
        }
    }
    return col;
}

// 主函数
void main() {
    // 使用 iResolution.x 和 iResolution.y
    vec2 uv = (gl_FragCoord.xy - 0.5 * iResolution) / iResolution.y;

    // 动态偏移，简化了 M 的处理
    vec2 M = vec2(sin(iTime * 0.22), cos(iTime * 0.22));

    // 时间影响
    float t = iTime * Velocity; 
    vec3 col = vec3(0);  

    // 多层渲染星星
    for (float i = 0.0; i < 1.0; i += 1.0 / NUM_LAYERS) {
        float depth = fract(i + t);
        float scale = mix(CanvasView, 0.5, depth);
        float fade = depth * smoothstep(1.0, 0.9, depth);
        col += StarLayer(uv * scale + i * 453.2 - iTime * 0.05 + M) * fade;
    }

    // 输出颜色
    fragColor = vec4(col, 1.0);
}