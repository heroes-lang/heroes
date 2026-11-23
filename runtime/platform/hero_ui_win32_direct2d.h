/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ui_win32_direct2d.h: DIRECT2D AND DIRECTWRITE AS THE WIN32 ARM'S
 * DRAWING AREA CALLS THEM (`hero_ui_canvas_win32.c`, the one file that
 * includes it), declared here because the SDK's `d2d1.h` and `dwrite.h`
 * declare them for C++ alone (measured on the Windows box, SDK 10.0.26100.0:
 * no C vtable, no C macro), as `hero_ui_web_win32.c` declares WebView2's.
 * Each method stands in the order the SDK's own headers give it, read that
 * day; a method the member never calls stands as a `void *` of its name, so
 * a slot cannot move. Where MinGW-w64's headers are the compiler's, which
 * declare the same interfaces for C from Wine's IDL, every slot the member
 * calls, every structure it hands over and every number it names is checked
 * against theirs at compile time, so two readings of Microsoft's interfaces
 * must agree before the member builds.
 *
 * It needs `windows.h`, `stddef.h` and `stdint.h` included first.
 */

#ifndef HERO_UI_WIN32_DIRECT2D_H
#define HERO_UI_WIN32_DIRECT2D_H

#if defined(__MINGW32__)
#include <d2d1.h>
#include <dwrite.h>
#endif

/* -- Direct2D and DirectWrite, as this member calls them ------------------- */

/* Direct2D's and DirectWrite's numbers this member names. */
#define HERO_D2_FACTORY_SINGLE_THREADED 0u
#define HERO_D2_TARGET_SOFTWARE 1u
#define HERO_D2_FORMAT_B8G8R8A8 87u
#define HERO_D2_ALPHA_PREMULTIPLIED 1u
#define HERO_D2_FILL_ALTERNATE 0u
#define HERO_D2_FILL_WINDING 1u
#define HERO_D2_FIGURE_FILLED 0u
#define HERO_D2_FIGURE_OPEN 0u
#define HERO_D2_FIGURE_CLOSED 1u
#define HERO_D2_CAP_FLAT 0u
#define HERO_D2_CAP_SQUARE 1u
#define HERO_D2_CAP_ROUND 2u
#define HERO_D2_JOIN_BEVEL 1u
#define HERO_D2_JOIN_ROUND 2u
#define HERO_D2_JOIN_MITER_OR_BEVEL 3u
#define HERO_D2_COMBINE_INTERSECT 1u
#define HERO_D2_ANTIALIAS_PER_PRIMITIVE 0u
#define HERO_D2_TEXT_ANTIALIAS_GRAYSCALE 2u
#define HERO_D2_INTERPOLATION_LINEAR 1u
#define HERO_D2_RECREATE_TARGET ((HRESULT)0x8899000Cu)
#define HERO_DW_FACTORY_SHARED 0u
#define HERO_DW_WEIGHT_NORMAL 400u
#define HERO_DW_WEIGHT_BOLD 700u
#define HERO_DW_STYLE_NORMAL 0u
#define HERO_DW_STYLE_ITALIC 2u
#define HERO_DW_STRETCH_NORMAL 5u
#define HERO_DW_NO_WRAP 1u

/* Core Graphics' miter limit, which Direct2D reads as SVG does: the miter's
 * length over the stroke's width. */
#define HERO_D2_MITER_LIMIT 10.0f

typedef struct hero_d2_point {
    FLOAT x;
    FLOAT y;
} hero_d2_point;

typedef struct hero_d2_rect {
    FLOAT left;
    FLOAT top;
    FLOAT right;
    FLOAT bottom;
} hero_d2_rect;

typedef struct hero_d2_color {
    FLOAT r;
    FLOAT g;
    FLOAT b;
    FLOAT a;
} hero_d2_color;

/* `D2D1_MATRIX_3X2_F`: _11 _12 _21 _22 _31 _32. */
typedef struct hero_d2_matrix {
    FLOAT m[6];
} hero_d2_matrix;

typedef struct hero_d2_size_u {
    UINT32 width;
    UINT32 height;
} hero_d2_size_u;

