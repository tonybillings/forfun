#pragma once

#include "GL.hpp"
#include "View.hpp"
#include "SavedState.hpp"
#include "Event.h"

namespace bones
{
	class INPUT 
	{
	public:
		Event<AInputEvent*> Received;
	};

	class APP
	{
	public:
		static android_app* Android(android_app* val = nullptr)
		{
			static android_app* _app;

			if (val != nullptr)
				_app = val;

			return _app;
		}

		static ASensorManager* SensorManager(ASensorManager* val = nullptr)
		{
			static ASensorManager* _sensorManager;

			if (val != nullptr)
				_sensorManager = val;

			return _sensorManager;
		}

		static const ASensor* AccelSensor(const ASensor* val = nullptr)
		{
			static const ASensor* _accelSensor;

			if (val != nullptr)
				_accelSensor = val;

			return _accelSensor;
		}

		static ASensorEventQueue* SensorEventQueue(ASensorEventQueue* val = nullptr)
		{
			static ASensorEventQueue* _sensorEventQueue;

			if (val != nullptr)
				_sensorEventQueue = val;

			return _sensorEventQueue;
		}

		static View* ActiveView(View* val = nullptr)
		{
			static View* _view;

			if (val != nullptr)
				_view = val;

			return _view;
		}

		static bool IsRunning(BOOL val = -1)
		{
			static BOOL _isRunning;

			if (val != -1)
				_isRunning = val;

			return (bool)_isRunning;
		}

		static SavedState* State(SavedState* val = nullptr)
		{
			static SavedState* _state;

			if (val != nullptr)
				_state = val;

			return _state;
		}

		static INPUT* Input()
		{
			static INPUT _input;
			return &_input;
		}

	public:
		static void Start(struct android_app* app, View* view)
		{
			Android(app);
			ActiveView(view);

			app->userData = new SavedState();
			memset(app->userData, 0, sizeof(struct SavedState));

			app->onAppCmd = HandleCommand;
			app->onInputEvent = HandleInput;

			SensorManager(ASensorManager_getInstance());
			AccelSensor(ASensorManager_getDefaultSensor(SensorManager(), ASENSOR_TYPE_ACCELEROMETER));
			SensorEventQueue(ASensorManager_createEventQueue(SensorManager(), app->looper, LOOPER_ID_USER, NULL, NULL));

			IsRunning(true);

			while (1)
			{
				INT ident;
				INT events;
				struct android_poll_source* source;

				while ((ident = ALooper_pollAll(IsRunning() ? 0 : -1, NULL, &events, (void**)&source)) >= 0)
				{
					if (source != NULL)
						source->process(app, source);

					if (ident == LOOPER_ID_USER)
					{
						if (AccelSensor() != NULL)
						{
							ASensorEvent event;
							while (ASensorEventQueue_getEvents(SensorEventQueue(), &event, 1) > 0)
							{
								// do something
							}
						}
					}

					if (app->destroyRequested != 0)
					{
						Terminate();
						return;
					}
				}

				if (IsRunning())
					Refresh();
			}
		}

		static void Init()
		{
			GL::Init(Android());
			if (ActiveView() != nullptr) ActiveView()->Init();
		}

		static void HandleCommand(struct android_app* app, int32_t cmd)
		{
			switch (cmd)
			{
			case APP_CMD_SAVE_STATE:

				break;
			case APP_CMD_INIT_WINDOW:
				if (app->window != NULL)
				{
					Init();
					Refresh();
				}
				break;
			case APP_CMD_TERM_WINDOW:
				Terminate();
				break;
			case APP_CMD_GAINED_FOCUS:
				if (AccelSensor() != NULL)
				{
					ASensorEventQueue_enableSensor(SensorEventQueue(), AccelSensor());
					ASensorEventQueue_setEventRate(SensorEventQueue(), AccelSensor(), (1000L / 60) * 1000);
				}
				break;
			case APP_CMD_LOST_FOCUS:
				if (AccelSensor() != NULL)
					ASensorEventQueue_disableSensor(SensorEventQueue(), AccelSensor());

				IsRunning(false);
				Refresh();
				break;
			}
		}

		static int32_t HandleInput(struct android_app* app, AInputEvent* event)
		{
			Input()->Received(event);
			return 0;
		}

		static void Refresh()
		{
			GL::BeginRefresh();
			View* view = ActiveView();
			if (view != nullptr) view->Refresh();
			GL::EndRefresh();
		}

		static void Terminate()
		{
			IsRunning(false);
			GL::Terminate();
		}
	};
}
