//
// Created by HADMARINE on 2020/09/20.
//

#include "../Assets.hpp"

using namespace std;

namespace Assets {
    void pauseUntilKeyPressed(char k, string mes) {
      CLogger::Info(mes.c_str());
      fgetc(stdin);
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void pauseUntilKeyPressed(char k) {
      fgetc(stdin);
      cin.ignore(numeric_limits<streamsize>::max(), k);
    }

    void pauseUntilKeyPressed(string mes) {
      CLogger::Info(mes.c_str());
      fgetc(stdin);
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void pauseUntilKeyPressed() {
      fgetc(stdin);
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    namespace Parser {
        wstring Parser::parseStringToWstring(const string str) {
          wstring *wstr = new wstring;
          wstr->assign(str.begin(), str.end());
          return *wstr;
        };
    }
}