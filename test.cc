#include <iostream>
#include <string>
#include <regex>

using namespace std;

int main ()
{
    /*
  std::string s ("Experience with application programming and languages ( e.g Android SDK , J2EE , VB , VBA , ASP , .Net , C , C# , COBOL4 , COBOL , XML , SQL , PL-SQL ) . ");
  std::smatch m;
  std::regex e ("[\\s|(|,|-|/](Java)[\\s|)|,|-|.|/]", regex_constants::icase);

  std::regex_search ( s, m, e );

  cout << m.str(1) << " at position " << m.position(1) << "," << m.position(1) + m.str(1).length() - 1 << endl;
*/

  string str = "\0\0\0\0";
  cout << (str == "\0") << endl;
  cout << str << " length " << str.length() << endl;
  return 0;
}