typedef struct hero_d2_bezier {
    hero_d2_point point1;
    hero_d2_point point2;
    hero_d2_point point3;
} hero_d2_bezier;

typedef struct hero_d2_quadratic {
    hero_d2_point point1;
    hero_d2_point point2;
} hero_d2_quadratic;

typedef struct hero_d2_pixel_format {
    UINT32 format;
    UINT32 alpha;
} hero_d2_pixel_format;

typedef struct hero_d2_target_properties {
    UINT32 type;
    hero_d2_pixel_format pixel_format;
    FLOAT dpi_x;
    FLOAT dpi_y;
    UINT32 usage;
    UINT32 min_level;
} hero_d2_target_properties;

typedef struct hero_d2_bitmap_properties {
    hero_d2_pixel_format pixel_format;
    FLOAT dpi_x;
    FLOAT dpi_y;
} hero_d2_bitmap_properties;

typedef struct hero_d2_stroke_properties {
    UINT32 start_cap;
    UINT32 end_cap;
    UINT32 dash_cap;
    UINT32 line_join;
    FLOAT miter_limit;
    UINT32 dash_style;
    FLOAT dash_offset;
} hero_d2_stroke_properties;

typedef struct hero_d2_geometry hero_d2_geometry;
typedef struct hero_d2_sink hero_d2_sink;
typedef struct hero_dw_format hero_dw_format;

typedef struct hero_d2_layer_parameters {
    hero_d2_rect content_bounds;
    hero_d2_geometry *geometric_mask;
    UINT32 mask_antialias_mode;
    hero_d2_matrix mask_transform;
    FLOAT opacity;
    void *opacity_brush;
    UINT32 layer_options;
} hero_d2_layer_parameters;

#define HERO_D2_UNKNOWN(self)                                                \
    HRESULT(STDMETHODCALLTYPE *QueryInterface)(self * me, REFIID iid, void **object); \
    ULONG(STDMETHODCALLTYPE *AddRef)(self * me);                             \
    ULONG(STDMETHODCALLTYPE *Release)(self * me)

/* IUnknown, for what this member only releases: a brush, a bitmap, a layer,
 * a stroke style, a font collection, a factory. */
typedef struct hero_d2_unknown hero_d2_unknown;
typedef struct hero_d2_unknown_vtbl {
    HERO_D2_UNKNOWN(hero_d2_unknown);
} hero_d2_unknown_vtbl;
struct hero_d2_unknown {
    const hero_d2_unknown_vtbl *lpVtbl;
};

/* ID2D1Geometry, and ID2D1PathGeometry after it. */
#define HERO_D2_GEOMETRY(self)                                                                                                                                 \
    HERO_D2_UNKNOWN(self);                                                                                                                                     \
    void *GetFactory;                                                                                                                                          \
    void *GetBounds;                                                                                                                                           \
    void *GetWidenedBounds;                                                                                                                                    \
    void *StrokeContainsPoint;                                                                                                                                 \
    void *FillContainsPoint;                                                                                                                                   \
    void *CompareWithGeometry;                                                                                                                                 \
    void *Simplify;                                                                                                                                            \
    void *Tessellate;                                                                                                                                          \
    HRESULT(STDMETHODCALLTYPE *CombineWithGeometry)(self * me, hero_d2_geometry * input, UINT32 mode, const hero_d2_matrix *transform, FLOAT tolerance, hero_d2_sink *sink); \
    void *Outline;                                                                                                                                             \
    void *ComputeArea;                                                                                                                                         \
    void *ComputeLength;                                                                                                                                       \
    void *ComputePointAtLength;                                                                                                                                \
    void *Widen

typedef struct hero_d2_geometry_vtbl {
    HERO_D2_GEOMETRY(hero_d2_geometry);
} hero_d2_geometry_vtbl;
struct hero_d2_geometry {
    const hero_d2_geometry_vtbl *lpVtbl;
};

