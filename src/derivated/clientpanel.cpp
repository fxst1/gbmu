#include <guiderivated.hpp>
#include <globals.hpp>
#include <thread>

ClientPanel::ClientPanel(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
						const std::map<std::string, std::string>& configmap):
	Gtk::Window(cobject),
	_refGlade(builder),
	_refConfig((std::map<std::string, std::string>&)configmap),
	_servers(NULL)
{
	this->set_icon_from_file("./images/gb.png");
	this->_refGlade->get_widget("client-status", this->_status);
	this->_refGlade->get_widget("client-name", this->_username);
	this->_refGlade->get_widget("client-port", this->_port);
	this->_refGlade->get_widget("client-cfg-auto", this->_auto);
	this->_refGlade->get_widget("client-cfg-kick", this->_kick);
	this->_refGlade->get_widget("client-cfg-disconnect", this->_disconnect);
	this->_refGlade->get_widget("client-spinner1", this->_spinner1);
	this->_refGlade->get_widget("client-spinner1", this->_spinner2);
	this->_refGlade->get_widget("client-server-new-host", this->_newhost);
	this->_refGlade->get_widget("client-server-new-port", this->_newport);
	this->_refGlade->get_widget("client-server-new-add", this->_add);

	this->_newhost->set_text("");
	this->_newport->set_text("");

	this->_add->signal_clicked().connect(sigc::mem_fun(this, &ClientPanel::eventAdd));
	this->_auto->property_active().signal_changed().connect(sigc::mem_fun(this, &ClientPanel::eventChangeAuto));
	this->_kick->property_active().signal_changed().connect(sigc::mem_fun(this, &ClientPanel::eventChangeKick));
	this->_disconnect->property_active().signal_changed().connect(sigc::mem_fun(this, &ClientPanel::eventChangeDisconnect));

	this->_username->set_text(Glib::ustring(client._username));
	this->_port->set_text(Glib::ustring( std::to_string(client._port) ));

	this->show_all();
	this->_refGlade->get_widget_derived("client-servers", this->_servers, this);

	this->_servers->show();

	this->setStatusLabel();
}

ClientPanel::~ClientPanel(void)
{
	delete this->_servers;
	delete this->_status;
	delete this->_username;
	delete this->_port;
	delete this->_auto;
	delete this->_kick;
	delete this->_disconnect;
	delete this->_newhost;
	delete this->_newport;
	delete this->_add;
	delete this->_spinner1;
	delete this->_spinner2;
}

void		ClientPanel::refresh(bool do_ping)
{
	if (do_ping)
		this->_servers->pingAll();
	else
	{
		this->setStatusLabel();
	}
}

void		ClientPanel::setStatusLabel(void)
{
	if (client._connected == false)
		this->_status->set_text(Glib::ustring( "Disconnected" ));
	else
	{
		std::string	s = "Connected to ";
		s += client._host;
		this->_status->set_text(Glib::ustring(s));
	}	
}

void		ClientPanel::eventChangeAuto(void)
{
	if (this->_auto->get_state())
		this->_refConfig["auto_connect"] = "true";
	else
		this->_refConfig["auto_connect"] = "false";
}

void		ClientPanel::eventChangeKick(void)
{
	if (this->_kick->get_state())
		this->_refConfig["notify_kick"] = "true";
	else
		this->_refConfig["notify_kick"] = "false";
}

void		ClientPanel::eventChangeDisconnect(void)
{
	if (this->_disconnect->get_state())
		this->_refConfig["notify_disconnect"] = "true";
	else
		this->_refConfig["notify_disconnect"] = "false";
}

static char	*__trim(char *s)
{
	char	*ret = s;
	char	*tmp = s;

	while (isspace(*s))
		s++;
	while (*s && !isspace(*s))
	{
		*tmp = *s;
		tmp++;
		s++;
	}
	*tmp = 0;
	return (ret);
}

