c:\Users\amity\OneDrive\Desktop\cat game\Cat_Clicker_Presentation.pptx#define SDL_MAIN_HANDLED

#if defined(__has_include)
#if __has_include(<SDL2/SDL.h>)
#include <SDL2/SDL.h>
#elif __has_include(<SDL.h>)
#include <SDL.h>
#endif
#else
#include <SDL.h>
#endif
#if defined(__has_include)
#if __has_include(<SDL2/SDL_mixer.h>)
#include <SDL2/SDL_mixer.h>
#elif __has_include(<SDL_mixer.h>)
#include <SDL_mixer.h>
#endif
#else
#include <SDL_mixer.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define HUD_HEIGHT 64
#define MAX_CATS 20

#define INITIAL_LIVES 3
#define INITIAL_SPAWN_INTERVAL 1200
#define MIN_SPAWN_INTERVAL 250

/* Level system */
#define MAX_LEVELS 10
#define CATS_PER_LEVEL 5

/* Theme modes */
#define THEME_DARK 0
#define THEME_LIGHT 1

typedef struct Cat {
    int x;
    int y;
    int width;
    int height;
    int type;   /* 0 = colorful, 1 = black */
    int active; /* 1 if visible */

    /* Extra fields for movement and color rendering. */
    float vx;
    float vy;
    SDL_Color color;
} Cat;

static SDL_Window *gWindow = NULL;
static SDL_Renderer *gRenderer = NULL;

/* Theme system */
static int gTheme = THEME_DARK;

/* Color theme structures */
typedef struct {
    SDL_Color background;
    SDL_Color hudBg;
    SDL_Color text;
    SDL_Color accentColor;
    SDL_Color houseColor;
    SDL_Color roofColor;
    SDL_Color windowLightColor;
    SDL_Color moonColor;
} ColorTheme;

/* Audio resources */
static Mix_Chunk *gSoundClick = NULL;
static Mix_Chunk *gSoundMeow = NULL;
static Mix_Chunk *gSoundHit = NULL;
static Mix_Chunk *gSoundGameOver = NULL;
static int gAudioEnabled = 0;

static Cat gCats[MAX_CATS];
static int gRunning = 1;
static int gGameOver = 0;

static int gScore = 0;
static int gLives = INITIAL_LIVES;

/* Level system */
static int gCurrentLevel = 1;
static int gCatsForNextLevel = CATS_PER_LEVEL;
static Mix_Chunk *gSoundLevelUp = NULL;

/* Game states */
#define STATE_THEME_SELECTION 0
#define STATE_PLAYING 1
#define STATE_GAME_OVER 2

static int gGameState = STATE_THEME_SELECTION;
static int gThemeSelected = 0; /* For theme selection menu */
static int gGameWon = 0; /* 1 if player won by filling screen with gray cats */
#define GRAY_CATS_WIN_THRESHOLD 8

static Uint32 gLastSpawnTime = 0;
static Uint32 gSpawnInterval = INITIAL_SPAWN_INTERVAL;
static Uint32 gGameStartTime = 0;
static float gSpeedMultiplier = 1.0f;

static int init(void);
static void handleEvents(void);
static void updateGame(float deltaTime);
static void render(void);
static void cleanup(void);

static void spawnCat(void);
static int checkClickCollision(int mouseX, int mouseY, const Cat *cat);
static void restartGame(void);

static void updateWindowTitle(void);
static void drawTextWithOutline(SDL_Renderer *renderer, const char *text, int x, int y, int scale, SDL_Color color, SDL_Color outlineColor);
static void drawText(SDL_Renderer *renderer, const char *text, int x, int y, int scale, SDL_Color color);
static void getGlyph(char c, unsigned char outRows[7]);
static void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius);
static void drawSun(SDL_Renderer *renderer);
static void drawClouds(SDL_Renderer *renderer, Uint32 time);
static void drawHUD(SDL_Renderer *renderer, ColorTheme theme);
static void initAudio(void);
static void cleanupAudio(void);
static Mix_Chunk *generateBeep(float frequency, float durationMs);
static Mix_Chunk *generateBoom(void);
static Mix_Chunk *generateGameOverSound(void);
static Mix_Chunk *generateLevelUpSound(void);
static Mix_Chunk *generateMeow(void);
static void playSound(Mix_Chunk *sound);
static void drawCat(SDL_Renderer *renderer, const Cat *cat, Uint32 time);
static void drawStarryBackground(SDL_Renderer *renderer, Uint32 time);
static void drawMoon(SDL_Renderer *renderer);

/* Get the current theme colors */
static ColorTheme getThemeColors(void) {
    ColorTheme light = {
        .background = {245, 245, 250, 255},      /* Light off-white */
        .hudBg = {230, 240, 250, 255},           /* Light blue-tinted */
        .text = {30, 35, 50, 255},               /* Dark gray-blue */
        .accentColor = {255, 140, 50, 255},      /* Orange */
        .houseColor = {200, 150, 100, 255},      /* Light brown */
        .roofColor = {155, 100, 50, 255},        /* Medium brown */
        .windowLightColor = {255, 200, 100, 255}, /* Warm yellow */
        .moonColor = {240, 240, 200, 255}        /* Pale moon */
    };
    
    ColorTheme dark = {
        .background = {18, 24, 32, 255},         /* Dark navy */
        .hudBg = {28, 37, 48, 255},              /* Slightly lighter navy */
        .text = {245, 245, 245, 255},            /* White */
        .accentColor = {255, 182, 72, 255},      /* Orange accent */
        .houseColor = {120, 80, 60, 255},        /* Brown */
        .roofColor = {100, 60, 40, 255},         /* Dark brown */
        .windowLightColor = {255, 255, 100, 255}, /* Yellow */
        .moonColor = {230, 230, 200, 255}        /* Cream */
    };
    
    return (gTheme == THEME_LIGHT) ? light : dark;
}

/* Draw the theme selection menu */
static void drawThemeSelection(SDL_Renderer *renderer) {
    SDL_Rect darkButton = {150, 250, 200, 80};
    SDL_Rect lightButton = {450, 250, 200, 80};
    SDL_Color darkBg = {18, 24, 32, 255};
    SDL_Color lightBg = {245, 245, 250, 255};
    SDL_Color darkText = {245, 245, 245, 255};
    SDL_Color lightText = {30, 35, 50, 255};
    SDL_Color accentOrange = {255, 182, 72, 255};
    SDL_Color accentOrange2 = {255, 140, 50, 255};

    /* Background: dark gradient suggestion with white center */
    SDL_SetRenderDrawColor(renderer, 40, 50, 70, 255);
    SDL_RenderClear(renderer);

    /* Title */
    drawText(renderer, "CHOOSE THEME", 200, 100, 4, darkText);

    /* Dark Mode Button */
    SDL_Color buttonBorderDark = (gThemeSelected == THEME_DARK) ? accentOrange : darkText;
    SDL_SetRenderDrawColor(renderer, buttonBorderDark.r, buttonBorderDark.g, buttonBorderDark.b, 255);
    SDL_RenderDrawRect(renderer, &darkButton);
    if (gThemeSelected == THEME_DARK) {
        SDL_RenderDrawRect(renderer, &(SDL_Rect){darkButton.x + 2, darkButton.y + 2, darkButton.w - 4, darkButton.h - 4});
    }
    
    SDL_SetRenderDrawColor(renderer, darkBg.r, darkBg.g, darkBg.b, 200);
    {
        SDL_Rect fillDark = {darkButton.x + 2, darkButton.y + 2, darkButton.w - 4, darkButton.h - 4};
        SDL_RenderFillRect(renderer, &fillDark);
    }
    drawText(renderer, "DARK MODE", 160, 270, 3, (gThemeSelected == THEME_DARK) ? accentOrange : darkText);

    /* Light Mode Button */
    SDL_Color lightModeBorder = (gThemeSelected == THEME_LIGHT) ? accentOrange2 : (SDL_Color){200, 200, 200, 255};
    SDL_SetRenderDrawColor(renderer, lightModeBorder.r, lightModeBorder.g, lightModeBorder.b, 255);
    SDL_RenderDrawRect(renderer, &lightButton);
    if (gThemeSelected == THEME_LIGHT) {
        SDL_RenderDrawRect(renderer, &(SDL_Rect){lightButton.x + 2, lightButton.y + 2, lightButton.w - 4, lightButton.h - 4});
    }
    
    SDL_SetRenderDrawColor(renderer, lightBg.r, lightBg.g, lightBg.b, 200);
    {
        SDL_Rect fillLight = {lightButton.x + 2, lightButton.y + 2, lightButton.w - 4, lightButton.h - 4};
        SDL_RenderFillRect(renderer, &fillLight);
    }
    drawText(renderer, "LIGHT MODE", 450, 270, 3, (gThemeSelected == THEME_LIGHT) ? accentOrange2 : lightText);

    /* Instructions */
    drawText(renderer, "CLICK OR PRESS D/L", 240, 430, 2, darkText);

    SDL_RenderPresent(renderer);
}