typedef struct hero_d2_path hero_d2_path;
typedef struct hero_d2_path_vtbl {
    HERO_D2_GEOMETRY(hero_d2_path);
    HRESULT(STDMETHODCALLTYPE *Open)(hero_d2_path *me, hero_d2_sink **sink);
    void *Stream;
    void *GetSegmentCount;
    void *GetFigureCount;
} hero_d2_path_vtbl;
struct hero_d2_path {
    const hero_d2_path_vtbl *lpVtbl;
};

/* ID2D1GeometrySink, which holds ID2D1SimplifiedGeometrySink's first. */
typedef struct hero_d2_sink_vtbl {
    HERO_D2_UNKNOWN(hero_d2_sink);
    void(STDMETHODCALLTYPE *SetFillMode)(hero_d2_sink *me, UINT32 mode);
    void *SetSegmentFlags;
    void(STDMETHODCALLTYPE *BeginFigure)(hero_d2_sink *me, hero_d2_point start, UINT32 begin);
    void *AddLines;
    void *AddBeziers;
    void(STDMETHODCALLTYPE *EndFigure)(hero_d2_sink *me, UINT32 end);
    HRESULT(STDMETHODCALLTYPE *Close)(hero_d2_sink *me);
    void(STDMETHODCALLTYPE *AddLine)(hero_d2_sink *me, hero_d2_point point);
    void(STDMETHODCALLTYPE *AddBezier)(hero_d2_sink *me, const hero_d2_bezier *bezier);
    void(STDMETHODCALLTYPE *AddQuadraticBezier)(hero_d2_sink *me, const hero_d2_quadratic *bezier);
    void *AddQuadraticBeziers;
    void *AddArc;
} hero_d2_sink_vtbl;
struct hero_d2_sink {
    const hero_d2_sink_vtbl *lpVtbl;
};

/* ID2D1DCRenderTarget, which holds ID2D1RenderTarget's first. */
typedef struct hero_d2_target hero_d2_target;
typedef struct hero_d2_target_vtbl {
    HERO_D2_UNKNOWN(hero_d2_target);
    void *GetFactory;
    HRESULT(STDMETHODCALLTYPE *CreateBitmap)(hero_d2_target *me, hero_d2_size_u size, const void *data, UINT32 pitch, const hero_d2_bitmap_properties *properties, hero_d2_unknown **bitmap);
    void *CreateBitmapFromWicBitmap;
    void *CreateSharedBitmap;
    void *CreateBitmapBrush;
    HRESULT(STDMETHODCALLTYPE *CreateSolidColorBrush)(hero_d2_target *me, const hero_d2_color *color, const void *properties, hero_d2_unknown **brush);
    void *CreateGradientStopCollection;
    void *CreateLinearGradientBrush;
    void *CreateRadialGradientBrush;
    void *CreateCompatibleRenderTarget;
    HRESULT(STDMETHODCALLTYPE *CreateLayer)(hero_d2_target *me, const void *size, hero_d2_unknown **layer);
    void *CreateMesh;
    void *DrawLine;
    void *DrawRectangle;
    void *FillRectangle;
    void *DrawRoundedRectangle;
    void *FillRoundedRectangle;
    void *DrawEllipse;
    void *FillEllipse;
    void(STDMETHODCALLTYPE *DrawGeometry)(hero_d2_target *me, hero_d2_geometry *geometry, hero_d2_unknown *brush, FLOAT width, hero_d2_unknown *style);
    void(STDMETHODCALLTYPE *FillGeometry)(hero_d2_target *me, hero_d2_geometry *geometry, hero_d2_unknown *brush, hero_d2_unknown *opacity);
    void *FillMesh;
    void *FillOpacityMask;
    void(STDMETHODCALLTYPE *DrawBitmap)(hero_d2_target *me, hero_d2_unknown *bitmap, const hero_d2_rect *box, FLOAT opacity, UINT32 interpolation, const hero_d2_rect *source);
    void(STDMETHODCALLTYPE *DrawText)(hero_d2_target *me, const WCHAR *text, UINT32 count, hero_dw_format *format, const hero_d2_rect *box, hero_d2_unknown *brush, UINT32 options, UINT32 measuring);
    void *DrawTextLayout;
    void *DrawGlyphRun;
    void(STDMETHODCALLTYPE *SetTransform)(hero_d2_target *me, const hero_d2_matrix *transform);
    void *GetTransform;
    void(STDMETHODCALLTYPE *SetAntialiasMode)(hero_d2_target *me, UINT32 mode);
    void *GetAntialiasMode;
    void(STDMETHODCALLTYPE *SetTextAntialiasMode)(hero_d2_target *me, UINT32 mode);
    void *GetTextAntialiasMode;
    void *SetTextRenderingParams;
    void *GetTextRenderingParams;
    void *SetTags;
    void *GetTags;
    void(STDMETHODCALLTYPE *PushLayer)(hero_d2_target *me, const hero_d2_layer_parameters *parameters, hero_d2_unknown *layer);
    void(STDMETHODCALLTYPE *PopLayer)(hero_d2_target *me);
    void *Flush;
    void *SaveDrawingState;
    void *RestoreDrawingState;
    void *PushAxisAlignedClip;
    void *PopAxisAlignedClip;
    void(STDMETHODCALLTYPE *Clear)(hero_d2_target *me, const hero_d2_color *color);
    void(STDMETHODCALLTYPE *BeginDraw)(hero_d2_target *me);
    HRESULT(STDMETHODCALLTYPE *EndDraw)(hero_d2_target *me, UINT64 *tag1, UINT64 *tag2);
    void *GetPixelFormat;
    void(STDMETHODCALLTYPE *SetDpi)(hero_d2_target *me, FLOAT x, FLOAT y);
    void *GetDpi;
    void *GetSize;
    void *GetPixelSize;
    void *GetMaximumBitmapSize;
    void *IsSupported;
    HRESULT(STDMETHODCALLTYPE *BindDC)(hero_d2_target *me, HDC dc, const RECT *area);
} hero_d2_target_vtbl;
struct hero_d2_target {
    const hero_d2_target_vtbl *lpVtbl;
};