void		ClientPanel::eventAdd(void)
{
	t_servercom*	newcom;
	char*			url;
	int				port;

	url = __trim((char*)this->_newhost->get_text().c_str());
	port = atoi(__trim((char*)this->_newport->get_text().c_str()));
	if (*url && port > 0)
	{
		newcom = (t_servercom*)malloc(sizeof(t_servercom*));
		if (!newcom)
			ConfirmDialog::create(*this, "Cannot allocate new server entry", Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
		else
		{
			newcom->host = strdup(url);
			newcom->port = (unsigned short)port;
			newcom->ready = false;
			this->_servers->addConnection(newcom, -1, this);
			client._servers.push_back(newcom);
		}
	}
	else
		ConfirmDialog::create(*this, "Missing/Invalid url or port", Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
}

std::string		ClientPanel::getClientXML(void)
{
	return (this->_refConfig["client_server_ui"]);
}

void			ClientPanel::activeMode(bool set)
{
	if (set)
	{
		this->_spinner1->start();
		this->_spinner2->start();
	}
	else
	{
		this->_spinner1->stop();
		this->_spinner2->stop();		
	}
}

ClientServers::ClientServers(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder, ClientPanel *s):
	Gtk::ListBox(cobject),
	_refGlade(builder),
	_list(std::vector<ClientServer*>())
{
	int		index = 0;

	for (auto it = client._servers.begin(); it != client._servers.end(); it++)
		this->addConnection(*it, index++, s);
}

ClientServers::~ClientServers(void)
{}

void	ClientServers::resetButtons(void)
{
	for (auto it=this->get_children().begin(); it != this->get_children().end(); it++)
	{
		((ClientServer*)(*it))->setStatus(0);
	}
}


void	ClientServers::addConnection(t_servercom *com, int index, ClientPanel *s)
{
	ClientServer	*server = NULL;
	Glib::RefPtr<Gtk::Builder>	tmp = Gtk::Builder::create();

	tmp->add_from_file(s->getClientXML());
	if (!com)
		return ;

	if (index == -1)
		index = this->get_children().size() - 1;
	else
		index++;
	std::cout << "ADD " << com->host << " at " << std::to_string(index) << std::endl;
	tmp->get_widget_derived("client-server", server, com, index, s);
	//this->_list.push_back(server);
	this->insert(*server, index);
	server->setStatus(0);
	//this->append(*server);
	std::cout << "ENDADD " << com->host << std::endl;
}

void	ClientServers::pingAll(void)
{
	for (auto it = this->get_children().begin(); it != this->get_children().end(); it++)
		((ClientServer*)(*it))->ping();
}

ClientServer::ClientServer(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder,
			t_servercom *com, int index, ClientPanel *s):
	Gtk::ListBoxRow(cobject),
	_refGlade(builder),
	_panelparent(s),
	_index(index),
	_com(com),
	_connected(false)
{
	std::cout << "========== CLIENT ==========" << std::endl;

	this->_refGlade->get_widget("client-server-name", this->_id);
	this->_refGlade->get_widget("client-server-status", this->_status);
	this->_refGlade->get_widget("client-server-connect", this->_connect);
	this->_refGlade->get_widget("client-server-disconnect", this->_disconnect);
	this->_refGlade->get_widget("client-server-forget", this->_forget);

	this->_id->set_text( std::string(com->host) + ":" + std::to_string(com->port) );

	this->_connect->signal_clicked().connect(sigc::mem_fun(this, &ClientServer::eventConnect));
	this->_disconnect->signal_clicked().connect(sigc::mem_fun(this, &ClientServer::eventDisconnect));
	this->_forget->signal_clicked().connect(sigc::mem_fun(this, &ClientServer::eventForget));

	this->_connect->show();
	this->_disconnect->hide();
	this->_forget->show();

	std::cout << "PARENT = " << (this->get_parent() != NULL ? "HAS" : "NO") << std::endl;

	std::cout << "========== ENDCLIENT ==========" << std::endl;
}

ClientServer::~ClientServer(void)
{
	delete this->_id;
	delete this->_status;
	delete this->_connect;
	delete this->_disconnect;
	delete this->_forget;
}

ClientPanel	*ClientServer::accessPanel(void)
{
	return (this->_panelparent);
}

void	__doPing(ClientServer *server)
{
	server->accessPanel()->activeMode(true);

	server->_com->ready = GameboyClient::ping(server->_com->host, server->_com->port);

	if (server->_com->ready)
	{
		if (server->_connected)
			server->setStatus(1);
		else
			server->setStatus(0);
	}
	else
		server->setStatus(-1);

	server->accessPanel()->activeMode(false);
}

void	ClientServer::ping(void)
{
	std::thread		th(__doPing, this);

	th.detach();
}

void	ClientServer::setStatus(int status)
{
	if (status == 0)
	{
		this->_connected = false;
		this->_connect->show();
		this->_disconnect->hide();
		//this->_status->set(Gtk::Stock::DISCONNECT.id);
	}
	else if (status == 1)
	{
		this->_connected = true;
		//this->_status->set(Gtk::Stock::CONNECT.id);
		this->_connect->hide();
		this->_disconnect->show();
	}
	else
	{
		this->_connected = false;
		this->_connect->show();
		this->_disconnect->hide();
		//this->_status->set(Gtk::Stock::DIALOG_WARNING.id);
	}
	this->_panelparent->setStatusLabel();
}

void	ClientServer::eventConnect(void)
{
	this->accessPanel()->activeMode(true);

	std::cout << "Connecting..." << std::endl;
	if (client._connected &&
		!ConfirmDialog::create(*this->_panelparent, "Abort your actual connection ?", Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO))
		return ;
	this->_connected = false;
	if (client.connect(this->_com->host, this->_com->port))
	{
		std::cout << "OK" << std::endl;
		this->setStatus(1);
		this->_connected = true;
	}
	else
		ConfirmDialog::create(*this->_panelparent, "Cannot connect to " + std::string(this->_com->host),
								Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
	this->setStatus(-1);
	this->_panelparent->setStatusLabel();	

	this->accessPanel()->activeMode(false);
}

void	ClientServer::eventDisconnect(void)
{
	this->accessPanel()->activeMode(true);

	if (client._connected &&
		ConfirmDialog::create(*this->_panelparent, "Abort connection ?", Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO))
		return ;
	this->_connected = false;
	client.disconnect();
	this->setStatus(0);
	this->_panelparent->refresh(true);
	this->_panelparent->setStatusLabel();

	this->accessPanel()->activeMode(false);
}

void	ClientServer::eventForget(void)
{
//	client._servers.erase(client._servers.begin() + this->_index);
	this->remove();
	if (this->_connected)
		this->eventDisconnect();
}
