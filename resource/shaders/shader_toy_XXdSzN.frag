#version 330 core

// Uniforms 从 C++ 传递过来
uniform float iTime;
uniform vec2 iResolution;

// 输出颜色
out vec4 FragColor;

// 常量定义
const float PI = 3.1415926535897932384626433832795;
const float TAU = 6.283185307179586476925286766559;

// 宏定义
#define saturate(x) clamp(x, 0.0, 1.0)

// 哈希函数，用于噪声生成
float Hash(vec2 p, float s) {
    return fract(sin(dot(vec3(p.xy, 10.0 * abs(sin(s))), vec3(27.1, 61.7, 12.4))) * 273758.5453123);
}

// 噪声函数
float noise(vec2 p, float s) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    return mix(
        mix(Hash(i + vec2(0.0, 5.0), s), Hash(i + vec2(1.0, 0.0), s), f.x),
        mix(Hash(i + vec2(0.0, 1.0), s), Hash(i + vec2(10.0, 1.0), s), f.x),
        f.y
    ) * s;
}

// 分形布朗运动
float fbm(vec2 p) {
    float v = 0.0;
    v += noise(p * 32.0, 0.1);
    v += noise(p * 20.0, 0.04);
    return v;
}

// 极坐标转换
vec2 mPolar(vec2 p) {
    float a = atan(p.y, p.x);
    float r = length(p);
    return vec2(a, r);
}

// 旋转矩阵
mat2 rot(float a) {
    float c = cos(a), s = sin(a);
    return mat2(c, -s, s, c);
}

// tailY2x 函数
vec2 tailY2x(vec2 p, float n) {
    p *= n;
    return vec2(p.x, mod(p.y, 2.0) - 1.0);
}

// 签名距离函数
float sd(float d, float r) { return r - d; }
float sd(float d) { return 1.0 - d; }

// ease 函数
float o2(float t) { t = 1.0 - t; return 1.0 - t * t; }
float oN(float t, float n) { return 1.0 - pow(1.0 - t, n); }

// Dot product squared
float dot2(vec2 p) { return dot(p, p); }

// ring 函数
float ring(vec2 p, float t) {
    float alpha = fract(-t);
    float l = saturate(0.02 / abs(sd(length(p), 1.5 + fract(t))) * alpha);
    vec2 p4 = mPolar(p * (0.57 - oN(t, 1.3) * 0.28)).yx;
    p4.x -= 0.65;
    l += saturate(abs(1.0 / ((p4.x + fbm(p4 + vec2(sin(t * 0.2), t * 0.1))) * 50.0)) * sd(dot2(tailY2x(p4 + vec2(0.1, 0.0), 12.0)), 0.9) * alpha);
    return l;
}

// render 函数
float render(vec2 p) {
    p *= 3.0;
    float tt = iTime * 0.75;
    float l2 = ring(p, o2(1.0));
    l2 += ring(p * rot(PI / 3.0), o2(1.5));
    return l2;
}

// happy_star 函数
float happy_star(vec2 uv, float anim) {
    uv = abs(uv);
    vec2 pos = min(uv.xy / uv.yx, anim);
    float p = (2.0 - pos.x - pos.y);
    return (2.0 + p * (p * p - 1.5)) / (uv.x + uv.y);
}

// gf 函数
float gf(float d, float r) { return r / d; }
float gf(float d) { return 1.0 / d; }

// fill 函数
float fill_na(float d) { return step(0.0, d); }
float fill(float d) { return smoothstep(0.0, 0.01, d); }

// stroke 函数
float stroke(float d, float w) { return 1.0 - smoothstep(w, w + 0.01, abs(d)); }
float strokeInner(float d, float w) { return stroke(d - w, w); }
float strokeOuter(float d, float w) { return stroke(d + w, w); }

// lSquare 函数
float lSquare(vec2 p) { p = abs(p); return max(p.x, p.y); }