/*
 * Build a small 5x7 bitmap font for letters/digits used in the UI,
 * so the game can show score/lives without requiring SDL_ttf.
 */
static void getGlyph(char c, unsigned char outRows[7]) {
    int i;
    for (i = 0; i < 7; ++i) {
        outRows[i] = 0;
    }

    switch ((unsigned char)toupper((unsigned char)c)) {
        case '0':
            outRows[0] = 0x0E;
            outRows[1] = 0x11;
            outRows[2] = 0x13;
            outRows[3] = 0x15;
            outRows[4] = 0x19;
            outRows[5] = 0x11;
            outRows[6] = 0x0E;
            break;
        case '1':
            outRows[0] = 0x04;
            outRows[1] = 0x0C;
            outRows[2] = 0x04;
            outRows[3] = 0x04;
            outRows[4] = 0x04;
            outRows[5] = 0x04;
            outRows[6] = 0x0E;
            break;
        case '2':
            outRows[0] = 0x0E;
            outRows[1] = 0x11;
            outRows[2] = 0x01;
            outRows[3] = 0x02;
            outRows[4] = 0x04;
            outRows[5] = 0x08;
            outRows[6] = 0x1F;
            break;
        case '3':
            outRows[0] = 0x1E;
            outRows[1] = 0x01;
            outRows[2] = 0x01;
            outRows[3] = 0x0E;
            outRows[4] = 0x01;
            outRows[5] = 0x01;
            outRows[6] = 0x1E;
            break;
        case '4':
            outRows[0] = 0x02;
            outRows[1] = 0x06;
            outRows[2] = 0x0A;
            outRows[3] = 0x12;
            outRows[4] = 0x1F;
            outRows[5] = 0x02;
            outRows[6] = 0x02;
            break;
        case '5':
            outRows[0] = 0x1F;
            outRows[1] = 0x10;
            outRows[2] = 0x10;
            outRows[3] = 0x1E;
            outRows[4] = 0x01;
            outRows[5] = 0x01;
            outRows[6] = 0x1E;
            break;
        case '6':
            outRows[0] = 0x0E;
            outRows[1] = 0x10;
            outRows[2] = 0x10;
            outRows[3] = 0x1E;
            outRows[4] = 0x11;
            outRows[5] = 0x11;
            outRows[6] = 0x0E;
            break;
        case '7':
            outRows[0] = 0x1F;
            outRows[1] = 0x01;
            outRows[2] = 0x02;
            outRows[3] = 0x04;
            outRows[4] = 0x08;
            outRows[5] = 0x08;
            outRows[6] = 0x08;
            break;
        case '8':
            outRows[0] = 0x0E;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x0E;
            outRows[4] = 0x11;
            outRows[5] = 0x11;
            outRows[6] = 0x0E;
            break;
        case '9':
            outRows[0] = 0x0E;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x0F;
            outRows[4] = 0x01;
            outRows[5] = 0x01;
            outRows[6] = 0x0E;
            break;
        case 'A':
            outRows[0] = 0x0E;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x1F;
            outRows[4] = 0x11;
            outRows[5] = 0x11;
            outRows[6] = 0x11;
            break;
        case 'B':
            outRows[0] = 0x1E;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x1E;
            outRows[4] = 0x11;
            outRows[5] = 0x11;
            outRows[6] = 0x1E;
            break;
        case 'C':
            outRows[0] = 0x0E;
            outRows[1] = 0x11;
            outRows[2] = 0x10;
            outRows[3] = 0x10;
            outRows[4] = 0x10;
            outRows[5] = 0x11;
            outRows[6] = 0x0E;
            break;
        case 'D':
            outRows[0] = 0x1E;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x11;
            outRows[4] = 0x11;
            outRows[5] = 0x11;
            outRows[6] = 0x1E;
            break;
        case 'E':
            outRows[0] = 0x1F;
            outRows[1] = 0x10;
            outRows[2] = 0x10;
            outRows[3] = 0x1E;
            outRows[4] = 0x10;
            outRows[5] = 0x10;
            outRows[6] = 0x1F;
            break;
        case 'F':
            outRows[0] = 0x1F;
            outRows[1] = 0x10;
            outRows[2] = 0x10;
            outRows[3] = 0x1E;
            outRows[4] = 0x10;
            outRows[5] = 0x10;
            outRows[6] = 0x10;
            break;
        case 'G':
            outRows[0] = 0x0E;
            outRows[1] = 0x11;
            outRows[2] = 0x10;
            outRows[3] = 0x17;
            outRows[4] = 0x11;
            outRows[5] = 0x11;
            outRows[6] = 0x0E;
            break;
        case 'H':
            outRows[0] = 0x11;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x1F;
            outRows[4] = 0x11;
            outRows[5] = 0x11;
            outRows[6] = 0x11;
            break;
        case 'I':
            outRows[0] = 0x1F;
            outRows[1] = 0x04;
            outRows[2] = 0x04;
            outRows[3] = 0x04;
            outRows[4] = 0x04;
            outRows[5] = 0x04;
            outRows[6] = 0x1F;
            break;
        case 'J':
            outRows[0] = 0x0F;
            outRows[1] = 0x02;
            outRows[2] = 0x02;
            outRows[3] = 0x02;
            outRows[4] = 0x02;
            outRows[5] = 0x12;
            outRows[6] = 0x0C;
            break;
        case 'K':
            outRows[0] = 0x11;
            outRows[1] = 0x12;
            outRows[2] = 0x14;
            outRows[3] = 0x18;
            outRows[4] = 0x14;
            outRows[5] = 0x12;
            outRows[6] = 0x11;
            break;
        case 'L':
            outRows[0] = 0x10;
            outRows[1] = 0x10;
            outRows[2] = 0x10;
            outRows[3] = 0x10;
            outRows[4] = 0x10;
            outRows[5] = 0x10;
            outRows[6] = 0x1F;
            break;
        case 'M':
            outRows[0] = 0x11;
            outRows[1] = 0x1B;
            outRows[2] = 0x15;
            outRows[3] = 0x11;
            outRows[4] = 0x11;
            outRows[5] = 0x11;
            outRows[6] = 0x11;
            break;
        case 'N':
            outRows[0] = 0x11;
            outRows[1] = 0x19;
            outRows[2] = 0x15;
            outRows[3] = 0x15;
            outRows[4] = 0x13;
            outRows[5] = 0x11;
            outRows[6] = 0x11;
            break;
        case 'O':
            outRows[0] = 0x0E;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x11;
            outRows[4] = 0x11;
            outRows[5] = 0x11;
            outRows[6] = 0x0E;
            break;
        case 'P':
            outRows[0] = 0x1E;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x1E;
            outRows[4] = 0x10;
            outRows[5] = 0x10;
            outRows[6] = 0x10;
            break;
        case 'Q':
            outRows[0] = 0x0E;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x11;
            outRows[4] = 0x15;
            outRows[5] = 0x12;
            outRows[6] = 0x0D;
            break;
        case 'R':
            outRows[0] = 0x1E;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x1E;
            outRows[4] = 0x14;
            outRows[5] = 0x12;
            outRows[6] = 0x11;
            break;
        case 'S':
            outRows[0] = 0x0F;
            outRows[1] = 0x10;
            outRows[2] = 0x10;
            outRows[3] = 0x0E;
            outRows[4] = 0x01;
            outRows[5] = 0x01;
            outRows[6] = 0x1E;
            break;
        case 'T':
            outRows[0] = 0x1F;
            outRows[1] = 0x04;
            outRows[2] = 0x04;
            outRows[3] = 0x04;
            outRows[4] = 0x04;
            outRows[5] = 0x04;
            outRows[6] = 0x04;
            break;
        case 'U':
            outRows[0] = 0x11;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x11;
            outRows[4] = 0x11;
            outRows[5] = 0x11;
            outRows[6] = 0x0E;
            break;
        case 'V':
            outRows[0] = 0x11;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x11;
            outRows[4] = 0x11;
            outRows[5] = 0x0A;
            outRows[6] = 0x04;
            break;
        case 'W':
            outRows[0] = 0x11;
            outRows[1] = 0x11;
            outRows[2] = 0x11;
            outRows[3] = 0x15;
            outRows[4] = 0x15;
            outRows[5] = 0x1B;
            outRows[6] = 0x11;
            break;
        case 'X':
            outRows[0] = 0x11;
            outRows[1] = 0x11;
            outRows[2] = 0x0A;
            outRows[3] = 0x04;
            outRows[4] = 0x0A;
            outRows[5] = 0x11;
            outRows[6] = 0x11;
            break;
        case 'Y':
            outRows[0] = 0x11;
            outRows[1] = 0x11;
            outRows[2] = 0x0A;
            outRows[3] = 0x04;
            outRows[4] = 0x04;
            outRows[5] = 0x04;
            outRows[6] = 0x04;
            break;
        case 'Z':
            outRows[0] = 0x1F;
            outRows[1] = 0x01;
            outRows[2] = 0x02;
            outRows[3] = 0x04;
            outRows[4] = 0x08;
            outRows[5] = 0x10;
            outRows[6] = 0x1F;
            break;
        case ':':
            outRows[0] = 0x00;
            outRows[1] = 0x04;
            outRows[2] = 0x04;
            outRows[3] = 0x00;
            outRows[4] = 0x04;
            outRows[5] = 0x04;
            outRows[6] = 0x00;
            break;
        case ' ':
            break;
        default:
            /* Unknown chars are rendered as blanks. */
            break;
    }
}

