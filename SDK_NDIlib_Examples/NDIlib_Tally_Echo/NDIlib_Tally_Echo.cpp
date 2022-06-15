#include <iostream>
#include <cstdio>
#include <string.h>
#include <chrono>
#include <thread>
#include <Processing.NDI.Lib.h>

#include "rapidxml/rapidxml.hpp"

#ifdef _WIN32
#define strcasecmp _stricmp
#ifdef _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x64.lib")
#else // _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x86.lib")
#endif // _WIN64
#endif // _WIN32

int main(int argc, char* argv[])
{	// Not required, but "correct" (see the SDK documentation)
	if (!NDIlib_initialize()) { 
		std::cout << "NDI Library initialization error\n"; 
		return 0; 
	}

	// Run a command line option to specify the source
	if (argc != 2) { 
		std::cout << "1 argument required\n"; 
		return 0;
	}

	// We are going to create a receiver that receives very little data from the source.
	NDIlib_recv_create_v3_t recv_desc;
	recv_desc.bandwidth = NDIlib_recv_bandwidth_lowest;
	recv_desc.source_to_connect_to.p_ndi_name = argv[1];
	
	// We now have at least one source, so we create a receiver to look at it.
	NDIlib_recv_instance_t pNDI_recv = NDIlib_recv_create_v3(&recv_desc);
	if (!pNDI_recv) return 0;

	// Run for one minute
	using namespace std::chrono;
	using namespace std::this_thread;

    int i = 0;
	for (const auto start = high_resolution_clock::now(); high_resolution_clock::now() - start < minutes(5);)
	{	// We are going to get meta-data from the source
        NDIlib_metadata_frame_t metadata;
        NDIlib_video_frame_v2_t frame;
        ++i;
        std::cout << "I = " << i << std::endl;
        switch (NDIlib_recv_capture_v2(pNDI_recv, &frame, nullptr, &metadata, 5000)) {
        case NDIlib_frame_type_none: {
            std::cout << "timeout" << std::endl;
            break;
        }
        case NDIlib_frame_type_video: {
            std::cout << "new video frame" << std::endl;
            (!frame.p_data)
                    ? printf("No video frame data\n")
                    : printf("Frame size = %d bytes\n", frame.data_size_in_bytes);
            NDIlib_recv_free_video_v2(pNDI_recv, &frame);
            break;
        }
        case NDIlib_frame_type_metadata: {
            std::cout << "new metadata frame" << std::endl;
            // Parse the XML
            rapidxml::xml_document<char> parser;
            parser.parse<0>((char*)metadata.p_data);

            // Get the tag
            rapidxml::xml_node<char>* p_node = parser.first_node();

            printf("xml parsed\n");
            std::cout << "name = " << p_node->name() << std::endl;

            // Check its a node
            if ((!p_node) || (p_node->type() != rapidxml::node_element))
            {	// Not a valid message
                printf("Not valid\n");
            }
            else if (!::strcasecmp(p_node->name(), "ndi_tally_echo"))
            {	// Get the zoom factor
                const rapidxml::xml_attribute<char>* p_on_program = p_node->first_attribute("on_program");
                const rapidxml::xml_attribute<char>* p_on_preview = p_node->first_attribute("on_preview");

                // Display the tally state
                printf("Tally, on_program = %s, on_preview = %s\n",
                       p_on_program ? p_on_program->value() : "false",
                       p_on_preview ? p_on_preview->value() : "false");
            }
            // Free any meta-data
            NDIlib_recv_free_metadata(pNDI_recv, &metadata);
//            sleep_for(seconds(5));
            break;
        }
        default:
            std::cout << "default case" << std::endl;
            break;
        } // END switch
    } // END for-loop

	// Destroy the receiver
	NDIlib_recv_destroy(pNDI_recv);

	// Not required, but nice
	NDIlib_destroy();

	// Finished
	return 0;
}
