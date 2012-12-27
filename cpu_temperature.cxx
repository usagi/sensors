// C++標準ヘッダー
#include <iostream> // I/O ストリーム
#include <functional> // 関数オブジェクト
#include <algorithm> // アルゴリズム
#include <stdexcept> // 標準的な例外型
// ユーザー定義ヘッダー
#include "usagi/sensors.hxx" // センサーライブラリー

// 名前空間の呼び捨て宣言
using std::cout; // 標準出力ストリーム
using std::cerr; // 標準エラー出力ストリーム
using std::endl; // 改行＋バッファーフラッシュ
using std::find_if; // <algorithm> 条件検索
using std::function; // <functional> 関数オブジェクト
using std::exception; // <stdexcept> 基本的な例外型
using std::runtime_error; // <stdexcept> 実行時例外型
// 長い名前を省略した別名の定義
using sensor            = usagi::sensors::sensors;
using sensor_chip       = usagi::sensors::sensors::chip_type;
using sensor_feature    = usagi::sensors::sensors::feature_type;

void print_feature(const sensor_feature& f){
  if(f.subfeatures.empty())
    throw runtime_error("subfeatures n/a");
  cout << "> feature name : " << f.name <<"\n";
  for(const auto& subfeature : f.subfeatures ){
    cout
      << "> > subfeature name  : " << subfeature.name << "\n"
      << "> > subfeature value : " << subfeature.value << endl;
  }
}

void print_all_sensors(){
  sensor s;
  auto chips = s();
  if(chips.empty())
    throw runtime_error("chips n/a");
  for(const auto& chip : chips){
    cout
      << "chip prefix : " << chip.prefix << "\n"
      << "chip path   : " << chip.path << "\n";
    for(const auto& feature : chip.features)
      print_feature(feature);
  }
}

void print_one_temperature(){
  sensor s;
  for(const auto& chip : s()){
    auto feature = find_if(
      chip.features.begin(),
      chip.features.end(),
      [](const sensor_feature& f){
        return f.type == usagi::sensors::SENSORS_FEATURE_TEMP;
      }
    );
    if(feature != chip.features.end()){
      print_feature(*feature);
      return;
    }
  }
  throw runtime_error("temperature sensor n/a");
}

int main() try {
  auto f = [](const char* message, function<void()> f_){
    cout << message << endl;
    f_();
    cout << endl;
  };
  f("print all sensors"    , print_all_sensors);
  f("print one temperature", print_one_temperature);
} catch (exception e) {
  cerr << "oops! " << e.what() << "\n";
}