static void drawTextWithOutline(SDL_Renderer *renderer, const char *text, int x, int y, int scale, SDL_Color color, SDL_Color outlineColor) {
    int i;
    int dx, dy;

    /* Draw thick outline - 3-pixel thick for maximum visibility */
    SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
    for (dx = -3; dx <= 3; ++dx) {
        for (dy = -3; dy <= 3; ++dy) {
            if (dx == 0 && dy == 0) continue;
            
            for (i = 0; text[i] != '\0'; ++i) {
                unsigned char rows[7];
                int row, col;
                int xOffset = x + dx + i * (6 * scale);
                int yOffset = y + dy;

                getGlyph(text[i], rows);

                for (row = 0; row < 7; ++row) {
                    for (col = 0; col < 5; ++col) {
                        if (rows[row] & (1 << (4 - col))) {
                            SDL_Rect pixel = {
                                xOffset + col * scale,
                                yOffset + row * scale,
                                scale,
                                scale
                            };
                            SDL_RenderFillRect(renderer, &pixel);
                        }
                    }
                }
            }
        }
    }

    /* Draw main text */
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (i = 0; text[i] != '\0'; ++i) {
        unsigned char rows[7];
        int row, col;
        int xOffset = x + i * (6 * scale);

        getGlyph(text[i], rows);

        for (row = 0; row < 7; ++row) {
            for (col = 0; col < 5; ++col) {
                if (rows[row] & (1 << (4 - col))) {
                    SDL_Rect pixel = {
                        xOffset + col * scale,
                        y + row * scale,
                        scale,
                        scale
                    };
                    SDL_RenderFillRect(renderer, &pixel);
                }
            }
        }
    }
}

static void drawText(SDL_Renderer *renderer, const char *text, int x, int y, int scale, SDL_Color color) {
    /* Determine outline color based on current color brightness */
    int brightness = color.r + color.g + color.b;
    SDL_Color outlineColor = (brightness > 383) ? (SDL_Color){0, 0, 0, 255} : (SDL_Color){255, 255, 255, 255};
    drawTextWithOutline(renderer, text, x, y, scale, color, outlineColor);
}

static void updateWindowTitle(void) {
    char title[128];
    if (gGameState == STATE_GAME_OVER) {
        snprintf(title, sizeof(title), "Cat Clicker - GAME OVER | Score: %d | Press R to Restart", gScore);
    } else if (gGameState == STATE_PLAYING) {
        snprintf(title, sizeof(title), "Cat Clicker | Score: %d | Lives: %d", gScore, gLives);
    } else {
        snprintf(title, sizeof(title), "Cat Clicker - Choose Your Theme");
    }
    SDL_SetWindowTitle(gWindow, title);
}

/* Generate a simple beep sound effect at given frequency and duration. */
static Mix_Chunk *generateBeep(float frequency, float durationMs) {
    int sampleRate = 22050;
    int numSamples = (int)(sampleRate * durationMs / 1000.0f);
    Uint8 *buffer = (Uint8 *)malloc(numSamples * sizeof(Uint8));

    if (!buffer) {
        return NULL;
    }

    for (int i = 0; i < numSamples; ++i) {
        float t = (float)i / sampleRate;
        float sample = sinf(2.0f * 3.14159f * frequency * t);

        /* Envelope: quick fade in/out */
        float envelope = 1.0f;
        if (i < sampleRate / 100) {
            envelope = (float)i / (sampleRate / 100);
        }
        if (i > numSamples - sampleRate / 100) {
            envelope = (float)(numSamples - i) / (sampleRate / 100);
        }

        buffer[i] = (Uint8)(128 + 127 * sample * envelope);
    }

    SDL_AudioSpec desiredSpec, obtainedSpec;
    SDL_zero(desiredSpec);
    desiredSpec.freq = sampleRate;
    desiredSpec.format = AUDIO_U8;
    desiredSpec.channels = 1;
    desiredSpec.samples = 4096;

    /* Create a mix chunk manually */
    Mix_Chunk *chunk = (Mix_Chunk *)malloc(sizeof(Mix_Chunk));
    if (!chunk) {
        free(buffer);
        return NULL;
    }

    chunk->allocated = 1;
    chunk->abuf = buffer;
    chunk->alen = numSamples;
    chunk->volume = MIX_MAX_VOLUME;

    return chunk;
}

/* Generate a dramatic BOOM sound for game over */
static Mix_Chunk *generateBoom(void) {
    int sampleRate = 22050;
    float durationMs = 1200.0f;  /* 1.2 seconds for dramatic effect */
    int numSamples = (int)(sampleRate * durationMs / 1000.0f);
    Uint8 *buffer = (Uint8 *)malloc(numSamples * sizeof(Uint8));

    if (!buffer) {
        return NULL;
    }

    for (int i = 0; i < numSamples; ++i) {
        float t = (float)i / sampleRate;
        
        /* Layered low-frequency tones for deep BOOM effect */
        float freq1 = 80.0f;   /* Deep bass */
        float freq2 = 120.0f;  /* Mid bass */
        float sample1 = sinf(2.0f * 3.14159f * freq1 * t);
        float sample2 = sinf(2.0f * 3.14159f * freq2 * t * 0.7f);  /* Slight freq variation */
        float sample = (sample1 * 0.7f + sample2 * 0.3f);  /* Mix the tones */

        /* Heavy attack, slow decay for dramatic impact */
        float envelope = 1.0f;
        
        /* Quick attack phase */
        if (i < sampleRate / 200) {
            envelope = (float)i / (sampleRate / 200);  /* Fast rise */
        }
        /* Slow decay phase */
        else if (i > numSamples - sampleRate / 3) {
            envelope = (float)(numSamples - i) / (sampleRate / 3);  /* Slow fade */
        }
        
        /* Amplify for impact */
        buffer[i] = (Uint8)(128 + 127 * sample * envelope * 0.9f);
    }

    SDL_AudioSpec desiredSpec;  
    SDL_zero(desiredSpec);
    desiredSpec.freq = sampleRate;
    desiredSpec.format = AUDIO_U8;
    desiredSpec.channels = 1;
    desiredSpec.samples = 4096;

    /* Create a mix chunk manually */
    Mix_Chunk *chunk = (Mix_Chunk *)malloc(sizeof(Mix_Chunk));
    if (!chunk) {
        free(buffer);
        return NULL;
    }

    chunk->allocated = 1;
    chunk->abuf = buffer;
    chunk->alen = numSamples;
    chunk->volume = MIX_MAX_VOLUME;

    return chunk;
}

/* Generate a cheerful level up sound - ascending tones */
static Mix_Chunk *generateLevelUpSound(void) {
    int sampleRate = 22050;
    float durationMs = 800.0f;  /* 0.8 seconds */
    int numSamples = (int)(sampleRate * durationMs / 1000.0f);
    Uint8 *buffer = (Uint8 *)malloc(numSamples * sizeof(Uint8));

    if (!buffer) {
        return NULL;
    }

    for (int i = 0; i < numSamples; ++i) {
        float t = (float)i / sampleRate;
        float progress = (float)i / numSamples;  /* 0 to 1 */
        
        /* Two ascending notes */
        float frequency;
        if (progress < 0.5f) {
            /* First note: ascending */
            frequency = 400.0f + (progress / 0.5f) * 200.0f;
        } else {
            /* Second note: even higher */
            frequency = 600.0f + ((progress - 0.5f) / 0.5f) * 100.0f;
        }
        
        float sample = sinf(2.0f * 3.14159f * frequency * t);
        
        /* Quick attack, smooth decay */
        float envelope = 1.0f;
        if (i < sampleRate / 100) {
            envelope = (float)i / (sampleRate / 100);
        }
        if (i > numSamples - sampleRate / 8) {
            envelope = (float)(numSamples - i) / (sampleRate / 8);
        }
        
        buffer[i] = (Uint8)(128 + 127 * sample * envelope * 0.9f);
    }

    Mix_Chunk *chunk = (Mix_Chunk *)malloc(sizeof(Mix_Chunk));
    if (!chunk) {
        free(buffer);
        return NULL;
    }

    chunk->allocated = 1;
    chunk->abuf = buffer;
    chunk->alen = numSamples;
    chunk->volume = MIX_MAX_VOLUME;

    return chunk;
}

