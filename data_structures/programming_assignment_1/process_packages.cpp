//
//  process_packages.cpp
//  Network Packet Processing Simulation
//
//  Created by Ryan Cormier on 3/27/18.
//  Copyright © 2018 Ryan Cormier. All rights reserved.
//

#include <iostream>
#include <queue>
#include <vector>

const static bool debug = false;

struct Request {
    Request(int arrival_time, int process_time):
    arrival_time(arrival_time),
    process_time(process_time)
    {}
    
    int arrival_time;
    int process_time;
};

struct Response {
    Response(bool dropped, int start_time):
    dropped(dropped),
    start_time(start_time)
    {}
    
    bool dropped;
    int start_time;
};

class Buffer {
public:
    Buffer(int size):
    size_(size),
    finish_time_()
    {}
    
    std::queue<int> finish_times()  const { return std::queue<int>(finish_time_); }
    
    void printQueue() const {
        std::queue<int> copy(finish_time_);
        std::cout << "{";
        int count = (int)copy.size();
        for (int i = 0; i < count; ++i) {
            if (i > 0) std::cout << ",";
            std::cout << " " << copy.front();
            copy.pop();
        }
        std::cout << " }";
    }
    
    Response Process(const Request &request) {
        // write your code here
        if (debug) {
            std::cout << "Request -> arrival time: " << request.arrival_time
            << "  process time: " << request.process_time << std::endl;
        }
        Response response(false, request.arrival_time);
        
        // remove past finish times
        if (debug) {
            std::cout << "Buffer -> size: " << size_ << "  finish times: ";
            printQueue();
            std::cout << std::endl;
        }
        while (!finish_time_.empty() &&
               request.arrival_time >= finish_time_.front()) {
            finish_time_.pop();
            if (debug) {
                printQueue();
                std::cout << std::endl;
            }
        }
        
        // empty buffer
        if (finish_time_.empty()) {
            if (size_ == 0) {
                if (debug) std::cout << "Dropping packet";
                response.dropped = true;
            } else {
                finish_time_.push(request.arrival_time + request.process_time);
            }
            return response;
        }
        
        // can it fit
        if (finish_time_.size() < size_) {
            
            response.start_time = finish_time_.back();
            finish_time_.push(response.start_time + request.process_time);
            if (debug) {
                std::cout << "Buffer not full => response start time: "
                << std::endl;
            }
        } else {
            response.dropped = true;
            if (debug) std::cout << "Buffer full => Dropping packet" << std::endl;
        }
        
        return response;
    }
    
private:
    int size_;
    std::queue <int> finish_time_;
};

std::vector <Request> ReadRequests() {
    std::vector <Request> requests;
    int count;
    std::cin >> count;
    for (int i = 0; i < count; ++i) {
        int arrival_time, process_time;
        std::cin >> arrival_time >> process_time;
        requests.push_back(Request(arrival_time, process_time));
    }
    return requests;
}

std::vector <Response> ProcessRequests(const std::vector <Request> &requests, Buffer *buffer) {
    std::vector <Response> responses;
    for (int i = 0; i < requests.size(); ++i)
        responses.push_back(buffer->Process(requests[i]));
    return responses;
}

void PrintResponses(const std::vector <Response> &responses) {
    for (int i = 0; i < responses.size(); ++i)
        std::cout << (responses[i].dropped ? -1 : responses[i].start_time) << std::endl;
}

int main() {
    int size;
    std::cin >> size;
    std::vector <Request> requests = ReadRequests();
    
    Buffer buffer(size);
    std::vector <Response> responses = ProcessRequests(requests, &buffer);
    
    PrintResponses(responses);
    return 0;
}

