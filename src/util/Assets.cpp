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

  int getRandomNumber(int min, int max) {
    srand((unsigned int) time(nullptr));
    return (rand() % (max + 1 - min)) + min;
  }

  namespace Parser {
    wstring parseStringToWstring(const string &str) {
      wstring wstr;
      wstr.assign(str.begin(), str.end());
      return wstr;
    };

    Json::Value parseStringToJson(const string &str) {
      Json::CharReaderBuilder rbuilder;
      string errs;
      Json::Value val;
      stringstream ss;

      ss << str;

      rbuilder["collectComments"] = false;

      bool isSuccess = parseFromStream(rbuilder, ss, &val, &errs);

      if (!isSuccess) {
        CLogger::Error("%s", errs.c_str());
      }

      return nullptr;
    }

    string parseJsonToString(const Json::Value &val) {
      Json::StreamWriterBuilder wbuilder;
      string str_data = writeString(wbuilder, val);
      return str_data;
    }
  }// namespace Parser
}// namespace Assets