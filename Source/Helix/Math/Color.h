#pragma once

#include <Helix/Math/Vector.h>

// Colors in the reference's shape: r, g, b, a (0..1 for LDR), the named colors as static constants, == approximately
// equal (as Vector4), Equals exact; HSV in 0..1.
namespace helix
{
    struct Color32;

    HX_STRUCT(Namespace = "HelixEngine")
    struct Color
    {
        HX_FIELD() f32 r = 0;
        HX_FIELD() f32 g = 0;
        HX_FIELD() f32 b = 0;
        HX_FIELD() f32 a = 0;

        // The named colors of the reference, with its values.
        static const Color aliceBlue, antiqueWhite, aquamarine, azure, beige, bisque, black, blanchedAlmond, blue,
            blueViolet, brown, burlywood, cadetBlue, chartreuse, chocolate, clear, coral, cornflowerBlue, cornsilk,
            crimson, cyan, darkBlue, darkCyan, darkGoldenRod, darkGray, darkGreen, darkKhaki, darkMagenta,
            darkOliveGreen, darkOrange, darkOrchid, darkRed, darkSalmon, darkSeaGreen, darkSlateBlue, darkSlateGray,
            darkTurquoise, darkViolet, deepPink, deepSkyBlue, dimGray, dodgerBlue, firebrick, floralWhite,
            forestGreen, gainsboro, ghostWhite, gold, goldenRod, gray, gray1, gray2, gray3, gray4, gray5, gray6,
            gray7, gray8, gray9, green, greenYellow, grey, honeydew, hotPink, indianRed, indigo, ivory, khaki,
            lavender, lavenderBlush, lawnGreen, lemonChiffon, lightBlue, lightCoral, lightCyan, lightGoldenRod,
            lightGoldenRodYellow, lightGray, lightGreen, lightPink, lightSalmon, lightSeaGreen, lightSkyBlue,
            lightSlateBlue, lightSlateGray, lightSteelBlue, lightYellow, limeGreen, linen, magenta, maroon,
            mediumAquamarine, mediumBlue, mediumOrchid, mediumPurple, mediumSeaGreen, mediumSlateBlue,
            mediumSpringGreen, mediumTurquoise, mediumVioletRed, midnightBlue, mintCream, mistyRose, moccasin,
            navajoWhite, navyBlue, oldLace, olive, oliveDrab, orange, orangeRed, orchid, paleGoldenRod, paleGreen,
            paleTurquoise, paleVioletRed, papayaWhip, peachPuff, peru, pink, plum, powderBlue, purple, rebeccaPurple,
            red, rosyBrown, royalBlue, saddleBrown, salmon, sandyBrown, seaGreen, seashell, sienna, silver, skyBlue,
            slateBlue, slateGray, snow, softBlue, softGreen, softRed, softYellow, springGreen, steelBlue, tan, teal,
            thistle, tomato, turquoise, violet, violetRed, wheat, white, whiteSmoke, yellow, yellowGreen, yellowNice;

        constexpr Color() = default;
        constexpr Color(f32 r0, f32 g0, f32 b0, f32 a0 = 1.0f) : r(r0), g(g0), b(b0), a(a0) {}
        constexpr explicit Color(Vector4 v) : r(v.x), g(v.y), b(v.z), a(v.w) {}
        constexpr operator Vector4() const { return {r, g, b, a}; }

        f32& operator[](int i) { return (&r)[i]; }
        f32 operator[](int i) const { return (&r)[i]; }

        Color linear() const;
        Color gamma() const;
        f32 grayscale() const { return 0.299f * r + 0.587f * g + 0.114f * b; }
        f32 maxColorComponent() const { return mathf::Max(mathf::Max(r, g), b); }
        constexpr bool Equals(Color o) const { return r == o.r && g == o.g && b == o.b && a == o.a; }

        static Color Lerp(Color a, Color b, f32 t);
        static constexpr Color LerpUnclamped(Color x, Color y, f32 t)
        {
            return {x.r + (y.r - x.r) * t, x.g + (y.g - x.g) * t, x.b + (y.b - x.b) * t, x.a + (y.a - x.a) * t};
        }
        static void RGBToHSV(Color rgbColor, f32& H, f32& S, f32& V);
        // Opaque; with hdr the result is not clamped to 0..1.
        static Color HSVToRGB(f32 H, f32 S, f32 V, bool hdr = true);

