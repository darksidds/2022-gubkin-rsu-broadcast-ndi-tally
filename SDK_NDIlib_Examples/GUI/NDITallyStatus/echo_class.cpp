#include "echo_class.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rapidxml/rapidxml.hpp"

#include <QTimer>
#include <string.h>


EchoClass::EchoClass(QObject *parent)
{ 

}


void EchoClass::endEchoClass()
{
    NDIlib_recv_destroy(pNDI_recv);
    qDebug() << "ndi destroyed\n";
}


void EchoClass::initEchoClass()
{
    QString ndi_name = mw->StringValue();
    QByteArray str = ndi_name.toLocal8Bit();
    char *term = str.data();
    char ch = '\0';
    strncat(term, &ch, 1);

    recv_desc.source_to_connect_to.p_ndi_name = term;
    pNDI_recv = NDIlib_recv_create_v3(&recv_desc);
    flag = 0;
    emit isInitialized();
}


/* void EchoClass::setMW(MainWindow* mainwindow)
{
    mw = mainwindow;

    initEchoClass(mw->StringValue());

    recv_desc.bandwidth = NDIlib_recv_bandwidth_lowest;

    NDI_tally.on_program = "on_program";

    pNDI_recv = NDIlib_recv_create_v3(&recv_desc);

    NDIlib_recv_set_tally(pNDI_recv, &NDI_tally);

    i = 0;
    flag = 0;
} */


void EchoClass::echoing()
{
    while(!flag)
    {
    NDIlib_metadata_frame_t metadata;
    NDIlib_video_frame_v2_t frame;

    qDebug() << "func start\n";
    qDebug() << recv_desc.source_to_connect_to.p_ndi_name << "\n";
    // We are going to get meta-data from the source

    //mw->textAppend(QString("Frame = %1").arg(i));

    bool sync_meta = 0;
    bool sync_vid = 0;
    bool sync_timeout = 0;


    while ((sync_vid == 0) || (sync_meta == 0))
        {
            qDebug() << "check flags\n";
            switch (NDIlib_recv_capture_v2(pNDI_recv, &frame, nullptr, &metadata, 5000))
            {
            case NDIlib_frame_type_none: {
                if (sync_timeout == 1) break;
        //                std::cout << "timeout\n";
                emit isnothingCaptured();
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
                NDIlib_recv_free_video_v2(pNDI_recv, &frame);
                emit isvideoCaptured();
                sync_vid = 1;
                break;
            }
            case NDIlib_frame_type_metadata: {
                qDebug() << "meta check\n";
                if (sync_meta == 1) break;
                //mw->textAppend("new metadata frame");
                // Parse the XML
                rapidxml::xml_document<char> parser;
                parser.parse<0>((char*)metadata.p_data);

                // Get the tag
                rapidxml::xml_node<char>* p_node = parser.first_node();

                //mw->textAppend("xml parsed");
                //mw->textAppend(QString("name = %1").arg(p_node->name()));

                // Check its a node
                if ((!p_node) || (p_node->type() != rapidxml::node_element))
                {	// Not a valid message
                    //mw->textAppend("Not valid");
                }
                else // if (!::strcasecmp(p_node->name(), "ndi_tally_echo"))
                {	// Get the zoom factor
                    const rapidxml::xml_attribute<char>* p_on_program = p_node->first_attribute("short_name");
                    const rapidxml::xml_attribute<char>* p_on_preview = p_node->first_attribute(0);

                    const auto& is_program = (p_on_program) ? p_on_program->value() : "false";
                    const auto& is_preview = (p_on_preview) ? p_on_preview->value() : "false";
                    // Display the tally state
                    //mw->textAppend(QString("Tally, on_program = %1, on_preview = %2").arg(is_program, is_preview));
                    qDebug() << "on_program " << is_program << "\n";
                }
                // Free any meta-data
                NDIlib_recv_free_metadata(pNDI_recv, &metadata);
                emit ismetadataCaptured();
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
        ++i;
        qDebug() << "flag = " << flag << "\n";
    // Finished
        }
}




