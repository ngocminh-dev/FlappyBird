
#include "BlockObject.h"

//Vị trí của các cột ban đầu
INT g_pos[] = { 1250, 1500, 1750, 2000, 2250, 2500 };

// định nghĩa class 1 cột
BlockObject::BlockObject()
{
    is_back_ = false;
}

BlockObject::~BlockObject()
{

}


bool BlockObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImageFile(path, screen);
    return ret;
}

void BlockObject::SetPos(const int& xp, const int& yp)
{
    this->SetRect(xp, yp);
}

void BlockObject::SetXPos(const int& xp)
{
    SDL_Rect rect = this->GetRect();
    this->SetRect(xp, rect.y);
}

void BlockObject::ShowImg(SDL_Renderer* screen)
{
    this->Render(screen);
}

//Cho cột di chuyển phụ thuộc vào x_val
void BlockObject::DoRun(UINT& x_val)
{
    this->rect_.x += x_val;
    if (rect_.x + rect_.w < 0)
    {
        is_back_ = true;
    }
}


// định nghĩa class cả 2 cột trên dưới
DoubleBlock::DoubleBlock()
{
    x_val_ = -3;
    is_back_ = false;
    is_pass_ = false;
}

DoubleBlock::~DoubleBlock()
{

}

bool DoubleBlock::InitBlock(SDL_Renderer* screen , const int& xp)
{
    //Lấy ngẫu nhiên 1 con số từ 1 đến 10
    int number = SDLCommonFunc::MyRandom(1, 10);
    int number2 = number;
    //Nhân với 14 thì number trong khoảng từ 14 đến 140, còn lí do tại sao thì ở dưới sẽ giải thích
    //các cột sẽ lên xuống ngẫu nhiên 1 khoảng là number2
    number = number * 14;

    bool ret1 = m_Topblock.LoadImg("img//topblock2.png", screen);
    bool ret2 = m_BottomBlock.LoadImg("img//bottomblock2.png", screen);

    if (number2 % 2 == 0) 
    {
        //khoảng không chúng ta để cho chú chim có thể vượt qua đều như nhau và bằng 160.
        //Chọn số 160 vì ảnh chim là 64x40
        // để vượt qua ống chỉ có 4 vị trí (160/40=4) đó là con số vừa đủ cũng như là tăng độ khó cho game
        // ảnh ống là 64*520
        //Vậy để 2 ống cách nhau 160 thì chúng ta chỉ cần vẽ hình rồi tính toán xem đặt y_pos làm sao cho hợp lí
        m_Topblock.SetPos(xp, -300 - number);
        m_BottomBlock.SetPos(xp, GROUND_MAP - 220 - number);
    }
    else
    {   
        //Nếu chia hết cho 2 thì ta sẽ hạ ống xuống còn nếu không chia hết cho 2 thì sẽ nâng ống lên
        //Nhưng vẫn đảm bảo 2 ống cách nhau 1 khoảng là 160
        m_Topblock.SetPos(xp, -300 + number);
        m_BottomBlock.SetPos(xp, GROUND_MAP - 220 + number);
    }
    
    //Nếu load được ảnh thì đương nhiên là trả về true rồi, còn không thì mặc định sẽ trả về false
    if (ret1 && ret2)
        return true;
    return false;
}

void DoubleBlock::RenderImg(SDL_Renderer* screen)
{
    //Hiển thị cột
    m_Topblock.ShowImg(screen);
    m_BottomBlock.ShowImg(screen);
}

void DoubleBlock::DoMoving()
{
    //Cho cả 2 cột trên dưới chạy về phía trước
    //Do x_val_ mình cài ban đầu là nó âm
    m_Topblock.DoRun(x_val_);
    m_BottomBlock.DoRun(x_val_);
    if (m_Topblock.GetStateBack() || m_BottomBlock.GetStateBack())
    {
        is_back_ = true;
    }
}

SDL_Rect DoubleBlock::GetTopRect()
{
   return m_Topblock.GetRectPos();
}

void DoubleBlock::SetRectVal(const UINT& xp)
{
    m_Topblock.SetXPos(xp);
    m_BottomBlock.SetXPos(xp);
}

void DoubleBlock::SetIsBack(bool isBack)
{
    is_back_ = isBack;
    m_Topblock.SetBack(isBack);
    m_BottomBlock.SetBack(isBack);
}

void DoubleBlock::GetRectSlot() //Hàm này tạo ra phần tính điểm
{
   
    SDL_Rect rect_top = m_Topblock.GetRectPos();

    slot_rect_.x = rect_top.x + rect_top.w;
    slot_rect_.y = rect_top.y + rect_top.h;
    slot_rect_.w = 5;
    slot_rect_.h = 160;
    //Sau ta chỉ cần check va chạm của 2 object là con chim và phần tính điểm
}


