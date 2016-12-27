#include "ConsoleSyle.h"

int main()
{

  namespace cs = ConsoleStyle;

  std::cout << cs::default_style() << "default_style" ; std::cout << std::endl;
  std::cout << cs::black() << "black" ; std::cout << std::endl;
  std::cout << cs::red() << "red" ; std::cout << std::endl;
  std::cout << cs::green() << "green" ; std::cout << std::endl;
  std::cout << cs::yellow() << "yellow" ; std::cout << std::endl;
  std::cout << cs::blue() << "blue" ; std::cout << std::endl;
  std::cout << cs::magenta() << "magenta" ; std::cout << std::endl;
  std::cout << cs::cyan() << "cyan" ; std::cout << std::endl;
  std::cout << cs::white() << "white" ; std::cout << std::endl;
  std::cout << cs::bg_black() << "bg_black" ; std::cout << std::endl;
  std::cout << cs::bg_red() << "bg_red" ; std::cout << std::endl;
  std::cout << cs::bg_green() << "bg_green" ; std::cout << std::endl;
  std::cout << cs::bg_yellow() << "bg_yellow" ; std::cout << std::endl;
  std::cout << cs::bg_blue() << "bg_blue" ; std::cout << std::endl;
  std::cout << cs::bg_magenta() << "bg_magenta" ; std::cout << std::endl;
  std::cout << cs::bg_cyan() << "bg_cyan" ; std::cout << std::endl;
  std::cout << cs::bg_white() << "bg_white" ; std::cout << std::endl;
  std::cout << cs::bright() << "bright" ; std::cout << std::endl;
  std::cout << cs::dim() << "dim" ; std::cout << std::endl;
  std::cout << cs::italic() << "italic" ; std::cout << std::endl;
  std::cout << cs::blink() << "blink" ; std::cout << std::endl;
  std::cout << cs::underline() << "underline" ; std::cout << std::endl;
  std::cout << cs::reverse() << "reverse" ; std::cout << std::endl;

  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << cs:: bright() << cs::default_style() << "bright default_style" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::black() << "bright black" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::red() << "bright red" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::green() << "bright green" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::yellow() << "bright yellow" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::blue() << "bright blue" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::magenta() << "bright magenta" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::cyan() << "bright cyan" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::white() << "bright white" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::bg_black() << "bright bg_black" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::bg_red() << "bright bg_red" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::bg_green() << "bright bg_green" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::bg_yellow() << "bright bg_yellow" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::bg_blue() << "bright bg_blue" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::bg_magenta() << "bright bg_magenta" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::bg_cyan() << "bright bg_cyan" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::bg_white() << "bright bg_white" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::bright() << "bright bright" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::dim() << "bright dim" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::italic() << "bright italic" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::blink() << "bright blink" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::underline() << "bright underline" ; std::cout << std::endl;
  std::cout << cs:: bright() << cs::reverse() << "bright reverse" ; std::cout << std::endl;

  {
    SET_SCOPED_CONSOLE_STYLE(std::cout, cs::yellow()+cs::reverse()+cs::underline());
    std::cout << "hello world" << std::endl;
    {
      SET_SCOPED_CONSOLE_STYLE(std::cout, cs::default_style());
      std::cout << "default" << std::endl;
    }
  }


  cs::set_cout_style_mode(cs::ConsoleMode::AUTO);
  std::cout << cs::red() << "auto red" << std::endl;
  cs::set_cout_style_mode(cs::ConsoleMode::FORCE_NO_COLORS);
  std::cout << cs::red() << "force no red" << std::endl;
  cs::set_cout_style_mode(cs::ConsoleMode::FORCE_COLORS);
  std::cout << cs::red() << "force red" << std::endl;


  return 0;
}
