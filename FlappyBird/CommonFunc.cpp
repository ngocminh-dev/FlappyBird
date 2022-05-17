
#include "CommonFunc.h"

//Hàm khởi tạo menu, hiển thị trên screen với font chữ, 2 menu chọn và hiển thị tấm ảnh tương ứng
int SDLCommonFunc::ShowMenu(SDL_Renderer* g_screen, TTF_Font* font, 
                            const std::string& menu1, 
                            const std::string& menu2,
                            const std::string& img_name)
{
    //chuyển các string về char* để dễ thao tác
    char* ch1 = (char*)menu1.c_str();
    char* ch2 = (char*)menu2.c_str();
    char* img_file = (char*)img_name.c_str();

    int size1 = menu1.length();
    int size2 = menu2.length();

    int time = 0;
    int x = 0;
    int y = 0;
    const int kMenuNum = 2;
    char* labels[kMenuNum];//Mảng này sẽ chứa các nhãn menu1 và menu2

    labels[0] = new char [size1 + 1];
    labels[1] = new char[size2 + 1];

    strcpy_s(labels[0], size1+1, ch1);
    strcpy_s(labels[1], size2+1, ch2);
    
    bool selected[kMenuNum] = { 0, 0 };//selected sẽ đổi thành 1 khi ta di chuột vào
    SDL_Color color[2] = { { 255, 255, 255 },{ 255, 0, 0 } };//Màu 1 là màu ban đầu, màu 2 là khi ta di chuột vào

    //Khởi tạo mảng text object và load 2 nhãn menu lên screen
    TextObject text_object[kMenuNum];
    text_object[0].SetText(labels[0]);
    text_object[0].setColor(color[0].r, color[0].g, color[0].b);

    text_object[0].loadFromRenderedText(font, g_screen);

    text_object[1].SetText(labels[1]);
    text_object[1].setColor(color[0].r, color[0].g, color[0].b);
    text_object[1].loadFromRenderedText(font, g_screen);


    //Vị trí của 2 menu được bố trí như sau
    SDL_Rect pos[kMenuNum];
    pos[0].x = SCREEN_WIDTH*0.3 - 150;
    pos[0].y = SCREEN_HEIGHT*0.8 - 40;
    pos[1].x = SCREEN_WIDTH*0.3 - 150;
    pos[1].y = SCREEN_HEIGHT*0.8 + 40;

    //Load background lên screen
    BaseObject gBackground;
    bool ret = gBackground.LoadImageFile(img_file, g_screen);

    SDL_Event event;
    while (1)
    {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                text_object[0].Free();
                text_object[0].Free();
                gBackground.Free();
                return 1;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < kMenuNum; ++i)
                {
                    //Nếu mà chuột di vào menu đó thì ta lại set text lại, nội dung vẫn thế nhưng thay đổi màu và load lên screen
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        if (!selected[i])
                        {
                            selected[i] = 1;
                            text_object[i].SetText(labels[i]);
                            text_object[i].setColor(color[1].r, color[1].g, color[1].b);
                            text_object[i].loadFromRenderedText(font, g_screen);
                        }
                    }
                    else//Khi chuột của chúng ta không còn ở trong khoảng menu đó nữa thì sẽ giải phóng chữ màu đấy đi
                        //set text lại với màu trắng
                    {
                        if (selected[i])
                        {
                            selected[i] = 0;
                            text_object[i].Free();
                            text_object[i].SetText(labels[i]);
                            text_object[i].setColor(color[0].r, color[0].g, color[0].b);
                            text_object[i].loadFromRenderedText(font, g_screen);
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN://Nếu click vào menu thì sẽ giải phóng hết hình ảnh, text của menu để chuyển sang mục tương ứng
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < kMenuNum; ++i) {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        text_object[0].Free();
                        text_object[1].Free();
                        gBackground.Free();
                        return i;
                    }
                }
                break;
            }
        }
        //Hiển thị background và text lên màn hình với tọa độ mình đã set ở trên
        gBackground.Render(g_screen, NULL);
        for (int i = 0; i < kMenuNum; ++i)
        {
            text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
            pos[i].w = text_object[i].getWidth();
            pos[i].h = text_object[i].getHeight();
        }

        //FPS
        SDL_RenderPresent(g_screen);
        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }

    return 0;
}


int SDLCommonFunc::MyRandom(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2) 
{
  int left_a = object1.x;
  int right_a = object1.x + object1.w;
  int top_a = object1.y;
  int bottom_a = object1.y + object1.h;

  int left_b = object2.x;
  int right_b = object2.x + object2.w;
  int top_b = object2.y;
  int bottom_b = object2.y + object2.h;

  // Trường hợp 1: object a < object b thì ta check 4 góc của object a xem có nằm trong vùng của object b ko 
  //Nếu có thì là va chạm -> trả về true
  if (left_a > left_b && left_a < right_b)
  {
    if (top_a > top_b && top_a < bottom_b)
    {
      return true;
    }
  }

  if (left_a > left_b && left_a < right_b)
  {
    if (bottom_a > top_b && bottom_a < bottom_b)
    {
      return true;
    }
  }

  if (right_a > left_b && right_a < right_b)
  {
    if (top_a > top_b && top_a < bottom_b)
    {
      return true;
    }
  }

  if (right_a > left_b && right_a < right_b)
  {
    if (bottom_a > top_b && bottom_a < bottom_b)
    {
      return true;
    }
  }

  // Trường hợp 2 là object a lớn hơn object b, ta làm tương tự với TH1
  if (left_b > left_a && left_b < right_a)
  {
    if (top_b > top_a && top_b < bottom_a)
    {
      return true;
    }
  }

  if (left_b > left_a && left_b < right_a)
  {
    if (bottom_b > top_a && bottom_b < bottom_a)
    {
      return true;
    }
  }

  if (right_b > left_a && right_b < right_a)
  {
    if (top_b > top_a && top_b < bottom_a)
    {
      return true;
    }
  }

  if (right_b > left_a && right_b < right_a)
  {
    if (bottom_b > top_a && bottom_b < bottom_a)
    {
      return true;
    }
  }

  // Trường hợp cuối cùng là 2 object đó bằng nhau
  if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
  {
    return true;
  }

  return false;
}