        constexpr Color operator+(Color o) const { return {r + o.r, g + o.g, b + o.b, a + o.a}; }
        constexpr Color operator-(Color o) const { return {r - o.r, g - o.g, b - o.b, a - o.a}; }
        constexpr Color operator*(Color o) const { return {r * o.r, g * o.g, b * o.b, a * o.a}; }
        constexpr Color operator*(f32 s) const { return {r * s, g * s, b * s, a * s}; }
        constexpr Color operator/(f32 s) const { return {r / s, g / s, b / s, a / s}; }
        constexpr bool operator==(Color o) const { return Vector4(r, g, b, a) == Vector4(o.r, o.g, o.b, o.a); }
        operator Color32() const;
    };

    inline constexpr Color Color::aliceBlue{0.9411765f, 0.9725491f, 1, 1};
    inline constexpr Color Color::antiqueWhite{0.9803922f, 0.9215687f, 0.8431373f, 1};
    inline constexpr Color Color::aquamarine{0.4980392f, 1, 0.8313726f, 1};
    inline constexpr Color Color::azure{0.9411765f, 1, 1, 1};
    inline constexpr Color Color::beige{0.9607844f, 0.9607844f, 0.8627452f, 1};
    inline constexpr Color Color::bisque{1, 0.8941177f, 0.7686275f, 1};
    inline constexpr Color Color::black{0, 0, 0, 1};
    inline constexpr Color Color::blanchedAlmond{1, 0.9215687f, 0.8039216f, 1};
    inline constexpr Color Color::blue{0, 0, 1, 1};
    inline constexpr Color Color::blueViolet{0.5411765f, 0.1686275f, 0.8862746f, 1};
    inline constexpr Color Color::brown{0.6470588f, 0.1647059f, 0.1647059f, 1};
    inline constexpr Color Color::burlywood{0.8705883f, 0.7215686f, 0.5294118f, 1};
    inline constexpr Color Color::cadetBlue{0.372549f, 0.6196079f, 0.627451f, 1};
    inline constexpr Color Color::chartreuse{0.4980392f, 1, 0, 1};
    inline constexpr Color Color::chocolate{0.8235295f, 0.4117647f, 0.1176471f, 1};
    inline constexpr Color Color::clear{0, 0, 0, 0};
    inline constexpr Color Color::coral{1, 0.4980392f, 0.3137255f, 1};
    inline constexpr Color Color::cornflowerBlue{0.3921569f, 0.5843138f, 0.9294118f, 1};
    inline constexpr Color Color::cornsilk{1, 0.9725491f, 0.8627452f, 1};
    inline constexpr Color Color::crimson{0.8627452f, 0.07843138f, 0.2352941f, 1};
    inline constexpr Color Color::cyan{0, 1, 1, 1};
    inline constexpr Color Color::darkBlue{0, 0, 0.5450981f, 1};
    inline constexpr Color Color::darkCyan{0, 0.5450981f, 0.5450981f, 1};
    inline constexpr Color Color::darkGoldenRod{0.7215686f, 0.5254902f, 0.04313726f, 1};
    inline constexpr Color Color::darkGray{0.6627451f, 0.6627451f, 0.6627451f, 1};
    inline constexpr Color Color::darkGreen{0, 0.3921569f, 0, 1};
    inline constexpr Color Color::darkKhaki{0.7411765f, 0.7176471f, 0.4196079f, 1};
    inline constexpr Color Color::darkMagenta{0.5450981f, 0, 0.5450981f, 1};
    inline constexpr Color Color::darkOliveGreen{0.3333333f, 0.4196079f, 0.1843137f, 1};
    inline constexpr Color Color::darkOrange{1, 0.5490196f, 0, 1};
    inline constexpr Color Color::darkOrchid{0.6f, 0.1960784f, 0.8000001f, 1};
    inline constexpr Color Color::darkRed{0.5450981f, 0, 0, 1};
    inline constexpr Color Color::darkSalmon{0.9137256f, 0.5882353f, 0.4784314f, 1};
    inline constexpr Color Color::darkSeaGreen{0.5607843f, 0.7372549f, 0.5607843f, 1};
    inline constexpr Color Color::darkSlateBlue{0.282353f, 0.2392157f, 0.5450981f, 1};
    inline constexpr Color Color::darkSlateGray{0.1843137f, 0.3098039f, 0.3098039f, 1};
    inline constexpr Color Color::darkTurquoise{0, 0.8078432f, 0.8196079f, 1};
    inline constexpr Color Color::darkViolet{0.5803922f, 0, 0.8274511f, 1};
    inline constexpr Color Color::deepPink{1, 0.07843138f, 0.5764706f, 1};
    inline constexpr Color Color::deepSkyBlue{0, 0.7490196f, 1, 1};
    inline constexpr Color Color::dimGray{0.4117647f, 0.4117647f, 0.4117647f, 1};
    inline constexpr Color Color::dodgerBlue{0.1176471f, 0.5647059f, 1, 1};
    inline constexpr Color Color::firebrick{0.6980392f, 0.1333333f, 0.1333333f, 1};
    inline constexpr Color Color::floralWhite{1, 0.9803922f, 0.9411765f, 1};
    inline constexpr Color Color::forestGreen{0.1333333f, 0.5450981f, 0.1333333f, 1};
    inline constexpr Color Color::gainsboro{0.8627452f, 0.8627452f, 0.8627452f, 1};
    inline constexpr Color Color::ghostWhite{0.9725491f, 0.9725491f, 1, 1};
    inline constexpr Color Color::gold{1, 0.8431373f, 0, 1};
    inline constexpr Color Color::goldenRod{0.854902f, 0.6470588f, 0.1254902f, 1};
    inline constexpr Color Color::gray{0.5f, 0.5f, 0.5f, 1};
    inline constexpr Color Color::gray1{0.1f, 0.1f, 0.1f, 1};
    inline constexpr Color Color::gray2{0.2f, 0.2f, 0.2f, 1};
    inline constexpr Color Color::gray3{0.3f, 0.3f, 0.3f, 1};
    inline constexpr Color Color::gray4{0.4f, 0.4f, 0.4f, 1};
    inline constexpr Color Color::gray5{0.5f, 0.5f, 0.5f, 1};
    inline constexpr Color Color::gray6{0.6f, 0.6f, 0.6f, 1};
    inline constexpr Color Color::gray7{0.7f, 0.7f, 0.7f, 1};
    inline constexpr Color Color::gray8{0.8f, 0.8f, 0.8f, 1};
    inline constexpr Color Color::gray9{0.9f, 0.9f, 0.9f, 1};
    inline constexpr Color Color::green{0, 1, 0, 1};
    inline constexpr Color Color::greenYellow{0.6784314f, 1, 0.1843137f, 1};
    inline constexpr Color Color::grey{0.5f, 0.5f, 0.5f, 1};
    inline constexpr Color Color::honeydew{0.9411765f, 1, 0.9411765f, 1};
    inline constexpr Color Color::hotPink{1, 0.4117647f, 0.7058824f, 1};
    inline constexpr Color Color::indianRed{0.8039216f, 0.3607843f, 0.3607843f, 1};
    inline constexpr Color Color::indigo{0.2941177f, 0, 0.509804f, 1};
    inline constexpr Color Color::ivory{1, 1, 0.9411765f, 1};
    inline constexpr Color Color::khaki{0.9411765f, 0.9019608f, 0.5490196f, 1};
    inline constexpr Color Color::lavender{0.9019608f, 0.9019608f, 0.9803922f, 1};
    inline constexpr Color Color::lavenderBlush{1, 0.9411765f, 0.9607844f, 1};
    inline constexpr Color Color::lawnGreen{0.4862745f, 0.9882354f, 0, 1};
    inline constexpr Color Color::lemonChiffon{1, 0.9803922f, 0.8039216f, 1};
    inline constexpr Color Color::lightBlue{0.6784314f, 0.8470589f, 0.9019608f, 1};
    inline constexpr Color Color::lightCoral{0.9411765f, 0.5019608f, 0.5019608f, 1};
    inline constexpr Color Color::lightCyan{0.8784314f, 1, 1, 1};
    inline constexpr Color Color::lightGoldenRod{0.9333334f, 0.8666667f, 0.509804f, 1};
    inline constexpr Color Color::lightGoldenRodYellow{0.9803922f, 0.9803922f, 0.8235295f, 1};
    inline constexpr Color Color::lightGray{0.8274511f, 0.8274511f, 0.8274511f, 1};
    inline constexpr Color Color::lightGreen{0.5647059f, 0.9333334f, 0.5647059f, 1};
    inline constexpr Color Color::lightPink{1, 0.7137255f, 0.7568628f, 1};
    inline constexpr Color Color::lightSalmon{1, 0.627451f, 0.4784314f, 1};
    inline constexpr Color Color::lightSeaGreen{0.1254902f, 0.6980392f, 0.6666667f, 1};
    inline constexpr Color Color::lightSkyBlue{0.5294118f, 0.8078432f, 0.9803922f, 1};
    inline constexpr Color Color::lightSlateBlue{0.5176471f, 0.4392157f, 1, 1};
    inline constexpr Color Color::lightSlateGray{0.4666667f, 0.5333334f, 0.6f, 1};
    inline constexpr Color Color::lightSteelBlue{0.6901961f, 0.7686275f, 0.8705883f, 1};
    inline constexpr Color Color::lightYellow{1, 1, 0.8784314f, 1};
    inline constexpr Color Color::limeGreen{0.1960784f, 0.8039216f, 0.1960784f, 1};
    inline constexpr Color Color::linen{0.9803922f, 0.9411765f, 0.9019608f, 1};
    inline constexpr Color Color::magenta{1, 0, 1, 1};
    inline constexpr Color Color::maroon{0.6901961f, 0.1882353f, 0.3764706f, 1};
    inline constexpr Color Color::mediumAquamarine{0.4f, 0.8039216f, 0.6666667f, 1};
    inline constexpr Color Color::mediumBlue{0, 0, 0.8039216f, 1};
    inline constexpr Color Color::mediumOrchid{0.7294118f, 0.3333333f, 0.8274511f, 1};
    inline constexpr Color Color::mediumPurple{0.5764706f, 0.4392157f, 0.8588236f, 1};
    inline constexpr Color Color::mediumSeaGreen{0.2352941f, 0.7019608f, 0.4431373f, 1};
    inline constexpr Color Color::mediumSlateBlue{0.482353f, 0.4078432f, 0.9333334f, 1};
    inline constexpr Color Color::mediumSpringGreen{0, 0.9803922f, 0.6039216f, 1};
    inline constexpr Color Color::mediumTurquoise{0.282353f, 0.8196079f, 0.8000001f, 1};
    inline constexpr Color Color::mediumVioletRed{0.7803922f, 0.08235294f, 0.5215687f, 1};
    inline constexpr Color Color::midnightBlue{0.09803922f, 0.09803922f, 0.4392157f, 1};
    inline constexpr Color Color::mintCream{0.9607844f, 1, 0.9803922f, 1};
    inline constexpr Color Color::mistyRose{1, 0.8941177f, 0.882353f, 1};
    inline constexpr Color Color::moccasin{1, 0.8941177f, 0.7098039f, 1};
    inline constexpr Color Color::navajoWhite{1, 0.8705883f, 0.6784314f, 1};
    inline constexpr Color Color::navyBlue{0, 0, 0.5019608f, 1};
    inline constexpr Color Color::oldLace{0.9921569f, 0.9607844f, 0.9019608f, 1};
    inline constexpr Color Color::olive{0.5019608f, 0.5019608f, 0, 1};
    inline constexpr Color Color::oliveDrab{0.4196079f, 0.5568628f, 0.1372549f, 1};
    inline constexpr Color Color::orange{1, 0.6470588f, 0, 1};
    inline constexpr Color Color::orangeRed{1, 0.2705882f, 0, 1};
    inline constexpr Color Color::orchid{0.854902f, 0.4392157f, 0.8392158f, 1};
    inline constexpr Color Color::paleGoldenRod{0.9333334f, 0.909804f, 0.6666667f, 1};
    inline constexpr Color Color::paleGreen{0.5960785f, 0.9843138f, 0.5960785f, 1};
    inline constexpr Color Color::paleTurquoise{0.6862745f, 0.9333334f, 0.9333334f, 1};
    inline constexpr Color Color::paleVioletRed{0.8588236f, 0.4392157f, 0.5764706f, 1};
    inline constexpr Color Color::papayaWhip{1, 0.937255f, 0.8352942f, 1};
    inline constexpr Color Color::peachPuff{1, 0.854902f, 0.7254902f, 1};
    inline constexpr Color Color::peru{0.8039216f, 0.5215687f, 0.2470588f, 1};
    inline constexpr Color Color::pink{1, 0.7529413f, 0.7960785f, 1};
    inline constexpr Color Color::plum{0.8666667f, 0.627451f, 0.8666667f, 1};
    inline constexpr Color Color::powderBlue{0.6901961f, 0.8784314f, 0.9019608f, 1};
    inline constexpr Color Color::purple{0.627451f, 0.1254902f, 0.9411765f, 1};
    inline constexpr Color Color::rebeccaPurple{0.4f, 0.2f, 0.6f, 1};
    inline constexpr Color Color::red{1, 0, 0, 1};
    inline constexpr Color Color::rosyBrown{0.7372549f, 0.5607843f, 0.5607843f, 1};
    inline constexpr Color Color::royalBlue{0.254902f, 0.4117647f, 0.882353f, 1};
    inline constexpr Color Color::saddleBrown{0.5450981f, 0.2705882f, 0.07450981f, 1};
    inline constexpr Color Color::salmon{0.9803922f, 0.5019608f, 0.4470589f, 1};
    inline constexpr Color Color::sandyBrown{0.9568628f, 0.6431373f, 0.3764706f, 1};
    inline constexpr Color Color::seaGreen{0.1803922f, 0.5450981f, 0.3411765f, 1};
    inline constexpr Color Color::seashell{1, 0.9607844f, 0.9333334f, 1};
    inline constexpr Color Color::sienna{0.627451f, 0.3215686f, 0.1764706f, 1};
    inline constexpr Color Color::silver{0.7529413f, 0.7529413f, 0.7529413f, 1};
    inline constexpr Color Color::skyBlue{0.5294118f, 0.8078432f, 0.9215687f, 1};
    inline constexpr Color Color::slateBlue{0.4156863f, 0.3529412f, 0.8039216f, 1};
    inline constexpr Color Color::slateGray{0.4392157f, 0.5019608f, 0.5647059f, 1};
    inline constexpr Color Color::snow{1, 0.9803922f, 0.9803922f, 1};
    inline constexpr Color Color::softBlue{0.1882353f, 0.682353f, 0.7490196f, 1};
    inline constexpr Color Color::softGreen{0.5490196f, 0.7882354f, 0.1411765f, 1};
    inline constexpr Color Color::softRed{0.8627452f, 0.1921569f, 0.1960784f, 1};
    inline constexpr Color Color::softYellow{1, 0.9333334f, 0.5490196f, 1};
    inline constexpr Color Color::springGreen{0, 1, 0.4980392f, 1};
    inline constexpr Color Color::steelBlue{0.2745098f, 0.509804f, 0.7058824f, 1};
    inline constexpr Color Color::tan{0.8235295f, 0.7058824f, 0.5490196f, 1};
    inline constexpr Color Color::teal{0, 0.5019608f, 0.5019608f, 1};
    inline constexpr Color Color::thistle{0.8470589f, 0.7490196f, 0.8470589f, 1};
    inline constexpr Color Color::tomato{1, 0.3882353f, 0.2784314f, 1};
    inline constexpr Color Color::turquoise{0.2509804f, 0.8784314f, 0.8156863f, 1};
    inline constexpr Color Color::violet{0.9333334f, 0.509804f, 0.9333334f, 1};
    inline constexpr Color Color::violetRed{0.8156863f, 0.1254902f, 0.5647059f, 1};
    inline constexpr Color Color::wheat{0.9607844f, 0.8705883f, 0.7019608f, 1};
    inline constexpr Color Color::white{1, 1, 1, 1};
    inline constexpr Color Color::whiteSmoke{0.9607844f, 0.9607844f, 0.9607844f, 1};
    inline constexpr Color Color::yellow{1, 0.921568632f, 0.0156862754f, 1};
    inline constexpr Color Color::yellowGreen{0.6039216f, 0.8039216f, 0.1960784f, 1};
    inline constexpr Color Color::yellowNice{1, 0.921568632f, 0.0156862754f, 1};

