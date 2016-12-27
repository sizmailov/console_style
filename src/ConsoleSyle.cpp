#include "console_style/ConsoleSyle.h"
#include <cassert>
#include <cstdio>

using namespace ConsoleStyle;


#ifdef _WIN32
#include <io.h>
namespace {
bool is_stdout_redirected(){
  return !_isatty( _fileno( stdout ) );
}
bool is_stderr_redirected(){
  return !_isatty( _fileno( stderr) );
}
}
#else
#include <unistd.h>
namespace {
bool is_stdout_redirected(){
  static const bool result = !isatty( fileno( stdout ) );
  if (!result) {
    errno =0;
  }
  return result;
}
bool is_stderr_redirected(){
  static const bool result = !isatty( fileno( stderr ) );
  if (!result) {
    errno=0;
  }
  return result;
}

void print_style(std::ostream& out, const Style& new_style, const Style& current_style) {
  assert(&out==&std::cout || &out==&std::cerr || &out == &std::clog);
  if (  current_style.special.fields!=new_style.special.fields
      ||new_style.foreground==Style::Color::NONE && current_style.foreground != Style::Color::NONE
      ||new_style.background==Style::Color::NONE && current_style.background != Style::Color::NONE){
    out << "\033[" + std::to_string(Style::Effect::RESET_ALL) +"m";
  }
  if (new_style.foreground!=Style::Color::NONE) {
    out << "\033["+std::to_string(static_cast<int>(new_style.foreground)+29) +"m";
  }
  if (new_style.background!=Style::Color::NONE) {
    out << "\033["+std::to_string(static_cast<int>(new_style.background)+39) +"m";
  }
  if (!new_style.special.is_none()) {
    for (auto &field : {
        Style::Effect::RESET_ALL,
        Style::Effect::BRIGHT,
        Style::Effect::DIM,
        Style::Effect::ITALIC,
        Style::Effect::UNDERSCORE,
        Style::Effect::BLINK,
        Style::Effect::REVERSE
    }){
      if (new_style.special.fields.test(field)) {
        out << "\033["+std::to_string(field) + "m";
      }
    }
  }
}

}
#endif


Style::Effect::Effect(int i) {
    assert(i==RESET_ALL||i==BRIGHT||i==DIM||i==ITALIC||i==UNDERSCORE||i==BLINK||i==REVERSE);
    // check we do support such effect
    fields.set(i);
}

bool Style::Effect::is_none() const {
  return fields.count()==0;
}

void Style::Effect::override_with(const Effect& other) {
  if (fields.test(RESET_ALL) || other.fields.test(RESET_ALL)){
    fields.reset();
    fields.set(RESET_ALL);
    return;
  }
  fields.set(RESET_ALL,false);
  fields |= other.fields;
}


StyleGuard::StyleGuard(std::ostream* out, Style::Effect special, Style::Color foreground,
    Style::Color background)
    : out(nullptr), style{special,foreground,background} {
  assert(!style.special.is_none()
      ||style.foreground!=Style::Color::NONE
      ||style.background!=Style::Color::NONE); // Style should change something

  if (out==&std::cout && global_state().colorize_cout){
    (*out) << *this;
  } else if ((out==&std::cerr || out == &std::clog) && global_state().colorize_cerr) {
    (*out) << *this;
  }
}
StyleGuard::StyleGuard(std::ostream& out, StyleGuard&&other) : out(nullptr), style(std::move(other.style)){
  if (&out==&std::cout && global_state().colorize_cout){
    out << *this;
  } else if ((&out==&std::cerr || &out == &std::clog) && global_state().colorize_cerr) {
    out << *this;
  }
}

StyleGuard StyleGuard::operator+(const StyleGuard& other) const{
  auto s = style;
  s.override_with(other.style);
  return StyleGuard(nullptr,s.special,s.foreground,s.background);
}

void Style::override_with(const Style& other) {
  special.override_with(other.special);
  if (other.foreground!=Color::NONE){ foreground = other.foreground;}
  if (other.background!=Color::NONE){ background = other.background;}
}

StyleGuard::~StyleGuard() {
  {
    if (out) {
      if (out==&std::cout){
        auto current = global_state().cout_style_stack.top();
        global_state().cout_style_stack.pop();
        print_style(*out, global_state().cout_style_stack.top(), current);
      } else if (out==&std::cerr || out == &std::clog){
        auto current = global_state().cerr_style_stack.top();
        global_state().cerr_style_stack.pop();
        print_style(*out, global_state().cerr_style_stack.top(), current);
      } else {
        assert(false); //unreachable;
      }
    }
  }
}

StyleGuard::GlobalState& StyleGuard::global_state() {
  static GlobalState state = []{
    GlobalState result;
    result.colorize_cout = !is_stdout_redirected();
    result.colorize_cerr = !is_stderr_redirected();
    result.cout_style_stack.push({});
    result.cerr_style_stack.push({});
    return result;
  }();
  return state;
}

