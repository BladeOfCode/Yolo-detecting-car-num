 /*************************************************************************
 * OPENALPR CONFIDENTIAL
 * 
 *  Copyright 2017 OpenALPR Technology, Inc.
 *  All Rights Reserved.
 * 
 * NOTICE:  All information contained herein is, and remains
 * the property of OpenALPR Technology Incorporated. The intellectual
 * and technical concepts contained herein are proprietary to OpenALPR  
 * Technology Incorporated and may be covered by U.S. and Foreign Patents.
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from OpenALPR Technology Incorporated.
 */

#ifndef VEHICLE_CLASSIFIER_C_H
#define VEHICLE_CLASSIFIER_C_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _WIN32
  #define VEHICLECLASSIFIERC_DLL_EXPORT __declspec( dllexport )
#else
  #define VEHICLECLASSIFIERC_DLL_EXPORT
#endif

typedef void VEHICLECLASSIFIER;

struct VehicleClassifierCRegionOfInterest
{
  int x;
  int y;
  int width;
  int height;
};

  
// Initializes the vehicle classifier library and returns a pointer to the VEHICLECLASSIFIER instance
// Device_type 0 = CPU, 1 = GPU
VEHICLECLASSIFIER* vehicleclassifier_init(const char* configFile, const char* runtimeDir, int device_type, int batch_size, int gpu_id);

// Returns 1 if the library was loaded successfully, 0 otherwise
int vehicleclassifier_is_loaded(VEHICLECLASSIFIER* instance);
      
// Recognizes the provided image and responds with JSON. 
// Image is expected to be raw pixel data (BGR, 3 channels)
// Caller must call free() on the returned object
char* vehicleclassifier_recognize_rawimage(VEHICLECLASSIFIER* instance, unsigned char* pixelData, int bytesPerPixel, int imgWidth, int imgHeight, struct VehicleClassifierCRegionOfInterest roi);

// Recognizes the encoded (e.g., JPEG, PNG) image.  bytes are the raw bytes for the image data.
char* vehicleclassifier_recognize_encodedimage(VEHICLECLASSIFIER* instance, unsigned char* bytes, long long length, struct VehicleClassifierCRegionOfInterest roi);

// Maximum number of candidates to return
void vehicleclassifier_set_topn(VEHICLECLASSIFIER* instance, int topN);

// Frees a char* response that was provided from a recognition request.
// This is required for interoperating with managed languages (e.g., C#) that can't free the memory themselves
void vehicleclassifier_free_response_string(char* response);

// Free the memory for the VEHICLECLASSIFIER instance created with vehicleclassifier_init
void vehicleclassifier_cleanup(VEHICLECLASSIFIER* instance);


      


#ifdef __cplusplus
}
#endif

#endif /* VEHICLE_CLASSIFIER_C_H */