/* ID2D1Factory. */
typedef struct hero_d2_factory hero_d2_factory;
typedef struct hero_d2_factory_vtbl {
    HERO_D2_UNKNOWN(hero_d2_factory);
    void *ReloadSystemMetrics;
    void *GetDesktopDpi;
    void *CreateRectangleGeometry;
    void *CreateRoundedRectangleGeometry;
    void *CreateEllipseGeometry;
    void *CreateGeometryGroup;
    HRESULT(STDMETHODCALLTYPE *CreateTransformedGeometry)(hero_d2_factory *me, hero_d2_geometry *source, const hero_d2_matrix *transform, hero_d2_geometry **transformed);
    HRESULT(STDMETHODCALLTYPE *CreatePathGeometry)(hero_d2_factory *me, hero_d2_path **path);
    HRESULT(STDMETHODCALLTYPE *CreateStrokeStyle)(hero_d2_factory *me, const hero_d2_stroke_properties *properties, const FLOAT *dashes, UINT32 dash_count, hero_d2_unknown **style);
    void *CreateDrawingStateBlock;
    void *CreateWicBitmapRenderTarget;
    void *CreateHwndRenderTarget;
    void *CreateDxgiSurfaceRenderTarget;
    HRESULT(STDMETHODCALLTYPE *CreateDCRenderTarget)(hero_d2_factory *me, const hero_d2_target_properties *properties, hero_d2_target **target);
} hero_d2_factory_vtbl;
struct hero_d2_factory {
    const hero_d2_factory_vtbl *lpVtbl;
};

/* IDWriteFontCollection. */
typedef struct hero_dw_fonts hero_dw_fonts;
typedef struct hero_dw_fonts_vtbl {
    HERO_D2_UNKNOWN(hero_dw_fonts);
    void *GetFontFamilyCount;
    void *GetFontFamily;
    HRESULT(STDMETHODCALLTYPE *FindFamilyName)(hero_dw_fonts *me, const WCHAR *family, UINT32 *index, BOOL *exists);
} hero_dw_fonts_vtbl;
struct hero_dw_fonts {
    const hero_dw_fonts_vtbl *lpVtbl;
};

