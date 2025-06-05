#include "kys_engine.h"
#include <math.h>
#include <io.h>

int DrawUText(char *text, int x_pos, int y_pos, Uint32 color, int engwidth)
{
    SDL_Texture *tex;
    SDL_Surface *sur;
    SDL_Color col;
    SDL_Rect rect;
    byte r, g, b;

    GetRGBA(color, &r, &g, &b, NULL);

    col.r = r;
    col.g = g;
    col.b = b;
    rect.x = x_pos;
    rect.y = y_pos;

    sur = TTF_RenderText_Blended(Font, text, 10,col);
    rect.h = (*sur).h;
    rect.w = (*sur).w;
    tex = SDL_CreateTextureFromSurface(render, sur);
    SDL_RenderTexture(render, tex, NULL, &rect);
    SDL_DestroySurface(sur);
    SDL_DestroyTexture(tex);
    return 0;
}

int DrawShadowText(char *text, int x_pos, int y_pos, Uint32 color1, Uint32 color2)
{
    DrawUText(text, x_pos + 1, y_pos, color2, 20);
    DrawUText(text, x_pos, y_pos, color1, 20);
}

char *ReadFileToBuffer(char *p, char *filename, int size, int needmalloc, int *filesize)
{
    FILE *fp;
    char *r = NULL;
    fp = fopen(filename, "r");
    if (fp != NULL)
    {
        if (size < 0)
        {
            fseek(fp, 0, SEEK_END);
            size = ftell(fp);
            if (filesize)
                *filesize = size;
        }
        if (needmalloc == 1)
        {
            p = (char *)malloc(size + 4);
            r = p;
        }
        fseek(fp, 0, SEEK_SET);
        fread(p, 1, size, fp);
        fclose(fp);
    }
    else
    {
        if (needmalloc == 1)
            r = NULL;
    }
    return r;
}

void FreeFileBuffer(char *p)
{
    if (!p)
        free(p);
}

int LoadPNGTiles(char *path, PNGIndex **P, SDL_Texture ***T, SDL_Surface ***S, int LoadPic)
{
    const int maxCount = 9999;

    FILE *fp;
    char c[255], c_0[255];
    short int *p;
    int size, Count, i, k, result;
    PNGIndex *pi;


    sprintf(c, "%s%s", path, "/index.ka");
    printf("Searching index of png files ... %s\n", c);

    p = (short int *)ReadFileToBuffer(NULL, c, -1, 1, &size);
    result = 0;

    for (i = size / 4; i >= 0; i--)
    {
        sprintf(c, "%s/%d.png", path, i);
        if (access(c, 0) == 0)
        {
            result = i + 1;
            break;
        }
        else
        {
            sprintf(c_0, "%s/%d.png", path, i);
            if (access(c_0, 0) == 0)
            {
                result = i + 1;
                break;
            }
        }
    }

    *P = (PNGIndex *)malloc(sizeof(PNGIndex) * result);
    pi = *P;
    Count = 0;
    for (i = 0; i < result; i++)
    {
        pi[i].FileNum = i;
        pi[i].PointerNum = -1;
        pi[i].Frame = 0;
        pi[i].CurPointer = NULL;
        sprintf(c, "%s/%s/%d.png", AppPath, path, i);
        if (access(c, 0) == 0)
        {
            pi[i].PointerNum = Count;
            pi[i].Frame = 1;
            Count++;
        }
        else
        {
            k = 0;
            sprintf(c, "%s/%s/%d_%d.png", AppPath, path, i, k);
            while(access(c, 0) == 0)
            {
                k++;
                if (k = 1)
                    pi[i].PointerNum = Count;
                Count++;
            }
            pi[i].Frame = k;
        }
        pi[i].x = p[i * 2];
        pi[i].y = p[i * 2 + 1];
        pi[i].Loaded = 0;
        pi[i].UseGRP = 0;
    }

    FreeFileBuffer((char *)p);

    *T = (SDL_Texture **)malloc(sizeof(SDL_Texture *) * Count);
    for(i = 0; i < result; i++)
        pi[i].BeginPointer = *T;


    if (LoadPic)
    {
        for(i = 0; i < result; i++)
        {
            LoadOnePNGTexture(path , NULL, &pi[i], LoadPic);
        }
    }
    return result;


}

