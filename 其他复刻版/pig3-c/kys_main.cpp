#include <stdio.h>
#include "kys_main.h"
#include "kys_engine.h"
#include "kys_draw.h"
#include "kys_type.h"

#include <string>

void Run() {
    SDL_Surface* Text;
    uint16_t word[2] = { 32, 0 };
    SDL_Color tempcolor;
    std::string AppPath;
    std::string str;

#ifdef UNIX
    AppPath = std::string(SDL_GetBasePath());
#else
    AppPath = "";
#endif

#ifdef ANDROID
    AppPath = std::string(SDL_AndroidGetExternalStoragePath()) + "/game/";
    str = std::string(SDL_AndroidGetExternalStoragePath()) + "/place_game_here";
    SDL_RWops* file = SDL_RWFromFile(str.c_str(), "wb");
    if (file) SDL_RWclose(file);
#endif

    ReadFiles();
    SetMODVersion();

    TTF_Init();
    TTF_Font* font = TTF_OpenFont((AppPath + CHINESE_FONT).c_str(), CHINESE_FONT_SIZE);
    TTF_Font* engfont = TTF_OpenFont((AppPath + ENGLISH_FONT).c_str(), ENGLISH_FONT_SIZE);

    Text = TTF_RenderUNICODE_Solid(font, word, tempcolor);
    CHNFONT_SPACEWIDTH = Text->w;
    SDL_FreeSurface(Text);

    // 初始化音频系统
    int SoundFlag = 0;
    if (SOUND3D == 1) SoundFlag |= BASS_DEVICE_3D;
    BASS_Init(-1, 22050, SoundFlag, nullptr, nullptr);

    // 初始化视频系统
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Quit();
        return;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    Uint32 ScreenFlag = SDL_WINDOW_RESIZABLE;
    SDL_Window* window = SDL_CreateWindow(TitleString, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RESOLUTIONX, RESOLUTIONY, ScreenFlag);

    if (!window) {
        SDL_Quit();
        return;
    }

    SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    SDL_Surface* screen = SDL_CreateRGBSurface(0, CENTER_X * 2, CENTER_Y * 2, 32, RMask, GMask, BMask, 0);
    SDL_Texture* screenTex = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, CENTER_X * 2, CENTER_Y * 2);

    InitialScript();
    InitialMusic();

    SDL_SetEventFilter(EventFilter, nullptr);
    SDL_mutex* mutex = SDL_CreateMutex();

    SDL_AddTimer(200, UpdateSceneAmi, nullptr);

    Start();
    Quit();
}

int Quit()
{
    SDL_Quit();
    return 0;
}

int SetMODVerion(void)
{
    MODVersion = 0;
    StartMusic = 59;
    TitleString = "Legend of Little Village III - 108 Brothers and Sisters";
    GetCurrentDirectory(MAX_PATH, AppPath);
    CHINESE_FONT = "font/chinese.ttf";
    CHINESE_FONT_SIZE = 20;
    CHINESE_FONT_REALSIZE = 20;
    ENGLISH_FONT = "font/english.ttf";
    ENGLISH_FONT_SIZE = 18;
    ENGLISH_FONT_REALSIZE = 18;

    OpenPicPosition.x = 0;
    OpenPicPosition.y = 0;

    versionstr = "金庸水滸傳演示版";
    //switch(MODVersion)
    return 0;
}

int ReadFiles(void)
{
    char inifile[255];
    strcpy(inifile, AppPath);
    strcat(inifile, "/kysmod.ini");

    RENDERER = GetPrivateProfileInt("system", "RENDERER", 0, inifile);
    CENTER_X = GetPrivateProfileInt("system", "CENTER_X", 480, inifile);
    CENTER_Y = GetPrivateProfileInt("system", "CENTER_Y", 270, inifile);
    RESOLUTIONX = GetPrivateProfileInt("system", "RESOLUTIONX", CENTER_X * 2, inifile);
    RESOLUTIONY = GetPrivateProfileInt("system", "RESOLUTIONY", CENTER_Y * 2, inifile);

    SOUND3D = GetPrivateProfileInt("music", "SOUND3D", 0, inifile);

    //printf("%s%d\n", inifile, SOUND3D);
    return 0;

}

