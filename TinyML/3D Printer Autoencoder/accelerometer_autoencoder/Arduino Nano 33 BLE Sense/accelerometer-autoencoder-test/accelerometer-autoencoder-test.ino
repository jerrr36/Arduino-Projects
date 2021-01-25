
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
#include "accel_model.h"
#include "main_functions.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include <Arduino_LSM9DS1.h>

// Globals, used for compatibility with Arduino-style sketches.
namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;
  TfLiteTensor* output = nullptr;

  constexpr int kTensorArenaSize = 2800;
  uint8_t tensor_arena[kTensorArenaSize];

  float numSamples = 5;
  float xSum, ySum, zSum, x, y, z, initSum, finalSum, loss;
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

  // Loading all ops
  // unfortunately micro mutable ops breaks mbed os
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


  // Initializing imu
  if (!IMU.begin()) {
    while (1);
  }

  IMU.setContinuousMode();

  TF_LITE_REPORT_ERROR(error_reporter, "Sample Rate: %f", IMU.accelerationSampleRate());


  pinMode(22, OUTPUT);
}

// The name of this function is important for Arduino compatibility.
void loop() {
   //TF_LITE_REPORT_ERROR(error_reporter, "Input Size: %d First layer: %d Second Layer: %d\n", input->dims->size, input->dims->data[0], input->dims->data[1]);
   long t1, t2, delta;
   for (int i = 0; i < numSamples; i++) {

    // loop until data is available
    while (!IMU.accelerationAvailable());

    IMU.readAcceleration(x, y, z);

    // updating sum
    xSum += x;
    ySum += y;
    zSum += z;
    
   }

   initSum = xSum / numSamples + ySum / numSamples + zSum / numSamples;
   // TF_LITE_REPORT_ERROR(error_reporter, "Initial Sum: %f", initSum);
   
   // Inputting data into input tensor
   input->data.f[0] = xSum / numSamples;
   input->data.f[1] = ySum / numSamples;  
   input->data.f[2] = zSum / numSamples; 

   xSum = 0;
   ySum = 0;
   zSum = 0;

   //TF_LITE_REPORT_ERROR(error_reporter, "x: %f y: %f z: %f", input->data.f[0], input->data.f[1], input->data.f[2]);
  
  // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();
  
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed");
    
  }
  
      
  //TF_LITE_REPORT_ERROR(error_reporter, "x2: %f y2: %f z2: %f", output->data.f[0], output->data.f[1], output->data.f[2]);

  finalSum = output->data.f[0] + output->data.f[1] + output->data.f[2];

  // TF_LITE_REPORT_ERROR(error_reporter, "Final Sum: %f", finalSum);

  loss = abs(initSum - finalSum);
  
  //TF_LITE_REPORT_ERROR(error_reporter, "Loss: %f", loss);

  if (loss > .1) {
    digitalWrite(22, LOW);
    TF_LITE_REPORT_ERROR(error_reporter, "Loss is greater than threshhold");
  }
  else {
    digitalWrite(22, HIGH);
    TF_LITE_REPORT_ERROR(error_reporter, "Loss is less than threshhold");
  }

  
}
