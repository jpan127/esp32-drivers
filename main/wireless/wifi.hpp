

static const char* TAG = "Wifi";

class Wifi
{
public:

    Wifi();

    void Initialize();

    void SetMode();
}

// C task
extern "C"
{
    void wifi_task(void *p)
    {

    }
}

Wifi::Wifi()
{
    xTaskCreate()
}