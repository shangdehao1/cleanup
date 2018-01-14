#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

using namespace std;
using namespace google::protobuf;


#include "../buf_handle.h"
#include "../tran_buf_pool.h"
#include "../buffer.h"
//#include "flow_controller.h"

void test_for_output_stream(string filename){

    int outfd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);

    //operation 1
    google::protobuf::io::ZeroCopyOutputStream* out_stream = 
        new google::protobuf::io::FileOutputStream(outfd, 1024);

    std::vector<std::string> contents;
    contents.push_back("shang");
    contents.push_back("de");
    contents.push_back("hao");

    void* buffer = NULL;
    int size = 0;

    for (size_t i = 0; i < contents.size(); ++i) {
        // malloc memory space from fileoutputstream
        // out_stream is raw pointer.
        // operation 2
        if (!out_stream->Next(&buffer, &size)) {
            cout<<"Next operation failed.."<<endl;
            break;
        }

        cout<<"buffer address is "<<buffer;
        cout<<" buufer size is "<<size;

        const std::string& content = contents[i];

        if (size > content.size() + 1) {
            //operation 3
            memcpy((char*)buffer, content.c_str(), content.size());
            memcpy((char*)buffer + content.size(), "\n", 1);
            // recover abundant memory space 
            //operation 4
            out_stream->BackUp(size - content.size() - 1);
            // operation 4
            cout<<" Now, total writen byte size is "<<out_stream->ByteCount()<<endl;
        }
    }

    delete out_stream;
    close(outfd);
}


void test_for_input_stream(string filename){

    int infd = open(filename.c_str(), O_RDONLY);
    // the second parameter is max byte num of every reading operation.
    google::protobuf::io::ZeroCopyInputStream* in_stream = 
        new google::protobuf::io::FileInputStream(infd, 10);

    // all reading operation will use this memory space, so 
    // if skip or backup operation have fails, reading content will wrong.
    const void* buffer;
    int size = 0; // every reading operation's btytes num.
    while (in_stream->Next(&buffer, &size)) {
        // skip skiped_len byte. When end of file, return false.
         //bool skiped = in_stream->Skip(skiped_len);
        /*
         // come back backup_len       
         if (backup_len < size) {
             in_stream->BackUp(backup_len);
        }
        */
        cout<<"buffer content is "<<(char*)buffer<<"byte num is "<<size<<endl;
        //printf("\n%d Next size:%d bytecount:%d\n", i++, size, in_stream->ByteCount());
    }
}



int main(){

    //ReadBuffer xx;
    
    cout<<"------"<<endl;
    test_for_output_stream("testfile_temp");
    cout<<"------"<<endl;
    test_for_input_stream("testfile_temp");

    return 0;
}
