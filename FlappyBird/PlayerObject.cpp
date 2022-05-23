
#include "PlayerObject.h"

PlayerObject::PlayerObject()
{
    //khởi tạo con chim chưa chạm đất
    is_falling_ = false;
    //y_val_ là giá trị biến thiên độ cao, lúc mới thì để nó nhỏ cho người chơi dễ bắt nhịp
    y_val_ = 1;
}

PlayerObject::~PlayerObject()
{
  Free();//kế thừa của base object, bên base object đã giải thích kĩ về các hàm này
}

void PlayerObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen) 
{
  if( events.type == SDL_KEYDOWN )//khi nhấn
  {
    switch( events.key.keysym.sym )
    {
    case SDLK_UP: //nút mũ tên lên
      {
        if (is_falling_ == false)
        {
            y_val_ = -8;
        }
      }
    }
  }
  else if( events.type == SDL_KEYUP )//khi thả ra
  {
    switch( events.key.keysym.sym )
    {
    case SDLK_UP://nút mũi tên lên
        {
            y_val_ = 5;
        }
        break;
    }
  }
}


bool PlayerObject::LoadImg(std::string path, SDL_Renderer* screen)
{
  bool ret = BaseObject::LoadImageFile(path, screen);//kế thừa của base object
  return ret;
}

void PlayerObject::Show(SDL_Renderer* des)
{
   BaseObject::Render(des);//kế thừa của base object
}

void PlayerObject::DoFalling(SDL_Renderer* des)
{
    rect_.y += y_val_;// để += là vì muốn nó rơi liên tục khi thả phím mũi tên lên
    if ((rect_.y + rect_.h) >= GROUND_MAP) // Nếu mà nó chạm đất thì game over
    {
        LoadImg("img//fl_bird2.png", des);//Load tấm ảnh con chim bị xỉu
        is_falling_ = true;// lúc này thì trạng thái nó đã bị rơi
        DoGround(des);// Rơi xuống đất rồi thì dùng hàm doGround để xử lí
    }
}

void PlayerObject::DoGround(SDL_Renderer* screen)
{
       // rơi xuống đất rồi thì ko được phép rơi nữa nên ta set y_val_=0
       // và con chim sẽ gãy cánh nằm ở đất nên tọa độ y( rect_.y) sẽ là tại đất
    y_val_ = 0;
    rect_.y = GROUND_MAP - rect_.h;
    is_die_ = true; // và chắc chắn nó sẽ die và game over
}