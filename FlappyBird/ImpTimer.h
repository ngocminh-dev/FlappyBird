
#ifndef IMP_TIMER_H_
#define IMP_TIMER_H_

class ImpTimer
{
public:
  ImpTimer();
  void start();
  void stop();
  void pause();
  void unpause();
  int get_ticks();
  bool is_started();
  bool is_paused();

private:
  //thời điểm bắt đâu
  int start_tick_;
  //thời điểm mà chúng ta tạm dừng
  int paused_ticks_;
  bool is_paused_;
  bool is_started_;
};

#endif