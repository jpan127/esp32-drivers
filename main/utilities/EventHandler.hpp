

class EventHandler
{
public:

    EventHandler();

    void CallbackFunction(void *ctx, void *event);

    system_event_t GetCallback();
};