/* IDWriteTextFormat. */
typedef struct hero_dw_format_vtbl {
    HERO_D2_UNKNOWN(hero_dw_format);
    void *SetTextAlignment;
    void *SetParagraphAlignment;
    HRESULT(STDMETHODCALLTYPE *SetWordWrapping)(hero_dw_format *me, UINT32 wrapping);
} hero_dw_format_vtbl;
struct hero_dw_format {
    const hero_dw_format_vtbl *lpVtbl;
};

/* IDWriteFactory. */
typedef struct hero_dw_factory hero_dw_factory;
typedef struct hero_dw_factory_vtbl {
    HERO_D2_UNKNOWN(hero_dw_factory);
    HRESULT(STDMETHODCALLTYPE *GetSystemFontCollection)(hero_dw_factory *me, hero_dw_fonts **fonts, BOOL check);
    void *CreateCustomFontCollection;
    void *RegisterFontCollectionLoader;
    void *UnregisterFontCollectionLoader;
    void *CreateFontFileReference;
    void *CreateCustomFontFileReference;
    void *CreateFontFace;
    void *CreateRenderingParams;
    void *CreateMonitorRenderingParams;
    void *CreateCustomRenderingParams;
    void *RegisterFontFileLoader;
    void *UnregisterFontFileLoader;
    HRESULT(STDMETHODCALLTYPE *CreateTextFormat)(hero_dw_factory *me, const WCHAR *family, hero_dw_fonts *fonts, UINT32 weight, UINT32 style, UINT32 stretch, FLOAT size, const WCHAR *locale, hero_dw_format **format);
} hero_dw_factory_vtbl;
struct hero_dw_factory {
    const hero_dw_factory_vtbl *lpVtbl;
};

