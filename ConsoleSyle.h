#pragma once
#include <iostream>
#include <bitset>
#include <cassert>
#include <stack>


#define CONSOLE_STYLE_CONCAT_(x,y) x##y
#define CONSOLE_STYLE_CONCAT(x,y) CONSOLE_STYLE_CONCAT_(x,y)
#define CONSOLE_STYLE_UNIQUE_GUARD_NAME CONSOLE_STYLE_CONCAT(style_guard_at_line_, __LINE__)
#define SET_SCOPED_CONSOLE_STYLE(out,style) ::ConsoleStyle::StyleGuard CONSOLE_STYLE_UNIQUE_GUARD_NAME = ::ConsoleStyle::StyleGuard(out,style);

namespace ConsoleStyle {

struct Style{
  struct Effect{
    std::bitset<10> fields;
    static const int
    RESET_ALL = 0,
    BRIGHT    = 1,
    DIM       = 2,
    ITALIC    = 3,
    UNDERSCORE= 4,
    BLINK     = 5,
    REVERSE   = 7;
    Effect(){}
    Effect(int i);
    bool is_none() const;
    void override_with(const Effect& other);
  };

  enum class Color{
    NONE = 0,
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
  };
  void override_with(const Style& other);
  Effect special;
  Color foreground;
  Color background;
};

struct StyleGuard;

std::ostream& operator<<(std::ostream& out, const StyleGuard& guard);

struct StyleGuard{
  StyleGuard(std::ostream* out, Style::Effect special, Style::Color foreground=Style::Color::NONE, Style::Color background=Style::Color::NONE);
  StyleGuard(std::ostream& out, StyleGuard&&);
  StyleGuard(StyleGuard&&) = default;
  StyleGuard(const StyleGuard&) = delete;

  StyleGuard& operator=(StyleGuard&&) = default;
  StyleGuard& operator=(const StyleGuard&) = delete;

  StyleGuard operator+(const StyleGuard&)const;

  ~StyleGuard();
private:
  struct GlobalState{
    bool colorize_cout;
    bool colorize_cerr;
    std::stack<Style> cout_style_stack;
    std::stack<Style> cerr_style_stack;
  };
  static GlobalState& global_state();
  Style style;
  mutable std::ostream* out;
  friend std::ostream& operator<<(std::ostream& out, const StyleGuard& guard);
};

inline namespace styles{

StyleGuard default_style();

// Foreground colors
StyleGuard black();
StyleGuard red();
StyleGuard green();
StyleGuard yellow();
StyleGuard blue();
StyleGuard magenta();
StyleGuard cyan();
StyleGuard white();

// Background colors
StyleGuard bg_black();
StyleGuard bg_red();
StyleGuard bg_green();
StyleGuard bg_yellow();
StyleGuard bg_blue();
StyleGuard bg_magenta();
StyleGuard bg_cyan();
StyleGuard bg_white();

// Style modifiers
StyleGuard bright();
StyleGuard dim();
StyleGuard italic();
StyleGuard blink();
StyleGuard underline();
StyleGuard reverse();

}

}


