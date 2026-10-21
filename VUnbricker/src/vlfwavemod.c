#include <string.h>
#include <math.h>

#include <psptypes.h>
#include <pspmoduleinfo.h>
#include <pspgu.h>
#include <pspgum.h>

#include <kubridge.h>
#include <cfwmacros.h>
#include <vlfwavemod.h>

#define WAVE_COLS 16
#define WAVE_ROWS 9
#define WAVE_POINTS (WAVE_COLS * WAVE_ROWS)
#define WAVE_PI 3.1415927410125732421875f
#define WAVE_TAU 6.283185482025146484375f
#define WAVE_TURNS(t)  ((t) * WAVE_TAU)
#define WAVE_BACKGROUND_COUNT 12

static __typeof__(&sceGuShadeModel) vlfsceGuShadeModel;
#define sceGuShadeModel vlfsceGuShadeModel
static __typeof__(&sceGuDisable) vlfsceGuDisable;
#define sceGuDisable vlfsceGuDisable
static __typeof__(&sceGuFrontFace) vlfsceGuFrontFace;
#define sceGuFrontFace vlfsceGuFrontFace
static __typeof__(&sceGuPatchPrim) vlfsceGuPatchPrim;
#define sceGuPatchPrim vlfsceGuPatchPrim
static __typeof__(&sceGuPatchDivide) vlfsceGuPatchDivide;
#define sceGuPatchDivide vlfsceGuPatchDivide
static __typeof__(&sceGuDepthMask) vlfsceGuDepthMask;
#define sceGuDepthMask vlfsceGuDepthMask
static __typeof__(&sceGuEnable) vlfsceGuEnable;
#define sceGuEnable vlfsceGuEnable
static __typeof__(&sceGuLight) vlfsceGuLight;
#define sceGuLight vlfsceGuLight
static __typeof__(&sceGuLightMode) vlfsceGuLightMode;
#define sceGuLightMode vlfsceGuLightMode
static __typeof__(&sceGuColorMaterial) vlfsceGuColorMaterial;
#define sceGuColorMaterial vlfsceGuColorMaterial
static __typeof__(&sceGuMaterial) vlfsceGuMaterial;
#define sceGuMaterial vlfsceGuMaterial
static __typeof__(&sceGuSpecular) vlfsceGuSpecular;
#define sceGuSpecular vlfsceGuSpecular
static __typeof__(&sceGuAmbient) vlfsceGuAmbient;
#define sceGuAmbient vlfsceGuAmbient
static __typeof__(&sceGuBlendFunc) vlfsceGuBlendFunc;
#define sceGuBlendFunc vlfsceGuBlendFunc
static __typeof__(&sceGuTexFunc) vlfsceGuTexFunc;
#define sceGuTexFunc vlfsceGuTexFunc
static __typeof__(&sceGuTexFilter) vlfsceGuTexFilter;
#define sceGuTexFilter vlfsceGuTexFilter
static __typeof__(&sceGuTexMode) vlfsceGuTexMode;
#define sceGuTexMode vlfsceGuTexMode
static __typeof__(&sceGuTexWrap) vlfsceGuTexWrap;
#define sceGuTexWrap vlfsceGuTexWrap
static __typeof__(&sceGuTexScale) vlfsceGuTexScale;
#define sceGuTexScale vlfsceGuTexScale
static __typeof__(&sceGuTexOffset) vlfsceGuTexOffset;
#define sceGuTexOffset vlfsceGuTexOffset
static __typeof__(&sceGuTexImage) vlfsceGuTexImage;
#define sceGuTexImage vlfsceGuTexImage
static __typeof__(&sceGuTexFlush) vlfsceGuTexFlush;
#define sceGuTexFlush vlfsceGuTexFlush
static __typeof__(&sceGuTexMapMode) vlfsceGuTexMapMode;
#define sceGuTexMapMode vlfsceGuTexMapMode
static __typeof__(&sceGumMatrixMode) vlfsceGumMatrixMode;
#define sceGumMatrixMode vlfsceGumMatrixMode
static __typeof__(&sceGumPushMatrix) vlfsceGumPushMatrix;
#define sceGumPushMatrix vlfsceGumPushMatrix
static __typeof__(&sceGumLoadIdentity) vlfsceGumLoadIdentity;
#define sceGumLoadIdentity vlfsceGumLoadIdentity
static __typeof__(&sceGumOrtho) vlfsceGumOrtho;
#define sceGumOrtho vlfsceGumOrtho
static __typeof__(&sceGumTranslate) vlfsceGumTranslate;
#define sceGumTranslate vlfsceGumTranslate
static __typeof__(&sceGumDrawSpline) vlfsceGumDrawSpline;
#define sceGumDrawSpline vlfsceGumDrawSpline
static __typeof__(&sceGumPopMatrix) vlfsceGumPopMatrix;
#define sceGumPopMatrix vlfsceGumPopMatrix
static __typeof__(&sceGuGetMemory) vlfsceGuGetMemory;
#define sceGuGetMemory vlfsceGuGetMemory
static __typeof__(&sceGuSync) vlfsceGuSync;
#define sceGuSync vlfsceGuSync
static __typeof__(&sceGuPixelMask) vlfsceGuPixelMask;
#define sceGuPixelMask vlfsceGuPixelMask