std::ostream& ConsoleStyle::operator<<(std::ostream& out, const StyleGuard& guard) {
  if (&out==&std::cout && StyleGuard::global_state().colorize_cout){
    auto top = StyleGuard::global_state().cout_style_stack.top();
    auto tmp = top;
    tmp.override_with(guard.style);
    print_style(out, tmp, top);
    StyleGuard::global_state().cout_style_stack.push(tmp);
  } else if ((&out==&std::cerr || &out == &std::clog) && StyleGuard::global_state().colorize_cerr){

    auto top = StyleGuard::global_state().cerr_style_stack.top();
    auto tmp = top;
    tmp.override_with(guard.style);
    print_style(out, tmp, top);
    StyleGuard::global_state().cerr_style_stack.push(tmp);
  } else {
    return out;
  }
  assert(guard.out==nullptr); // reuse of guards is disallowed
  guard.out = &out;
  return out;
}


void ConsoleStyle::set_cout_style_mode(ConsoleMode mode) {
  auto& state = StyleGuard::global_state();
  assert(state.cout_style_stack.size()==1);
  if (mode==ConsoleMode::AUTO){
    state.colorize_cout=!is_stdout_redirected();
  }else if (mode==ConsoleMode::FORCE_COLORS){
    state.colorize_cout=true;
  }else{
    state.colorize_cout=false;
  }
}

void ConsoleStyle::set_cerr_style_mode(ConsoleMode mode) {
  auto& state = StyleGuard::global_state();
  assert(state.cerr_style_stack.size()==1);
  if (mode==ConsoleMode::AUTO){
    state.colorize_cerr=!is_stderr_redirected();
  }else if (mode==ConsoleMode::FORCE_COLORS){
    state.colorize_cerr=true;
  }else{
    state.colorize_cerr=false;
  }
}

StyleGuard ConsoleStyle::default_style(){ return StyleGuard(nullptr,Style::Effect::RESET_ALL);}

StyleGuard ConsoleStyle::black()        { return StyleGuard(nullptr,{},Style::Color::BLACK);}
StyleGuard ConsoleStyle::red()          { return StyleGuard(nullptr,{},Style::Color::RED);}
StyleGuard ConsoleStyle::green()        { return StyleGuard(nullptr,{},Style::Color::GREEN);}
StyleGuard ConsoleStyle::yellow()        { return StyleGuard(nullptr,{},Style::Color::YELLOW);}
StyleGuard ConsoleStyle::blue()         { return StyleGuard(nullptr,{},Style::Color::BLUE);}
StyleGuard ConsoleStyle::magenta()       { return StyleGuard(nullptr,{},Style::Color::MAGENTA);}
StyleGuard ConsoleStyle::cyan()         { return StyleGuard(nullptr,{},Style::Color::CYAN);}
StyleGuard ConsoleStyle::white()   { return StyleGuard(nullptr,{},Style::Color::WHITE);}

StyleGuard ConsoleStyle::bg_black()        { return StyleGuard(nullptr,{},{},Style::Color::BLACK);}
StyleGuard ConsoleStyle::bg_red()          { return StyleGuard(nullptr,{},{},Style::Color::RED);}
StyleGuard ConsoleStyle::bg_green()        { return StyleGuard(nullptr,{},{},Style::Color::GREEN);}
StyleGuard ConsoleStyle::bg_yellow()        { return StyleGuard(nullptr,{},{},Style::Color::YELLOW);}
StyleGuard ConsoleStyle::bg_blue()         { return StyleGuard(nullptr,{},{},Style::Color::BLUE);}
StyleGuard ConsoleStyle::bg_magenta()       { return StyleGuard(nullptr,{},{},Style::Color::MAGENTA);}
StyleGuard ConsoleStyle::bg_cyan()         { return StyleGuard(nullptr,{},{},Style::Color::CYAN);}
StyleGuard ConsoleStyle::bg_white()   { return StyleGuard(nullptr,{},{},Style::Color::WHITE);}

StyleGuard ConsoleStyle::bright(){ return StyleGuard(nullptr,Style::Effect::BRIGHT);}
StyleGuard ConsoleStyle::dim(){ return StyleGuard(nullptr,Style::Effect::DIM);}
StyleGuard ConsoleStyle::italic(){ return StyleGuard(nullptr,Style::Effect::ITALIC);}
StyleGuard ConsoleStyle::blink(){ return StyleGuard(nullptr,Style::Effect::BLINK);}
StyleGuard ConsoleStyle::underline(){ return StyleGuard(nullptr,Style::Effect::UNDERSCORE);}
StyleGuard ConsoleStyle::reverse(){ return StyleGuard(nullptr,Style::Effect::REVERSE);}

