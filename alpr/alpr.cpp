#include <stdio.h>
#include <stdlib.h>
#include <alpr_c.h>
#include "json.hpp" 
#include <iostream>

using namespace std;

long read_file(const char* file_path, unsigned char** buffer)
{
    FILE* fileptr;
    long filelen;

    fileptr = fopen(file_path, "rb");     // Open the file in binary mode
    if (!fileptr)
        return 0;

    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    *buffer = (unsigned char*)malloc((filelen + 1) * sizeof(char)); // Enough memory for file + \0
    fread(*buffer, filelen, 1, fileptr); // Read in the entire file
    fclose(fileptr); // Close the file

    return filelen;
}

int main(int argc, char* argv[])
{
    OPENALPR* alpr_obj;
    //// Optional module to classify vehicle make/model/color
    //VEHICLECLASSIFIER* vehicle_classifier_obj;

    if (argc != 3)
    {
        printf("Usage: %s [path to image file]\n", argv[0]);
        return 1;
    }

    const char* file_path = argv[1];
    const char* OPENALPR_LICENSE_KEY = "";
    const char* COUNTRY = argv[2];
    //const char* COUNTRY = "eg";
    // Leave the config and runtime directory blank to look for these in the current directory.
    alpr_obj = openalpr_init(COUNTRY, "", "", OPENALPR_LICENSE_KEY);

    if (!openalpr_is_loaded(alpr_obj))
    {
        std::cout << "Error loading the OpenALPR library" << std::endl;
        return 1;
    }
   /* if (!vehicleclassifier_is_loaded(vehicle_classifier_obj))
    {
        std::cout << "Error loading the Vehicle Classifier library" << std::endl;
        return 1;
    }*/

    if (openalpr_is_loaded(alpr_obj))
    {
        // We don't want to restrict the size of the recognition area, so we set this to an extremely large pixel size
        // rather than decode and find the actual image width/height.
        struct AlprCRegionOfInterest roi;
        roi.x = 0;
        roi.y = 0;
        roi.width = 10000;
        roi.height = 10000;

        // Read the image file
        unsigned char* buffer;
        long long length = read_file(file_path, &buffer);

        if (length == 0)
        {
            std::cout << "Unable to read file: " << file_path << std::endl;
            return 1;
        }

        if (length > 0)
        {
            char* plate_response = openalpr_recognize_encodedimage(alpr_obj, buffer, length, roi);
            //printf("Alpr response:\n%s\n", plate_response);

            // Parse the JSON that comes back
            nlohmann::json parsed_plate_data = nlohmann::json::parse(plate_response);
            // Free the JSON string now that we are done with it
            printf("Trent\n");
            printf("plate0: %d results\n", parsed_plate_data["results"].size());
            //printf("%s\n", parsed_plate_data.at("candidates")[0].at("plate"));
            //cout << parsed_plate_data.at("candidates");
            openalpr_free_response_string(plate_response);
            for (nlohmann::json result : parsed_plate_data["results"])
            {
                //printf("%s\n", result["plate"]);
                std::cout << "    - " << result["plate"] << "      confidence: " << result["confidence"] << endl;
                //std::cout << "plate: " << (std::string)result["plate"] << " - " << (std::string)result["region"] << " (" << result["confidence"] << ")" << std::endl;
            }
        }

        free(buffer);
    }
    openalpr_cleanup(alpr_obj);

    return 0;
}