typedef struct WaveColor
{
    float r, g, b, a;
} WaveColor;

typedef struct WaveOscillator
{
    float bias, amplitude, velocity, frequency, phase;
} WaveOscillator;

typedef struct WaveVertex
{
    float u, v;
    u32 color;
    float x, y, z;
} WaveVertex;

typedef struct __attribute__((aligned(16))) WaveState
{
    WaveVertex surface[2][WAVE_POINTS] __attribute__((aligned(16)));
    u32 texture[32 * 64] __attribute__((aligned(16)));
    WaveOscillator slow;
    WaveOscillator primary[3];
    WaveOscillator amplitudeMod[3];
    WaveOscillator frequencyMod[3];
    WaveOscillator velocityMod[3];
    WaveOscillator ripple[4];
    float sectionHeightFactor[9];
    float sectionDepthFactor[9];
    WaveColor color[2];
    float amplitude;
    float speed;
    float transition;
    float yOffset;
    float opacity;
    double accumulator;
    int textureVariant;
} WaveState;

static u32 vlfBaseAddr;
static u32 initialized;

static WaveState waveState;
static u64 lastTime;

// VLF does not have sceGuPatchFrontFace
static void guPatchFrontFace(unsigned mode)
{
    if (mode)
        sceGuEnable(GU_PATCH_FACE);
    else
        sceGuDisable(GU_PATCH_FACE);
}
#define sceGuPatchFrontFace guPatchFrontFace

// VLF+0x159CC encodes lu | (lv << 8), modern PSPSDK uses (lu << 8) | lv.
#undef sceGuTexMapMode
static void vlfTextMapModeFix(int mode, unsigned lu, unsigned lv)
{
    vlfsceGuTexMapMode(mode, lv, lu);
}
#define sceGuTexMapMode vlfTextMapModeFix

