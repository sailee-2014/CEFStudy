#pragma once
#include <string>
#include "cef_client.h"
#include "cefclient/util.h"

using namespace std;


class ClientHandler
	:public CefClient
{
public:
	ClientHandler(void);
	~ClientHandler(void);

public:
	// Returns the startup URL.
	std::string GetStartupURL() { return m_StartupURL; }

protected:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(ClientHandler);


protected:
	string m_StartupURL;
	

};

