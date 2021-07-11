/* INFO & USAGE ****************************************************************

 This header file implementes an event system similar to the .NET implementation.
 The usage section example is based on a Button class with a "Clicked" event.
 
 --------------------------
 
 Declare the event in the Button class (three examples):
 
 Event<> Clicked;                           // no arguments
 Event<Button*> Clicked;                    // pointer to Button (sender)
 Event<Button*, vector<string>> Clicked;    // pointer to Button (sender) and event args (can be any type)

 --------------------------
 
 Create event handlers using global functions (for the three examples above):
 
 void OnButtonClicked(){};
 void OnButtonClicked(Button* sender){};
 void OnButtonClicked(Button* sender, vector<string> args){};
 
 Note: You can also create event handlers using member functions.
 
 --------------------------
 
 Wire-up event handlers (subscribe) to the event using global functions (for the three examples above):
 
 Button myButton;
 myButton.Clicked += new EventHandler<GlobalFunction>(OnButtonClicked);
 myButton.Clicked += new EventHandler<GlobalFunction, Button*>(OnButtonClicked);
 myButton.Clicked += new EventHandler<GlobalFunction, Button*, vector<string>>(OnButtonClicked);
 
 --------------------------
 
 Wire-up event handlers (subscribe) to the event using member functions of the Form class:
 
 Button myButton;
 myButton.Clicked += new EventHandler<Form>(myForm, &Form::OnButtonClicked);
 myButton.Clicked += new EventHandler<Form, Button*>(myForm, &Form::OnButtonClicked);
 myButton.Clicked += new EventHandler<Form, Button*, vector<string>>(myForm, &Form::OnButtonClicked);
 
 Note: the first template argument for the EventHandler class is the type of the object containing
 the member function.  The first constructor argument for the EventHandler class is a pointer to an 
 instance of that type and the second argument is address of the member function.
 
--------------------------
 
 Raise event within the Button class' Click() method (for the three examples above):
 
 Clicked();
 Clicked(this);
 Clicked(this, args);
 
 --------------------------
 
 Unhook event handler (unsubscribe) from the event using a global function:
 
 const IEventHandler* clickedHandler;
 clickedHandler = myButton.Clicked += new EventHandler<GlobalFunction>(OnButtonClicked);
 myButton.Clicked -= clickedHandler; // this is the actual unhook/unsubscribe statement.
 
 Note: works the same with member functions.  Simply save the returned IEventHandler pointer when
 subscribing to the event and use it to unsubscribe at a later time.
 
********************************************************************************/

#ifndef __Event__
#define __Event__

/* DEFINES *********************************************************************/

#define EVT_HNDL_CAP 10
#define EVT_HNDL_CAP_INC 10
#define GLOBALFUNC GlobalFunction
#define NULLCLASS NullClass

/* EMPTY CLASSES ***************************************************************/

class NullClass {};
class GlobalFunction {};
class IEventArg {};

/* EventArg ********************************************************************/

template <typename T>
class EventArg : public IEventArg
{
public:
    T Value;
public:
    EventArg(T value) : Value(value) {};
};

/* IEventHandler ***************************************************************/

class IEventHandler
{
public:
    virtual void SetId(int id) {};
    virtual int GetId() const { return -1; }
public:
    virtual void operator() () {};
    virtual void operator() (const IEventArg*) {};
    virtual void operator() (const IEventArg*, const IEventArg*) {};
};

/* EventHandler ****************************************************************/

template <class T, typename U = NULLCLASS, typename V = NULLCLASS>
class EventHandler : public IEventHandler
{
    typedef void(*GlobalFuncNoParam)();
    typedef void(*GlobalFuncOneParam)(U);
    typedef void(*GlobalFuncTwoParam)(U, V);
    typedef void(T::*MemberFuncNoParam)();
    typedef void(T::*MemberFuncOneParam)(U);
    typedef void(T::*MemberFuncTwoParam)(U, V);
    
private:
    int handlerId;
    T* obj;
    bool isGlobal;
    union FUNCPTR
    {
        GlobalFuncNoParam sf0;
        GlobalFuncOneParam sf1;
        GlobalFuncTwoParam sf2;
        MemberFuncNoParam mf0;
        MemberFuncOneParam mf1;
        MemberFuncTwoParam mf2;
    } funcptr;
public:
    EventHandler(GlobalFuncNoParam func)
    {
        funcptr.sf0 = func;
        isGlobal = true;
    }
    
