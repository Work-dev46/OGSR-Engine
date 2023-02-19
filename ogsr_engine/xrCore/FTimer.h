#pragma once

class CTimer_paused;

class XRCORE_API pauseMngr
{
    xr_vector<CTimer_paused*> m_timers;
    bool paused;

public:
    pauseMngr();
    bool Paused() const { return paused; }
    void Pause(const bool b);
    void Register(CTimer_paused& t);
    void UnRegister(CTimer_paused& t);
};

extern XRCORE_API pauseMngr g_pauseMngr;

class XRCORE_API CTimerBase
{
public:
    using Clock = std::chrono::high_resolution_clock;
    using Time = std::chrono::time_point<Clock>;
    using Duration = Time::duration;

protected:
    Time startTime;
    Duration pauseDuration;
    Duration pauseAccum;
    bool paused;

public:
    constexpr CTimerBase() noexcept : startTime(), pauseDuration(), pauseAccum(), paused(false) {}
    void Start()
    {
        if (paused)
            return;
        startTime = Now() - pauseAccum;
    }

    virtual Duration getElapsedTime() const
    {
        if (paused)
            return pauseDuration;
        else
            return Now() - startTime - pauseAccum;
    }

    u32 GetElapsed_ms() const
    {
        using namespace std::chrono;
        return u32(duration_cast<milliseconds>(getElapsedTime()).count());
    }

    float GetElapsed_sec() const
    {
        using namespace std::chrono;
        return duration_cast<duration<float>>(getElapsedTime()).count();
    }

    Time Now() const { return Clock::now(); }

    void Dump() const { Msg("* Elapsed time (sec): %f", GetElapsed_sec()); }
};

class XRCORE_API CTimer : public CTimerBase
{
    float m_time_factor;
    Duration realTime;
    Duration time;

    inline Duration getElapsedTime(const Duration& current) const
    {
        const auto delta = current - realTime;
        const double deltaD = double(delta.count());
        const double time = deltaD * m_time_factor + .5;
        const auto result = u64(time);
        return Duration(this->time.count() + result);
    }

public:
    constexpr CTimer() noexcept : m_time_factor(1.f), realTime(0), time(0) {}

    void Start() noexcept
    {
        if (paused)
            return;

        realTime = std::chrono::nanoseconds(0);
        time = std::chrono::nanoseconds(0);
        __super::Start();
    }

    float time_factor() const noexcept { return m_time_factor; }

    void time_factor(const float time_factor) noexcept
    {
        const Duration current = __super::getElapsedTime();
        time = getElapsedTime(current);
        realTime = current;
        m_time_factor = time_factor;
    }

    virtual Duration getElapsedTime() const { return getElapsedTime(__super::getElapsedTime()); }
};

class XRCORE_API CTimer_paused_ex : public CTimer
{
    Time save_clock;

public:
    CTimer_paused_ex() noexcept : save_clock() {}
    virtual ~CTimer_paused_ex() = default;
    bool Paused() const noexcept { return paused; }

    void Pause(const bool b) noexcept
    {
        if (paused == b)
            return;

        const auto current = Now();
        if (b)
        {
            save_clock = current;
            pauseDuration = CTimerBase::getElapsedTime();
        }
        else
        {
            pauseAccum += current - save_clock;
        }
        paused = b;
    }
};

class XRCORE_API CTimer_paused : public CTimer_paused_ex
{
public:
    CTimer_paused() { g_pauseMngr.Register(*this); }
    virtual ~CTimer_paused() { g_pauseMngr.UnRegister(*this); }
};

extern XRCORE_API BOOL g_bEnableStatGather;

class XRCORE_API CStatTimer
{
    using Duration = CTimerBase::Duration;

public:
    CTimer T;
    Duration accum;
    float result;
    u32 count;

    CStatTimer() : T(), accum(), result(.0f), count(0) {}
    void FrameStart();
    void FrameEnd();

    void Begin()
    {
        if (!g_bEnableStatGather)
            return;
        count++;
        T.Start();
    }

    void End()
    {
        if (!g_bEnableStatGather)
            return;
        accum += T.getElapsedTime();
    }

    Duration getElapsedTime() const { return accum; }

    u32 GetElapsed_ms() const
    {
        using namespace std::chrono;
        return u32(duration_cast<milliseconds>(getElapsedTime()).count());
    }

    float GetElapsed_sec() const
    {
        using namespace std::chrono;
        return duration_cast<duration<float>>(getElapsedTime()).count();
    }
};
