
#include <atomic>
#include <chrono>
#include <functional>
#include <thread>
#include <zmqpp/zmqpp.hpp>

#include "constants.hh"

std::atomic<bool> should_run = true;

using namespace std::literals::chrono_literals;

std::string process_state(const std::string &state) {
  std::cout << state << std::endl;
  return "";
}

int main() {
  zmqpp::context_t ctx;
  zmqpp::socket game_to_bot_socket(ctx, zmqpp::socket_type::subscribe);
  zmqpp::socket bot_to_game_socket(ctx, zmqpp::socket_type::publish);
  bot_to_game_socket.connect(bot_to_game_address);

  game_to_bot_socket.connect(game_to_bot_address);
  game_to_bot_socket.subscribe(game_to_bot_topic);

  std::string input;

  std::chrono::time_point last_message_time =
      std::chrono::high_resolution_clock::now();
  while (should_run) {
    if (game_to_bot_socket.receive(input, zmqpp::socket::dont_wait)) {
      const std::string output_command = process_state(input);
      if (output_command.size() > 0) {
        std::cout << "Sending: " << output_command << std::endl;
        bot_to_game_socket.send(bot_to_game_topic + " " + output_command);
        last_message_time = std::chrono::high_resolution_clock::now();
      }
    }

    const auto dt =
        std::chrono::high_resolution_clock::now() - last_message_time;
    if ((dt > 3s)) {
      bot_to_game_socket.send(bot_to_game_topic + " state");
      last_message_time = std::chrono::high_resolution_clock::now();
    }
    std::this_thread::sleep_for(250ms);
  }
}
