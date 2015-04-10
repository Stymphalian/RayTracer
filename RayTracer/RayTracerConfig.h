#ifndef RAY_TRACER_CONFIG_H_
#define RAY_TRACER_CONFIG_H_

class RayTracerConfig{
public:
    RayTracerConfig();

    // configuration stuff...
    int max_depth;
    float defaultRefractionIndex;
    float min_dist;
    float max_dist;
    bool  jitterSamplingEnabled;
    int numberJitterRows;
    int numberJitterCols;
    bool softShadowsEnabled;
    int numSoftShadowSamples; // must be a sqrt-able number
};

#endif