static const unsigned char waveAlpha[512] = {
    217, 217, 217, 217, 217, 217, 217, 217, 217, 217, 215, 195, 141,  94,  80,  76,
    217, 217, 217, 217, 217, 217, 217, 217, 217, 212, 158,  82,  65,  64,  64,  64,
    217, 217, 217, 217, 217, 217, 217, 217, 209, 128,  68,  64,  64,  64,  64,  64,
    217, 217, 217, 217, 217, 217, 217, 212, 122,  64,  64,  64,  64,  64,  64,  64,
    217, 217, 217, 217, 217, 217, 215, 137,  65,  64,  64,  64,  64,  64,  64,  64,
    217, 217, 217, 217, 217, 217, 173,  66,  64,  64,  64,  64,  64,  64,  64,  64,
    217, 217, 217, 217, 217, 206,  77,  65,  65,  65,  65,  65,  65,  65,  65,  65,
    217, 217, 217, 217, 216, 125,  66,  66,  66,  66,  66,  66,  66,  66,  66,  66,
    217, 217, 217, 217, 197,  68,  67,  67,  67,  67,  67,  67,  67,  67,  67,  67,
    218, 218, 218, 218, 114,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,  68,
    218, 218, 218, 203,  74,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,  70,
    219, 219, 219, 137,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
    220, 220, 216,  77,  75,  75,  75,  75,  75,  75,  75,  75,  75,  75,  75,  75,
    221, 221, 187,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,  78,
    221, 221, 124,  81,  81,  81,  81,  81,  81,  81,  81,  81,  81,  81,  81,  81,
    222, 219,  93,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,  84,
    223, 202,  87,  87,  87,  87,  87,  87,  87,  87,  87,  87,  87,  87,  87,  87,
    221, 156,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,  90,
    218, 115,  93,  93,  93,  93,  93,  93,  93,  93,  93,  93,  93,  93,  93,  93,
    214, 104,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,  96,
    203,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,  99,
    182, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102,
    154, 105, 108, 105, 105, 108, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105,
    130, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108,
    130, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108,
    130, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108,
    130, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108,
    109, 109, 109, 109, 109, 108, 108, 108, 109, 109, 109, 108, 109, 108, 108, 109,
    109, 109, 109, 109, 109, 108, 108, 108, 109, 109, 109, 108, 109, 108, 108, 109,
    109, 109, 109, 109, 109, 108, 108, 108, 109, 109, 109, 108, 109, 108, 108, 109,
    109, 109, 109, 109, 109, 108, 108, 108, 109, 109, 109, 108, 109, 108, 108, 109,
    109, 109, 109, 109, 109, 108, 108, 108, 109, 109, 109, 108, 109, 108, 108, 109,
};

static const WaveColor waveBgColours[2][WAVE_BACKGROUND_COUNT][2] = {
    {
        { {0.24f, 0.24f, 0.23f, 0.60f}, {0.24f, 0.24f, 0.23f, 0.90f} },
        { {0.24f, 0.30f, 0.00f, 0.60f}, {0.24f, 0.30f, 0.00f, 0.90f} },
        { {0.50f, 0.26f, 0.03f, 0.60f}, {0.50f, 0.26f, 0.03f, 0.90f} },
        { {0.32f, 0.55f, 0.16f, 0.60f}, {0.32f, 0.55f, 0.16f, 0.90f} },
        { {0.60f, 0.47f, 0.16f, 0.60f}, {0.60f, 0.47f, 0.16f, 0.90f} },
        { {0.72f, 0.42f, 0.24f, 0.60f}, {0.72f, 0.42f, 0.24f, 0.90f} },
        { {0.52f, 0.42f, 0.18f, 0.60f}, {0.52f, 0.42f, 0.18f, 0.90f} },
        { {0.38f, 0.43f, 0.45f, 0.60f}, {0.38f, 0.43f, 0.45f, 0.90f} },
        { {0.74f, 0.22f, 0.38f, 0.60f}, {0.74f, 0.22f, 0.38f, 0.90f} },
        { {0.20f, 0.38f, 0.00f, 0.60f}, {0.20f, 0.38f, 0.00f, 0.90f} },
        { {0.51f, 0.33f, 0.05f, 0.60f}, {0.51f, 0.33f, 0.05f, 0.90f} },
        { {0.74f, 0.76f, 0.10f, 0.60f}, {0.74f, 0.76f, 0.10f, 0.90f} },
    },
    {
        { {0.24f, 0.24f, 0.23f, 0.60f}, {0.24f, 0.24f, 0.23f, 0.90f} },
        { {0.22f, 0.26f, 0.00f, 0.60f}, {0.22f, 0.26f, 0.00f, 0.90f} },
        { {0.56f, 0.38f, 0.06f, 0.60f}, {0.56f, 0.38f, 0.06f, 0.90f} },
        { {0.32f, 0.55f, 0.16f, 0.60f}, {0.32f, 0.55f, 0.16f, 0.90f} },
        { {0.60f, 0.47f, 0.16f, 0.60f}, {0.60f, 0.47f, 0.16f, 0.90f} },
        { {0.72f, 0.42f, 0.24f, 0.60f}, {0.72f, 0.42f, 0.24f, 0.90f} },
        { {0.60f, 0.48f, 0.20f, 0.60f}, {0.60f, 0.48f, 0.20f, 0.90f} },
        { {0.38f, 0.43f, 0.45f, 0.60f}, {0.38f, 0.43f, 0.45f, 0.90f} },
        { {0.74f, 0.22f, 0.38f, 0.60f}, {0.74f, 0.22f, 0.38f, 0.90f} },
        { {0.10f, 0.48f, 0.18f, 0.60f}, {0.10f, 0.48f, 0.18f, 0.90f} },
        { {0.80f, 0.34f, 0.05f, 0.60f}, {0.80f, 0.34f, 0.05f, 0.90f} },
        { {0.29f, 0.50f, 0.12f, 0.60f}, {0.29f, 0.50f, 0.12f, 0.90f} },
    },
};