/* Generate a distinct "game over" sound - sad descending tones */
static Mix_Chunk *generateGameOverSound(void) {
    int sampleRate = 22050;
    float durationMs = 2000.0f;  /* 2 seconds */
    int numSamples = (int)(sampleRate * durationMs / 1000.0f);
    Uint8 *buffer = (Uint8 *)malloc(numSamples * sizeof(Uint8));

    if (!buffer) {
        return NULL;
    }

    for (int i = 0; i < numSamples; ++i) {
        float t = (float)i / sampleRate;
        float progress = (float)i / numSamples;  /* 0 to 1 */
        
        /* Create a sad descending pattern */
        /* Start high, go down in steps like "game... over" */
        float frequency;
        if (progress < 0.3f) {
            /* First syllable - higher pitch */
            frequency = 300.0f - (progress / 0.3f) * 50.0f;
        } else if (progress < 0.6f) {
            /* Middle silence/pause */
            frequency = 250.0f;
        } else {
            /* Second syllable - lower pitch, descending */
            frequency = 250.0f - ((progress - 0.6f) / 0.4f) * 150.0f;
        }
        
        float sample = sinf(2.0f * 3.14159f * frequency * t);
        
        /* Sad envelope - punch in, fade out */
        float envelope = 1.0f;
        if (i < sampleRate / 100) {
            envelope = (float)i / (sampleRate / 100);
        }
        if (i > numSamples - sampleRate / 4) {
            envelope = (float)(numSamples - i) / (sampleRate / 4);
        }
        
        buffer[i] = (Uint8)(128 + 127 * sample * envelope * 0.8f);
    }

    SDL_AudioSpec desiredSpec;  
    SDL_zero(desiredSpec);
    desiredSpec.freq = sampleRate;
    desiredSpec.format = AUDIO_U8;
    desiredSpec.channels = 1;
    desiredSpec.samples = 4096;

    Mix_Chunk *chunk = (Mix_Chunk *)malloc(sizeof(Mix_Chunk));
    if (!chunk) {
        free(buffer);
        return NULL;
    }

    chunk->allocated = 1;
    chunk->abuf = buffer;
    chunk->alen = numSamples;
    chunk->volume = MIX_MAX_VOLUME;

    return chunk;
}

/* Generate a cute meow sound for colorful cats */
static Mix_Chunk *generateMeow(void) {
    int sampleRate = 22050;
    float durationMs = 300.0f;  /* 0.3 seconds */
    int numSamples = (int)(sampleRate * durationMs / 1000.0f);
    Uint8 *buffer = (Uint8 *)malloc(numSamples * sizeof(Uint8));

    if (!buffer) {
        return NULL;
    }

    for (int i = 0; i < numSamples; ++i) {
        float t = (float)i / sampleRate;
        float progress = (float)i / numSamples;  /* 0 to 1 */
        
        /* Cat meow: starts high, descends slightly, with natural variation */
        float frequency;
        if (progress < 0.4f) {
            /* Attack phase: rise to peak meow */
            frequency = 250.0f + (progress / 0.4f) * 200.0f;
        } else if (progress < 0.7f) {
            /* Sustain phase: slight descent */
            frequency = 450.0f - ((progress - 0.4f) / 0.3f) * 100.0f;
        } else {
            /* Release phase: further descent */
            frequency = 350.0f - ((progress - 0.7f) / 0.3f) * 200.0f;
        }
        
        /* Add slight vibrato for realism */
        float vibrato = 20.0f * sinf(2.0f * 3.14159f * 4.0f * t);
        frequency += vibrato;
        
        float sample = sinf(2.0f * 3.14159f * frequency * t);
        
        /* Natural envelope - quick attack, smooth decay */
        float envelope = 1.0f;
        if (i < sampleRate / 150) {
            envelope = (float)i / (sampleRate / 150);  /* Quick attack */
        }
        if (i > numSamples - sampleRate / 20) {
            envelope = (float)(numSamples - i) / (sampleRate / 20);  /* Smooth tail */
        }
        
        buffer[i] = (Uint8)(128 + 127 * sample * envelope * 0.8f);
    }

    SDL_AudioSpec desiredSpec;  
    SDL_zero(desiredSpec);
    desiredSpec.freq = sampleRate;
    desiredSpec.format = AUDIO_U8;
    desiredSpec.channels = 1;
    desiredSpec.samples = 4096;

    Mix_Chunk *chunk = (Mix_Chunk *)malloc(sizeof(Mix_Chunk));
    if (!chunk) {
        free(buffer);
        return NULL;
    }

    chunk->allocated = 1;
    chunk->abuf = buffer;
    chunk->alen = numSamples;
    chunk->volume = MIX_MAX_VOLUME;

    return chunk;
}

static void playSound(Mix_Chunk *sound) {
    if (!sound || !gAudioEnabled) {
        return;
    }
    Mix_PlayChannel(-1, sound, 0);
}

static void initAudio(void) {
    if (Mix_OpenAudio(22050, AUDIO_U8, 1, 4096) < 0) {
        fprintf(stderr, "Mix_OpenAudio failed: %s\n", Mix_GetError());
        gAudioEnabled = 0;
        return;
    }

    gAudioEnabled = 1;

    /* Generate sound effects */
    gSoundClick = generateBeep(800.0f, 50.0f);
    gSoundMeow = generateMeow();  /* Cute meow for colorful cats */
    gSoundHit = generateBoom();  /* BOOM sound when hitting gray cat */
    gSoundGameOver = generateGameOverSound();  /* Sad descending tones for game over */
    gSoundLevelUp = generateLevelUpSound();  /* Cheerful ascending tones for level up */

    if (!gSoundClick || !gSoundHit || !gSoundGameOver || !gSoundLevelUp) {
        fprintf(stderr, "Warning: Failed to generate sound effects\n");
    }
}

static void cleanupAudio(void) {
    if (gSoundClick) {
        Mix_FreeChunk(gSoundClick);
        gSoundClick = NULL;
    }
    if (gSoundMeow) {
        Mix_FreeChunk(gSoundMeow);
        gSoundMeow = NULL;
    }
    if (gSoundHit) {
        Mix_FreeChunk(gSoundHit);
        gSoundHit = NULL;
    }
    if (gSoundGameOver) {
        Mix_FreeChunk(gSoundGameOver);
        gSoundGameOver = NULL;
    }
    if (gSoundLevelUp) {
        Mix_FreeChunk(gSoundLevelUp);
        gSoundLevelUp = NULL;
    }
    if (gAudioEnabled) {
        Mix_CloseAudio();
        gAudioEnabled = 0;
    }
}