    constexpr Color operator*(f32 s, Color c) { return c * s; }

    HX_STRUCT(Namespace = "HelixEngine")
    struct Color32
    {
        HX_FIELD() u8 r = 0;
        HX_FIELD() u8 g = 0;
        HX_FIELD() u8 b = 0;
        HX_FIELD() u8 a = 0;

        constexpr Color32() = default;
        constexpr Color32(u8 r0, u8 g0, u8 b0, u8 a0) : r(r0), g(g0), b(b0), a(a0) {}
        constexpr operator Color() const { return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f}; }

        u8& operator[](int i) { return (&r)[i]; }
        u8 operator[](int i) const { return (&r)[i]; }

        static Color32 Lerp(Color32 a, Color32 b, f32 t) { return LerpUnclamped(a, b, mathf::Clamp01(t)); }
        static Color32 LerpUnclamped(Color32 a, Color32 b, f32 t)
        {
            return Color32(u8(f32(a.r) + f32(b.r - a.r) * t), u8(f32(a.g) + f32(b.g - a.g) * t), u8(f32(a.b) + f32(b.b - a.b) * t),
                           u8(f32(a.a) + f32(b.a - a.a) * t));
        }
        constexpr bool operator==(const Color32&) const = default;
    };

    namespace mathf
    {
        // The color of a black body at the temperature in Kelvin (1000 to 40000), linear and normalized.
        Color CorrelatedColorTemperatureToRGB(f32 kelvin);
    }
}
