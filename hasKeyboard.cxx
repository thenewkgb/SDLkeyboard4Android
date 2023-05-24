#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

static SDL_Texture *loadText(SDL_Renderer *renderer, const char *text)
{
    if (text == NULL)
    {
        std::string db = "Null text";

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "title",
            db.c_str(),
            NULL);
    }

    if (TTF_Init() == -1)
    {
        std::string db = "TTF_Init Error";

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "title",
            db.c_str(),
            NULL);

        return NULL;
    }
    TTF_Font *font = TTF_OpenFont("/system/fonts/Roboto-Regular.ttf", 256);
    if (font == NULL)
    {
        std::string db = "TTF_OpenFont Error";

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "title",
            db.c_str(),
            NULL);

        return NULL;
    }
    SDL_Color color = {255, 255, 255};

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);

    if (surface == NULL)
    {
        std::string db = "Surface NULL";

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "title",
            db.c_str(),
            NULL);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if (texture == NULL)
    {
        std::string db = "Error: Texture NULL";

        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "title",
            db.c_str(),
            NULL);

        return NULL;
    }
    return texture;
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("Hello SDL2-ttf", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    if (window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    //
    // keyboard
    //
    bool hasKey = SDL_HasScreenKeyboardSupport();

    std::string temp{};
    if (hasKey)
        temp = "true";
    else
        temp = "false";

    int sw, sh;
    SDL_Event e{};
    SDL_TouchFingerEvent f{};
    SDL_TextInputEvent t{};

    SDL_Rect touch{};
    touch.x = 0;
    touch.y = 0;
    touch.w = 1;
    touch.h = 1;
    SDL_GetRendererOutputSize(renderer, &sw, &sh);

    SDL_Rect input{};
    input.x = 0;
    input.y = 0;
    input.h = 300;
    input.w = sw;

    SDL_Rect quit;
    quit.x = 0;
    quit.y = 300;
    quit.h = 300;
    quit.w = sw;

    // show device is capable

    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        "title", temp.c_str(), NULL);

    SDL_SetTextInputRect(&input);
    //SDL_SCANCODE_RETURN
    //SDLK_RETURN '\r'

    /*
    std::string on = "1";
   SDL_SetHint(
       SDL_HINT_IME_INTERNAL_EDITING,
       on.c_str());
   */

    char myText[10]{0};

    SDL_bool loop{SDL_TRUE};

    while (loop)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_FINGERUP:
            {
                f = e.tfinger;
                touch.x = f.x * sw;
                touch.y = f.y * sh;
                touch.w = 1;
                touch.h = 1;

                if (SDL_HasIntersection(&touch, &quit))
                {
                    loop = SDL_FALSE;
                }

                if (SDL_HasIntersection(&touch, &input))
                {
                    SDL_StartTextInput();
                }
                break;
            }
            case SDL_TEXTINPUT:
            {
                // read input once
                t = e.text;
                //myText = t.text;
                // concatenate!
                strcat(myText, t.text);
                //stoi will cut leading zeros
                break;
            }
            default:
            {
                if (e.key.keysym.sym == SDL_GetKeyFromScancode(SDL_SCANCODE_RETURN))
                {
                    loop = SDL_FALSE;
                }
                break;
            }
            }
        }

        // update screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // input box
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderFillRect(renderer, &input);
        // quitbox
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &quit);

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();

    //std::string dude = "dude";
    //strcat(myText, dude.c_str());

    // some to_int practice
    /*
    std::string s{"12"};
    int num = 30;
    
    num = std::stoi(myText);
    num += 100;
    s = std::to_string(num);
    strcpy(finalAns, s.c_str());
    */

    SDL_SetHint(
        SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_Texture *textTexture = loadText(renderer, myText);
    /*
    SDL_Texture *textTexture =
        loadText(renderer, finalAns);
    */

    if (textTexture == NULL)
    {
        std::string db = "Out of loadImage Error";
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "title",
            db.c_str(),
            NULL);

        return 1;
    }

    int tw, th;
    SDL_QueryTexture(textTexture, NULL, NULL, &tw, &th);
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    SDL_Rect dest;
    // Our text always has width much bigger than height, use this
    dest.x = 0;
    dest.w = w;
    dest.h = th * w / tw;
    dest.y = (h - dest.h) / 2;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, textTexture, NULL, &dest);
    SDL_RenderPresent(renderer);
    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
