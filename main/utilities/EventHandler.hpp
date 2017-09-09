
// [TODO] Make a generic event handler that can be inherited
// by any task class
class EventHandler
{
public:

    EventHandler();

    void CallbackFunction(void *ctx, void *event);

    system_event_t GetCallback();
};