    EventHandler(GlobalFuncOneParam func)
    {
        funcptr.sf1 = func;
        isGlobal = true;
    }
    
    EventHandler(GlobalFuncTwoParam func)
    {
        funcptr.sf2 = func;
        isGlobal = true;
    }
    
    EventHandler(T* object, MemberFuncNoParam func)
    {
        obj = object;
        funcptr.mf0 = func;
        isGlobal = false;
    }
    
    EventHandler(T* object, MemberFuncOneParam func)
    {
        obj = object;
        funcptr.mf1 = func;
        isGlobal = false;
    }
    
    EventHandler(T* object, MemberFuncTwoParam func)
    {
        obj = object;
        funcptr.mf2 = func;
        isGlobal = false;
    }
public:
    void SetId(int id)
    {
        handlerId = id;
    }
    
    int GetId() const
    {
        return handlerId;
    }
public:
    void operator() ()
    {
        if (isGlobal)
            funcptr.sf0();
        else
            (obj->*funcptr.mf0)();
    }
    
    void operator() (const IEventArg* arg1)
    {
        U val = (static_cast<const EventArg<U>*>(arg1))->Value;
        
        if (isGlobal)
            funcptr.sf1(val);
        else
            (obj->*funcptr.mf1)(val);
    }
    
    void operator() (const IEventArg* arg1, const IEventArg* arg2)
    {
        U val1 = (static_cast<const EventArg<U>*>(arg1))->Value;
        V val2 = (static_cast<const EventArg<V>*>(arg2))->Value;
        
        if (isGlobal)
            funcptr.sf2(val1, val2);
        else
            (obj->*funcptr.mf2)(val1, val2);
    }
};

/* Event ***********************************************************************/

template <typename T = NULLCLASS, typename U = NULLCLASS>
class Event
{
private:
    int identity;
    int count;
    int capacity;
    IEventHandler** handlers;
public:
    Event()
    {
        identity = 0;
        count = 0;
        capacity = EVT_HNDL_CAP;
        handlers = new IEventHandler*[capacity];
    }
    
    ~Event()
    {
        for (int i = 0; i < count; i++)
            delete handlers[i];
        
        delete[] handlers;
    }
public:
    const IEventHandler* operator+= (IEventHandler* handler)
    {
        if (count < capacity)
        {
            handlers[count++] = handler;
        }
        else
        {
            IEventHandler** temp = new IEventHandler*[capacity + EVT_HNDL_CAP_INC];
            
            for (int i = 0; i < capacity; i++)
                temp[i] = handlers[i];
            
            delete[] handlers;
            handlers = temp;
            capacity += EVT_HNDL_CAP_INC;
            handlers[count++] = handler;
        }
        handler->SetId(identity++);
        return handler;
    }
    
    void operator-= (const IEventHandler* handler)
    {
        IEventHandler** temp = new IEventHandler*[capacity];
        int index = 0;
        int currentCount = count;
        
        for (int i = 0; i < currentCount; i++)
        {
            if (handlers[i]->GetId() != handler->GetId())
                temp[index++] = handlers[i];
            else
                count--;
        }
        
        delete[] handlers;
        handlers = temp;
        delete handler;
    }
    
    void operator() ()
    {
        for (int i = 0; i < count; i++)
            (*(handlers[i]))();
    }
    
    void operator()(T arg)
    {
        EventArg<T> a(arg);
        
        for (int i = 0; i < count; i++)
            (*(handlers[i]))(&a);
    }
    
    void operator()(T arg1, U arg2)
    {
        EventArg<T> a(arg1);
        EventArg<U> b(arg2);
        
        for (int i = 0; i < count; i++)
            (*(handlers[i]))(&a, &b);
    }
};

#endif
