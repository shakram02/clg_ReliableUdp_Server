//
// Created by ahmed on 12/23/16.
//

#include <cstring>
#include "WorkerFacade.h"
#include "BinarySerializer.h"

#define FRAGMENT_SIZE 256   // MAX UDP transfer is 256 bytes
#define MAX_FAIL_COUNT 10
#define SERV_FILESZ_HEADER "FILESIZE-"

void WorkerFacade::StartWorking()
{
    // TODO incoomplete

    int fail_count = 0;

    cout << "void WorkerFacade::StartWorking()" << endl;

    basic_string<char> file_send_header(SERV_FILESZ_HEADER);
    string num(to_string(this->fragmenter.GetFragmentCount()));
    file_send_header.append(num);

    cout << "WorkerFacade#Sending number of fragments:" << num << endl;
    worker_socket.SendPacket((void *) file_send_header.c_str(), (unsigned int) file_send_header.length());

    // Send dummy test data
    string d("Dummyy");
    unsigned int pck_seq = 6;
    DataPacket dummy_packet((void *) d.c_str(), (unsigned short) d.size(), pck_seq);

    void *dummy_raw_pckt_ptr;

    BinarySerializer::SerializeDataPacket(&dummy_packet, &dummy_raw_pckt_ptr);

    // sizeof(DataPacket) will return a size with the full array of 128 chars,
    // on the receiver size, the size will be re-fit using the length field
    worker_socket.SendPacket(dummy_raw_pckt_ptr, sizeof(DataPacket));


    void *ack_raw = calloc(1, sizeof(AckPacket));
    int ack_size;
    bool result = worker_socket.ReceivePacket(sizeof(AckPacket), ack_raw, &ack_size);

    if (!result) {
        cerr << "Nothing received" << endl;
        cout << endl;
        return;
    }
    cout << "Received:" << ack_size << " bytes" << endl;
    AckPacket *ack_pck;
    BinarySerializer::DeserializeAckPacket(ack_raw, &ack_pck);

    cout << "ACK-> Num:" << ack_pck->ack_num
         << " Checksum:" << ack_pck->chksum
         << endl;

    free(ack_raw);

//    if (worker_socket.ReceiveAckPacket(ack_pck)) {
//        cout << "ACK-> Num:" << ack_pck.ack_num
//             << " Checksum:" << ack_pck.chksum
//             << " Length:" << ack_pck.len << endl;
//
//        if (ack_pck.ack_num == pck_seq) {
//            cout << "ACK Success:" << ack_pck.ack_num << endl;
//        } else {
//            cerr << "Bad seq number!!" << endl;
//            cout << endl;
//        }
//
//    } else {
//        cerr << "Failed to be ACKed!!" << endl;
//        cout << endl;
//    }
//

    int i = 0;
    while (false && !fragmenter.EndOfFile() && is_working && fail_count < MAX_FAIL_COUNT) {
//        // Generate the dummy_packet * window_size
//
//        void *buff =0;
//        int frag_size = fragmenter.NextFragment(&buff);
//        // Send it to the worker_socket, wait for the worker_socket response for the whole window
//        // -use Select/Non blocking IO?-
//
//        DataPacket dummy_packet();
//
//        BinarySerializer::SerializeDataPacket(dummy_packet, &buff);
//        worker_socket.SendPacket(buff, (unsigned int) frag_size);
//        cout << "Fragmenter#Fragment size:" << frag_size << " bytes, #" << i++ << endl;
//
//
//        void *rcv_buff;
//        int recv_size;
//        worker_socket.ReceiveAckPacket(FRAGMENT_SIZE, &rcv_buff, &recv_size);
//
//        cout << "ACK:" << string((char *) rcv_buff) << endl;
//
//        // Read the worker_socket response(s)
//        // ACK -> WillAdvance = true
//        // NACK -> WillAdvance = false
//
//        free(buff);
//        free(rcv_buff);
    }

}

WorkerFacade::WorkerFacade(sock_descriptor sockfd) :
        worker_socket(sockfd)
{
    string file_name = this->worker_socket.GetRequestedFile();
    cout << "WorkerFacade#Requested filename:" << file_name << endl;
    this->fragmenter.SetFragmentSize(FRAGMENT_SIZE);
    if (!this->fragmenter.SetFilePath(file_name)) {
        cerr << "Worker#Failed to set the file path" << endl;
    }
    this->is_working = true;
}

void WorkerFacade::StopWorking()
{
    this->is_working = false;
}

WorkerFacade::~WorkerFacade()
{
    // TODO releease resources
}