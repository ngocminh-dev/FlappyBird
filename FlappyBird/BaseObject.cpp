
#include "BaseObject.h"


BaseObject::BaseObject()
{
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}

BaseObject::~BaseObject()
{
    Free();
}
void BaseObject::Free()
{
    if (p_object_ != NULL)
    {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}

bool BaseObject::LoadImageFile(std::string path, SDL_Renderer* screen)
{
    //giải phóng bộ nhớ trước đó còn lưu
    Free();
    SDL_Texture* newTexture = NULL;

    //Load ảnh từ path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface != NULL)
    {
        //Color key image
        Uint32 key = SDL_MapRGB(loadedSurface->format,
                                COLOR_KEY_R,
                                COLOR_KEY_G,
                                COLOR_KEY_B);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, key);

        //tạo texture từ surface, tất cả các thông số của tấm ảnh được nằm trong texture
        newTexture = SDL_CreateTextureFromSurface(screen, loadedSurface);
        if (newTexture != NULL)
        {
            //gán kích thước dài rộng của ảnh vào rect
            rect_.w = loadedSurface->w;
            rect_.h = loadedSurface->h;
        }

        //giải phóng surface
        SDL_FreeSurface(loadedSurface);
    }

    p_object_ = newTexture;
    return p_object_ != NULL;
}


void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip)
{
    SDL_Rect renderQuad = { rect_.x, rect_.y, rect_.w, rect_.h };
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    //chuyển toàn bộ thông tin của object lên des với vị trí
    //và kích thước được lưu trong biến renderQuad
    SDL_RenderCopy(des, p_object_, clip, &renderQuad);
    
}