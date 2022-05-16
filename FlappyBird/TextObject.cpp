#include "TextObject.h"


TextObject::TextObject(void)
{
    //Màu trắng
  text_color_.r = 255;
  text_color_.g = 255;
  text_color_.b = 255;
}


TextObject::~TextObject(void)
{
}


bool TextObject::loadFromRenderedText(TTF_Font* gFont, SDL_Renderer* screen)
{
    //hàm này giống hàm load ảnh vậy
  //chuyển các thông số của font vào surface
  SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, str_val_.c_str(), text_color_);
  if( textSurface != NULL )
  {
    //load texture từ surface, các thông số của surface đều được biến texture_ nắm giữ
    texture_ = SDL_CreateTextureFromSurface(screen, textSurface );
    if( texture_ != NULL )
    {
      //gán kích thước dài rộng của chữ
      width_ = textSurface->w;
      height_ = textSurface->h;
    }

    //ko dùng đến surface nữa nên ta giải phóng nó
    SDL_FreeSurface( textSurface );
  }

  //trả về kết quả true nếu texture_ mà khác null tức là quá trình khởi
  //tạo ban đầu không có lỗi gì
  return texture_ != NULL;
}

void TextObject::Free()
{
  if(texture_ != NULL)
  {
    SDL_DestroyTexture(texture_);
    texture_ = NULL;
  }
}

//ở đây có 2 hàm setColor, 1 cái là set thẳng mã màu RGB,
//2 là set theo cái mà mình đặt sẵn trong enum vừa nãy

void TextObject::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
  text_color_.r = red;
  text_color_.g = green;
  text_color_.b = blue;
}

void TextObject::setColor(int type)
{
  if (type == RED_TEXT)
  {
    SDL_Color color = {255, 0, 0};
    text_color_ = color;
  }
  else if (type == WHITE_TEXT)
  {
    SDL_Color color = {255, 255, 255};
    text_color_ = color;
  }
  else
  {
    SDL_Color color = {0, 0, 0};
    text_color_ = color;
  }
}
void TextObject::RenderText(SDL_Renderer* screen, int x, int y, SDL_Rect* clip , double angle , SDL_Point* center , SDL_RendererFlip flip )
{
  //khởi tạo vị trí và kích thước khi render text ra màn hình, lưu nó trong renderQuad
  SDL_Rect renderQuad = { x, y, width_, height_ };

  //gán kích thước cho clip
  if( clip != NULL )
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  //xuất text ra màn hình với các thông số mình đã khởi tạo ở trên
  SDL_RenderCopyEx(screen, texture_, clip, &renderQuad, angle, center, flip );
}
