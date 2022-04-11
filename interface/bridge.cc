
#include <atomic>
#include <csignal>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <zmqpp/zmqpp.hpp>

#include "interface/constants.hh"

std::atomic<bool> should_run = true;

void signal_handler(int) {
  std::cerr << "Bridge Terminating..." << std::endl;
  should_run = false;
}

struct ThreadParams {
  std::vector<std::string> game_to_bot_queue;
  std::vector<std::string> bot_to_game_queue;
  std::mutex game_to_bot_lock;
  std::mutex bot_to_game_lock;
};

int zmq_thread(ThreadParams &params) {
  zmqpp::context ctx;
  zmqpp::socket game_to_bot_socket(ctx, zmqpp::socket_type::publish);
  zmqpp::socket bot_to_game_socket(ctx, zmqpp::socket_type::subscribe);

  game_to_bot_socket.bind(game_to_bot_endpoint);
  bot_to_game_socket.bind(bot_to_game_endpoint);
  bot_to_game_socket.subscribe(bot_to_game_topic);

  std::string message_from_bot;
  while (should_run) {
    {
      // Publish any message to the bot that have been received
      std::lock_guard<std::mutex> guard(params.game_to_bot_lock);
      for (const auto &item : params.game_to_bot_queue) {
        game_to_bot_socket.send(game_to_bot_topic + " " + item);
      }
      params.game_to_bot_queue.clear();
    }
    {
      // Poll for any new message from the bot
      std::lock_guard<std::mutex> guard(params.bot_to_game_lock);
      while (bot_to_game_socket.receive(message_from_bot,
                                        zmqpp::socket::dont_wait)) {
        std::cout << message_from_bot.substr(bot_to_game_topic.size()+1) << std::endl;
      }
    }
  }
  std::cerr << "ZMQ thread exiting..." << std::endl;
  return 0;
}

int main() {
  std::signal(SIGINT, signal_handler);
  ThreadParams params;
  std::thread other_thread(zmq_thread, std::ref(params));

  std::string input(65536, '\0');
  std::cout << "ready" << std::endl;

  std::ofstream log_file("/tmp/interface.log");

  while (should_run) {
    std::getline(std::cin, input);
    log_file << input << std::endl;
    {
      std::lock_guard<std::mutex> guard(params.game_to_bot_lock);
      params.game_to_bot_queue.push_back(input);
    }
  }
  other_thread.join();
}
