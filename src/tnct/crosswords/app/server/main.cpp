#include <iostream>
#include <memory>
#include <vector>

#include <ixwebsocket/IXWebSocketServer.h>
#include <nlohmann/json.hpp>

#include "tnct/crosswords/app/server/connection.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cerr.h"
#include "tnct/log/cpt/macros.h"

using tnct::crosswords::app::server::connection;
using logger = tnct::log::cerr;
using tnct::format::fmt;

using connection_ptr = std::unique_ptr<connection>;
using connections    = std::vector<connection_ptr>;

connections::iterator create_connection(connections &p_connections,
                                        logger      &p_logger)
{
  for (connections::iterator _ite = p_connections.begin();
       _ite != p_connections.end(); ++_ite)
  {
    if (!(*_ite)->is_working())
    {
      TNCT_LOG_INF(p_logger, fmt("reusing connection # ",
                                 std::distance(p_connections.begin(), _ite)));
      return _ite;
    }
  }
  TNCT_LOG_INF(p_logger, "creating new 'connection'");
  connection_ptr _connection{std::make_unique<connection>(p_logger)};
  if (!_connection)
  {
    TNCT_LOG_ERR(p_logger, "could not create 'connection'");
    return p_connections.end();
  }
  TNCT_LOG_INF(p_logger, "'connection' created");
  p_connections.push_back(std::move(_connection));
  return std::prev(p_connections.end());
}

int main()
{
  logger _logger;

  // _logger.set_deb();

  std::vector<connection_ptr> _connections;

  // Create a server on port 9002
  ix::WebSocketServer server(9002);

  // Set up connection handling
  server.setOnConnectionCallback(
      [&](std::weak_ptr<ix::WebSocket> wsWeak,
          std::shared_ptr<ix::ConnectionState> /*state*/)
      {
        if (auto ws = wsWeak.lock())
        {
          ws->setOnMessageCallback(
              [ws, &_logger, &_connections](const ix::WebSocketMessagePtr &msg)
              {
                if (msg->type == ix::WebSocketMessageType::Message)
                {
                  connections::iterator _connection_ite{
                      create_connection(_connections, _logger)};

                  if (_connection_ite != _connections.end())
                  {
                    (*_connection_ite)->run(std::move(msg->str), ws);
                  }
                }
                else if (msg->type == ix::WebSocketMessageType::Open)
                {
                  TNCT_LOG_INF(_logger, "New client connected!");
                }
                else if (msg->type == ix::WebSocketMessageType::Close)
                {
                  TNCT_LOG_INF(_logger, "Client disconnected.");
                }
              });
        }
      });

  // Start the server
  auto res = server.listen();
  if (!res.first)
  {
    TNCT_LOG_ERR(_logger, fmt("Error starting server: ", res.second));
    return 1;
  }

  server.start();
  std::cout << "WebSocket server listening on ws://localhost:9002" << std::endl;

  // Run forever
  server.wait();

  return 0;
}