static float clamp(float x, float low, float high)
{
    return x < low ? low : (x > high ? high : x);
}

static u32 clampToByte(float x)
{
    return (u32)clamp(x, 0.0f, 255.0f);
}

static float waveEval(const WaveOscillator *o, float x)
{
    return o->bias + o->amplitude * sinf(o->phase - x * o->frequency);
}

static void waveAdvance(WaveOscillator *o, float delta)
{
    o->phase += o->velocity * delta;

    while (o->phase > WAVE_TAU)
        o->phase -= WAVE_TAU;
    while (o->phase < -WAVE_TAU)
        o->phase += WAVE_TAU;
}

static void waveGenerateTexture(WaveState *w, int variant)
{
    int x, y;
    w->textureVariant = variant == 1 ? 1 : 0;
    for (y = 0; y < 64; ++y)
    {
        for (x = 0; x < 32; ++x)
        {
            unsigned alpha;
            if (!w->textureVariant)
            {
                int xx = x < 16 ? x : 31 - x;
                int yy = y < 32 ? y : 63 - y;
                alpha = waveAlpha[yy * 16 + xx];
            }
            else
            {
                float xx = (x - 16) * (1.0f / 16.0f);
                float yy = (y - 32) * (1.0f / 32.0f);
                float radius = sqrtf(xx * xx + yy * yy);
                float t;
                if (radius > 1.0f)
                    radius = 1.0f;
                t = 1.0f - sqrtf(1.0f - radius * radius);
                alpha = (unsigned)(((t + t * t) * 0.5f * 255.0f) * 0.9f);
            }
            w->texture[y * 32 + x] = 0x00ffffff | (alpha << 24);
        }
    }

    sceKernelDcacheWritebackRange(w->texture, sizeof(w->texture));
}

