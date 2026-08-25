%{
#include "audio/source.h"
%}

%unique_ptr(audio::source)

%nspace audio::source;

%inline %{
namespace audio {

class source
{
public:
    static std::unique_ptr<source> create()
    {
        auto result = std::unique_ptr<source>(new source());

        if (!st_audio_source_create(&result->m_source))
            return nullptr;

        return result;
    }

    ~source()
    {
        st_audio_source_destroy(m_source);
    }

    st_audio_source_t *get() { return &m_source; }

    bool select(sound &snd)
    {
        return st_audio_source_select(this->m_source, *snd.get());
    }

    bool set_pitch(st_float32 pitch)
    {
        return st_audio_source_set_pitch(&this->m_source, pitch);
    }

    bool set_gain(st_float32 gain)
    {
        return st_audio_source_set_gain(&this->m_source, gain);
    }

    bool set_loop(bool loop)
    {
        return st_audio_source_set_loop(&this->m_source, loop);
    }

    bool set_position(math::vec3 &pos)
    {
        return st_audio_source_set_position(&this->m_source, {(*pos.get()).x, (*pos.get()).y, (*pos.get()).z});
    }

    bool set_velocity(math::vec3 &vel)
    {
        return st_audio_source_set_velocity(&this->m_source, {(*vel.get()).x, (*vel.get()).y, (*vel.get()).z});
    }

    bool play()
    {
        return st_audio_source_play(this->m_source);
    }

    bool pause()
    {
        return st_audio_source_pause(this->m_source);
    }

    bool resume()
    {
        return st_audio_source_resume(this->m_source);
    }

    bool stop()
    {
        return st_audio_source_stop(this->m_source);
    }

    bool is_playing()
    {
        return st_audio_source_is_playing(this->m_source);
    }

    bool is_paused()
    {
        return st_audio_source_is_paused(this->m_source);
    }

    bool is_stopped()
    {
        return st_audio_source_is_stopped(this->m_source);
    }

private:
    st_audio_source_t m_source;

    source(const source &) = delete;
    source &operator=(const source &) = delete;

    source() : m_source{} {}
};

}
%}
