#pragma once

#include "stdafx.h"

typedef websocketpp::server<websocketpp::config::asio> WsServer;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

enum action_type 
{
	SUBSCRIBE,
	UNSUBSCRIBE,
	MESSAGE
};

struct action 
{
	action(action_type t, connection_hdl h) : type(t), hdl(h) {}
	action(action_type t, std::string m) : type(t), msg(m) {}

	action_type type;
	websocketpp::connection_hdl hdl;
	std::string msg;
};

class WebsocketServer 
{
private:
	typedef std::set<connection_hdl, std::owner_less<connection_hdl> > con_list;

	WsServer m_server;
	con_list m_connections;
	std::queue<action> m_actions;

	mutex m_action_lock;
	mutex m_connection_lock;
	condition_variable m_action_cond;

public:
	WebsocketServer() 
	{
		m_server.clear_access_channels(websocketpp::log::alevel::all);

		m_server.init_asio();

		m_server.set_open_handler(bind(&WebsocketServer::on_open, this, ::_1));
		m_server.set_close_handler(bind(&WebsocketServer::on_close, this, ::_1));
		m_server.set_message_handler(bind(&WebsocketServer::on_message, this, ::_1, ::_2));
	}

public:
	void run(uint16_t port)
	{
		m_server.listen(port);
		m_server.start_accept();

		try
		{
			m_server.run();
		}
		catch (const std::exception & e) {}
	}

	void on_open(connection_hdl hdl) 
	{
		{
			lock_guard<mutex> guard(m_action_lock);
			m_actions.push(action(SUBSCRIBE, hdl));
		}
		m_action_cond.notify_one();
	}

	void on_close(connection_hdl hdl) 
	{
		{
			lock_guard<mutex> guard(m_action_lock);
			m_actions.push(action(UNSUBSCRIBE, hdl));
		}
		m_action_cond.notify_one();
	}

	void on_message(connection_hdl hdl, WsServer::message_ptr msg) 
	{

	}

	void process_messages() 
	{
		while (1) 
		{
			unique_lock<mutex> lock(m_action_lock);

			while (m_actions.empty()) 
				m_action_cond.wait(lock);

			action a = m_actions.front();
			m_actions.pop();

			lock.unlock();

			if (a.type == SUBSCRIBE) 
			{
				lock_guard<mutex> guard(m_connection_lock);
				m_connections.insert(a.hdl);
			}
			else if (a.type == UNSUBSCRIBE) 
			{
				lock_guard<mutex> guard(m_connection_lock);
				m_connections.erase(a.hdl);
			}
			else if (a.type == MESSAGE) 
			{
				lock_guard<mutex> guard(m_connection_lock);

				con_list::iterator it;
				for (it = m_connections.begin(); it != m_connections.end(); it++) 
					m_server.send(*it, a.msg, websocketpp::frame::opcode::text);
			}
		}
	}

	void broadcast(const std::string& msg)
	{
		{
			lock_guard<mutex> guard(m_action_lock);
			m_actions.push(action(MESSAGE, msg));
		}
		m_action_cond.notify_one();
	}
};

