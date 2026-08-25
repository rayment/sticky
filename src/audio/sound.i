%{
#include "audio/sound.h"
%}

%unique_ptr(audio::sound)

%nspace audio::sound;

%inline %{
namespace audio {

class sound
{
public:
    static std::unique_ptr<sound> load_file(const std::string &path)
    {
        auto result = std::unique_ptr<sound>(new sound());

        st_string_t string{
            .data = path.data(),
            .len = path.size()
        };

        if (!st_audio_sound_load_file(&result->m_sound, string))
            return nullptr;

        return result;
    }

    ~sound()
    {
        if (m_sound.valid)
            st_audio_sound_destroy(m_sound);
    }

    st_audio_sound_t *get() { return &m_sound; }

private:
    st_audio_sound_t m_sound;

    sound(const sound &) = delete;
    sound &operator=(const sound &) = delete;

    sound() : m_sound{} {}
};

}
%}