int SetFontSize(int Chnsize, int engsize, int force)
{
    Font = TTF_OpenFont(CHINESE_FONT, CHINESE_FONT_REALSIZE);
    return 0;

}

int Start()
{
    int i;
    LoadPNGTiles("resource/title", &TitlePNGIndex, &TitlePNGTex, &TitlePNGSur, 1);

    NewStartAmi();
    wprintf(L"%ls", versionstr);
    return 0;

}

void NewStartAmi()
{
    boolean breakami;
    int i, j, x, y;
    SDL_Rect src, dest;

    Where = 4;
    x = CENTER_X - 34;
    y = CENTER_Y - 75;
    for(i = 0; i <= 20; i++)
    {
        Redraw();
        DrawTPic(9, x, y, NULL, 0, 100 - i * 5, 0, 0, 0);
        UpdateAllScreen();
        SDL_Delay(20);
        if(SDL_PollEvent(&event) >= 0)
        {
            CleanKeyValue();
            if((event.key.key == SDLK_ESCAPE) || (event.button.button == SDL_BUTTON_RIGHT))
            {
                return;
            }
            CheckBasicEvent();
        }
    }

    for(i = 0; i <= 60; i++)
    {
        Redraw();
        x = x - 5;
        y = y - 2;
        DrawTPic(9, x, y, NULL, 0, 0, 0, 0, 0);
        UpdateAllScreen();
        SDL_Delay(20);
        if(SDL_PollEvent(&event) >= 0)
        {
            if((event.key.key == SDLK_ESCAPE) || (event.button.button == SDL_BUTTON_RIGHT))
            {
                return;
            }
            CheckBasicEvent();
        }
    }

    src.x = 0;
    src.y = 0;
    src.w = TitlePNGIndex[12].w;
    src.h = TitlePNGIndex[12].h;
    dest.x = x;
    dest.y = y + 10;
    for(i = 0; i <= 89; i++)
    {
        Redraw();
        src.w = i * 5 + 50;
        if(src.w > 490) break;
        DrawTPic(12, x, y + 10, &src, 0, 0, 0, 0, 0);
        DrawTPic(10, x, y, NULL, 0, 0, 0, 0, 0);
        DrawTPic(10, x + i * 5 + 34, y, NULL, 0, 0, 0, 0, 0);
        UpdateAllScreen();
        SDL_Delay(20);
        if(SDL_PollEvent(&event) >= 0)
        {
            if((event.key.key == SDLK_ESCAPE) || (event.button.button == SDL_BUTTON_RIGHT))
            {
                return;
            }
            CheckBasicEvent();
        }
    }

    Where = 3;
    for(i = 0; i <= 2; i++)
    {
        Redraw();
        DrawTPic(14 + i, CENTER_X + 50, CENTER_Y, NULL, 0, 75, 0, 0, 0);
        DrawTPic(14 + i,  CENTER_X + 50,  CENTER_Y + 50, NULL, 0, 50, 0, 0, 0);
        DrawTPic(14 + i,  CENTER_X + 50,  CENTER_Y + 100, NULL, 0, 25, 0, 0, 0);
        UpdateAllScreen();
        SDL_Delay(20);
        if(SDL_PollEvent(&event) >= 0)
        {
            if((event.key.key == SDLK_ESCAPE) || (event.button.button == SDL_BUTTON_RIGHT))
            {
                return;
            }
            CheckBasicEvent();
        }
    }

    for(i = 0; i <= 20; i++)
    {
        Redraw();
        for(j = 0; j <= 2; j++)
        {
            DrawTPic(16, CENTER_X + 50, CENTER_Y + j * 50, NULL, 0, 25, 0, 0, 0);
            DrawTPic(3 + j, CENTER_X + 90, CENTER_Y + j * 50, NULL, 0, 100 - i * 5, 0, 0, 0);
        }
        UpdateAllScreen();
        SDL_Delay(20);
        if(SDL_PollEvent(&event) >= 0)
        {
            if((event.key.key == SDLK_ESCAPE) || (event.button.button == SDL_BUTTON_RIGHT))
            {
                return;
            }
            CheckBasicEvent();
        }
    }
}