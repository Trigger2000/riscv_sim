#ifndef __PC_H_INCLUDED__
#define __PC_H_INCLUDED__

class ProgramCounter
{
  public:
    ProgramCounter() : current_pc_(0), next_pc_(1)
    {}
    ~ProgramCounter() = default;

    void reset()
    {
        current_pc_ = 0;
        next_pc_ = current_pc_ + 1;
    }

    void move_next()
    {
        current_pc_++;
        next_pc_ = current_pc_ + 1;
    }

    void move_offset(int32_t offset)
    {
        current_pc_ += offset;
        next_pc_ = current_pc_ + 1;
    }

    uint32_t get_current() const
    {
        return current_pc_;
    }

  private:
    uint32_t current_pc_ = 0;
    uint32_t next_pc_ = 1;
};

#endif