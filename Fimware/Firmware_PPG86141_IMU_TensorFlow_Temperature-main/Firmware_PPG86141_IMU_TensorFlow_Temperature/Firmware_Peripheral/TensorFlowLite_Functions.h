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

void configureTensorFlow() {
  Serial.println("####  CONFIG TENSORFLOW  ####");
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
      "Model provided is schema version %d not equal "
      "to supported version %d.",
      model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroMutableOpResolver resolver;
  resolver.AddBuiltin(
    tflite::BuiltinOperator_DEPTHWISE_CONV_2D,
    tflite::ops::micro::Register_DEPTHWISE_CONV_2D());
  resolver.AddBuiltin(
    tflite::BuiltinOperator_MAX_POOL_2D,
    tflite::ops::micro::Register_MAX_POOL_2D());
  resolver.AddBuiltin(tflite::BuiltinOperator_CONV_2D,
                      tflite::ops::micro::Register_CONV_2D());
  resolver.AddBuiltin(
    tflite::BuiltinOperator_FULLY_CONNECTED,
    tflite::ops::micro::Register_FULLY_CONNECTED());
  resolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX,
                      tflite::ops::micro::Register_SOFTMAX());
  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    //TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    error_reporter->Report("Set up failed\n");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  tflInputTensor = interpreter->input(0);
  tflOutputTensor = interpreter->output(0);

  errorTens = false;
  Serial.println();

}

void updateTensorFlow() {
  int samplesRead = 50;

  while (samplesRead == numSamples) {

    float aSum = fabs(aX) + fabs(aY) + fabs(aZ);
    //Serial.println("aSum: "+String(aSum));
    if (aSum >= accelerationThreshold) {
      samplesRead = 0;
      //Serial.println("sampleRead: " + String(samplesRead));
    }
  }
  while (samplesRead < numSamples) {
    //Serial.println("SNSKK ");
    tflInputTensor->data.f[samplesRead * 6 + 0] = (aX + 4.0) / 8.0;
    tflInputTensor->data.f[samplesRead * 6 + 1] = (aY + 4.0) / 8.0;
    tflInputTensor->data.f[samplesRead * 6 + 2] = (aZ + 4.0) / 8.0;
    tflInputTensor->data.f[samplesRead * 6 + 3] = (gX + 2000.0) / 4000.0;
    tflInputTensor->data.f[samplesRead * 6 + 4] = (gY + 2000.0) / 4000.0;
    tflInputTensor->data.f[samplesRead * 6 + 5] = (gZ + 2000.0) / 4000.0;

    samplesRead++;

    TfLiteStatus invokeStatus = interpreter->Invoke();
    if (invokeStatus != kTfLiteOk) {
      Serial.println("Invoke failed!");
      while (1);
      return;
    }
  }
  for (int i = 0; i < NUM_GESTURES; i++) {

    possibility[i]  = tflOutputTensor->data.f[i];
    //poss = possibility;
    poss[i]  = tflOutputTensor->data.f[i] * 10000;
    p = (uint8_t*)&poss;
    if (i == 0)
    {
      int tp1;
      tp1 = p[3];
      tp1 = (tp1  << 8) + p[2];
      tp1 = (tp1  << 8) + p[1];
      tp1 = (tp1  << 8) + p[0];
      //Serial.println("--- " + String(tp1));
    }
    if (i == 1)
    {
      int tp2;
      tp2 = p[7];
      tp2 = (tp2  << 8) + p[6];
      tp2 = (tp2  << 8) + p[5];
      tp2 = (tp2  << 8) + p[4];
      //Serial.println("--- " + String(tp2));
    }
    if (i == 2)
    {
      int tp3;
      tp3 = p[11];
      tp3 = (tp3  << 8) + p[10];
      tp3 = (tp3  << 8) + p[9];
      tp3 = (tp3  << 8) + p[8];
      //Serial.println("--- " + String(tp3));
    }

    Serial.println("----- TensorFlow data ----- :");

    Serial.print(GESTURES[i]);
    Serial.print(": ");
    Serial.print("Float = " + String(possibility[i]));
    Serial.print("  ");
    Serial.print("Int = " + String(poss[i]));
    Serial.print(" UInt = " + String(p[i]));
    Serial.print("  ");
    Serial.print(tflOutputTensor->data.f[i], 6);
    Serial.println();

  }
}