#if defined(__MINGW32__)
/* The second reading: MinGW-w64's declarations of the same interfaces. */
#define HERO_D2_SAME_SLOT(mine, theirs, method, their_method) _Static_assert(offsetof(mine, method) == offsetof(theirs, their_method), "the slot of " #method " is MinGW-w64's")
HERO_D2_SAME_SLOT(hero_d2_factory_vtbl, ID2D1FactoryVtbl, Release, Base.Release);
HERO_D2_SAME_SLOT(hero_d2_factory_vtbl, ID2D1FactoryVtbl, CreateTransformedGeometry, CreateTransformedGeometry);
HERO_D2_SAME_SLOT(hero_d2_factory_vtbl, ID2D1FactoryVtbl, CreatePathGeometry, CreatePathGeometry);
HERO_D2_SAME_SLOT(hero_d2_factory_vtbl, ID2D1FactoryVtbl, CreateStrokeStyle, CreateStrokeStyle);
HERO_D2_SAME_SLOT(hero_d2_factory_vtbl, ID2D1FactoryVtbl, CreateDCRenderTarget, CreateDCRenderTarget);
HERO_D2_SAME_SLOT(hero_d2_geometry_vtbl, ID2D1GeometryVtbl, CombineWithGeometry, CombineWithGeometry);
HERO_D2_SAME_SLOT(hero_d2_geometry_vtbl, ID2D1GeometryVtbl, Widen, Widen);
HERO_D2_SAME_SLOT(hero_d2_path_vtbl, ID2D1PathGeometryVtbl, CombineWithGeometry, Base.CombineWithGeometry);
HERO_D2_SAME_SLOT(hero_d2_path_vtbl, ID2D1PathGeometryVtbl, Open, Open);
HERO_D2_SAME_SLOT(hero_d2_sink_vtbl, ID2D1GeometrySinkVtbl, SetFillMode, Base.SetFillMode);
HERO_D2_SAME_SLOT(hero_d2_sink_vtbl, ID2D1GeometrySinkVtbl, BeginFigure, Base.BeginFigure);
HERO_D2_SAME_SLOT(hero_d2_sink_vtbl, ID2D1GeometrySinkVtbl, EndFigure, Base.EndFigure);
HERO_D2_SAME_SLOT(hero_d2_sink_vtbl, ID2D1GeometrySinkVtbl, Close, Base.Close);
HERO_D2_SAME_SLOT(hero_d2_sink_vtbl, ID2D1GeometrySinkVtbl, AddLine, AddLine);
HERO_D2_SAME_SLOT(hero_d2_sink_vtbl, ID2D1GeometrySinkVtbl, AddBezier, AddBezier);
HERO_D2_SAME_SLOT(hero_d2_sink_vtbl, ID2D1GeometrySinkVtbl, AddQuadraticBezier, AddQuadraticBezier);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, CreateBitmap, Base.CreateBitmap);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, CreateSolidColorBrush, Base.CreateSolidColorBrush);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, CreateLayer, Base.CreateLayer);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, DrawGeometry, Base.DrawGeometry);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, FillGeometry, Base.FillGeometry);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, DrawBitmap, Base.DrawBitmap);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, DrawText, Base.DrawText);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, SetTransform, Base.SetTransform);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, SetAntialiasMode, Base.SetAntialiasMode);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, SetTextAntialiasMode, Base.SetTextAntialiasMode);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, PushLayer, Base.PushLayer);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, PopLayer, Base.PopLayer);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, Clear, Base.Clear);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, BeginDraw, Base.BeginDraw);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, EndDraw, Base.EndDraw);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, SetDpi, Base.SetDpi);
HERO_D2_SAME_SLOT(hero_d2_target_vtbl, ID2D1DCRenderTargetVtbl, BindDC, BindDC);
HERO_D2_SAME_SLOT(hero_dw_factory_vtbl, IDWriteFactoryVtbl, Release, Release);
HERO_D2_SAME_SLOT(hero_dw_factory_vtbl, IDWriteFactoryVtbl, GetSystemFontCollection, GetSystemFontCollection);
HERO_D2_SAME_SLOT(hero_dw_factory_vtbl, IDWriteFactoryVtbl, CreateTextFormat, CreateTextFormat);
HERO_D2_SAME_SLOT(hero_dw_fonts_vtbl, IDWriteFontCollectionVtbl, FindFamilyName, FindFamilyName);
HERO_D2_SAME_SLOT(hero_dw_format_vtbl, IDWriteTextFormatVtbl, SetWordWrapping, SetWordWrapping);
_Static_assert(sizeof(hero_d2_layer_parameters) == sizeof(D2D1_LAYER_PARAMETERS) && offsetof(hero_d2_layer_parameters, geometric_mask) == offsetof(D2D1_LAYER_PARAMETERS, geometricMask) && offsetof(hero_d2_layer_parameters, mask_transform) == offsetof(D2D1_LAYER_PARAMETERS, maskTransform) && offsetof(hero_d2_layer_parameters, opacity_brush) == offsetof(D2D1_LAYER_PARAMETERS, opacityBrush) && offsetof(hero_d2_layer_parameters, layer_options) == offsetof(D2D1_LAYER_PARAMETERS, layerOptions), "D2D1_LAYER_PARAMETERS is MinGW-w64's");
_Static_assert(sizeof(hero_d2_target_properties) == sizeof(D2D1_RENDER_TARGET_PROPERTIES) && offsetof(hero_d2_target_properties, dpi_x) == offsetof(D2D1_RENDER_TARGET_PROPERTIES, dpiX) && offsetof(hero_d2_target_properties, min_level) == offsetof(D2D1_RENDER_TARGET_PROPERTIES, minLevel), "D2D1_RENDER_TARGET_PROPERTIES is MinGW-w64's");
_Static_assert(sizeof(hero_d2_bitmap_properties) == sizeof(D2D1_BITMAP_PROPERTIES) && sizeof(hero_d2_stroke_properties) == sizeof(D2D1_STROKE_STYLE_PROPERTIES) && offsetof(hero_d2_stroke_properties, miter_limit) == offsetof(D2D1_STROKE_STYLE_PROPERTIES, miterLimit), "the properties are MinGW-w64's");
_Static_assert(sizeof(hero_d2_matrix) == sizeof(D2D1_MATRIX_3X2_F) && sizeof(hero_d2_bezier) == sizeof(D2D1_BEZIER_SEGMENT) && sizeof(hero_d2_quadratic) == sizeof(D2D1_QUADRATIC_BEZIER_SEGMENT) && sizeof(hero_d2_color) == sizeof(D2D1_COLOR_F) && sizeof(hero_d2_size_u) == sizeof(D2D1_SIZE_U), "the structures are MinGW-w64's");
_Static_assert(HERO_D2_TARGET_SOFTWARE == D2D1_RENDER_TARGET_TYPE_SOFTWARE && HERO_D2_FORMAT_B8G8R8A8 == DXGI_FORMAT_B8G8R8A8_UNORM && HERO_D2_ALPHA_PREMULTIPLIED == D2D1_ALPHA_MODE_PREMULTIPLIED && HERO_D2_FILL_ALTERNATE == D2D1_FILL_MODE_ALTERNATE && HERO_D2_FILL_WINDING == D2D1_FILL_MODE_WINDING && HERO_D2_FIGURE_FILLED == D2D1_FIGURE_BEGIN_FILLED && HERO_D2_FIGURE_OPEN == D2D1_FIGURE_END_OPEN && HERO_D2_FIGURE_CLOSED == D2D1_FIGURE_END_CLOSED, "Direct2D's numbers are MinGW-w64's");
_Static_assert(HERO_D2_CAP_FLAT == D2D1_CAP_STYLE_FLAT && HERO_D2_CAP_SQUARE == D2D1_CAP_STYLE_SQUARE && HERO_D2_CAP_ROUND == D2D1_CAP_STYLE_ROUND && HERO_D2_JOIN_BEVEL == D2D1_LINE_JOIN_BEVEL && HERO_D2_JOIN_ROUND == D2D1_LINE_JOIN_ROUND && HERO_D2_JOIN_MITER_OR_BEVEL == D2D1_LINE_JOIN_MITER_OR_BEVEL && HERO_D2_COMBINE_INTERSECT == D2D1_COMBINE_MODE_INTERSECT, "Direct2D's numbers are MinGW-w64's");
_Static_assert(HERO_D2_ANTIALIAS_PER_PRIMITIVE == D2D1_ANTIALIAS_MODE_PER_PRIMITIVE && HERO_D2_TEXT_ANTIALIAS_GRAYSCALE == D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE && HERO_D2_INTERPOLATION_LINEAR == D2D1_BITMAP_INTERPOLATION_MODE_LINEAR && HERO_D2_RECREATE_TARGET == D2DERR_RECREATE_TARGET && HERO_D2_FACTORY_SINGLE_THREADED == D2D1_FACTORY_TYPE_SINGLE_THREADED, "Direct2D's numbers are MinGW-w64's");
_Static_assert(HERO_DW_FACTORY_SHARED == DWRITE_FACTORY_TYPE_SHARED && HERO_DW_WEIGHT_NORMAL == DWRITE_FONT_WEIGHT_NORMAL && HERO_DW_WEIGHT_BOLD == DWRITE_FONT_WEIGHT_BOLD && HERO_DW_STYLE_NORMAL == DWRITE_FONT_STYLE_NORMAL && HERO_DW_STYLE_ITALIC == DWRITE_FONT_STYLE_ITALIC && HERO_DW_STRETCH_NORMAL == DWRITE_FONT_STRETCH_NORMAL && HERO_DW_NO_WRAP == DWRITE_WORD_WRAPPING_NO_WRAP, "DirectWrite's numbers are MinGW-w64's");
#endif

/* The interfaces asked of the two factories' functions: IID_ID2D1Factory
 * and IID_IDWriteFactory. */
static const GUID hero_ui_canvas_d2d_iid = {0x06152247, 0x6f50, 0x465a, {0x92, 0x45, 0x11, 0x8b, 0xfd, 0x3b, 0x60, 0x07}};
static const GUID hero_ui_canvas_dwrite_iid = {0xb859ee5a, 0xd838, 0x4b5b, {0xa2, 0xe8, 0x1a, 0xdc, 0x7d, 0x93, 0xdb, 0x48}};

typedef HRESULT(WINAPI *hero_ui_canvas_d2d_create)(UINT32 type, const GUID *iid, const void *options, void **factory);
typedef HRESULT(WINAPI *hero_ui_canvas_dwrite_create)(UINT32 type, const GUID *iid, void **factory);

#endif