void LoadOnePNGTexture(char *path, char *p, PNGIndex *P, int forceLoad)
{
    char c[255];
    SDL_Texture *temptex;

    P->CurPointerT = (SDL_Texture **)P->BeginPointer;
    P->CurPointerT += P->PointerNum;

    sprintf(c, "%s/%s/%d.png", AppPath, path, P->FileNum);

    if (((P->Loaded == 0) || (forceLoad == 1)) && (P->PointerNum >= 0) && (P->Frame > 0))
    {
        *(P->CurPointerT) = IMG_LoadTexture(render, c);
        //SDL_QueryTexture(*(P->CurPointerT), NULL, NULL, &P->w, &P->h);
    }

    P->Loaded = 1;

}


int DrawPNGTile(SDL_Renderer *render, PNGIndex P, int FrameNum,
                int px, int py, SDL_Rect *region, int shadow, int alpha, Uint32 mixColor, int mixAlpha,
                double scalex, double scaley, double angle, SDL_Point *center)
{
    SDL_Rect rect;
    byte r, g, b, a, r1, g1, b1;
    bool newtex = 0;
    SDL_Texture *tex, *tex1, *ptex;

    if (SW_SURFACE != 0)
    {
        //DrawPNGTileS(CurTargetSurface, PNGIndex, FrameNum, px, py, region, shadow, alpha, mixColor,
        //mixAlpha, scalex, scaley, angle);
        return 0;
    }
    //shadow设置混合方式, 以及预设值等
    if ((P.CurPointerT == NULL) || (*(P.CurPointerT) == NULL))
    {
        return 0;
    }
    if (P.Frame > 1)
    {
        P.CurPointerT += FrameNum % P.Frame;
    }
    tex = *(P.CurPointerT);
    rect.x = px - P.x;
    rect.y = py - P.y;
    if (region == NULL)
    {
        rect.w = P.w;
        rect.h = P.h;
    }
    else
    {
        rect.w = region->w;
        rect.h = region->h;
    }
    if ((scalex != 1) || (scaley != 1))
    {
        rect.w = (int)floor(rect.w * scalex);
        rect.h = (int)floor(rect.h * scaley);
    }
    else
    {

    }

    if ((shadow < 0) && (mixAlpha == 0))
    {
        mixColor = 0;
        mixAlpha = -25 * shadow;
    }
    if ((shadow > 0) && (mixAlpha == 0))
    {
        mixColor = 0xFFFFFFFF;
        mixAlpha = shadow * 10;
    }

    //注意混合色的问题, 该算法很奇怪, 若强制指定混合色用mixAlpha < 0
    if ((mixAlpha > 0) && (shadow <= 0))
    {
        GetRGBA(mixColor, &r, &g, &b, NULL);
        r1 = max(0, 255 - (255 + g + b) * mixAlpha / 100);
        g1 = max(0, 255 - (255 + r + b) * mixAlpha / 100);
        b1 = max(0, 255 - (255 + r + g) * mixAlpha / 100);
        SDL_SetTextureColorMod(tex, r1, g1, b1);
    }
    else
    {
        SDL_SetTextureColorMod(tex, 255, 255, 255);
    }
    if (mixAlpha < 0)
    {
        GetRGBA(mixColor, &r, &g, &b, NULL);
        SDL_SetTextureColorMod(tex, r, g, b);
    }
    if ((mixAlpha > 0) && (shadow > 0))
    {
        GetRGBA(mixColor, &r, &g, &b, NULL);
        ptex = SDL_GetRenderTarget(render);
        tex1 = tex;
        tex = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, P.w, P.h);
        newtex = 1;
        SDL_SetRenderTarget(render, tex);
        SDL_SetTextureBlendMode(tex1, SDL_BLENDMODE_NONE);
        SDL_RenderTexture(render, tex1, NULL, NULL);
        SDL_SetTextureBlendMode(tex1, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_ADD);
        SDL_SetRenderDrawColor(render, r, g, b, 255 * mixAlpha / 100);
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(render, NULL);
        SDL_SetRenderTarget(render, ptex);
    }

    if (alpha > 0)
    {
        SDL_SetTextureAlphaMod(tex, 255 * (100 - alpha) / 100);
    }
    else
    {
        SDL_SetTextureAlphaMod(tex, 255);
    }
    //SDL_RenderTexture(render, tex, region, &rect, angle, center, SDL_FLIP_NONE);
    //SDL_RenderCopy(render, tex, nil, nil);
    if (newtex)
    {
        SDL_DestroyTexture(tex);
    }
    return 0;
}