/* Draw a simple but cute cat */
static void drawCat(SDL_Renderer *renderer, const Cat *cat, Uint32 time) {
    int x = cat->x;
    int y = cat->y;
    int w = cat->width;
    int h = cat->height;

    SDL_SetRenderDrawColor(renderer, cat->color.r, cat->color.g, cat->color.b, 255);

    /* Body - rounded rectangle look */
    SDL_Rect body = {x + w / 8, y + h / 3, w * 3 / 4, h / 2};
    SDL_RenderFillRect(renderer, &body);

    /* Head - circle-like */
    SDL_Rect head = {x + w / 5, y + h / 12, w * 3 / 5, h / 3};
    SDL_RenderFillRect(renderer, &head);
    
    /* Left ear - triangle */
    SDL_Point leftEar[3] = {
        {x + w / 3, y + h / 12},
        {x + w / 3 - w / 14, y - h / 12},
        {x + w / 3 + w / 14, y + h / 10}
    };
    for (int i = 0; i < 3; i++) {
        int next = (i + 1) % 3;
        SDL_RenderDrawLineF(renderer, leftEar[i].x, leftEar[i].y, leftEar[next].x, leftEar[next].y);
    }

    /* Right ear - triangle */
    SDL_Point rightEar[3] = {
        {x + w * 2 / 3, y + h / 12},
        {x + w * 2 / 3 - w / 14, y + h / 10},
        {x + w * 2 / 3 + w / 14, y - h / 12}
    };
    for (int i = 0; i < 3; i++) {
        int next = (i + 1) % 3;
        SDL_RenderDrawLineF(renderer, rightEar[i].x, rightEar[i].y, rightEar[next].x, rightEar[next].y);
    }

    /* Eyes */
    if (cat->type == 1) {
        /* Gray cat: bright turquoise eyes */
        SDL_SetRenderDrawColor(renderer, 0, 230, 255, 255);
    } else {
        /* Colorful cat: yellow eyes */
        SDL_SetRenderDrawColor(renderer, 255, 220, 100, 255);
    }
    SDL_Rect leftEye = {x + w / 3 - w / 20, y + h / 6, w / 9, w / 9};
    SDL_Rect rightEye = {x + w * 2 / 3 - w / 20, y + h / 6, w / 9, w / 9};
    SDL_RenderFillRect(renderer, &leftEye);
    SDL_RenderFillRect(renderer, &rightEye);

    /* Pupils - vertical slit */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int pupilLeftX = x + w / 3 - w / 20 + w / 18;
    int pupilRightX = x + w * 2 / 3 - w / 20 + w / 18;
    int pupilTop = y + h / 6 + 1;
    int pupilBottom = y + h / 6 + w / 9 - 1;
    SDL_RenderDrawLineF(renderer, pupilLeftX, pupilTop, pupilLeftX, pupilBottom);
    SDL_RenderDrawLineF(renderer, pupilRightX, pupilTop, pupilRightX, pupilBottom);

    /* Nose - small pink triangle */
    SDL_SetRenderDrawColor(renderer, 255, 150, 200, 255);
    int noseX = x + w / 2;
    int noseY = y + h / 3;
    SDL_RenderDrawPoint(renderer, noseX, noseY);
    SDL_RenderDrawPoint(renderer, noseX - 1, noseY + 1);
    SDL_RenderDrawPoint(renderer, noseX + 1, noseY + 1);

    /* Mouth - simple smile */
    SDL_RenderDrawPoint(renderer, noseX - 2, noseY + 3);
    SDL_RenderDrawPoint(renderer, noseX + 2, noseY + 3);

    /* Front left paw */
    SDL_SetRenderDrawColor(renderer, cat->color.r, cat->color.g, cat->color.b, 255);
    SDL_Rect leftPaw = {x + w / 4, y + h * 5 / 6, w / 8, h / 5};
    SDL_RenderFillRect(renderer, &leftPaw);

    /* Front right paw */
    SDL_Rect rightPaw = {x + w * 5 / 8, y + h * 5 / 6, w / 8, h / 5};
    SDL_RenderFillRect(renderer, &rightPaw);

    /* Simple tail - curved line */
    SDL_RenderDrawLineF(renderer, x + w * 7 / 8, y + h / 2, x + w * 7 / 8 + w / 5, y - h / 6);
    SDL_RenderDrawLineF(renderer, x + w * 7 / 8 + 1, y + h / 2, x + w * 7 / 8 + w / 5 + 1, y - h / 6);

    /* Shiny effect for bonus life cat - animated halo */
    if (cat->type == 2) {
        /* Animate halo with pulsing effect */
        float pulse = 0.5f + 0.5f * sinf(time / 500.0f);
        int baseHaloRadius = w / 2 + 8;
        int haloRadius = baseHaloRadius + (int)(3 * pulse);
        int centerX = x + w / 2;
        int centerY = y + h / 2;
        
        /* Animated opacity */
        Uint8 alpha = (Uint8)(100 + 100 * pulse);
        SDL_SetRenderDrawColor(renderer, 255, 255, 200, alpha);
        
        /* Draw a glowing animated halo */
        for (int r = haloRadius; r > haloRadius - 4; r--) {
            for (int angle = 0; angle < 360; angle += 12) {
                float radian = angle * 3.14159f / 180.0f;
                int px = centerX + (int)(r * cosf(radian));
                int py = centerY + (int)(r * sinf(radian));
                SDL_RenderDrawPoint(renderer, px, py);
            }
        }
    }
}

/* Draw a sparkling/glittering starry background */
static void drawStarryBackground(SDL_Renderer *renderer, Uint32 time) {
    if (gTheme == THEME_LIGHT) {
        /* Light mode: subtle clouds background */
        SDL_SetRenderDrawColor(renderer, 230, 240, 255, 255);
        SDL_Rect fadeRect = {0, HUD_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - HUD_HEIGHT};
        SDL_RenderFillRect(renderer, &fadeRect);
        return;
    }
    
    /* Dark mode: stars */
    int numStars = 100;
    int i;

    srand(42);

    for (i = 0; i < numStars; ++i) {
        int sx = (i * 137) % WINDOW_WIDTH;
        int sy = (i * 211) % (WINDOW_HEIGHT - HUD_HEIGHT) + HUD_HEIGHT;
        
        int brightness = (int)(128 + 127 * sinf(2.0f * 3.14159f * (time + i * 100) / 1000.0f));
        SDL_SetRenderDrawColor(renderer, brightness, brightness, brightness / 2, 255);
        
        SDL_RenderDrawPoint(renderer, sx, sy);
        SDL_RenderDrawPoint(renderer, sx + 1, sy);
        SDL_RenderDrawPoint(renderer, sx, sy + 1);
    }

    srand((unsigned int)time);
}

/* Draw a moon in the top right corner */
/* Draw sun for light mode */
static void drawSun(SDL_Renderer *renderer) {
    int sunX = 700;
    int sunY = 100;
    int sunRadius = 40;
    int x, y, i, j, k;

    /* Sun halo - outer glow (dim) */
    SDL_SetRenderDrawColor(renderer, 255, 220, 100, 100);
    for (y = -sunRadius - 15; y <= sunRadius + 15; ++y) {
        for (x = -sunRadius - 15; x <= sunRadius + 15; ++x) {
            if (x * x + y * y <= (sunRadius + 15) * (sunRadius + 15) && x * x + y * y > sunRadius * sunRadius) {
                SDL_RenderDrawPoint(renderer, sunX + x, sunY + y);
            }
        }
    }

    /* Sun body - bright yellow */
    SDL_SetRenderDrawColor(renderer, 255, 230, 20, 255);
    for (y = -sunRadius; y <= sunRadius; ++y) {
        for (x = -sunRadius; x <= sunRadius; ++x) {
            if (x * x + y * y <= sunRadius * sunRadius) {
                SDL_RenderDrawPoint(renderer, sunX + x, sunY + y);
            }
        }
    }

    /* Sun rays - longer and thicker */
    SDL_SetRenderDrawColor(renderer, 255, 210, 50, 255);
    int rayLength = 75;
    int rayThickness = 4;
    
    /* 12 rays for more sun-like appearance */
    for (i = 0; i < 12; ++i) {
        float angle = 3.14159f * 2.0f * i / 12.0f;
        for (j = 0; j < rayLength; ++j) {
            for (k = -rayThickness; k <= rayThickness; ++k) {
                int rayX = (int)(sunX + (sunRadius + j) * cosf(angle) + k * sinf(angle));
                int rayY = (int)(sunY + (sunRadius + j) * sinf(angle) - k * cosf(angle));
                SDL_RenderDrawPoint(renderer, rayX, rayY);
            }
        }
    }
}

/* Draw clouds for light mode */
static void drawClouds(SDL_Renderer *renderer, Uint32 time) {
    SDL_Color cloudColor = {240, 240, 240, 255};
    
    /* Slow floating effect */
    float offset = sinf(2.0f * 3.14159f * time / 8000.0f) * 50.0f;

    /* Cloud 1 - top left */
    int c1X = (int)(100 + offset);
    int c1Y = 80;
    
    /* Draw bulbous cloud shape with overlapping circles */
    SDL_SetRenderDrawColor(renderer, cloudColor.r, cloudColor.g, cloudColor.b, cloudColor.a);
    drawCircle(renderer, c1X + 15, c1Y + 25, 22);
    drawCircle(renderer, c1X + 45, c1Y + 15, 28);
    drawCircle(renderer, c1X + 75, c1Y + 25, 22);
    
    /* Fill gaps with rectangle */
    SDL_Rect gap1 = {c1X, c1Y + 20, 90, 20};
    SDL_RenderFillRect(renderer, &gap1);

    /* Cloud 2 - top right */
    int c2X = (int)(550 - offset);
    int c2Y = 120;
    
    SDL_SetRenderDrawColor(renderer, cloudColor.r, cloudColor.g, cloudColor.b, cloudColor.a);
    drawCircle(renderer, c2X + 20, c2Y + 27, 24);
    drawCircle(renderer, c2X + 60, c2Y + 12, 32);
    drawCircle(renderer, c2X + 100, c2Y + 27, 24);
    
    /* Fill gaps with rectangle */
    SDL_Rect gap2 = {c2X, c2Y + 22, 120, 22};
    SDL_RenderFillRect(renderer, &gap2);
}

