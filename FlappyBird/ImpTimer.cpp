
#include "ImpTimer.h"
#include "CommonFunc.h"

ImpTimer::ImpTimer()
{
  start_tick_ = 0;
  paused_ticks_ = 0;
  is_paused_ = false;
  is_started_ = false;
}

void ImpTimer::start()
{
  is_started_ = true;
  is_paused_ = false;
  start_tick_ = SDL_GetTicks();
}

void ImpTimer::stop()
{
  is_started_ = false;
  is_paused_ = false;
}

void ImpTimer::pause()
{
  if( ( is_started_ == true ) && ( is_paused_ == false ) )
  {
    is_paused_ = true;
    // Lưu lại thời điểm tạm dừng
    paused_ticks_ = SDL_GetTicks() - start_tick_;
  }
}

void ImpTimer::unpause()
{
  if( is_paused_ == true )
  {
    is_paused_ = false;
    //ta dừng 1 khoảng thời gian rồi chơi tiếp, vậy nên cta sẽ
    //coi như bỏ thời gian tạm dừng ấy lên đầu và chơi xuyên suốt ko nghỉ
    // ví dụ như start =0 xong phút thứ 3 mình nghỉ, phút thứ 4 mình chơi tiếp
    // vậy coi start cùa mình =1 và đến phút thứ  5 thì ta đã chơi tổng cộng
    // là 5 - start = 4 phút
    start_tick_ = SDL_GetTicks() - paused_ticks_;
    //ko tạm dừng nữa vậy thì ta sẽ để pause_tick =0
    paused_ticks_ = 0;
  }
}

int ImpTimer::get_ticks()
{
  if( is_started_ == true )
  {
    //Nếu thời gian đang tạm dừng thì trả về lúc đang tạm dừng đó
    if( is_paused_ == true )
    {
      return paused_ticks_;
    }
    else//đây là thời gian đang tiếp tục trôi thì trả về tại chính thời điểm đó
    {
      return SDL_GetTicks() - start_tick_;
    }
  }

  //thêm phần chắc chắn thì return 0 nếu thời gian ko chạy
  return 0;
}

bool ImpTimer::is_started()
{
  return is_started_;
}

bool ImpTimer::is_paused()
{
  return is_paused_;
}