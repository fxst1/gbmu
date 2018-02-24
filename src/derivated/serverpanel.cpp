#include <guiderivated.hpp>
#include <globals.hpp>
#include <thread>

ServerPanel::ServerPanel(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder):
	Gtk::Window(cobject),
	_refGlade(builder),
	_clients(NULL)
{
	this->set_icon_from_file("./images/gb.png");
	this->_refGlade->get_widget_derived("server-clients", this->_clients);
	this->_refGlade->get_widget("server-hostname", this->_hostname);
	this->_refGlade->get_widget("server-port", this->_port);
	this->_refGlade->get_widget("server-slots", this->_slot);
	this->_refGlade->get_widget("server-btn-start", this->_start);
	this->_refGlade->get_widget("server-btn-stop", this->_stop);
	this->_refGlade->get_widget("server-btn-reload", this->_reload);
	this->_refGlade->get_widget("server-spinner1", this->_spinner);

	this->_start->signal_clicked().connect(sigc::mem_fun(this, &ServerPanel::eventStart));
	this->_stop->signal_clicked().connect(sigc::mem_fun(this, &ServerPanel::eventStop));
	this->_reload->signal_clicked().connect(sigc::mem_fun(this, &ServerPanel::restart));

	this->_hostname->set_text(Glib::ustring(server._host));
	this->_port->set_text(Glib::ustring( std::to_string(server._port) ));
	this->_slot->set_text(Glib::ustring( std::to_string(server._ncom) ));

	this->show_all();
	this->refreshBox();
}

ServerPanel::~ServerPanel(void)
{
	delete this->_clients;
	delete this->_hostname;
	delete this->_port;
	delete this->_slot;
	delete this->_start;
	delete this->_stop;
	delete this->_reload;
	delete this->_spinner;
}

void	ServerPanel::eventStart(void)
{
	unsigned short	port = (unsigned short)atoi(this->_port->get_text().c_str());
	int				value;

	this->_spinner->start();
	if (port != server._port)
	{
		value = ConfirmDialog::create(*this, "Port changed, all clients should be kicked", "Continue ?",
					Gtk::MESSAGE_INFO, Gtk::BUTTONS_YES_NO);
		if (value == Gtk::RESPONSE_YES)
		{
			server.stop();
			::shutdown(server._socket, 2);
			::closesocket(server._socket);
			server._started = false;
			server._used = false;
			server._listening = false;
		}
		else
			return ;
	}

	this->save();

	if (!server._started)
	{
		server = GameboyServer(server._ncom, server._port);
		if (!server._started)
		{
			ConfirmDialog::create(*this, "Cannot start server ...", Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
			this->refreshBox();
			this->_spinner->stop();
			return ;
		}
	}


	if (!server._listening)
		this->proceed();

	this->refreshBox();
	this->_spinner->stop();
}

void	ServerPanel::eventStop(void)
{
	this->_spinner->start();
	server._listening = false;
	while (server._used)
	{
		usleep(1);
	}
	server.stop();
	server._listening = false;
	this->refreshBox();
	this->_spinner->stop();
}

static void		__server_exec(GameboyServer* s)
{
	while (s->_listening)
	{
		s->_used = true;

		s->accept();
		s->relay();

		s->_used = false;

		usleep(1);
	}
	s->stop();
	s->_used = false;
	s->_listening = false;
}

void	ServerPanel::proceed(void)
{
	server._listening = true;
	std::thread	th(__server_exec, &server);

	th.detach();
}

void	ServerPanel::restart(void)
{
	this->eventStop();
	this->eventStart();
}

void	ServerPanel::save(void)
{
	size_t			ncom = (size_t)atoi(this->_slot->get_text().c_str());
	unsigned short	port = (unsigned short)atoi(this->_port->get_text().c_str());
	int				value = 0;

	if (ncom == 0 || ncom > 100)
		return ;
	else if (ncom < server._clients.size())
	{
		value = ConfirmDialog::create(*this, "Do some clients may be kicked", "Continue ?",
					Gtk::MESSAGE_INFO, Gtk::BUTTONS_YES_NO);
		if (value == Gtk::RESPONSE_OK)
			server.stop();
		else
			return ;
	}

	server._host = (char*)this->_hostname->get_text().c_str();
	server._port = port;
	server._ncom = ncom;
}

void	ServerPanel::refreshBox(void)
{
	if (server._started && server._listening)
	{
		std::cout << "ON" << std::endl;
		this->_start->hide();
		this->_stop->show();
		this->_reload->show();
	}
	else
	{
		std::cout << "OFF" << std::endl;
		this->_start->show();
		this->_stop->hide();
		this->_reload->hide();		
	}
}

ServerClients::ServerClients(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder):
	Gtk::ListBox(cobject),
	_refGlade(builder),
	_socks(std::vector<ServerClient*>())
{
	for (auto it = server._clients.begin(); it != server._clients.end(); it++)
	{
		if (it->first != INVALID_SOCKET)
		{
			ServerClient	*client = NULL;

			this->_refGlade->get_widget_derived("server-client", client, it->first, it->second);
			this->_socks.push_back(client);
			this->append(*client);
		}
		else
			server.kick(it->first);
	}
}

ServerClients::~ServerClients(void)
{}

ServerClient::ServerClient(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
			const SOCKET& id, const SOCKADDR_IN info):
	Gtk::ListBoxRow(cobject),
	_refGlade(builder),
	_sock((SOCKET&)id),
	_info((SOCKADDR_IN&)info)
{
	this->_refGlade->get_widget("server-client-id", this->_id);
	this->_refGlade->get_widget("server-client-status", this->_status);
	this->_refGlade->get_widget("server-client-action-accept", this->_accept);
	this->_refGlade->get_widget("server-client-action-deny", this->_deny);
	this->_refGlade->get_widget("server-client-action-kick", this->_kick);
}

ServerClient::~ServerClient(void)
{}