/* Helper: Draw filled circle for clouds */
static void drawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius) {
    int x, y;
    for (y = -radius; y <= radius; ++y) {
        for (x = -radius; x <= radius; ++x) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

static void drawMoon(SDL_Renderer *renderer) {
    if (gTheme == THEME_LIGHT) {
        return;  /* No moon in light mode */
    }
    
    ColorTheme theme = getThemeColors();
    int moonX = 700;
    int moonY = 100;
    int moonRadius = 55;
    int x, y;

    /* Stars around moon */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderDrawPoint(renderer, moonX - 100, moonY - 80);
    SDL_RenderDrawPoint(renderer, moonX + 110, moonY - 60);
    SDL_RenderDrawPoint(renderer, moonX + 90, moonY + 100);
    SDL_RenderDrawPoint(renderer, moonX - 120, moonY + 70);

    /* Moon glow - enhanced halo with better gradation */
    for (int dist = 25; dist > 0; --dist) {
        int alpha = (int)(80 * dist / 25);
        SDL_SetRenderDrawColor(renderer, 100 + dist, 100 + dist, 150 + dist/2, alpha);
        int glowRadius = moonRadius + dist;
        for (y = -glowRadius; y <= glowRadius; ++y) {
            for (x = -glowRadius; x <= glowRadius; ++x) {
                if (x * x + y * y == glowRadius * glowRadius) {
                    SDL_RenderDrawPoint(renderer, moonX + x, moonY + y);
                }
            }
        }
    }

    /* Draw filled moon circle - bright yellow-white */
    SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);
    for (y = -moonRadius; y <= moonRadius; ++y) {
        for (x = -moonRadius; x <= moonRadius; ++x) {
            if (x * x + y * y <= moonRadius * moonRadius) {
                SDL_RenderDrawPoint(renderer, moonX + x, moonY + y);
            }
        }
    }

    /* Moon craters - multiple craters with varied shadows */
    SDL_Color craterColor = {150, 140, 120, 200};
    SDL_SetRenderDrawColor(renderer, craterColor.r, craterColor.g, craterColor.b, craterColor.a);
    
    /* Large crater - top left */
    for (y = -12; y <= 12; ++y) {
        for (x = -12; x <= 12; ++x) {
            if (x * x + y * y <= 144) {
                SDL_RenderDrawPoint(renderer, moonX - 20 + x, moonY - 20 + y);
            }
        }
    }
    
    /* Medium crater - right side */
    for (y = -8; y <= 8; ++y) {
        for (x = -8; x <= 8; ++x) {
            if (x * x + y * y <= 64) {
                SDL_RenderDrawPoint(renderer, moonX + 25 + x, moonY + 15 + y);
            }
        }
    }
    
    /* Small crater - bottom right */
    for (y = -5; y <= 5; ++y) {
        for (x = -5; x <= 5; ++x) {
            if (x * x + y * y <= 25) {
                SDL_RenderDrawPoint(renderer, moonX + 5 + x, moonY + 30 + y);
            }
        }
    }

    /* Additional small craters for more detail */
    SDL_SetRenderDrawColor(renderer, 140, 130, 110, 180);
    
    /* Tiny crater 1 */
    for (y = -3; y <= 3; ++y) {
        for (x = -3; x <= 3; ++x) {
            if (x * x + y * y <= 9) {
                SDL_RenderDrawPoint(renderer, moonX - 35 + x, moonY + 10 + y);
            }
        }
    }
    
    /* Tiny crater 2 */
    for (y = -3; y <= 3; ++y) {
        for (x = -3; x <= 3; ++x) {
            if (x * x + y * y <= 9) {
                SDL_RenderDrawPoint(renderer, moonX + 15 + x, moonY - 30 + y);
            }
        }
    }

    /* Bright highlight on moon - gives it shiny appearance */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    for (y = -8; y <= 8; ++y) {
        for (x = -8; x <= 8; ++x) {
            if (x * x + y * y <= 64) {
                SDL_RenderDrawPoint(renderer, moonX - 15 + x, moonY - 25 + y);
            }
        }
    }
}

/* Draw houses on both sides of the road */
static void drawHouses(SDL_Renderer *renderer, Uint32 time) {
    ColorTheme theme = getThemeColors();
    SDL_Color houseColor = theme.houseColor;
    SDL_Color roofColor = theme.roofColor;
    int houseWidth = 90;
    int houseHeight = 140;
    int roofHeight = 40;
    int windowSize = 20;
    int x, y;

    /* Light flicker effect */
    int lightBrightness = (int)(gTheme == THEME_LIGHT ? 
        (200 + 35 * sinf(2.0f * 3.14159f * time / 500.0f)) :
        (200 + 55 * sinf(2.0f * 3.14159f * time / 500.0f)));
    SDL_Color flickeringLight = theme.windowLightColor;
    flickeringLight.r = (Uint8)lightBrightness;
    if (gTheme == THEME_DARK) {
        flickeringLight.g = (Uint8)lightBrightness;
    } else {
        flickeringLight.g = (Uint8)(lightBrightness - 50);
    }

    /* Left side houses */
    for (y = 0; y < 3; ++y) {
        int houseY = HUD_HEIGHT + y * 160;
        int houseX = 10;

        /* House body */
        SDL_SetRenderDrawColor(renderer, houseColor.r, houseColor.g, houseColor.b, houseColor.a);
        {
            SDL_Rect house = {houseX, houseY, houseWidth, houseHeight};
            SDL_RenderFillRect(renderer, &house);
        }

        /* Roof (triangle drawn with lines) */
        SDL_SetRenderDrawColor(renderer, roofColor.r, roofColor.g, roofColor.b, roofColor.a);
        SDL_RenderDrawLineF(renderer, houseX, houseY, houseX + houseWidth / 2, houseY - roofHeight);
        SDL_RenderDrawLineF(renderer, houseX + houseWidth / 2, houseY - roofHeight, houseX + houseWidth, houseY);
        SDL_RenderDrawLineF(renderer, houseX, houseY, houseX + houseWidth, houseY);

        /* Windows with lights */
        SDL_SetRenderDrawColor(renderer, flickeringLight.r, flickeringLight.g, flickeringLight.b, flickeringLight.a);
        {
            SDL_Rect window1 = {houseX + 15, houseY + 30, windowSize, windowSize};
            SDL_Rect window2 = {houseX + 55, houseY + 30, windowSize, windowSize};
            SDL_Rect window3 = {houseX + 15, houseY + 80, windowSize, windowSize};
            SDL_RenderFillRect(renderer, &window1);
            SDL_RenderFillRect(renderer, &window2);
            SDL_RenderFillRect(renderer, &window3);
        }
    }

    /* Right side houses */
    for (y = 0; y < 3; ++y) {
        int houseY = HUD_HEIGHT + y * 160;
        int houseX = WINDOW_WIDTH - houseWidth - 10;

        /* House body */
        SDL_SetRenderDrawColor(renderer, houseColor.r, houseColor.g, houseColor.b, houseColor.a);
        {
            SDL_Rect house = {houseX, houseY, houseWidth, houseHeight};
            SDL_RenderFillRect(renderer, &house);
        }

        /* Roof (triangle drawn with lines) */
        SDL_SetRenderDrawColor(renderer, roofColor.r, roofColor.g, roofColor.b, roofColor.a);
        SDL_RenderDrawLineF(renderer, houseX, houseY, houseX + houseWidth / 2, houseY - roofHeight);
        SDL_RenderDrawLineF(renderer, houseX + houseWidth / 2, houseY - roofHeight, houseX + houseWidth, houseY);
        SDL_RenderDrawLineF(renderer, houseX, houseY, houseX + houseWidth, houseY);

        /* Windows with lights */
        SDL_SetRenderDrawColor(renderer, flickeringLight.r, flickeringLight.g, flickeringLight.b, flickeringLight.a);
        {
            SDL_Rect window1 = {houseX + 15, houseY + 30, windowSize, windowSize};
            SDL_Rect window2 = {houseX + 55, houseY + 30, windowSize, windowSize};
            SDL_Rect window3 = {houseX + 15, houseY + 80, windowSize, windowSize};
            SDL_RenderFillRect(renderer, &window1);
            SDL_RenderFillRect(renderer, &window2);
            SDL_RenderFillRect(renderer, &window3);
        }
    }
}

static int init(void) {
    int i;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 0;
    }

    gWindow = SDL_CreateWindow(
        "Cat Clicker",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!gWindow) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return 0;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!gRenderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return 0;
    }

    srand((unsigned int)time(NULL));

    for (i = 0; i < MAX_CATS; ++i) {
        gCats[i].active = 0;
    }

    gGameState = STATE_THEME_SELECTION;
    gThemeSelected = THEME_DARK;  /* Default highlight */
    updateWindowTitle();
    initAudio();

    return 1;
}

static int checkClickCollision(int mouseX, int mouseY, const Cat *cat) {
    if (!cat || !cat->active) {
        return 0;
    }

    return (mouseX >= cat->x && mouseX <= (cat->x + cat->width) &&
            mouseY >= cat->y && mouseY <= (cat->y + cat->height));
}

