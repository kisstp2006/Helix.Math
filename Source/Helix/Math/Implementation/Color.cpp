#include <Helix/Math/Color.h>

namespace helix
{
    using namespace mathf;

    Color Color::linear() const { return {GammaToLinearSpace(r), GammaToLinearSpace(g), GammaToLinearSpace(b), a}; }
    Color Color::gamma() const { return {LinearToGammaSpace(r), LinearToGammaSpace(g), LinearToGammaSpace(b), a}; }
    Color Color::Lerp(Color x, Color y, f32 t) { return LerpUnclamped(x, y, Clamp01(t)); }

    Color::operator Color32() const
    {
        auto c = [](f32 v) { return u8(Round(Clamp01(v) * 255.0f)); };
        return {c(r), c(g), c(b), c(a)};
    }

    void Color::RGBToHSV(Color rgb, f32& h, f32& s, f32& v)
    {
        f32 maxc = rgb.maxColorComponent();
        f32 minc = mathf::Min(mathf::Min(rgb.r, rgb.g), rgb.b);
        f32 delta = maxc - minc;
        v = maxc;
        if (delta == 0.0f)
        {
            h = 0.0f;
            s = 0.0f;
            return;
        }
        s = maxc > 0.0f ? delta / maxc : 0.0f;
        if (rgb.r == maxc)
            h = (rgb.g - rgb.b) / delta;
        else if (rgb.g == maxc)
            h = 2.0f + (rgb.b - rgb.r) / delta;
        else
            h = 4.0f + (rgb.r - rgb.g) / delta;
        h /= 6.0f;
        if (h < 0.0f)
            h += 1.0f;
    }

    Color Color::HSVToRGB(f32 h, f32 s, f32 v, bool hdr)
    {
        if (s == 0.0f)
            return {v, v, v, 1.0f};
        f32 hh = Repeat(h, 1.0f) * 6.0f;
        int sector = int(Floor(hh));
        f32 f = hh - f32(sector);
        f32 p = v * (1.0f - s), q = v * (1.0f - s * f), t = v * (1.0f - s * (1.0f - f));
        Color c;
        switch (sector % 6)
        {
        case 0: c = {v, t, p, 1}; break;
        case 1: c = {q, v, p, 1}; break;
        case 2: c = {p, v, t, 1}; break;
        case 3: c = {p, q, v, 1}; break;
        case 4: c = {t, p, v, 1}; break;
        default: c = {v, p, q, 1}; break;
        }
        if (!hdr)
            c = {Clamp01(c.r), Clamp01(c.g), Clamp01(c.b), 1};
        return c;
    }

    Color mathf::CorrelatedColorTemperatureToRGB(f32 kelvin)
    {
        // A fit of the black body curve (sRGB 0..255 by the temperature in hundreds of Kelvin), to linear, the
        // largest component 1.
        f32 t = Clamp(kelvin, 1000.0f, 40000.0f) / 100.0f;
        f32 red = t <= 66.0f ? 255.0f : 329.698727446f * Pow(t - 60.0f, -0.1332047592f);
        f32 green = t <= 66.0f ? 99.4708025861f * Log(t) - 161.1195681661f : 288.1221695283f * Pow(t - 60.0f, -0.0755148492f);
        f32 blue = t >= 66.0f ? 255.0f : t <= 19.0f ? 0.0f : 138.5177312231f * Log(t - 10.0f) - 305.0447927307f;
        Color c(GammaToLinearSpace(Clamp(red, 0.0f, 255.0f) / 255.0f), GammaToLinearSpace(Clamp(green, 0.0f, 255.0f) / 255.0f),
                GammaToLinearSpace(Clamp(blue, 0.0f, 255.0f) / 255.0f), 1.0f);
        f32 m = c.maxColorComponent();
        return m > 0.0f ? Color(c.r / m, c.g / m, c.b / m, 1.0f) : c;
    }
}