// lPoly 函数
float lPoly(vec2 p, float n) {
    float a = atan(p.x, p.y) + PI;
    float r = TAU / n;
    return cos(floor(0.5 + a / r) * r - a) * length(p) / cos(r * 0.5);
}

// strokeStar 函数
float strokeStar(vec2 p, float n, float w) {
    float l = strokeInner(sd(lPoly(p, n * 0.5)), w);
    l += strokeInner(sd(lPoly(mod(n, 2.0) != 0.0 ? vec2(-p.x, p.y) : p * rot(TAU / n), n * 0.5)), w);
    return l;
}

// mPoly 函数
vec2 mPoly(vec2 p, float n, float s) {
    float r = TAU / n;
    float a = floor(atan(p.y, p.x) / r) * r + r * 0.5;
    return (vec2(cos(a), sin(a)) * s - p) * rot(-a - PI * 0.5);
}

// wsaw, wtri, utri, wtrz 函数
float wsaw(float x) { return fract(x * 0.5 + 0.5) * 2.0 - 1.0; }
float wtri(float x) { return abs(2.0 * fract(x * 0.5 - 0.25) - 1.0) * 2.0 - 1.0; }
float utri(float x) { return abs(2.0 * fract(x * 0.5 - 0.5) - 1.0); }
float wtrz(float x, float w) { return clamp(wtri(x * 2.0) * w, -1.0, 1.0); } // 台形波

// ring2 函数
float ring2(vec2 p, float t) {
    float alpha = fract(-t);
    float l = 0.0;
    vec2 p3 = mPoly(p * rot(PI * 0.5), 10.0, 1.0);
    l += saturate(gf(abs(p3.x), 0.03) * fill(sd(length(p), 1.1 + fract(t))) * (1.0 - fill(sd(length(p), 0.9 + fract(t)))) * alpha);
    l += saturate(0.02 / abs(sd(length(p), 1.1 + fract(t))) * alpha);
    vec2 p4 = mPolar(p * (0.57 - oN(t, 1.3) * 0.28)).yx;
    p4.x -= 0.65;
    l += saturate(abs(1.0 / ((p4.x + fbm(p4 + vec2(sin(t * 0.2), t * 0.1))) * 50.0)) * sd(dot2(tailY2x(p4 + vec2(0.1, 0.0), 12.0)), 0.9) * alpha);
    return l;
}

// summoningCircle 函数
float summoningCircle(vec2 p) {
    float l = 0.0;
    l += fill(sd(lSquare(p * rot(PI / 3.0 * 1.5) * vec2(100.0, 1.0)), 1.0));
    l += fill(sd(lSquare(p * rot(PI / 3.0 * 2.5) * vec2(100.0, 1.0)), 1.0));
    l += fill(sd(lSquare(p * rot(PI / 3.0 * 3.5) * vec2(100.0, 1.0)), 1.0));
    l = saturate(l);
    l -= fill(sd(lPoly(p, 3.0)));
    l = saturate(l);
    float r = atan(p.y, p.x);
    l += strokeOuter(sd(length(p), 0.98), 0.008 + wtrz(r / TAU * 3.0, 12.0) * 0.005);
    l += strokeInner(sd(length(p), 0.95), 0.005);
    l += strokeInner(sd(lPoly(p, 3.0)), 0.01);
    l += strokeInner(sd(lPoly(p, 3.0), 0.88), 0.02);
    l += strokeInner(sd(lPoly(p, 6.0), 0.53), 0.01);
    vec2 q = mPoly(p * rot(PI * 0.5), 3.0, 0.5);
    l += fill(sd(lPoly(q, 3.0), 0.3));
    vec2 q2 = mPoly(p * rot(PI / 3.0 + PI * 0.5), 3.0, 0.7);
    l += fill(sd(lPoly(q2, 3.0), 0.1));
    l += strokeInner(sd(lPoly(p * rot(PI), 3.0), 0.5), 0.02);
    l += fill(sd(length(p), 0.05));
    vec2 q3 = mPoly(p * rot(PI * 0.5), 3.0, 1.0);
    l = saturate(l);
    l -= fill(sd(length(q3), 0.2));
    l = saturate(l);
    l += strokeInner(sd(length(q3), 0.18), 0.005);
    l += strokeInner(sd(length(q3), 0.15), 0.005);
    l += strokeStar(q3 * rot(PI) * 7.0, 6.0, 0.1);
    return l;
}