static void spawnCat(void) {
    int i;

    for (i = 0; i < MAX_CATS; ++i) {
        if (!gCats[i].active) {
            int randomTypeRoll = rand() % 100;
            float speedBase;

            gCats[i].width = 40 + rand() % 46;
            gCats[i].height = 30 + rand() % 36;
            
            /* Spawn in a line at the bottom of the screen */
            /* Calculate position based on index for linear progression */
            gCats[i].x = 120 + (i % 10) * 60;  /* Space cats horizontally */
            if (gCats[i].x + gCats[i].width > WINDOW_WIDTH - 120) {
                gCats[i].x = WINDOW_WIDTH - 120 - gCats[i].width;
            }
            /* All spawn at bottom and move upward */
            gCats[i].y = WINDOW_HEIGHT - 80;
            
            /* Type distribution: 60% normal, 35% danger, 5% bonus life */
            if (randomTypeRoll < 60) {
                gCats[i].type = 0;  /* Normal cat */
            } else if (randomTypeRoll < 95) {
                gCats[i].type = 1;  /* Danger cat (gray) */
            } else {
                gCats[i].type = 2;  /* Bonus life cat (shiny) */
            }
            gCats[i].active = 1;

            /* Base movement speed gets harder over time via multiplier. */
            speedBase = (float)(50 + rand() % 90);
            gCats[i].vx = ((rand() % 2) ? 1.0f : -1.0f) * speedBase;
            /* Move upward in a line */
            gCats[i].vy = -speedBase * 0.7f;  /* Negative for upward movement */

            if (gCats[i].type == 0) {
                gCats[i].color.r = (Uint8)(80 + rand() % 176);
                gCats[i].color.g = (Uint8)(80 + rand() % 176);
                gCats[i].color.b = (Uint8)(80 + rand() % 176);
                gCats[i].color.a = 255;
            } else if (gCats[i].type == 1) {
                /* Black cat: darker gray */
                gCats[i].color.r = 75;
                gCats[i].color.g = 75;
                gCats[i].color.b = 75;
                gCats[i].color.a = 255;
            } else {
                /* Shiny golden cat */
                gCats[i].color.r = 255;
                gCats[i].color.g = 215;
                gCats[i].color.b = 0;
                gCats[i].color.a = 255;
            }

            return;
        }
    }
}

static void restartGame(void) {
    int i;

    gScore = 0;
    gLives = INITIAL_LIVES;
    gGameState = STATE_PLAYING;
    gGameWon = 0;
    
    /* Reset level system */
    gCurrentLevel = 1;
    gCatsForNextLevel = CATS_PER_LEVEL;

    gSpawnInterval = INITIAL_SPAWN_INTERVAL;
    gSpeedMultiplier = 1.0f;

    gGameStartTime = SDL_GetTicks();
    gLastSpawnTime = gGameStartTime;

    for (i = 0; i < MAX_CATS; ++i) {
        gCats[i].active = 0;
    }

    updateWindowTitle();
}

static void handleEvents(void) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            gRunning = 0;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                gRunning = 0;
            } else if (gGameState == STATE_THEME_SELECTION) {
                /* Theme selection keyboard input */
                if (event.key.keysym.sym == SDLK_d) {
                    gTheme = THEME_DARK;
                    gThemeSelected = THEME_DARK;
                    gGameState = STATE_PLAYING;
                    restartGame();
                } else if (event.key.keysym.sym == SDLK_l) {
                    gTheme = THEME_LIGHT;
                    gThemeSelected = THEME_LIGHT;
                    gGameState = STATE_PLAYING;
                    restartGame();
                }
            } else if (event.key.keysym.sym == SDLK_t && gGameState == STATE_PLAYING) {
                /* Toggle theme during gameplay */
                gTheme = (gTheme == THEME_DARK) ? THEME_LIGHT : THEME_DARK;
            } else if (event.key.keysym.sym == SDLK_r && gGameState == STATE_GAME_OVER) {
                gGameState = STATE_THEME_SELECTION;
                gThemeSelected = 0;
            }
        } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            if (gGameState == STATE_THEME_SELECTION) {
                /* Dark Mode button */
                SDL_Rect darkButton = {150, 250, 200, 80};
                if (mouseX >= darkButton.x && mouseX <= darkButton.x + darkButton.w &&
                    mouseY >= darkButton.y && mouseY <= darkButton.y + darkButton.h) {
                    gTheme = THEME_DARK;
                    gThemeSelected = THEME_DARK;
                    gGameState = STATE_PLAYING;
                    restartGame();
                    return;
                }

                /* Light Mode button */
                SDL_Rect lightButton = {450, 250, 200, 80};
                if (mouseX >= lightButton.x && mouseX <= lightButton.x + lightButton.w &&
                    mouseY >= lightButton.y && mouseY <= lightButton.y + lightButton.h) {
                    gTheme = THEME_LIGHT;
                    gThemeSelected = THEME_LIGHT;
                    gGameState = STATE_PLAYING;
                    restartGame();
                    return;
                }
            } else if (gGameState == STATE_GAME_OVER) {
                /* Dark Mode button */
                SDL_Rect darkButton = {150, 360, 200, 80};
                if (mouseX >= darkButton.x && mouseX <= darkButton.x + darkButton.w &&
                    mouseY >= darkButton.y && mouseY <= darkButton.y + darkButton.h) {
                    gTheme = THEME_DARK;
                    gThemeSelected = THEME_DARK;
                    gGameState = STATE_PLAYING;
                    restartGame();
                    return;
                }

                /* Light Mode button */
                SDL_Rect lightButton = {450, 360, 200, 80};
                if (mouseX >= lightButton.x && mouseX <= lightButton.x + lightButton.w &&
                    mouseY >= lightButton.y && mouseY <= lightButton.y + lightButton.h) {
                    gTheme = THEME_LIGHT;
                    gThemeSelected = THEME_LIGHT;
                    gGameState = STATE_PLAYING;
                    restartGame();
                    return;
                }
            } else if (gGameState == STATE_PLAYING) {
                /* Playing state: click cats */
                int i;
                for (i = MAX_CATS - 1; i >= 0; --i) {
                    if (checkClickCollision(mouseX, mouseY, &gCats[i])) {
                        if (gCats[i].type == 0) {
                            gScore += 1;
                            playSound(gSoundMeow);
                            
                            /* Check for level up */
                            if (gScore >= gCatsForNextLevel && gCurrentLevel < MAX_LEVELS) {
                                gCurrentLevel++;
                                gCatsForNextLevel += CATS_PER_LEVEL;
                                playSound(gSoundLevelUp);
                            }
                        } else if (gCats[i].type == 1) {
                            gLives -= 1;
                            playSound(gSoundHit);
                            if (gLives <= 0) {
                                gLives = 0;
                                gGameState = STATE_GAME_OVER;
                                playSound(gSoundGameOver);
                            }
                        } else if (gCats[i].type == 2) {
                            /* Bonus life cat - gain a life */
                            gLives += 1;
                            playSound(gSoundLevelUp);
                        }

                        gCats[i].active = 0;
                        updateWindowTitle();
                        break;
                    }
                }
            }
        }
    }
}

static void updateGame(float deltaTime) {
    int i;
    Uint32 now = SDL_GetTicks();

    if (gGameState != STATE_PLAYING) {
        updateWindowTitle();
        return;
    }

    /* Increase difficulty over time by reducing spawn interval. */
    {
        Uint32 elapsed = now - gGameStartTime;
        Uint32 reduction = elapsed / 140;

        if (reduction > (INITIAL_SPAWN_INTERVAL - MIN_SPAWN_INTERVAL)) {
            reduction = INITIAL_SPAWN_INTERVAL - MIN_SPAWN_INTERVAL;
        }

        gSpawnInterval = INITIAL_SPAWN_INTERVAL - reduction;
        if (gSpawnInterval < MIN_SPAWN_INTERVAL) {
            gSpawnInterval = MIN_SPAWN_INTERVAL;
        }
        
        /* Reduce spawn interval further based on level */
        int levelReduction = gCurrentLevel * 50;  /* 50ms per level */
        if (gSpawnInterval - levelReduction > MIN_SPAWN_INTERVAL) {
            gSpawnInterval -= levelReduction;
        } else {
            gSpawnInterval = MIN_SPAWN_INTERVAL;
        }

        gSpeedMultiplier = 1.0f + (elapsed / 35000.0f);
        if (gSpeedMultiplier > 1.8f) {
            gSpeedMultiplier = 1.8f;
        }
        
        /* Increase speed multiplier based on level */
        gSpeedMultiplier += (gCurrentLevel - 1) * 0.15f;  /* 0.15 per level */
    }

    if (now - gLastSpawnTime >= gSpawnInterval) {
        spawnCat();
        gLastSpawnTime = now;
    }

    for (i = 0; i < MAX_CATS; ++i) {
        if (!gCats[i].active) {
            continue;
        }

        gCats[i].x += (int)(gCats[i].vx * gSpeedMultiplier * deltaTime);
        gCats[i].y += (int)(gCats[i].vy * gSpeedMultiplier * deltaTime);

        if (gCats[i].x <= 0) {
            gCats[i].x = 0;
            gCats[i].vx *= -1.0f;
        } else if (gCats[i].x + gCats[i].width >= WINDOW_WIDTH) {
            gCats[i].x = WINDOW_WIDTH - gCats[i].width;
            gCats[i].vx *= -1.0f;
        }

        if (gCats[i].y <= HUD_HEIGHT) {
            gCats[i].y = HUD_HEIGHT;
            gCats[i].vy *= -1.0f;
        } else if (gCats[i].y + gCats[i].height >= WINDOW_HEIGHT) {
            gCats[i].y = WINDOW_HEIGHT - gCats[i].height;
            gCats[i].vy *= -1.0f;
        }
    }

    /* Check for victory condition: if ONLY gray cats are on screen (no colorful cats) and at least 30 gray cats */
    {
        int colorfulCount = 0;
        int grayCount = 0;
        int i;
        for (i = 0; i < MAX_CATS; ++i) {
            if (gCats[i].active) {
                if (gCats[i].type == 0) {
                    colorfulCount++;
                } else {
                    grayCount++;
                }
            }
        }
        
        /* Victory: no colorful cats AND at least 30 gray cats */
        if (colorfulCount == 0 && grayCount >= 30 && gGameState == STATE_PLAYING) {
            gGameState = STATE_GAME_OVER;
            gGameWon = 1;
        }
    }

    updateWindowTitle();
}

