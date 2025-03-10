// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for char_traits, operator+, string, basic_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref

#include <iostream>

using namespace ftxui;

std::string displayFront(std::string additional_text) {
  std::string ans;
  std::string input;

  Component input_text = Input(&input, "definition");

  // The component tree:
  auto component = Container::Vertical({
      input_text,
  });

  auto screen = ScreenInteractive::Fullscreen();
    // Tweak how the component tree is rendered:
  auto renderer = Renderer(component, [&] {
    return vbox({
              text("Write your definition to " + additional_text) | center | flex,
              separator(),
              hbox(text("Your answer: "), input_text->Render()),
         }) | border;
  });

  auto breaker = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Return) {
      ans = input;
      input.clear();
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });

  screen.Loop(breaker);
  return ans;
}

std::string displayBack(std::string additional_text) {
  std::string ans;
  std::string input;

  Component input_text = Input(&input, "");
  input_text |= CatchEvent([&](Event event) {
    if (event.is_character() && (event.character() == "y" || event.character() == "Y" || event.character() == "n" || event.character() == "N") && input.size() < 1) {
      return false;
    }
    if (event == Event::Backspace && !input.empty()) {
      input.pop_back();
      return true;
    }
    return true;
  });

  // The component tree:
  auto component = Container::Vertical({
      input_text,
  });

  auto screen = ScreenInteractive::Fullscreen();
  auto renderer = Renderer(component, [&] {
    return vbox({
              text("The definition is: " + additional_text) | center | flex,
              separator(),
              hbox(text("Write y if you got it right and n else: "), input_text->Render()),
         }) | border;
  });

  auto breaker = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Return) {
      ans = input;
      input.clear();
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });

  screen.Loop(breaker);
  return ans;
}

void displayText(std::string to_display) {
  auto screen = ScreenInteractive::Fullscreen();

  auto renderer = Renderer([&] {
    return vbox({
              text(to_display) | center | flex,
         }) | border;
  });

  auto breaker = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Return) {
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });
  screen.Loop(breaker);
}

void displayTextColor(std::string to_display, Color front_color = Color::Default, Color back_color = Color::Default) {
  auto screen = ScreenInteractive::Fullscreen();

  auto renderer = Renderer([&] {
    return vbox({
              text(to_display) | center | flex,
         }) | border | color(front_color) | bgcolor(back_color);
  });

  auto breaker = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Return) {
      screen.ExitLoopClosure()();
      return true;
    }
    return false;
  });
  screen.Loop(breaker);
}