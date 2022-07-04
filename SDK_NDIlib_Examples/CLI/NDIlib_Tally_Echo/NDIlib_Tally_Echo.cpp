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

    NDIlib_tally_t NDI_tally;
    NDI_tally.on_program = "on_program";
    NDIlib_recv_set_tally(pNDI_recv, &NDI_tally);

	// Run for one minute
	using namespace std::chrono;
	using namespace std::this_thread;


    int i = 0;
    for (const auto start = high_resolution_clock::now(); high_resolution_clock::now() - start < minutes(5);)
    {	// We are going to get meta-data from the source
        NDIlib_metadata_frame_t metadata;
        NDIlib_video_frame_v2_t frame;
        std::cout << "I = " << ++i << std::endl;
        bool sync_meta = 0;
        bool sync_vid = 0;
        bool sync_timeout = 0;
        while ((sync_vid == 0) || (sync_meta == 0))
        {
            switch (NDIlib_recv_capture_v2(pNDI_recv, &frame, nullptr, &metadata, 5000))
            {
            case NDIlib_frame_type_none: {
                if (sync_timeout == 1) break;
                std::cout << "timeout\n";
                sync_timeout = 1;
                break;
            }
            case NDIlib_frame_type_video: {
                if (sync_vid == 1) break;
                std::cout << "new video frame\n";
                (!frame.p_data)
                    ? std::cout << "No video frame data\n"
                    : std::cout << "Frame size = " << frame.data_size_in_bytes << " bytes\n";
                NDIlib_recv_free_video_v2(pNDI_recv, &frame);
                sync_vid = 1;
                break;
            }
            case NDIlib_frame_type_metadata: {
                if (sync_meta == 1) break;
                std::cout << "new metadata frame\n";
                // Parse the XML
                rapidxml::xml_document<char> parser;
                parser.parse<0>((char*)metadata.p_data);

                // Get the tag
                rapidxml::xml_node<char>* p_node = parser.first_node();

                std::cout << "xml parsed\n";
                std::cout << "name = " << p_node->name() << "\n";

                // Check its a node
                if ((!p_node) || (p_node->type() != rapidxml::node_element))
                {	// Not a valid message
                    std::cout << "Not valid\n";
                }
                else // if (!::strcasecmp(p_node->name(), "ndi_tally_echo"))
                {	// Get the zoom factor
                    const rapidxml::xml_attribute<char>* p_on_program = p_node->first_attribute("short_name");
                    const rapidxml::xml_attribute<char>* p_on_preview = p_node->first_attribute(0);

                    const auto& is_program = (p_on_program) ? p_on_program->value() : "false";
                    const auto& is_preview = (p_on_preview) ? p_on_preview->value() : "false";
                    // Display the tally state
                    std::cout << "Tally, on_program = " << is_program << ", on_preview = " << is_preview << std::endl;
                }
                // Free any meta-data
                NDIlib_recv_free_metadata(pNDI_recv, &metadata);
                sync_meta = 1;
                break;
            }
            default: {
                std::cout << "default case" << std::endl;
                break;
            } 
                   // END switch
            }
            // END for-loop
            if (sync_timeout == 1) break;
        }
    }
	// Destroy the receiver
	NDIlib_recv_destroy(pNDI_recv);

	// Not required, but nice
	NDIlib_destroy();

	// Finished
	return 0;
}