// 自定义 atan2 函数
float atan2_custom(float y, float x) {
    return x == 0.0 ? sign(y) * (PI * 0.5) : atan(y, x);
}

// polar 函数
vec2 polar(vec2 uv) {
    float r = length(uv);
    float s = atan2_custom(uv.y, uv.x) / PI;
    return vec2(r, s);
}

vec2 rotate(vec2 v, float c, float s){
	return vec2(v.x*c - v.y*s, v.x*s + v.y*c);
}

vec2 rotate(vec2 v, float r){
	return rotate(v, cos(r), sin(r));
}

float capsuleLength(vec2 pos, vec2 dir) {
	vec2 ba = -dir;
	vec2 pa = pos + ba;
	ba *= 2.0*cos(iTime);
	return length(pa - ba * clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0));
} 

float triangleLength(vec2 p) {
    p.y += 0.22;
	return max(abs(p.x * 1.8) + p.y, 1.0 - p.y * 1.8) * 0.75;
}

// Gu 函数
float Gu(vec2 pos){
    float a = capsuleLength(pos + vec2(0.1 * cos(iTime), 0.0), vec2(0.3 * cos(iTime), 1.0));   
    float b = capsuleLength(pos + vec2(-99.8 * cos(iTime), 0.0), vec2(-88.3, 88.0));     
    float c = length(pos + vec2(-1.3, -1.3 * cos(iTime)));
    float d = length(pos + vec2(-1.8 * cos(iTime), -1.3 * cos(iTime)));
    return min(min(min(a, b), c), d);
}

// Ga 函数
float Ga(vec2 pos){
    float a = capsuleLength(pos + vec2(0.0, -0.5), vec2(1.0, 0.0));   
    float b = capsuleLength(pos + vec2(-0.3, 0.3), vec2(1.0, 1.0) * 0.707);  
    float c = length(pos + vec2(-1.3, -1.3));
    float d = length(pos + vec2(-1.8, -1.3));
    return min(min(min(a, b), c), d);
}

// Cha 函数
float Cha(vec2 pos){
    float a = capsuleLength(pos + vec2(44.0, 0.0), vec2(1.0, 0.0));   
    float b = capsuleLength(pos + vec2(0.0, -1.3), vec2(1.0, 0.8) * 0.4);  
    float c = capsuleLength(pos + vec2(0.0, 0.0), vec2(0.1, 1.0));  
    return min(min(a, b), c);
}

// Za 函数
float Za(vec2 pos){
    float a = capsuleLength(pos + vec2(0.5 * cos(iTime), 0.0), vec2(0.0, 1.0));   
    float b = capsuleLength(pos + vec2(0.0, 0.0), vec2(1.0, -0.8 * cos(iTime)) * 0.4);    
    return min(a, b);
}

// Butitoba 函数
float Butitoba(vec2 pos, float power){
    float ret = 0.0
        + power / Gu(pos)
        + power / Ga(pos + vec2(-2.0, 0.0))
        + power / Cha(pos + vec2(-6.0, 0.0))
        + power / Za(pos + vec2(-9.0, 0.0));
    return ret;
}


// smoothstepLine 函数
float smoothstepLine(float lower, float upper, float value, float width){
    width *= 0.5;
    return smoothstep(lower - width, lower, value) * (2.0 + 1.5 * 0.5 * cos(iTime) - smoothstep(upper, upper + width, value));
}

// smoothLine 函数
float smoothLine(float value, float target, float width){
    return width / abs(value - target);
}

// smoothLine2 函数
vec2 smoothLine2(float value, float target, float width){
    return vec2(step(0.0, value - target), width / abs(value - target));
}