void waveInit(WaveState *w)
{
    lastTime = 0;

    float angles[9] = { 0, 9, 16, 25, 36, 49, 64, 80, 90 };

     WaveOscillator amplitudeMod[3] = {
        { 60.0f, 10.0f, 1.0f/60.0f, 0, 0 },
        { 60.0f, 10.0f, 1.0f/41.0f, 0, 0 },
        { 20.0f, 10.0f, 1.0f/63.0f, 0, 0 }
    };
    
    WaveOscillator velocityMod[3] = {
        { WAVE_TURNS(0.029f), 0.4f * WAVE_TURNS(0.029f), 1.0f/32.0f, 0, 0 },
        { WAVE_TURNS(0.010f), 0.4f * WAVE_TURNS(0.010f), 1.0f/15.0f, 0, 0 },
        { WAVE_TURNS(0.012f), 0.4f * WAVE_TURNS(0.012f), 1.0f/47.0f, 0, 0 }
    };

    WaveOscillator frequencyMod[3] = {
        { WAVE_TURNS(0.70f), 0.1f * WAVE_TURNS(0.70f), 1.0f/45.0f, 0, 0 },
        { WAVE_TURNS(0.59f), 0.4f * WAVE_TURNS(0.59f), 1.0f/42.0f, 0, 0 },
        { WAVE_TURNS(0.29f), 0.4f * WAVE_TURNS(0.29f), 1.0f/18.0f, 0, 0 }
    };
    
    memset(w, 0, sizeof(*w));
    
    w->slow = (WaveOscillator){ 1.05f, 0.05f, 1.0f/60.0f, 0, 0 };
    
    memcpy(w->amplitudeMod, amplitudeMod, sizeof(amplitudeMod));
    memcpy(w->frequencyMod, frequencyMod, sizeof(frequencyMod));
    
    w->ripple[0] = (WaveOscillator){ 0.0f, 1.0f, 0.35f, 0.0f, 0 };
    w->ripple[1] = (WaveOscillator){ 0.0f, 2.5f, 1.0f,  0.6f, 0 };
    w->ripple[2] = (WaveOscillator){ 0.0f, 5.5f, 1.0f,  0.9f, 0 };
    w->ripple[3] = (WaveOscillator){ 0.0f, 5.0f, 0.3f,  0.6f, 0 };
    
    w->amplitude = w->speed = w->transition = w->opacity = 1.0f;
    
    w->color[0] = (WaveColor){ 0.24f, 0.24f, 0.23f, 0.60f };
    w->color[1] = (WaveColor){ 0.24f, 0.24f, 0.23f, 0.90f };
    
    int i;
    for (i = 0; i < 9; ++i)
    {
        float a = (angles[i] * WAVE_PI) / 180.0f;
        w->sectionHeightFactor[i] = cosf(a);
        w->sectionDepthFactor[i] = sinf(a);
    }
    
    for (i = 0; i < WAVE_POINTS; ++i)
    {
        float x = (((i % 16) - 7.5f) * 480.0f) / 13.0f;
        w->surface[0][i].x = w->surface[1][i].x = x;
    }
    
    memcpy(w->velocityMod, velocityMod, sizeof(velocityMod));
    
    waveGenerateTexture(w, 0);
}

static void waveTick(WaveState *w)
{
    float delta;
    int i;
    
    waveAdvance(&w->slow, 1.0f / 60.0f);
    
    delta = ((waveEval(&w->slow, 0) * 1.5f) / 60.0f) * w->speed;
    
    for (i = 0; i < 4; ++i) {
        waveAdvance(&w->ripple[i], delta);
    }
    
    for (i = 0; i < 3; ++i)
    {
        waveAdvance(&w->amplitudeMod[i], delta);
        waveAdvance(&w->frequencyMod[i], delta);
        waveAdvance(&w->velocityMod[i], delta);
        
        w->primary[i].amplitude = waveEval(&w->amplitudeMod[i], 0);
        w->primary[i].velocity = waveEval(&w->velocityMod[i], 0);
        w->primary[i].frequency = waveEval(&w->frequencyMod[i], 0);
        
        waveAdvance(&w->primary[i], delta);
    }
}

void waveUpdate(WaveState *w, float elapsedSeconds)
{
    const double step = 1.0 / 60.0;
    
    if (!isfinite(elapsedSeconds) || elapsedSeconds <= 0)
        return;
    
    if (elapsedSeconds > 0.25f)
        elapsedSeconds = 0.25f;
    
    w->accumulator += elapsedSeconds;
    
    while (w->accumulator >= step)
    {
        waveTick(w);
        w->accumulator -= step;
    }
}

