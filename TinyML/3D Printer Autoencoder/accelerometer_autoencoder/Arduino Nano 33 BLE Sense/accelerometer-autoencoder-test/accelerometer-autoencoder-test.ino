
/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include <TensorFlowLite.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "accel_model.h"
#include "main_functions.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;
  TfLiteTensor* output = nullptr;

  constexpr int kTensorArenaSize = 80 * 1024;
  uint8_t tensor_arena[kTensorArenaSize];
}





// The name of this function is important for Arduino compatibility.
void setup() {
  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(accel_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // Loading only the necessary ops
  /*
  tflite::MicroMutableOpResolver<2> micro_op_resolver; // NOLINT
  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddTanh();
  */
  static tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

  for (int i = 0; i < 20; i++) {
    TF_LITE_REPORT_ERROR(error_reporter, "Giving time to monitor");
    delay(1000);
  }

}

// The name of this function is important for Arduino compatibility.
void loop() {
   //TF_LITE_REPORT_ERROR(error_reporter, "Input Size: %d First layer: %d Second Layer: %d\n", input->dims->size, input->dims->data[0], input->dims->data[1]);

   float x = -0.10, y = x, z = 1.0;

   input->data.f[0] = x;
   input->data.f[1] = y;  
   input->data.f[2] = z; 
   
   TF_LITE_REPORT_ERROR(error_reporter, "x: %f y: %f z: %f\n", input->data.f[0], input->data.f[1], input->data.f[2]);

  

  
  // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();
  /*
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed");
    
  }
  */
      
  TF_LITE_REPORT_ERROR(error_reporter, "x2: %f y2: %f z2: %f", output->data.f[0], output->data.f[1], output->data.f[2]);
  
  delay(1000);
  //*/
  
}
