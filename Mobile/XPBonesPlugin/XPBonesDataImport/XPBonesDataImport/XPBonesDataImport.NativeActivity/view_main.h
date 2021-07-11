#pragma once

#include "colors.h"
#include "gauge_heading.h"
#include "gauge_attitude.h"
#include "gauge_altitude.h"
#include "gauge_speed.h"

using namespace std;
using namespace bones;
using namespace vmml;
using asio::ip::udp;
using asio::io_service;

class MainView : public View
{
private:
	MATRIX _viewMatrix;
	MATRIX _projMatrix;

	thread* _serverThread;
	io_service _ioService;
	udp::socket _socket;
	char* _data;

	HeadingGauge* _gaugeHeading;
	AttitudeGauge* _gaugeAttitude;
	AltitudeGauge* _gaugeAltitude;
	SpeedGauge* _gaugeSpeed;

private:
	void ServerReceive()
	{
		_socket.async_receive(asio::buffer(_data, 1024), [this](error_code errorCode, size_t bytesReceived)
		{
			if (errorCode)
				throw;

			if (bytesReceived > 0)
			{
				_data[bytesReceived] = 0;

				string str(_data);
				string buf;
				stringstream ss(str);
				vector<string> tokens;
				while (ss >> buf) { tokens.push_back(buf); }

				for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); it++)
				{
					if (it->length() < 2) continue;

					char t = (*it)[0];
					string val = it->substr(1, it->length() - 1);

					switch (t)
					{
					case 'H':
						_gaugeHeading->SetTargetHeading(atof(val.c_str()));
						break;
					case 'R':
						_gaugeAttitude->SetTargetRoll(atof(val.c_str()));
						break;
					case 'P':
						_gaugeAttitude->SetTargetPitch(atof(val.c_str()));
						break;
					case 'A':
						_gaugeAltitude->SetTargetAltitude(atof(val.c_str()));
						break;
					case 'S':
						_gaugeSpeed->SetTargetSpeed(atof(val.c_str()));
						break;
					}
				}
			}

			ServerReceive();
		});
	}

	void CreatePrograms()
	{
		GL::CreateProgram("basic", { "basic.vert", "basic.frag" });
	}

	void AddComponents()
	{
		_gaugeHeading = new HeadingGauge();
		_gaugeAttitude = new AttitudeGauge();
		_gaugeAltitude = new AltitudeGauge();
		_gaugeSpeed = new SpeedGauge();

		_components.push_back(_gaugeHeading);
		_components.push_back(_gaugeAttitude);		
		_components.push_back(_gaugeAltitude);
		_components.push_back(_gaugeSpeed);
	}

public:
	MainView() : _socket(_ioService, udp::endpoint(udp::v4(), 6666)) { }

	~MainView()
	{
		delete _data;
		delete _gaugeHeading;
		delete _gaugeAttitude;
		delete _gaugeAltitude;
		delete _gaugeSpeed;

		_serverThread->~thread();
		delete _serverThread;
	}

public:
	void Init() override
	{
		CreatePrograms();
		AddComponents();
		GL::BackgroundColor(BACK_COLOR);
		View::Init();

		_serverThread = new thread([this]() {
			_data = new char[1025];
			ServerReceive();
			_ioService.run();
		});
	}
};