static float waveTransitionHeight(float y, float q, float threshold)
{
    if (q < threshold)
        return 136.0f;

    if (q <= threshold + 1.0f)
    {
        float t = q - threshold;
        return y * t + ((1.0f - t) * 272.0f) * 0.5f;
    }

    return y;
}

void waveBuildVertices(WaveState *w)
{
    float heights[2][16];

    static const unsigned rowAlpha[9] = { 16, 255, 255, 255, 255, 255, 255, 255, 255 };
    
    float threshold = 1.0f - 2.0f * w->transition;
    float amp = w->amplitude * 0.5f;
    
    int i, row, surface;
    
    for (i = 0; i < 16; ++i)
    {
        float x = i;
        float u = x * (1.0f / 16.0f);
        float q = 1.0f - u;
        float h = waveEval(&w->primary[0], u);
        
        h += waveEval(&w->primary[1], u) * u;
        h += waveEval(&w->primary[2], u) * q;
        h = amp * h + 110.0f;
        h += waveEval(&w->ripple[2], x) * (u * 0.9f + 0.1f);
        h += waveEval(&w->ripple[3], x);
        heights[0][i] = waveTransitionHeight(h, q, threshold) + w->yOffset;

        u = x * (1.0f / 16.0f) + 0.05f;
        q = 1.0f - u;
        h = waveEval(&w->primary[0], u);
        h += waveEval(&w->primary[1], u) * u;
        h += waveEval(&w->primary[2], u) * q;
        h = amp * h + 110.0f;
        h += waveEval(&w->ripple[1], x) * ((x * (1.0f / 16.0f)) * 0.9f + 0.1f);
        h *= 1.0f + waveEval(&w->ripple[0], 0) * 0.03f;
        heights[1][i] = waveTransitionHeight(h, q, threshold) + w->yOffset;
    }
    
    for (surface = 0; surface < 2; ++surface)
    {
        const WaveColor *c = &w->color[surface];
        u32 rgb = clampToByte(c->r * 255.0f) | (clampToByte(c->g * 255.0f) << 8) | (clampToByte(c->b * 255.0f) << 16);
        for (row = 0; row < 9; ++row)
        {
            float cs = w->sectionHeightFactor[row];
            float sn = w->sectionDepthFactor[row];
            u32 color = rgb | (clampToByte(rowAlpha[row] * c->a) << 24);
            for (i = 0; i < 16; ++i)
            {
                WaveVertex *v = &w->surface[surface][row * 16 + i];
                float h = heights[surface][i];
                if (surface == 0)
                {
                    v->u = i * (1.0f / 16.0f);
                    v->v = row / 9.0f;
                }
                v->color = color;
                v->y = cs * (h + (1.0f - cs) * (h - 90.0f));
                v->z = sn * 90.0f;
            }
        }
    }
}

int updateWaveColours(WaveState *w, unsigned int index, unsigned int bank)
{
    if (!w || index >= WAVE_BACKGROUND_COUNT || bank > 1)
        return -1;

    w->color[0] = waveBgColours[bank][index][0];
    w->color[1] = waveBgColours[bank][index][1];
    return 0;
}

