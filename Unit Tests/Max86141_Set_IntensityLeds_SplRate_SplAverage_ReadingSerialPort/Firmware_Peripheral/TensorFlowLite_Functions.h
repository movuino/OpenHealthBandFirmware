BLEService        TensorFlowService = BLEService(0x1500);
BLECharacteristic PossCharacteristic = BLECharacteristic(0x1501);

bool errorTens = false;

#include <TensorFlowLite.h>
  #include "tensorflow/lite/experimental/micro/kernels/micro_ops.h"
  #include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
  #include "tensorflow/lite/experimental/micro/micro_interpreter.h"
  #include "tensorflow/lite/experimental/micro/micro_mutable_op_resolver.h"
  #include "tensorflow/lite/schema/schema_generated.h"
  #include "tensorflow/lite/version.h"
  #include "model.h"

  namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* tflInputTensor = nullptr;
  TfLiteTensor* tflOutputTensor = nullptr;
  int inference_count = 0;

  constexpr int kTensorArenaSize = 8 * 1024;
  alignas(8) uint8_t tensor_arena[kTensorArenaSize];
  }  // namespace
  const float accelerationThreshold = 1; // threshold of significant in G's
  const int numSamples = 50;

  int imucounter;
  //int samplesRead = 119;

  //tinyml variables
  const char* GESTURES[] = {
  "flap",
  "still",
  "walk"
  };

  #define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))
  float possibility[NUM_GESTURES];
  int poss[NUM_GESTURES];
  uint8_t* p;