// circleTriangle 函数
float circleTriangle(vec2 pos){
    float circle = length(pos * 0.5);
    float triangle = triangleLength(pos * 0.3);    
    return smoothLine(circle, 1.0, 0.025) + smoothLine(triangle, 1.0, 0.025);
}

// circleTriangle2 函数
vec2 circleTriangle2(vec2 pos){
    float circle2 = length(pos * 0.35);
    vec2 ret = smoothLine2(circle2, 1.0, 0.025);
    ret.y += circleTriangle(pos);
    return ret;
}

// ButitobaCircle 函数
float ButitobaCircle(vec2 pos){
    vec2 pp = polar(rotate(pos, -iTime) * 0.75);
    return Butitoba(mod(rotate(pp * vec2(1.0, 32.0), PI * 0.5), vec2(16.0, 4.0)) - 1.5, 0.05) * smoothstepLine(6.0, 7.5, pp.x, 1.5);
}

// ButitobaCircle2 函数
float ButitobaCircle2(vec2 pos, float scale, float x, float y, float x2, float y2, float lower, float upper, float r){
    vec2 pp = polar(rotate(pos, r) * scale);
    return Butitoba(mod(rotate(pp * vec2(x, y), PI * 0.5), vec2(x2, y2)) - 1.5, 0.03) * smoothstepLine(lower, upper, pp.x, 0.2);
}

// drawCircles 函数
float drawCircles(vec2 uv)
{
    float v = 0.0;
    vec2 ar = vec2(0.5, 0.5);
    v += pow(1.0 - clamp(abs(length(uv - vec2(0.0)) - 0.6), 0.0, 1.0), 32.0);
    v += pow(1.0 - clamp(abs(length(uv - vec2(0.0)) - 0.45), 0.0, 1.0), 128.0);
    v += pow(1.0 - clamp(abs(length(uv - vec2(0.0)) - 0.3), 0.0, 1.0), 64.0);
    for(int i = 0; i < 4; ++i)
    {
        float angle = float(i) * 0.25 * TAU + iTime;
        float x = cos(angle);
        float y = sin(angle);
        v += pow(1.0 - clamp(abs(length(uv - vec2(x * 0.3, y * 0.3)) - 0.63 * 0.001), 0.0, 1.0), 64.0);
    }
    for(int i = 0; i < 4; ++i)
    {
        float angle = float(i) * 0.25 * TAU + PI * 0.25 + iTime;
        float x = cos(angle);
        float y = sin(angle);
        v += pow(1.0 - clamp(abs(length(uv - vec2(x * 0.25, y * 0.25)) - 0.6 * 0.001), 0.0, 1.0), 128.0);
        v += pow(1.0 - clamp(abs(length(uv - vec2(x * 0.235, y * 0.235)) - 0.01), 0.0, 1.0), 256.0);
    }
    float az = acos(dot(normalize(uv), vec2(0.0, 1.0)));
    v += (sin(4.0 * az + PI * 1.5 - iTime * 4.0) > 0.0 ? 1.0 : 0.0) * pow(1.0 - clamp(abs(length(uv - vec2(0.0)) - 0.57), 0.0, 1.0), 64.0);
    for(int i = 0; i < 8; ++i)
    {
        float angle = float(i) * 0.125 * TAU + PI * 0.125 + iTime;
        float x = cos(angle);
        float y = sin(angle);
        v += pow(1.0 - clamp(abs(length(uv - vec2(x * 0.63, y * 0.63)) - 0.57 * 0.001), 0.0, 1.0), 256.0);
    }
    az = acos(dot(normalize(uv), vec2(0.0, 1.0)));
    v += (cos(4.0 * az + iTime * 16.0) > 0.0 ? 1.0 : 0.0) * pow(1.0 - clamp(abs(length(uv - vec2(0.0)) - 0.33), 0.0, 1.0), 128.0);
    for(int i = 0; i < 8; ++i)
    {
        float angle = float(i) * 0.125 * TAU + PI * 0.125 - iTime;
        float x = cos(angle);
        float y = sin(angle);
        v += pow(1.0 - clamp(abs(length(uv - vec2(x * 0.36, y * 0.36)) - 0.3 * 0.001), 0.0, 1.0), 256.0);
    }
    for(int i = 0; i < 4; ++i)
    {
        float angle = float(i) * 0.25 * TAU + PI * 0.135 - iTime;
        float x = cos(angle);
        float y = sin(angle);
        v += pow(1.0 - clamp(abs(length(uv - vec2(x * 0.585, y * 0.585)) - 0.01), 0.0, 1.0), 256.0);
        angle = float(i) * 0.25 * TAU + PI * 0.365;
        x = cos(angle);
        y = sin(angle);
        v += pow(1.0 - clamp(abs(length(uv - vec2(x * 0.585, y * 0.585)) - 0.01), 0.0, 1.0), 256.0);
    }
    for(int i = 0; i < 4; ++i)
    {
        float angle = float(i) * 0.25 * TAU + PI * 0.11 - iTime;
        float x = cos(angle);
        float y = sin(angle);
        v += pow(1.0 - clamp(abs(length(uv - vec2(x * 0.315, y * 0.315)) - 0.01), 0.0, 1.0), 256.0);
        angle = float(i) * 0.25 * TAU + PI * 0.39 - iTime;
        x = cos(angle);
        y = sin(angle);
        v += pow(1.0 - clamp(abs(length(uv - vec2(x * 0.315, y * 0.315)) - 0.01), 0.0, 1.0), 256.0);
    }
    return v;
}

