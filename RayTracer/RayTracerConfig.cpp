#include "RayTracerConfig.h"

RayTracerConfig::RayTracerConfig(){
    // defalt configuration variables
    this->max_depth = 5;
    this->defaultRefractionIndex = 1.0f;
    this->min_dist = 0.001f;
    this->max_dist = 2000.0f;
    this->jitterSamplingEnabled = true;
    this->numberJitterRows = 4;
    this->numberJitterCols = 4;
    this->softShadowsEnabled = true;
    this->numSoftShadowSamples = 16;
}