static void drawHUD(SDL_Renderer *renderer, ColorTheme theme) {
    int i;
    char scoreText[64];
    char livesText[64];
    char levelText[64];
    char progressText[64];
    char themeText[16];
    
    /* Calculate progress to next level */
    int catsClicked = gScore;
    int previousLevelGoal = (gCurrentLevel - 1) * CATS_PER_LEVEL;
    int currentLevelGoal = gCurrentLevel * CATS_PER_LEVEL;
    int progressInLevel = catsClicked - previousLevelGoal;
    int catsNeeded = currentLevelGoal - previousLevelGoal;
    
    /* Draw HUD background */
    SDL_SetRenderDrawColor(renderer, theme.hudBg.r, theme.hudBg.g, theme.hudBg.b, theme.hudBg.a);
    SDL_Rect hud = {0, 0, WINDOW_WIDTH, HUD_HEIGHT};
    SDL_RenderFillRect(renderer, &hud);
    
    /* Draw horizontal separator line */
    SDL_SetRenderDrawColor(renderer, theme.text.r, theme.text.g, theme.text.b, 100);
    SDL_RenderDrawLine(renderer, 0, HUD_HEIGHT - 1, WINDOW_WIDTH, HUD_HEIGHT - 1);
    
    /* Draw vertical separators */
    SDL_RenderDrawLine(renderer, 200, 0, 200, HUD_HEIGHT);
    SDL_RenderDrawLine(renderer, 400, 0, 400, HUD_HEIGHT);
    SDL_RenderDrawLine(renderer, 600, 0, 600, HUD_HEIGHT);
    
    /* Left section: SCORE */
    snprintf(scoreText, sizeof(scoreText), "SCORE: %d", gScore);
    SDL_Color outlineColor = (theme.text.r > 128) ? (SDL_Color){0, 0, 0, 255} : (SDL_Color){255, 255, 255, 255};
    drawTextWithOutline(renderer, scoreText, 10, 15, 2, theme.text, outlineColor);
    
    /* Middle-left section: LEVEL with progress bar */
    snprintf(levelText, sizeof(levelText), "LV %d", gCurrentLevel);
    drawTextWithOutline(renderer, levelText, 220, 15, 2, theme.accentColor, outlineColor);
    
    /* Draw progress bar background */
    SDL_Rect progressBarBg = {220, 35, 155, 18};
    SDL_SetRenderDrawColor(renderer, theme.text.r, theme.text.g, theme.text.b, 80);
    SDL_RenderFillRect(renderer, &progressBarBg);
    
    /* Draw progress bar fill */
    if (catsNeeded > 0) {
        int fillWidth = (int)(150.0f * progressInLevel / catsNeeded);
        if (fillWidth > 150) fillWidth = 150;
        if (fillWidth > 0) {
            SDL_Rect progressBarFill = {223, 37, fillWidth, 14};
            SDL_SetRenderDrawColor(renderer, theme.accentColor.r, theme.accentColor.g, theme.accentColor.b, 255);
            SDL_RenderFillRect(renderer, &progressBarFill);
        }
    }
    
    /* Draw progress text inside bar */
    snprintf(progressText, sizeof(progressText), "%d/%d", progressInLevel, catsNeeded);
    drawTextWithOutline(renderer, progressText, 255, 36, 1, theme.text, outlineColor);
    
    /* Middle-right section: LIVES */
    snprintf(livesText, sizeof(livesText), "LIVES: %d", gLives);
    drawTextWithOutline(renderer, livesText, 420, 15, 2, theme.text, outlineColor);
    
    /* Draw heart symbols for lives */
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    for (i = 0; i < gLives; ++i) {
        int heartX = 440 + i * 12;
        int heartY = 35;
        /* Simple heart representation */
        SDL_Rect heart = {heartX, heartY, 8, 8};
        SDL_RenderFillRect(renderer, &heart);
    }
    
    /* Right section: Theme indicator */
    snprintf(themeText, sizeof(themeText), "[T] %s", gTheme == THEME_DARK ? "M" : "S");
    drawTextWithOutline(renderer, themeText, 620, 15, 2, theme.text, outlineColor);
}

static void render(void) {
    int i;

    if (gGameState == STATE_THEME_SELECTION) {
        drawThemeSelection(gRenderer);
        return;
    }

    /* Playing or Game Over state */
    ColorTheme theme = getThemeColors();
    Uint32 currentTime = SDL_GetTicks();

    SDL_SetRenderDrawColor(gRenderer, theme.background.r, theme.background.g, theme.background.b, 255);
    SDL_RenderClear(gRenderer);

    /* Draw sparkling background and scene */
    drawStarryBackground(gRenderer, currentTime);
    drawMoon(gRenderer);
    
    /* Light mode: draw sun and clouds */
    if (gTheme == THEME_LIGHT) {
        drawSun(gRenderer);
        drawClouds(gRenderer, currentTime);
    }

    /* HUD bar */
    drawHUD(gRenderer, theme);

    for (i = 0; i < MAX_CATS; ++i) {
        if (!gCats[i].active) {
            continue;
        }

        drawCat(gRenderer, &gCats[i], currentTime);
    }

    if (gGameState == STATE_GAME_OVER) {
        SDL_Rect overlay = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_Rect darkButton = {150, 360, 200, 80};
        SDL_Rect lightButton = {450, 360, 200, 80};

        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
        
        if (gTheme == THEME_LIGHT) {
            SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 160);
        } else {
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 160);
        }
        
        SDL_RenderFillRect(gRenderer, &overlay);

        if (gGameWon) {
            drawText(gRenderer, "YOU HAVE WON!", 180, 210, 5, theme.accentColor);
        } else {
            drawText(gRenderer, "GAME OVER", 230, 210, 5, theme.text);
        }
        
        drawText(gRenderer, "Choose a theme to continue", 160, 300, 2, theme.text);

        /* Dark Mode button */
        SDL_SetRenderDrawColor(gRenderer, theme.accentColor.r, theme.accentColor.g, theme.accentColor.b, theme.accentColor.a);
        SDL_RenderDrawRect(gRenderer, &darkButton);
        drawText(gRenderer, "DARK MODE", 175, 395, 2, theme.accentColor);

        /* Light Mode button */
        SDL_RenderDrawRect(gRenderer, &lightButton);
        drawText(gRenderer, "LIGHT MODE", 465, 395, 2, theme.accentColor);
        
        drawText(gRenderer, "PRESS R", 320, 480, 3, theme.text);

        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
    }

    SDL_RenderPresent(gRenderer);
}

static void cleanup(void) {
    if (gRenderer) {
        SDL_DestroyRenderer(gRenderer);
        gRenderer = NULL;
    }

    if (gWindow) {
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
    }

    cleanupAudio();
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    Uint32 previousTicks;
    (void)argc;
    (void)argv;

    if (!init()) {
        cleanup();
        return 1;
    }

    previousTicks = SDL_GetTicks();

    while (gRunning) {
        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - previousTicks) / 1000.0f;

        if (deltaTime > 0.05f) {
            deltaTime = 0.05f;
        }

        previousTicks = currentTicks;

        handleEvents();
        updateGame(deltaTime);
        render();
    }

    cleanup();
    return 0;
}
