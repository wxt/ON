#include <iostream>
#include <map>
#include <string>
#include <iostream>

//
// supporting tools and software
//
// Validate and test your json commands
// https://jsonlint.com/

// RapidJSON : lots and lots of examples to help you use it properly
// https://github.com/Tencent/rapidjson
//

// std::function
// std::bind
// std::placeholders
// std::map
// std::make_pair

#include <functional>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define INLINE inline

using namespace rapidjson;
using namespace std;

bool g_done = false;

//
// TEST COMMANDS
//
auto help_command = R"(
 {
  "command":"help",
  "payload": {
    "usage":"Enter json command in 'command':'<command>','payload': { // json payload of arguments }"
  }
 }
)";

auto exit_command = R"(
 {
  "command":"exit",
  "payload": {
     "reason":"Exiting program on user request."
  }
 }
)";

auto authenticate_command = R"(
 {
  "command":"authenticate",
  "payload": {
     "key":"1kjdi3idmvid."
  }
 }
)";

auto reloadUser_command = R"(
 {
  "command":"reloadUser",
  "payload": {
     "token":"1kjdi3idmvid."
  }
 }
)";

auto deviceHealth_command = R"(
 {
  "command":"deviceHealth",
  "payload": {
     "status":"healthy."
  }
 }
)";

// ----------------------------------------------------------------------------------------------- //

///
/// @brief Check the json object is NULL or not
///
/// @param json The json to check
///
INLINE bool isNull(const rapidjson::Value &JSON )
{
	return false;
}

// ----------------------------------------------------------------------------------------------- //

///
/// @brief Output message to console
///
/// @param str The string message to output to console.
///
INLINE void consoleOut(const std::string &str)
{
	std::cout << str << std::endl;
}

// ----------------------------------------------------------------------------------------------- //

///
/// @brief controller class
///
class Controller {
public:

// ----------------------------------------------------------------------------------------------- //

    ///
	/// @brief output command usage
    ///
    /// @param payload The json message for with command usage
    ///
    bool help(rapidjson::Value &payload)
    {
        cout << "Controller::help: command: ";

		const rapidjson::Value &payloadJSON = payload["payload"];
		if( !payloadJSON.HasMember("usage") )
		{
			//malformed json error
			consoleOut("Malformed json, missing usage field.");
		}

		const rapidjson::Value &usageJSON = payloadJSON[ "usage" ];

		if( usageJSON.IsNull() && !usageJSON.IsString())
		{
			consoleOut("Malformed json, usage field.");
		}
		

		std::cout << usageJSON.GetString() << std::endl;

        return true;
    }

// ----------------------------------------------------------------------------------------------- //

    ///
	/// @brief exit this application
    ///
    /// @param payload The json message for the reason to exit
    ///
    bool exit(rapidjson::Value &payload)
    {
        cout << "Controller::exit: command: \n";

        // implement

		const rapidjson::Value &payloadJSON = payload["payload"];
		if ( !payloadJSON.HasMember("reason") )
		{
			consoleOut("Malformed json, missing reason field.");
			return false;
		}

		const rapidjson::Value &reasonJSON = payloadJSON["reason"];

		if ( reasonJSON.IsNull() && !reasonJSON.IsString() )
		{
			consoleOut("Malformed json, reason type.");
			return false;
		}

		std::cout << reasonJSON.GetString() << std::endl;

		g_done = true;

        return true;
    }

// ----------------------------------------------------------------------------------------------- //

    ///
	/// @brief authenticate the user
    ///
    /// @param payload The json message for authentication
    ///
	bool authenticate( rapidjson::Value &payload )
	{
		const rapidjson::Value &payloadJSON = payload["payload"];
		if (!payloadJSON.HasMember("key"))
		{
			consoleOut("Malformed json, missing key field.");
			return false;
		}

		const rapidjson::Value &keyJSON = payloadJSON["key"];

		if (keyJSON.IsNull() && !keyJSON.IsString())
		{
			consoleOut("Malformed json, key type.");
			return false;
		}

		//authentication process

		consoleOut( "User authentiated" );

		return true;
	}

// ----------------------------------------------------------------------------------------------- //

	///
	/// @brief attempts to reload the current authenticated user
    ///
    /// @param payload The json message for reloading the user
    ///
	bool reloadUser( rapidjson::Value &payload )
	{
		const rapidjson::Value &payloadJSON = payload["payload"];
		if (!payloadJSON.HasMember("token"))
		{
			consoleOut("Malformed json, missing key field.");
			return false;
		}

		const rapidjson::Value &tokenJSON = payloadJSON["token"];

		if (tokenJSON.IsNull() && !tokenJSON.IsString())
		{
			consoleOut("Malformed json, key type.");
			return false;
		}

		//authentication prodecures via token

		consoleOut("User reloaded");

		return true;
	}

// ----------------------------------------------------------------------------------------------- //

	///
	/// @brief query the health status
    ///
    /// @param payload The json message for the device
    ///
	bool deviceHealth( rapidjson::Value &payload ) 
	{
		const rapidjson::Value &payloadJSON = payload["payload"];
		if (!payloadJSON.HasMember("status"))
		{
			consoleOut("Malformed json, missing status field.");
			return false;
		}

		const rapidjson::Value &statusJSON = payloadJSON["status"];

		if (statusJSON.IsNull() && !statusJSON.IsString())
		{
			consoleOut("Malformed json, status type.");
			return false;
		}

		consoleOut(std::string{ "Device health status: " } + std::string{statusJSON.GetString()} );

		return true;
	}

// ----------------------------------------------------------------------------------------------- //

};

// ----------------------------------------------------------------------------------------------- //