void GetRGBA(Uint32 color, byte *r, byte *g, byte *b, byte *a)
{
#ifdef FAST
    unsigned char *tmpcolor = (unsigned char *)&color;
    if(b)*b = *tmpcolor++ & 0xff;
    if(g)*g = *tmpcolor++ & 0xff;
    if(r)*r = *tmpcolor++ & 0xff;
    if(a)*a = *tmpcolor & 0xff;
#else
    if (a) *a = color >> 24 & 0xff ;
    if (r) *r = color >> 16 & 0xff ;
    if (g) *g = color >> 8 & 0xff ;
    if (b) *b = color & 0xff ;
#endif

}

void UpdateAllScreen()
{
    SDL_RenderPresent(render);
}

void CleanKeyValue()
{
    event.key.scancode = 0;
    event.button.button = 0;
}

void CheckBasicEvent()
{}


void DrawRLE8Pic(
    const uint8_t *colorPanel, int num, int px, int py,
    const int *Pidx, const uint8_t *Ppic, const SDL_Rect *RectArea, SDL_Surface *Image,
    int widthI, int heightI, int sizeI, int shadow, int alpha,
    const uint8_t *BlockImageW, const int *BlockPosition,
    int widthW, int heightW, int sizeW, int depth, uint32_t mixColor, int mixAlpha)
{
    int w, h, xs, ys, x, y, blockx = 0, blocky = 0;
    int offset, length, p, isAlpha, lenInt;
    int l, l1, ix, iy, pixdepth, curdepth, alpha1;
    uint32_t pix, colorin;
    uint8_t pix1, pix2, pix3, pix4, color1, color2, color3, color4;
    SDL_Rect area;

    if (num == 0)
    {
        offset = 0;
    }
    else
    {
        offset = Pidx[num - 1];
    }

    Ppic += offset;
    w = *(int16_t *)Ppic;
    Ppic += 2;
    h = *(int16_t *)Ppic;
    Ppic += 2;
    xs = *(int16_t *)Ppic;
    Ppic += 2;
    ys = *(int16_t *)Ppic;
    Ppic += 2;
    pixdepth = 0;

    if (!Image)
    {
        Image = SDL_GetWindowSurface(SDL_GetWindowFromID(1));
    }

    if (RectArea)
    {
        area = *RectArea;
    }
    else
    {
        area.x = 0;
        area.y = 0;
        area.w = Image->w;
        area.h = Image->h;
    }

    if (BlockPosition)
    {
        blockx = BlockPosition[0];
        blocky = BlockPosition[1];
    }

    alpha1 = (alpha >> 8) & 0xFF;

    if ((w > 1 || h > 1) && (px - xs + w >= area.x) && (px - xs < area.x + area.w) && (py - ys + h >= area.y) && (py - ys < area.y + area.h))
    {
        for (iy = 1; iy <= h; iy++)
        {
            l = *Ppic++;
            w = 1;
            p = 0;
            for (ix = 1; ix <= l; ix++)
            {
                l1 = *Ppic++;
                if (p == 0)
                {
                    w += l1;
                    p = 1;
                }
                else if (p == 1)
                {
                    p = 2 + l1;
                }
                else if (p > 2)
                {
                    p--;
                    x = w - xs + px;
                    y = iy - ys + py;
                    if (x >= area.x && y >= area.y && x < area.x + area.w && y < area.y + area.h)
                    {
                        pix1 = colorPanel[l1 * 3] * (4 + shadow);
                        pix2 = colorPanel[l1 * 3 + 1] * (4 + shadow);
                        pix3 = colorPanel[l1 * 3 + 2] * (4 + shadow);
                        pix4 = 0;

                        if (Image == SDL_GetWindowSurface(SDL_GetWindowFromID(1)))
                        {
                            pix = SDL_MapRGBA(Image->format, pix1, pix2, pix3, pix4);
                            if ((alpha < 100) || (pixdepth <= curdepth))
                            {
                                SDL_Rect pixelRect = { x, y, 1, 1 };
                                SDL_FillRect(Image, &pixelRect, pix);
                            }
                        }
                    }
                    w++;
                    if (p == 2)
                    {
                        p = 0;
                    }
                }
            }
        }
    }
}
