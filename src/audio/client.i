%{
#include "audio/client.h"
%}

%unique_ptr(audio::client)

%nspace audio::client;

%inline %{
namespace audio {

class client
{
public:
    static std::unique_ptr<client> create()
    {
        auto result = std::unique_ptr<client>(new client());

        if (!st_audio_client_create(&result->m_client))
            return nullptr;

        return result;
    }

    ~client()
    {
        st_audio_client_destroy(m_client);
    }

    bool select()
    {
        return st_audio_client_select(this->m_client);
    }

    bool set_position(math::vec3 &pos)
    {
        return st_audio_client_set_position(&this->m_client, {(*pos.get()).x, (*pos.get()).y, (*pos.get()).z});
    }

    bool set_velocity(math::vec3 &vel)
    {
        return st_audio_client_set_velocity(&this->m_client, {(*vel.get()).x, (*vel.get()).y, (*vel.get()).z});
    }

private:
    st_audio_client_t m_client;

    client(const client &) = delete;
    client &operator=(const client &) = delete;

    client() : m_client{} {}
};

}
%}
