#pragma once

#include <vector>
#include <string>
#include <stdexcept>

namespace usagi {
  namespace sensors {
    #include <sensors/sensors.h>

    struct sensors {

      struct subfeature_type {
        std::string name;
        int         number;
        sensors_subfeature_type type;
        int         mapping;
        unsigned    flags;
        double      value;
      };
      using subfeatures_type = std::vector<subfeature_type>;

      struct feature_type {
        std::string name;
        int         number;
        sensors_feature_type type;
        subfeatures_type     subfeatures;
      };
      using features_type = std::vector<feature_type>;
      
      struct chip_type {
        std::string    prefix;
        sensors_bus_id bus;
        int            addr;
        std::string    path;
        features_type features;
      };
      using chips_type = std::vector<chip_type>;

      std::string get_libsensors_version(){
        return libsensors_version;
      }

      sensors() {
        sensors_init(nullptr);
      }

      chips_type poll(){
        chips_type r;
        
        sensors_chip_name const * tmp_chip_name;
        int cn = 0;
        while ((tmp_chip_name = sensors_get_detected_chips(nullptr, &cn)) != nullptr) {
          chip_type c;
          c.prefix = tmp_chip_name -> prefix;
          c.bus    = tmp_chip_name -> bus;
          c.addr   = tmp_chip_name -> addr;
          c.path   = tmp_chip_name -> path;
          sensors_feature const * tmp_feature;
          int fn = 0;
          while ((tmp_feature = sensors_get_features(tmp_chip_name, &fn)) != nullptr) {
            feature_type f;
            f.name   = tmp_feature -> name;
            f.number = tmp_feature -> number;
            f.type   = tmp_feature -> type;
            sensors_subfeature const * tmp_subfeature;
            int sfn = 0;
            while (
              (
                tmp_subfeature = sensors_get_all_subfeatures(
                  tmp_chip_name, tmp_feature, &sfn
                )
              ) != nullptr
            ) {
              subfeature_type sf; 
              sf.name    = tmp_subfeature -> name;
              sf.number  = tmp_subfeature -> number;
              sf.type    = tmp_subfeature -> type;
              sf.mapping = tmp_subfeature -> mapping;
              sf.flags   = tmp_subfeature -> flags;
              if(
                bool(sf.flags & SENSORS_MODE_R) && 
                (sensors_get_value(tmp_chip_name, tmp_subfeature->number, &sf.value) < 0)
              )
                  throw std::runtime_error("sensors_get_value failed");
              f.subfeatures.push_back(sf);
            }
            c.features.push_back(f);
          }
          r.push_back(c);
        }
        
        return move(r);
      }

      chips_type operator()(){
        return poll();
      }

    };
  }
}