static void waveDraw(WaveState *w)
{
    WaveVertex *vertices;
    void *allocation;
    u32 alpha;

    ScePspFVector3 lightU = { -1, 0, 0 };
    ScePspFVector3 lightV = { 0, -1, 0 };
    ScePspFVector3 translation = { 0, -136, 0 };
    ScePspFVector3 camTranslation = { 0, 0, -500 };
    
    alpha = clampToByte(w->opacity * 255.0f);
    if (alpha < 7)
        return;
   
    waveBuildVertices(w);
    
    allocation = sceGuGetMemory(sizeof(w->surface) + 15);
    if (!allocation)
        return;
    
    vertices = (WaveVertex *)(((u32)allocation + 15) & ~(u32)15);
    memcpy(vertices, w->surface, sizeof(w->surface));
    sceKernelDcacheWritebackRange(vertices, sizeof(w->surface));

    sceGuShadeModel(GU_SMOOTH);
    sceGuDisable(GU_CULL_FACE);
    sceGuFrontFace(GU_CW);
    sceGuPatchFrontFace(GU_CCW);
    sceGuDisable(GU_PATCH_CULL_FACE);
    sceGuPatchPrim(GU_TRIANGLE_STRIP);
    sceGuPatchDivide(4, 4);
    sceGuDisable(GU_DEPTH_TEST);
    sceGuDepthMask(GU_TRUE);
    sceGuDisable(GU_ALPHA_TEST);
    sceGuDisable(GU_STENCIL_TEST);
    sceGuDisable(GU_COLOR_TEST);
    sceGuDisable(GU_FOG);
    sceGuEnable(GU_CLIP_PLANES);
    sceGuEnable(GU_LIGHTING);
    sceGuDisable(GU_LIGHT0);
    sceGuDisable(GU_LIGHT1);
    sceGuDisable(GU_LIGHT2);
    sceGuDisable(GU_LIGHT3);
    sceGuLight(2, GU_DIRECTIONAL, GU_DIFFUSE, &lightU);
    sceGuLight(3, GU_DIRECTIONAL, GU_DIFFUSE, &lightV);
    sceGuLightMode(GU_SEPARATE_SPECULAR_COLOR);
    sceGuColorMaterial(GU_AMBIENT | GU_DIFFUSE | GU_SPECULAR);
    sceGuMaterial(GU_AMBIENT | GU_DIFFUSE | GU_SPECULAR, 0xffffffff);
    sceGuSpecular(0);
    sceGuAmbient(0x00ffffff | (alpha << 24));
    sceGuEnable(GU_BLEND);
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_FIX, 0, 0xffffffff);
    sceGuEnable(GU_TEXTURE_2D);
    sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
    sceGuTexFilter(GU_LINEAR, GU_LINEAR);
    sceGuTexMode(GU_PSM_8888, 0, 0, 0);
    sceGuTexWrap(GU_CLAMP, GU_CLAMP);
    sceGuTexScale(1, 1);
    sceGuTexOffset(0, 0);
    sceGuTexImage(0, 32, 64, 32, w->texture);
    sceGuTexFlush();
    sceGuTexMapMode(GU_ENVIRONMENT_MAP, 3, 2);
    sceGumMatrixMode(GU_PROJECTION);
    sceGumPushMatrix();
    sceGumLoadIdentity();
    sceGumOrtho(-240, 240, -136, 136, 0, 1100);
    sceGumMatrixMode(GU_VIEW);
    sceGumPushMatrix();
    sceGumLoadIdentity();
    sceGumTranslate(&camTranslation);
    sceGumMatrixMode(GU_MODEL);
    sceGumPushMatrix();
    sceGumLoadIdentity();
    sceGumTranslate(&translation);
    sceGumDrawSpline(GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF,
                     16, 9, 0, 3, NULL, vertices);
    sceGumDrawSpline(GU_TEXTURE_32BITF | GU_COLOR_8888 | GU_VERTEX_32BITF,
                     16, 9, 0, 3, NULL, vertices + WAVE_POINTS);
    sceGumPopMatrix();
    sceGumMatrixMode(GU_VIEW);
    sceGumPopMatrix();
    sceGumMatrixMode(GU_PROJECTION);
    sceGumPopMatrix();
    sceGumMatrixMode(GU_MODEL);
    sceGuTexMapMode(GU_TEXTURE_COORDS, 2, 3);
}

