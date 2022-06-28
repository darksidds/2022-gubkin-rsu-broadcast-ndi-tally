//#pragma once

#include <iostream>
#include <cstdio>
#include <string.h>
#include <chrono>
#include <thread>
#include <Processing.NDI.Lib.h>

#include <QDebug>

#include "qglobal.h"
#include "rapidxml/rapidxml.hpp"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#ifdef _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x64.lib")
#else // _WIN64
#pragma comment(lib, "Processing.NDI.Lib.x86.lib")
#endif // _WIN64
#endif // _WIN32



void tally_echo(Ui::MainWindow* ui, int* flag, int* i, NDIlib_recv_instance_t* pNDI_recv, NDIlib_recv_create_v3_t* recv_desc, NDIlib_metadata_frame_t* metadata, NDIlib_video_frame_v2_t* frame)
{
    qDebug() << "func start\n";
    qDebug() << recv_desc->source_to_connect_to.p_ndi_name << "\n";
    // We are going to get meta-data from the source

    ui->plainTextEdit->appendPlainText(QString("Frame = %1").arg(*i));
    bool sync_meta = 0;
    bool sync_vid = 0;
    bool sync_timeout = 0;

    while ((sync_vid == 0) || (sync_meta == 0))
    {
        qDebug() << "check flags\n";
        switch (NDIlib_recv_capture_v2(*pNDI_recv, frame, nullptr, metadata, 5000))
        {
        case NDIlib_frame_type_none: {
            if (sync_timeout == 1) break;
//                std::cout << "timeout\n";
            ui->plainTextEdit->appendPlainText("timeout");
            qDebug() << "timeout\n";
            sync_timeout = 1;
            break;
        }
        case NDIlib_frame_type_video: {
            if (sync_vid == 1) break;
//                std::cout << "new video frame\n";
//                (!frame.p_data);
//                    ? std::cout << "No video frame data\n"
//                    : std::cout << "Frame size = " << frame.data_size_in_bytes << " bytes\n";
            NDIlib_recv_free_video_v2(*pNDI_recv, frame);
            sync_vid = 1;
            break;
        }
        case NDIlib_frame_type_metadata: {
            qDebug() << "meta check\n";
            if (sync_meta == 1) break;
            ui->plainTextEdit->appendPlainText("new metadata frame");
            // Parse the XML
            rapidxml::xml_document<char> parser;
            parser.parse<0>((char*)metadata->p_data);

            // Get the tag
            rapidxml::xml_node<char>* p_node = parser.first_node();

            ui->plainTextEdit->appendPlainText("xml parsed");
            ui->plainTextEdit->appendPlainText(QString("name = %1").arg(p_node->name()));

            // Check its a node
            if ((!p_node) || (p_node->type() != rapidxml::node_element))
            {	// Not a valid message
                ui->plainTextEdit->appendPlainText("Not valid");
            }
            else // if (!::strcasecmp(p_node->name(), "ndi_tally_echo"))
            {	// Get the zoom factor
                const rapidxml::xml_attribute<char>* p_on_program = p_node->first_attribute("short_name");
                const rapidxml::xml_attribute<char>* p_on_preview = p_node->first_attribute(0);

                const auto& is_program = (p_on_program) ? p_on_program->value() : "false";
                const auto& is_preview = (p_on_preview) ? p_on_preview->value() : "false";
                // Display the tally state
                ui->plainTextEdit->appendPlainText(QString("Tally, on_program = %1, on_preview = %2").arg(is_program, is_preview));
                qDebug() << "on_program " << is_program << "\n";
            }
            // Free any meta-data
            NDIlib_recv_free_metadata(*pNDI_recv, metadata);
            sync_meta = 1;
            break;
        }
        default: {
//                std::cout << "default case" << std::endl;
            break;
        }
        // END switch
        }

        if (sync_timeout == 1) break;

    }
    // END for-loop
    ++(*i);
    qDebug() << "flag = " << *flag << "\n";


// Finished
}
