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
        const auto stringLength = static_cast<int>(str.length());
        JSONCPP_STRING err;
        Json::Value root;

        Json::CharReaderBuilder builder;
        const unique_ptr<Json::CharReader> reader(builder.newCharReader());
        if (!reader->parse(str.c_str(), str.c_str() + stringLength, &root,
                           &err)) {
            CLogger::Debug("Failed to parse json string");
            throw runtime_error("Failed to parse json string");
        }
        return root;
    }

    string parseJsonToString(const Json::Value &val) {
      Json::StreamWriterBuilder wbuilder;
      string str_data = writeString(wbuilder, val);
      return str_data;
    }
  }// namespace Parser
}// namespace Assets