static void resetRenderState(void)
{
    sceGuDisable(GU_LIGHTING);
    sceGuDisable(GU_FOG);
    sceGuDisable(GU_CULL_FACE);
    sceGuDisable(GU_DEPTH_TEST);
    sceGuDepthMask(0);
    sceGuDisable(GU_ALPHA_TEST);
    sceGuPixelMask(0);
    sceGumMatrixMode(GU_MODEL);
    sceGumLoadIdentity();
}

static void vlfRenderWavePatched(void)
{
    u64 now = sceKernelGetSystemTimeWide();
    
    float elapsed = lastTime ? (now - lastTime) * 0.000001f : 1.0f / 60.0f;
    lastTime = now;

    u32 modelPtr = *(volatile u32 *)(vlfBaseAddr + 0xea18c);

    if (modelPtr)
    {
        waveUpdate(&waveState, elapsed);
        waveDraw(&waveState);
    }
    resetRenderState();
}

void updateWaveForBackground(u32 model, u32 index)
{
    u32 bank = model == 4 ? 1 : 0;
    updateWaveColours(&waveState, index, bank);
}

static void getVlfGuFunctions(u32 base)
{
    vlfsceGuShadeModel = (void *)(base + 0x15688);
    vlfsceGuDisable = (void *)(base + 0x17cdc);
    vlfsceGuFrontFace = (void *)(base + 0x15648);
    vlfsceGuPatchPrim = (void *)(base + 0x15430);
    vlfsceGuPatchDivide = (void *)(base + 0x15404);
    vlfsceGuDepthMask = (void *)(base + 0x15bc8);
    vlfsceGuEnable = (void *)(base + 0x14684);
    vlfsceGuLight = (void *)(base + 0x16b4c);
    vlfsceGuLightMode = (void *)(base + 0x156c8);
    vlfsceGuColorMaterial = (void *)(base + 0x154e4);
    vlfsceGuMaterial = (void *)(base + 0x17388);
    vlfsceGuSpecular = (void *)(base + 0x1561c);
    vlfsceGuAmbient = (void *)(base + 0x154a4);
    vlfsceGuBlendFunc = (void *)(base + 0x15d3c);
    vlfsceGuTexFunc = (void *)(base + 0x158dc);
    vlfsceGuTexFilter = (void *)(base + 0x158b0);
    vlfsceGuTexMode = (void *)(base + 0x15860);
    vlfsceGuTexWrap = (void *)(base + 0x1594c);
    vlfsceGuTexScale = (void *)(base + 0x1683c);
    vlfsceGuTexOffset = (void *)(base + 0x16940);
    vlfsceGuTexImage = (void *)(base + 0x17230);
    vlfsceGuTexFlush = (void *)(base + 0x15a70);
    vlfsceGuTexMapMode = (void *)(base + 0x159cc);
    vlfsceGumMatrixMode = (void *)(base + 0x11db8);
    vlfsceGumPushMatrix = (void *)(base + 0x121a8);
    vlfsceGumLoadIdentity = (void *)(base + 0x119a8);
    vlfsceGumOrtho = (void *)(base + 0x12a1c);
    vlfsceGumTranslate = (void *)(base + 0x11c2c);
    vlfsceGumDrawSpline = (void *)(base + 0x11f2c);
    vlfsceGumPopMatrix = (void *)(base + 0x11e10);
    vlfsceGuGetMemory = (void *)(base + 0x144c4);
    vlfsceGuSync = (void *)(base + 0x16290);
    vlfsceGuPixelMask = (void *)(base + 0x15bec);
}

int originalWavesInit()
{
    SceModule mod;

    int res = kuKernelFindModuleByName("VLF_Module", &mod);
    if (res < 0)
        return res;

    vlfBaseAddr = mod.text_addr;

    getVlfGuFunctions(vlfBaseAddr);
    waveInit(&waveState);

    REDIRECT_FUNCTION(vlfBaseAddr + 0x1d9c, vlfRenderWavePatched);

    sceKernelDcacheWritebackInvalidateAll();
    sceKernelIcacheInvalidateAll();

    initialized = 1;
    
    return 0;
}