bool DoubleBlock::CheckPass(SDL_Rect rect)
{
    //Check va chạm với phần tính điểm, 
    //nếu chạm vào phần tính điểm đó( tức là qua được cột) trả về true, không qua được trả về false
    bool ret = false;
    ret = SDLCommonFunc::CheckCollision(rect, slot_rect_);
    return ret;
}

bool DoubleBlock::CheckCol(SDL_Rect pl_rect)
{
    //Check va chạm với cột trên và dưới
    bool ret1 = SDLCommonFunc::CheckCollision(pl_rect, m_Topblock.GetRectPos());
    bool ret2 = SDLCommonFunc::CheckCollision(pl_rect, m_BottomBlock.GetRectPos());

    if (ret1 || ret2)
    {
        return true;
    }
    
    return false;
}

//Quản lý các block

BlockManager::BlockManager()
{
    xp_max_ = 0;
    stop_moving_ = false;
    m_count = 0;
    is_col_ = false;
}

BlockManager::~BlockManager()
{
    FreeBlock();
}

void BlockManager::FreeBlock()//Giải phóng các block 
{
    for (int i = 0; i < m_BlockList.size(); i++)
    {
        DoubleBlock* pBlock = m_BlockList.at(i);
        delete pBlock;
        pBlock = NULL;
    }

    m_BlockList.clear();
}

bool BlockManager::InitBlockList(SDL_Renderer* screen)
{
    //Khởi tạo list block vì chướng ngại vật của chúng ta không chỉ có 1 cái mà có nhiều cái
    //vậy nên sẽ có 1 list để chứa các block đó
    DoubleBlock* m_block1 = new DoubleBlock();
    DoubleBlock* m_block2 = new DoubleBlock();
    DoubleBlock* m_block3 = new DoubleBlock();
    DoubleBlock* m_block4 = new DoubleBlock();
    DoubleBlock* m_block5 = new DoubleBlock();
    DoubleBlock* m_block6 = new DoubleBlock();

    //Load các block đó lên màn hình và đưa nó vào blocklist
    int ret = m_block1->InitBlock(screen, g_pos[0]);
    if (ret == false) return false;

    ret = m_block2->InitBlock(screen, g_pos[1]);
    if (ret == false) return false;

    ret = m_block3->InitBlock(screen, g_pos[2]);
    if (ret == false) return false;

    ret = m_block4->InitBlock(screen, g_pos[3]);
    if (ret == false) return false;

    ret = m_block5->InitBlock(screen, g_pos[4]);
    if (ret == false) return false;

    ret = m_block6->InitBlock(screen, g_pos[5]);
    if (ret == false) return false;

    m_BlockList.push_back(m_block1);
    m_BlockList.push_back(m_block2);
    m_BlockList.push_back(m_block3);
    m_BlockList.push_back(m_block4);
    m_BlockList.push_back(m_block5);
    m_BlockList.push_back(m_block6);

    xp_max_ = 5;
    return true;
}

void BlockManager::SetStopMoving(const bool& stop)
{
    stop_moving_ = stop;
}

void BlockManager::Render(SDL_Renderer* screen)
{
    for (int i = 0; i < m_BlockList.size(); i++)
    {
        DoubleBlock* pBlock = m_BlockList.at(i);
        // mỗi cột thì lại tạo ra phần tính điểm
        pBlock->GetRectSlot();

        if (!stop_moving_)
        {
            pBlock->DoMoving();//Cho nó chạy về phía trước

            bool ret = pBlock->GetIsBack();
            if (ret == true)//Nếu nó đang chạy về phía trước thì ta mới tạo ra cột chướng ngại vật tiếp theo
            {
                DoubleBlock* endBlock = m_BlockList.at(xp_max_);//Lấy tất cả thông số của cột cuối cùng
                SDL_Rect end_rect = endBlock->GetTopRect();//Lấy rect của cột đó
                UINT xp = end_rect.x + 250;//Lấy tọa độ x và cộng thêm 250 để các cột cách đều nhau
                pBlock->SetRectVal(xp);
                pBlock->SetIsBack(false);
                pBlock->SetIsPass(false);
                xp_max_ = i;
            }

            //Load âm thanh nếu chạm vào cột
            bool isCol = pBlock->CheckCol(player_rect_);
            if (isCol == true)
            {
                is_col_ = true;
                Mix_Chunk* beep_sound = Mix_LoadWAV("sound//punch.wav");
                if (beep_sound != NULL)
                    Mix_PlayChannel(-1, beep_sound, 0);
                break;
            }

            //Load âm thanh nếu vượt qua được 1 cột
            ret = pBlock->CheckPass(player_rect_);
            if (ret == true)
            {
                if (pBlock->GetIsPass() == false)
                {
                    Mix_Chunk* beep_sound = Mix_LoadWAV("sound//ting.wav");
                    if (beep_sound != NULL)
                        Mix_PlayChannel(-1, beep_sound, 0);
                    m_count++;
                    pBlock->SetIsPass(true);
                }
            }
        }
        
        pBlock->RenderImg(screen);

    }
}