// Bonus Question: why did I type cast this?
/*
I can think of the following to typecasting:
	1] Whenever the prototype needs to be changed then changes is happening only in one place.
	2] Less typing whenever it's being used
*/
typedef std::function<bool(rapidjson::Value &)> CommandHandler;




// ----------------------------------------------------------------------------------------------- //

///
/// @brief Command dispatcher class to emplace command handler and dispatching.
///
class CommandDispatcher {
public:

// ----------------------------------------------------------------------------------------------- //

	///
	/// @brief emple constrcutor
	///
    CommandDispatcher()  
    {
    }

// ----------------------------------------------------------------------------------------------- //

    ///
    /// @brief simple destructor
    ///
    virtual ~CommandDispatcher()
    {
        // question why is it virtual? Is it needed in this case?
		/*
		Making the destrutor virutal will get called from the derived class.
		It's needed here if this class is extended.
		*/

		/*
		If executing the handler in a thread then wait for all tasks to finish but the command
		displatcher is a linear blocking call.  Not sure anything needs to be here.
		*/
    }

// ----------------------------------------------------------------------------------------------- //

    ///
    /// @brief add handler to command
    ///
    /// @param command The command string for the handler.
    /// @param handler The handler to handle the command.
    ///
    bool addCommandHandler(std::string command, CommandHandler handler)
    {
        cout << "CommandDispatcher: addCommandHandler: " << command << std::endl;

		auto it = command_handlers_.find( command );

		//add new handler for the command
		if ( it != command_handlers_.end() ) 
		{
			command_handlers_.erase( it );
		}

		command_handlers_.emplace( command, std::move(handler) );

        return true;
    }

// ----------------------------------------------------------------------------------------------- //

    ///
    /// @brief Dispatch commands
    ///
    /// @param command_json The json object with a command strind and it's respective payload
    ///
    bool dispatchCommand(std::string command_json)
    {
        cout << "COMMAND: " << command_json << endl;

		//contruct json from string
		rapidjson::Document command;

		try{
			command.Parse( command_json.c_str() );
		}
		catch(const std::runtime_error &er)
		{
			std::cout << "Malformed command json string." << std::endl;
		}

		//check to see if command is present and valid
		if (!command.IsObject() || !command.HasMember("command"))
		{
			std::cout << "Malformed json object." << std::endl;
			return false;
		}

		const rapidjson::Value &commandJSON = command["command"];
		if (commandJSON.IsNull() && !commandJSON.IsString())
		{
			std::cout << "Malformed json, missing payload." << std::endl;
			return false;
		}

		const char *commandptr = commandJSON.GetString();
		auto commandItr = command_handlers_.find( commandptr );


		//check to see if the command has a handler 
		if (commandItr == command_handlers_.end())
		{
			std::cout << "Malformed json, missing command." << std::endl;
			return false;
		}

		//check to see if the payload is present
		if (!command.HasMember("payload"))
		{
			std::cout << "Malformed json, missing payload." << std::endl;
			return false;
		}
		
		//execute the command handler
		try 
		{
			command_handlers_[commandptr]( command );
		}
		catch (const std::runtime_error &er)
		{
			std::cout << "Dispatch handler running time error for command: " << 
				std::string{ commandptr } << " Reason: " << er.what() << std::endl;
		}
		
        return true;
    }

// ----------------------------------------------------------------------------------------------- //


private:
    std::map<std::string, CommandHandler> command_handlers_;  ///< The container for handlers

    // Question: why delete these?
	/*
	Reasons I can think of for no copy constructor and assignment:
		1] keep integrity, only the desired contructed instance should be used.
		2] Only one controller should be present at any given time, singleton?

	*/

    // delete unused constructors
    CommandDispatcher (const CommandDispatcher&) = delete;
    CommandDispatcher& operator= (const CommandDispatcher&) = delete;

};


// ----------------------------------------------------------------------------------------------- //

void init_dispatcher( CommandDispatcher &dispatcher, Controller &controller)
{
	auto exitFunc = std::bind(&Controller::exit, controller, std::placeholders::_1 );
	dispatcher.addCommandHandler("exit", std::move(exitFunc));

	auto helpFunc = std::bind(&Controller::help, controller, std::placeholders::_1);
	dispatcher.addCommandHandler("help", std::move(helpFunc));

	auto authenticateFunc = std::bind(&Controller::authenticate, controller, std::placeholders::_1);
	dispatcher.addCommandHandler("authenticate", std::move(authenticateFunc));

	auto reloaduserFunc = std::bind(&Controller::reloadUser, controller, std::placeholders::_1);
	dispatcher.addCommandHandler("reloadUser", std::move(reloaduserFunc));

	auto deviceHealthFunc = std::bind(&Controller::deviceHealth, controller, std::placeholders::_1);
	dispatcher.addCommandHandler("deviceHealth", std::move(deviceHealthFunc));
}

// ----------------------------------------------------------------------------------------------- //

int main()
{
    std::cout << "COMMAND DISPATCHER: STARTED" << std::endl;

    CommandDispatcher command_dispatcher;
    Controller controller;                 // controller class of functions to "dispatch" from Command Dispatcher

    // add command handlers in Controller class to CommandDispatcher using addCommandHandler
	init_dispatcher( command_dispatcher, controller );

    // command line interface for testing
    string command;
    while( ! g_done ) {
        cout << "COMMANDS: {\"command\":\"exit\", \"payload\":{\"reason\":\"User requested exit.\"}}\n";
        cout << "\tenter command : ";
        getline(cin, command);
        command_dispatcher.dispatchCommand(command);
    }

    std::cout << "COMMAND DISPATCHER: ENDED" << std::endl;
    return 0;
}