void main()
{
    // 计算片段坐标
    vec2 fragCoord = gl_FragCoord.xy;

    // 归一化坐标
    vec2 uv = (fragCoord - 1.0 * iResolution.xy) / iResolution.x;
    uv *= length(uv) * 10.0;

    vec2 uv4 = (fragCoord - 1.0 * iResolution.xy) / iResolution.x;
    uv4 *= length(uv4) * 10.0;

    vec2 uv3 = (fragCoord - 1.0 * iResolution.xy) / iResolution.x;
    uv3 *= length(uv3) * 10.0;
    uv3 *= cos(iTime);
    uv4 *= sin(iTime);

    // 初始化颜色变量
    vec4 O = vec4(0.0);
    vec2 p = (fragCoord * 2.0 - iResolution) / max(iResolution.x, iResolution.y);
    float l = (render(p) + render(p + vec2(0.0, 1.0 / min(iResolution.x, iResolution.y)))) * 2.0;

    float l2 = (render(p) + render(p + vec2(0.0, 0.5 / min(iResolution.x, iResolution.y)))) * 0.25;
    float l3 = (render(p * rot(iTime) * 2.0) + render(p * rot(iTime) * 2.0 + vec2(0.0, 0.15 / min(iResolution.x, iResolution.y)))) * 1.25;

    vec2 uv2 = (fragCoord - 1.0 * iResolution.xy) / iResolution.y;
    vec2 pos = (fragCoord - 1.0 * iResolution.xy) / min(iResolution.x, iResolution.y);
    vec3 color = vec3(0.0);

    const float ncircle = 7.0;
    const float orbit_radius = 0.105;
    const float circle_radius = 0.0112;

    // 绘制多个圆
    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime + 0.2);
        float y = orbit_radius * sin(angle - iTime + 0.2);
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.65) * 0.75;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 1.0) * 1.2;
        float y = orbit_radius * sin(angle - iTime - 1.0) * 1.2;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.65) * 0.75;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 1.4) * 1.55;
        float y = orbit_radius * sin(angle - iTime - 1.4) * 1.55;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.7;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 1.25) * 2.0;
        float y = orbit_radius * sin(angle - iTime - 1.25) * 2.0;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.65;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 1.1) * 2.5;
        float y = orbit_radius * sin(angle - iTime - 1.1) * 2.5;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.6;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime + 0.95) * 2.9;
        float y = orbit_radius * sin(angle + iTime - 0.95) * 2.9;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.54;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 5.80) * 3.3;
        float y = orbit_radius * sin(angle - iTime - 5.75) * 3.3;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.44;
    }

    for(float angle = 0.0; angle < 5.0 * PI; angle += 5.0 * PI / ncircle) { // 修改步长以避免无限循环
        float x = orbit_radius * cos(angle - iTime - 0.58) * 3.5;
        float y = orbit_radius * sin(angle + iTime + 0.58) * 3.5;
        color += circle_radius / length(pos + vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.35;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 0.58) * 3.5;
        float y = orbit_radius * sin(angle - iTime + 0.58) * 3.5;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.35;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 0.91) * 1.6;
        float y = orbit_radius * sin(angle - iTime - 0.91) * 1.6;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.7;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 0.8) * 2.0;
        float y = orbit_radius * sin(angle - iTime - 0.8) * 2.0;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.65;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 0.69) * 2.3;
        float y = orbit_radius * sin(angle - iTime - 0.69) * 2.3;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.57;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 0.49) * 2.6;
        float y = orbit_radius * sin(angle - iTime - 0.49) * 2.6;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.5;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 0.25) * 2.8;
        float y = orbit_radius * sin(angle - iTime - 0.25) * 2.8;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.45;
    }

    for(float angle = 0.0; angle < TAU; angle += TAU / ncircle) {
        float x = orbit_radius * cos(angle - iTime - 0.05) * 3.9;
        float y = orbit_radius * sin(angle - iTime - 0.05) * 3.9;
        color += circle_radius / length(pos - vec2(x, y)) * vec3(0.2, 0.2, 0.8) * 0.35;
    }

    // 光照和其他效果
    vec3 O_vec3 = vec3(0.0);
    vec3 d = normalize(vec3((fragCoord * 2.0 - iResolution) / iResolution.y, 1.0));
    for(float i = 0.0, a, s, e, g = 0.0; i < 110.0; ++i) {
        vec3 n1 = g * d;
        a = 10.0;
        n1 = mod(n1 - a, a * 2.0) - a;
        s = 2.0;
        for(int j = 0; j < 8; ++j) {
            n1 = 0.3 - abs(n1);
            if(n1.x < n1.z) { n1 = n1.zyx; }
            if(n1.z < n1.y) { n1 = n1.xzy; }
            if(n1.y < n1.x) { n1 = n1.zyx; }
            if(n1.z < n1.y) { n1 = n1.xyz; }
            s *= e = 1.4 + sin(iTime * 0.234) * 0.1;
            n1 = abs(n1) * e - vec3(
                5.0 + cos(iTime * 0.3 + 0.5 * cos(iTime * 0.3)) * 3.0,
                120.0,
                8.0 + cos(iTime * 0.5) * 5.0
            ) * color * l2 * l3;
        }
        g += e = length(n1.yx) / s;
        O_vec3 += mix(vec3(1.0), cos(vec3(g * 0.1, 13.0, 1.0) * 1.3) * 0.1 + 0.5, sin(0.8)) * 1.0 / e / 8000.0;
    }

    // 计算最终颜色
    vec4 fragColor = vec4(O_vec3 * l2 * l * l3 + color, 0.0);
    uv2 *= 2.0 * (cos(iTime * 2.0) - 2.5); // scale
    float anim = sin(iTime * 12.0) * 0.1 + 1.0;  // anim between 0.9 - 1.1
    fragColor *= vec4(happy_star(uv2, anim) * vec3(0.55, 0.5, 0.55) * 0.30325, 1.0);
    fragColor += vec4(0.8, 0.5, 0.2, 1.0) * drawCircles(uv * 1.0);
    fragColor += vec4(0.1, 0.5, 0.9, 1.0) * drawCircles(uv3 * 3.0);
    fragColor += vec4(0.1, 0.1, 0.9, 1.0) * drawCircles(uv4 * 0.5);
    fragColor *= vec4(O_vec3, 1.0); // 'O_vec3' 是 vec3, 扩展为 vec4

    // 设置输出颜色
    FragColor = fragColor;
}